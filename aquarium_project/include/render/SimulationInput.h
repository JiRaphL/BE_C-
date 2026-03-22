#ifndef _SIMULATION_INPUT_H_
#define _SIMULATION_INPUT_H_

struct SimulationInput
{
   bool requestClose = false;
   bool togglePause = false;
   bool triggerFoodPulse = false;
   bool triggerPollution = false;
   bool triggerDisturbance = false;
   int forcedBirths = 0;
   int forcedDeaths = 0;
   int forcedBehaviorSwitches = 0;
};

#endif
