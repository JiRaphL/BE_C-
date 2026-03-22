#ifndef _CIMG_RENDERER_H_
#define _CIMG_RENDERER_H_

#include "render/Renderer.h"
#include "UImg.h"

#include <CImg.h>


class CImgRenderer : public Renderer
{
private:
   cimg_library::CImgDisplay displayWindow;
   bool showHud;

private:
   void drawHud( UImg & frame, const Milieu & milieu ) const;

public:
   CImgRenderer( int width, int height, const char * title );

   bool shouldClose( void ) const override;
   SimulationInput processInput( void ) override;
   void render( const Milieu & milieu ) override;
   void wait( int delayMs ) override;
};


#endif
