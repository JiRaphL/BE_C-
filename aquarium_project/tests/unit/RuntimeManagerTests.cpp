#include "framework/TestFramework.h"

#include "Bestiole.h"
#include "BestioleFactory.h"
#include "SimulationConfig.h"
#include "managers/CollisionManager.h"
#include "managers/LifecycleManager.h"

#include "behavior/GregariousBehavior.h"

#include <memory>
#include <vector>


TEST_CASE( collision_manager_kills_on_certain_death_probability )
{
   std::vector<std::unique_ptr<Bestiole>> population;
   auto a = std::make_unique<Bestiole>( std::make_unique<GregariousBehavior>() );
   auto b = std::make_unique<Bestiole>( std::make_unique<GregariousBehavior>() );
   a->setCoords( 50, 50 );
   b->setCoords( 50, 50 );
   population.push_back( std::move( a ) );
   population.push_back( std::move( b ) );

   CollisionManager manager( 10.0, 1.0 );
   manager.resolve( population );

   EXPECT_TRUE( !population[0]->isAlive() );
   EXPECT_TRUE( !population[1]->isAlive() );
}


TEST_CASE( collision_manager_bounces_when_no_death_risk )
{
   std::vector<std::unique_ptr<Bestiole>> population;
   auto a = std::make_unique<Bestiole>( std::make_unique<GregariousBehavior>() );
   auto b = std::make_unique<Bestiole>( std::make_unique<GregariousBehavior>() );
   a->setCoords( 50, 50 );
   b->setCoords( 50, 50 );
   a->setOrientation( 0.0 );
   b->setOrientation( 1.0 );
   population.push_back( std::move( a ) );
   population.push_back( std::move( b ) );

   CollisionManager manager( 10.0, 0.0 );
   manager.resolve( population );

   EXPECT_TRUE( population[0]->isAlive() );
   EXPECT_TRUE( population[1]->isAlive() );
}


TEST_CASE( lifecycle_manager_ages_and_removes_dead )
{
   std::vector<std::unique_ptr<Bestiole>> population;
   auto b = std::make_unique<Bestiole>( std::make_unique<GregariousBehavior>() );
   b->setAge( 0 );
   b->setMaxAge( 1 );
   population.push_back( std::move( b ) );

   LifecycleManager manager( 5, 5 );
   manager.ageAndCull( population );

   EXPECT_EQ( population.size(), static_cast<std::size_t>( 0 ) );
}


TEST_CASE( lifecycle_manager_processes_clone_and_spontaneous_birth )
{
   SimulationConfig config;
   config.setInitialPopulation( 1 );
   config.setBehaviorPercentages( 100.0, 0.0, 0.0, 0.0 );
   config.setSpontaneousBirthRate( 1.0 );
   config.setCloningRate( 1.0 );

   BestioleFactory factory( config );
   std::vector<std::unique_ptr<Bestiole>> population;
   population.push_back( factory.createFromConfig() );

   LifecycleManager manager( 8, 8 );
   manager.initializeNewborn( *population[0] );
   manager.processBirthsAndClones( population, factory, 200, 200 );

   EXPECT_TRUE( population.size() >= static_cast<std::size_t>( 3 ) );
}
