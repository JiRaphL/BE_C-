#include "managers/LifecycleManager.h"

#include "Bestiole.h"
#include "BestioleFactory.h"

#include <algorithm>
#include <utility>


LifecycleManager::LifecycleManager( int minLifespanValue, int maxLifespanValue )
   : minLifespan( std::max( 1, minLifespanValue ) ),
     maxLifespan( std::max( minLifespan, maxLifespanValue ) ),
     rng( std::random_device{}() )
{
}


int LifecycleManager::sampleLifespan( void ) const
{
   std::uniform_int_distribution<int> lifespanDist( minLifespan, maxLifespan );
   return lifespanDist( rng );
}


void LifecycleManager::initializeNewborn( Bestiole & bestiole ) const
{
   bestiole.revive();
   bestiole.setAge( 0 );
   bestiole.setMaxAge( sampleLifespan() );
}


LifecycleStats LifecycleManager::ageAndCull( std::vector<std::unique_ptr<Bestiole>> & population ) const
{
   LifecycleStats stats;
   for ( std::vector<std::unique_ptr<Bestiole>>::iterator it = population.begin(); it != population.end(); ++it )
   {
      if ( !( *it )->isAlive() )
         continue;

      ( *it )->incrementAge();
      if ( ( *it )->getAge() >= ( *it )->getMaxAge() )
      {
         ( *it )->kill();
         ++stats.deathsByAge;
      }
   }

   population.erase(
      std::remove_if( population.begin(),
                      population.end(),
                      []( const std::unique_ptr<Bestiole> & b ) { return !b->isAlive(); } ),
      population.end() );

   return stats;
}


LifecycleStats LifecycleManager::processBirthsAndClones( std::vector<std::unique_ptr<Bestiole>> & population,
                                                         BestioleFactory & factory,
                                                         int width,
                                                         int height ) const
{
   LifecycleStats stats;
   std::vector<std::unique_ptr<Bestiole>> newborns;
   const std::size_t baseCount = population.size();

   for ( std::size_t i = 0; i < baseCount; ++i )
   {
      if ( !population[ i ]->isAlive() )
         continue;

      std::unique_ptr<Bestiole> clone = factory.tryCreateClone( *( population[ i ] ) );
      if ( clone )
      {
         initializeNewborn( *clone );
         clone->setCoords( population[ i ]->getX(), population[ i ]->getY() );
         newborns.push_back( std::move( clone ) );
         ++stats.birthsByClone;
      }
   }

   std::unique_ptr<Bestiole> spontaneousBirth = factory.tryCreateSpontaneousBirth();
   if ( spontaneousBirth )
   {
      initializeNewborn( *spontaneousBirth );
      spontaneousBirth->initCoords( width, height );
      newborns.push_back( std::move( spontaneousBirth ) );
      ++stats.birthsSpontaneous;
   }

   for ( std::vector<std::unique_ptr<Bestiole>>::iterator it = newborns.begin(); it != newborns.end(); ++it )
      population.push_back( std::move( *it ) );

   return stats;
}
