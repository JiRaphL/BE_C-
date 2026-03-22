#include "behavior/KamikazeBehavior.h"

#include "Bestiole.h"
#include "Milieu.h"

#include <memory>


std::unique_ptr<IBehavior> KamikazeBehavior::clone( void ) const
{
   return std::make_unique<KamikazeBehavior>( *this );
}


void KamikazeBehavior::apply( Bestiole & bestiole, Milieu & milieu ) const
{
   const Bestiole * voisine = milieu.plusProcheVoisine( bestiole );
   if ( voisine != nullptr )
   {
      bestiole.orienterVers( voisine->getX(), voisine->getY(), 0.45 );
      bestiole.setVitesse( bestiole.getVitesseBase() * 1.7 );
   }
   else
   {
      bestiole.resetVitesse();
   }

   bestiole.avance( milieu );
}


const char * KamikazeBehavior::name( void ) const
{
   return "Kamikaze";
}
