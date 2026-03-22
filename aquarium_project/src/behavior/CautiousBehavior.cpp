#include "behavior/CautiousBehavior.h"

#include "Bestiole.h"
#include "Milieu.h"

#include <memory>


std::unique_ptr<IBehavior> CautiousBehavior::clone( void ) const
{
   return std::make_unique<CautiousBehavior>( *this );
}


void CautiousBehavior::apply( Bestiole & bestiole, Milieu & milieu ) const
{
   const Bestiole * voisine = milieu.plusProcheVoisine( bestiole );
   if ( voisine != nullptr && bestiole.distance( *voisine ) < 40.0 )
   {
      bestiole.orienterOppose( voisine->getX(), voisine->getY(), 0.50 );
      bestiole.setVitesse( bestiole.getVitesseBase() * 0.85 );
   }
   else
   {
      bestiole.resetVitesse();
   }

   bestiole.avance( milieu );
}


const char * CautiousBehavior::name( void ) const
{
   return "Cautious";
}
