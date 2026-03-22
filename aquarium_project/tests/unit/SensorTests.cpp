#include "framework/TestFramework.h"
#include "support/DomainModels.h"


TEST_CASE( eye_sensor_detects_target_in_fov_and_range )
{
   testsupport::AgentState observer{ 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, true, 0, 100 };
   testsupport::AgentState target{ 10.0, 0.0, 0.0, 1.0, 0.2, 1.0, true, 0, 100 };
   testsupport::EyeSensor eye{ testsupport::PI / 2.0, 30.0, 0.8 };

   EXPECT_TRUE( eye.detects( observer, target ) );
}


TEST_CASE( eye_sensor_rejects_target_outside_fov )
{
   testsupport::AgentState observer{ 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, true, 0, 100 };
   testsupport::AgentState target{ -10.0, 0.0, 0.0, 1.0, 0.2, 1.0, true, 0, 100 };
   testsupport::EyeSensor eye{ testsupport::PI / 2.0, 30.0, 0.9 };

   EXPECT_TRUE( !eye.detects( observer, target ) );
}


TEST_CASE( ear_sensor_detects_omnidirectionally )
{
   testsupport::AgentState observer{ 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, true, 0, 100 };
   testsupport::AgentState target{ -12.0, 8.0, 0.0, 1.0, 0.3, 1.0, true, 0, 100 };
   testsupport::EarSensor ear{ 20.0, 0.7 };

   EXPECT_TRUE( ear.detects( observer, target ) );
}


TEST_CASE( ear_sensor_respects_camouflage_threshold )
{
   testsupport::AgentState observer{ 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, true, 0, 100 };
   testsupport::AgentState target{ 5.0, 0.0, 0.0, 1.0, 0.9, 1.0, true, 0, 100 };
   testsupport::EarSensor ear{ 20.0, 0.8 };

   EXPECT_TRUE( !ear.detects( observer, target ) );
}
