#include "framework/TestFramework.h"

#include "BestioleFactory.h"
#include "Milieu.h"
#include "SimulationConfig.h"
#include "app/Simulation.h"
#include "app/StatisticsCollector.h"
#include "render/Renderer.h"

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <vector>


namespace
{
   class StubRenderer : public Renderer
   {
   private:
      int frameCount;
      int closeAfter;

   public:
      explicit StubRenderer( int closeAfterFrames ) : frameCount( 0 ), closeAfter( closeAfterFrames ) {}

      bool shouldClose( void ) const override { return frameCount >= closeAfter; }
      SimulationInput processInput( void ) override { return SimulationInput{}; }
      void render( const Milieu & ) override { ++frameCount; }
      void wait( int ) override {}
   };
}


TEST_CASE( simulation_records_statistics_every_tick )
{
   SimulationConfig config;
   config.setInitialPopulation( 10 );
   config.setBehaviorPercentages( 25.0, 25.0, 25.0, 25.0 );
   config.setSpontaneousBirthRate( 0.0 );
   config.setCloningRate( 0.0 );
   config.setTiming( 0, 5 );

   BestioleFactory factory( config );
   Milieu milieu( 300, 200 );
   milieu.configureRuntime( factory, config );

   std::vector<std::unique_ptr<Bestiole>> initialPopulation = factory.createInitialPopulation();
   for ( std::vector<std::unique_ptr<Bestiole>>::iterator it = initialPopulation.begin(); it != initialPopulation.end(); ++it )
      milieu.addMember( std::move( *it ) );

   StubRenderer renderer( 100 );
   StatisticsCollector collector;
   Simulation simulation( milieu, renderer, collector, config.getDelayMs(), config.getMaxTicks() );
   simulation.run();

   EXPECT_EQ( collector.history().size(), static_cast<std::size_t>( 5 ) );
   const PopulationSnapshot * last = collector.last();
   EXPECT_TRUE( last != nullptr );
   EXPECT_EQ( last->tick, 4 );
   EXPECT_TRUE( collector.exportCsv( "test_stats.csv" ) );
   EXPECT_TRUE( collector.exportJson( "test_stats.json" ) );

   std::ifstream csv( "test_stats.csv" );
   std::ifstream json( "test_stats.json" );
   EXPECT_TRUE( csv.is_open() );
   EXPECT_TRUE( json.is_open() );

   std::string csvLine;
   std::getline( csv, csvLine );
   EXPECT_TRUE( csvLine.find( "collisions" ) != std::string::npos );

   std::string jsonContent;
   std::getline( json, jsonContent );
   EXPECT_TRUE( jsonContent.find( "[" ) != std::string::npos );

   std::remove( "test_stats.csv" );
   std::remove( "test_stats.json" );
}
