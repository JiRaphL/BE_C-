#include "behavior/GregariousBehavior.h"

#include "Bestiole.h"
#include "Milieu.h"

#include <memory>


std::unique_ptr<IBehavior> GregariousBehavior::clone( void ) const
{
   return std::make_unique<GregariousBehavior>( *this );
}


void GregariousBehavior::apply( Bestiole & bestiole, Milieu & milieu ) const
{
   const Bestiole * voisine = milieu.plusProcheVoisine( bestiole );
   if ( voisine != nullptr )
      bestiole.orienterVers( voisine->getX(), voisine->getY(), 0.20 );

   bestiole.resetVitesse();
   bestiole.avance( milieu );
}


const char * GregariousBehavior::name( void ) const
{
   return "Gregarious";
}
