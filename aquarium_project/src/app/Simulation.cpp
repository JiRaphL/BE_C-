#include "app/Simulation.h"

#include "Milieu.h"
#include "app/StatisticsCollector.h"
#include "render/Renderer.h"


Simulation::Simulation( Milieu & milieu, Renderer & renderer, StatisticsCollector & collector, int delayMilliseconds, int maxTicksValue )
   : milieuRef( milieu ),
     rendererRef( renderer ),
     statsRef( collector ),
     delayMs( delayMilliseconds ),
     tick( 0 ),
     maxTicks( maxTicksValue )
{
}


void Simulation::run( void )
{
   bool paused = false;

   while ( !rendererRef.shouldClose() )
   {
      if ( maxTicks >= 0 && tick >= maxTicks )
         break;

      const SimulationInput input = rendererRef.processInput();
      if ( input.requestClose )
         break;

      if ( input.togglePause )
         paused = !paused;

      // External events are injected before the next simulation step.
      if ( input.triggerFoodPulse )
         milieuRef.triggerFoodPulse();
      if ( input.triggerPollution )
         milieuRef.triggerPollution();
      if ( input.triggerDisturbance )
         milieuRef.triggerDisturbance();
      if ( input.forcedBirths > 0 )
         milieuRef.forceExternalBirths( input.forcedBirths );
      if ( input.forcedDeaths > 0 )
         milieuRef.forceExternalDeaths( input.forcedDeaths );
      if ( input.forcedBehaviorSwitches > 0 )
         milieuRef.forceExternalBehaviorSwitches( input.forcedBehaviorSwitches );

      if ( !paused )
      {
         milieuRef.step();
         statsRef.record( tick, milieuRef );
         ++tick;
      }

      rendererRef.render( milieuRef );
      rendererRef.wait( delayMs );
   }
}
