#include "framework/TestFramework.h"
#include "support/DomainModels.h"


TEST_CASE( fin_accessory_increases_speed )
{
   testsupport::AgentState agent;
   agent.speed = 2.0;
   testsupport::FinAccessory fin{ 1.5 };

   fin.apply( agent );

   EXPECT_NEAR( agent.speed, 3.0, 1e-9 );
}


TEST_CASE( shell_accessory_reduces_speed_and_increases_resistance )
{
   testsupport::AgentState agent;
   agent.speed = 6.0;
   agent.deathResistance = 1.0;
   testsupport::ShellAccessory shell{ 2.0, 3.0 };

   shell.apply( agent );

   EXPECT_NEAR( agent.speed, 3.0, 1e-9 );
   EXPECT_NEAR( agent.deathResistance, 3.0, 1e-9 );
}


TEST_CASE( camouflage_accessory_updates_camouflage_factor )
{
   testsupport::AgentState agent;
   agent.camouflage = 0.0;
   testsupport::CamouflageAccessory camouflage{ 0.65 };

   camouflage.apply( agent );

   EXPECT_NEAR( agent.camouflage, 0.65, 1e-9 );
}
