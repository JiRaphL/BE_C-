#include "framework/TestFramework.h"
#include "support/DomainModels.h"

#include <cmath>


TEST_CASE( collision_manager_applies_shell_resistance )
{
   testsupport::AgentState a;
   testsupport::AgentState b;
   a.x = 0.0; a.y = 0.0; a.deathResistance = 1.0;
   b.x = 0.0; b.y = 0.0; b.deathResistance = 4.0;

   testsupport::CollisionManagerModel::resolve( a, b, 1.0, 0.8, 0.6, 0.6 );

   EXPECT_TRUE( !a.alive );
   EXPECT_TRUE( b.alive );
}


TEST_CASE( collision_manager_bounces_survivors )
{
   testsupport::AgentState a;
   testsupport::AgentState b;
   a.x = 0.0; a.y = 0.0; a.orientation = 0.0;
   b.x = 0.0; b.y = 0.0; b.orientation = 1.0;

   testsupport::CollisionManagerModel::resolve( a, b, 1.0, 0.5, 0.9, 0.9 );

   EXPECT_TRUE( a.alive );
   EXPECT_TRUE( b.alive );
   EXPECT_NEAR( std::fabs( a.orientation ), testsupport::PI, 1e-9 );
}


TEST_CASE( lifecycle_manager_kills_when_max_age_reached )
{
   testsupport::AgentState a;
   a.age = 4;
   a.maxAge = 5;
   a.alive = true;

   testsupport::LifecycleManagerModel lifecycle( 0.1, 0.2 );
   lifecycle.ageOneTick( a );

   EXPECT_TRUE( !a.alive );
}


TEST_CASE( lifecycle_manager_birth_and_clone_thresholds )
{
   testsupport::LifecycleManagerModel lifecycle( 0.2, 0.3 );

   EXPECT_TRUE( lifecycle.shouldSpawn( 0.1 ) );
   EXPECT_TRUE( !lifecycle.shouldSpawn( 0.25 ) );
   EXPECT_TRUE( lifecycle.shouldClone( 0.29 ) );
   EXPECT_TRUE( !lifecycle.shouldClone( 0.31 ) );
}
