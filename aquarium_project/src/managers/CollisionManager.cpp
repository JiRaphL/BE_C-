#include "managers/CollisionManager.h"

#include "Bestiole.h"

#include <algorithm>


CollisionManager::CollisionManager( double collisionDistanceValue, double baseDeathProbabilityValue )
   : collisionDistance( std::max( 0.0, collisionDistanceValue ) ),
     baseDeathProbability( std::max( 0.0, std::min( baseDeathProbabilityValue, 1.0 ) ) ),
     rng( std::random_device{}() )
{
}


CollisionStats CollisionManager::resolve( std::vector<std::unique_ptr<Bestiole>> & population ) const
{
   CollisionStats stats;
   std::uniform_real_distribution<double> random01( 0.0, 1.0 );

   for ( std::size_t i = 0; i < population.size(); ++i )
   {
      for ( std::size_t j = i + 1; j < population.size(); ++j )
      {
         Bestiole & a = *( population[ i ] );
         Bestiole & b = *( population[ j ] );

         if ( !a.isAlive() || !b.isAlive() )
            continue;

         if ( a.distance( b ) > collisionDistance )
            continue;

         ++stats.collisions;
         const double deathProbA = std::max( 0.0, std::min( baseDeathProbability / std::max( 1.0, a.getDeathResistance() ), 1.0 ) );
         const double deathProbB = std::max( 0.0, std::min( baseDeathProbability / std::max( 1.0, b.getDeathResistance() ), 1.0 ) );

         if ( random01( rng ) < deathProbA )
         {
            a.kill();
            ++stats.deaths;
         }
         else a.demiTour();

         if ( random01( rng ) < deathProbB )
         {
            b.kill();
            ++stats.deaths;
         }
         else b.demiTour();
      }
   }

   return stats;
}
