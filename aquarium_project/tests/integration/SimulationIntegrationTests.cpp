#include "framework/TestFramework.h"

#include "BestioleFactory.h"
#include "Milieu.h"
#include "SimulationConfig.h"

#include <memory>
#include <vector>


TEST_CASE( full_pipeline_config_factory_milieu_steps )
{
   SimulationConfig config;
   config.setInitialPopulation( 40 );
   config.setBehaviorPercentages( 40.0, 30.0, 20.0, 10.0 );
   config.setSpontaneousBirthRate( 0.0 );
   config.setCloningRate( 0.0 );

   BestioleFactory factory( config );
   std::vector<std::unique_ptr<Bestiole>> initialPopulation = factory.createInitialPopulation();

   Milieu milieu( 640, 480 );
   for ( std::vector<std::unique_ptr<Bestiole>>::iterator it = initialPopulation.begin(); it != initialPopulation.end(); ++it )
      milieu.addMember( std::move( *it ) );

   EXPECT_EQ( milieu.populationSize(), static_cast<std::size_t>( 40 ) );

   for ( int i = 0; i < 30; ++i )
      milieu.step();

   EXPECT_EQ( milieu.populationSize(), static_cast<std::size_t>( 40 ) );
}


TEST_CASE( factory_supports_spontaneous_birth_and_clone_hooks )
{
   SimulationConfig config;
   config.setInitialPopulation( 1 );
   config.setBehaviorPercentages( 25.0, 25.0, 25.0, 25.0 );
   config.setSpontaneousBirthRate( 1.0 );
   config.setCloningRate( 1.0 );

   BestioleFactory factory( config );
   std::unique_ptr<Bestiole> spawned = factory.tryCreateSpontaneousBirth();
   EXPECT_TRUE( spawned != nullptr );

   std::unique_ptr<Bestiole> original = factory.createFromConfig();
   std::unique_ptr<Bestiole> clone = factory.tryCreateClone( *original );
   EXPECT_TRUE( clone != nullptr );
}
