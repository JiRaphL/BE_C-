#include "BestioleFactory.h"
#include "Milieu.h"
#include "SimulationConfig.h"
#include "app/Simulation.h"
#include "app/StatisticsCollector.h"
#include "render/CImgRenderer.h"

#include <iostream>

using namespace std;


int main()
{

   const int width = 640;
   const int height = 480;

   SimulationConfig config;
   config.setInitialPopulation( 20 );
   config.setBehaviorPercentages( 35.0, 25.0, 20.0, 20.0 );
   config.setSpontaneousBirthRate( 0.01 );
   config.setCloningRate( 0.005 );
   config.setSensorRates( 0.80, 0.50 );
   config.setAccessoryRates( 0.35, 0.20, 0.20 );
   config.setLifespanTicks( 300, 1200 );
   config.setCollisionParams( 14.0, 0.20 );
   config.setTiming( 30, -1 );

   BestioleFactory factory( config );
   Milieu milieu( width, height );
   milieu.configureRuntime( factory, config );
   std::vector<std::unique_ptr<Bestiole>> initialPopulation = factory.createInitialPopulation();
   for ( std::vector<std::unique_ptr<Bestiole>>::iterator it = initialPopulation.begin() ; it != initialPopulation.end() ; ++it )
      milieu.addMember( std::move( *it ) );

   CImgRenderer renderer( width, height, "Simulation d'ecosysteme" );
   StatisticsCollector stats;
   Simulation simulation( milieu, renderer, stats, config.getDelayMs(), config.getMaxTicks() );
   simulation.run();

   const PopulationSnapshot * finalSnapshot = stats.last();
   if ( finalSnapshot )
      std::cout << "Final population: " << finalSnapshot->population << ", tick: " << finalSnapshot->tick << std::endl;

   stats.exportCsv( "simulation_stats.csv" );
   stats.exportJson( "simulation_stats.json" );


   return 0;

}
