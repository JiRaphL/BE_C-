#include "behavior/FearfulBehavior.h"

#include "Bestiole.h"
#include "Milieu.h"

#include <memory>


std::unique_ptr<IBehavior> FearfulBehavior::clone( void ) const
{
   return std::make_unique<FearfulBehavior>( *this );
}


void FearfulBehavior::apply( Bestiole & bestiole, Milieu & milieu ) const
{
   const int voisins = milieu.nbVoisins( bestiole );
   if ( voisins >= 3 )
   {
      bestiole.demiTour();
      bestiole.setVitesse( bestiole.getVitesseBase() * 1.8 );
   }
   else
   {
      bestiole.resetVitesse();
   }

   bestiole.avance( milieu );
}


const char * FearfulBehavior::name( void ) const
{
   return "Fearful";
}
