#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "render/SimulationInput.h"

class Milieu;


class Renderer
{
public:
   virtual ~Renderer( void ) = default;

   virtual bool shouldClose( void ) const = 0;
   virtual SimulationInput processInput( void ) = 0;
   virtual void render( const Milieu & milieu ) = 0;
   virtual void wait( int delayMs ) = 0;
};


#endif
