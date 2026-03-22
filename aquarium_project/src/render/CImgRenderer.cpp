#include "render/CImgRenderer.h"

#include "Milieu.h"

#include <cstdio>


CImgRenderer::CImgRenderer( int width, int height, const char * title )
   : displayWindow( width, height, title ),
     showHud( true )
{
}


bool CImgRenderer::shouldClose( void ) const
{
   return displayWindow.is_closed();
}


SimulationInput CImgRenderer::processInput( void )
{
   SimulationInput input;

   if ( displayWindow.is_keyESC() )
   {
      displayWindow.close();
      input.requestClose = true;
   }

   if ( displayWindow.is_keySPACE() )
      input.togglePause = true;
   if ( displayWindow.is_keyN() )
      input.forcedBirths += 1;
   if ( displayWindow.is_keyK() )
      input.forcedDeaths += 1;
   if ( displayWindow.is_keyC() )
      input.forcedBehaviorSwitches += 1;
   if ( displayWindow.is_keyF() )
      input.triggerFoodPulse = true;
   if ( displayWindow.is_keyP() )
      input.triggerPollution = true;
   if ( displayWindow.is_keyD() )
      input.triggerDisturbance = true;
   if ( displayWindow.is_keyH() )
      showHud = !showHud;

   if ( displayWindow.is_key() )
      displayWindow.flush();

   return input;
}


void CImgRenderer::render( const Milieu & milieu )
{
   UImg frame( milieu );
   drawHud( frame, milieu );
   displayWindow.display( frame );
}


void CImgRenderer::wait( int delayMs )
{
   displayWindow.wait( delayMs );
}


void CImgRenderer::drawHud( UImg & frame, const Milieu & milieu ) const
{
   if ( !showHud )
      return;

   const T black[3] = { 0, 0, 0 };
   const T white[3] = { 255, 255, 255 };
   const T red[3] = { 220, 70, 70 };
   const T blue[3] = { 70, 70, 220 };
   const T green[3] = { 70, 170, 70 };
   const T amber[3] = { 220, 170, 70 };

   frame.draw_rectangle( 4, 4, 620, 118, black, 0.6f );
   frame.draw_text( 10, 10, "IHM: ESC Quitter | SPACE Pause/Resume | H Masquer HUD", white );

   char line[160];
   std::snprintf( line, sizeof( line ), "Population: %d", static_cast<int>( milieu.populationSize() ) );
   frame.draw_text( 10, 30, line, white );

   const RuntimeStepStats & step = milieu.stepStats();
   std::snprintf( line, sizeof( line ),
                  "Tick events: coll=%d deathColl=%d deathAge=%d birthClone=%d birthSpont=%d",
                  step.collisions, step.deathsByCollision, step.deathsByAge, step.birthsByClone, step.birthsSpontaneous );
   frame.draw_text( 10, 48, line, white );

   const ExternalEventStats & ext = milieu.externalEventStats();
   std::snprintf( line, sizeof( line ),
                  "External effects: births=%d deaths=%d switches=%d",
                  ext.birthsByExternalEvent, ext.deathsByExternalEvent, ext.behaviorSwitchesByExternalEvent );
   frame.draw_text( 10, 66, line, white );

   std::snprintf( line, sizeof( line ),
                  "External triggers: food(F)=%d pollution(P)=%d disturbance(D)=%d",
                  ext.foodPulses, ext.pollutionEvents, ext.disturbanceEvents );
   frame.draw_text( 10, 84, line, white );

   frame.draw_text( 10, 102, "Manual events: N naissance | K mort | C changement comportement", white );

   frame.draw_circle( 445, 83, 3, blue );
   frame.draw_text( 452, 77, "Sensor Eye", white );
   frame.draw_rectangle( 445, 94, 451, 100, amber );
   frame.draw_text( 452, 91, "Sensor Ear", white );
   frame.draw_triangle( 545, 83, 551, 83, 548, 77, green );
   frame.draw_text( 555, 77, "Fin", white );
   frame.draw_rectangle( 545, 94, 551, 100, black );
   frame.draw_text( 555, 91, "Shell/Camo marks on fish tail", white );

   frame.draw_circle( 446, 111, 2, red );
   frame.draw_text( 452, 105, "Camo marker red, Shell marker black", white );
}
