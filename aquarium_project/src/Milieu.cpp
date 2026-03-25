#include "Milieu.h"

#include "BestioleFactory.h"
#include "SimulationConfig.h"
#include "managers/CollisionManager.h"
#include "managers/LifecycleManager.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>


const T    Milieu::white[] = { (T)255, (T)255, (T)255 };


Milieu::Milieu( int _width, int _height ) : UImg( _width, _height, 1, 3 ),
                                            width(_width), height(_height),
                                            bestioleFactory( nullptr ),
                                            rng( std::random_device{}() )
{

   std::cout << "const Milieu" << std::endl;

   std::srand( time(NULL) );

}


Milieu::~Milieu( void )
{

   std::cout << "dest Milieu" << std::endl;

}


void Milieu::step( void )
{
   lastStepStats = RuntimeStepStats{};

   for ( std::vector<std::unique_ptr<Bestiole>>::iterator it = listeBestioles.begin() ; it != listeBestioles.end() ; ++it )
      ( *it )->action( *this );

   if ( collisionManager )
   {
      const CollisionStats collision = collisionManager->resolve( listeBestioles );
      lastStepStats.collisions = collision.collisions;
      lastStepStats.deathsByCollision = collision.deaths;
   }

   if ( lifecycleManager )
   {
      const LifecycleStats deaths = lifecycleManager->ageAndCull( listeBestioles );
      lastStepStats.deathsByAge = deaths.deathsByAge;
      if ( bestioleFactory )
      {
         const LifecycleStats births = lifecycleManager->processBirthsAndClones( listeBestioles, *bestioleFactory, width, height );
         lastStepStats.birthsByClone = births.birthsByClone;
         lastStepStats.birthsSpontaneous = births.birthsSpontaneous;
      }
   }

   cimg_forXY( *this, x, y ) fillC( x, y, 0, white[0], white[1], white[2] );
   for ( std::vector<std::unique_ptr<Bestiole>>::iterator it = listeBestioles.begin() ; it != listeBestioles.end() ; ++it )
   {
      ( *it )->draw( *this );

   } // for

}


void Milieu::configureRuntime( BestioleFactory & factory, const SimulationConfig & config )
{

   bestioleFactory = &factory;
   collisionManager = std::make_unique<CollisionManager>( config.getCollisionDistance(), config.getCollisionDeathProbability() );
   lifecycleManager = std::make_unique<LifecycleManager>( config.getMinLifespanTicks(), config.getMaxLifespanTicks() );

}


void Milieu::addMember( std::unique_ptr<Bestiole> b )
{

   if ( !b )
      return;

   if ( lifecycleManager )
      lifecycleManager->initializeNewborn( *b );

   b->initCoords( width, height );
   listeBestioles.push_back( std::move( b ) );

}


std::size_t Milieu::forceExternalBirths( int count )
{
   if ( !bestioleFactory || count <= 0 )
      return 0;

   std::size_t created = 0;
   for ( int i = 0; i < count; ++i )
   {
      std::unique_ptr<Bestiole> newborn = bestioleFactory->createFromConfig();
      if ( !newborn )
         continue;

      if ( lifecycleManager )
         lifecycleManager->initializeNewborn( *newborn );
      newborn->initCoords( width, height );
      listeBestioles.push_back( std::move( newborn ) );
      ++created;
   }

   externalEventStatsValue.birthsByExternalEvent += static_cast<int>( created );
   return created;
}


std::size_t Milieu::forceExternalDeaths( int count )
{
   if ( count <= 0 || listeBestioles.empty() )
      return 0;

   const std::size_t killCount = std::min<std::size_t>( static_cast<std::size_t>( count ), listeBestioles.size() );
   std::size_t killed = 0;

   for ( std::size_t i = 0; i < killCount; ++i )
   {
      if ( listeBestioles.empty() )
         break;

      std::uniform_int_distribution<std::size_t> pick( 0, listeBestioles.size() - 1 );
      const std::size_t idx = pick( rng );
      listeBestioles[ idx ]->kill();
      ++killed;
      listeBestioles.erase( listeBestioles.begin() + static_cast<std::ptrdiff_t>( idx ) );
   }

   externalEventStatsValue.deathsByExternalEvent += static_cast<int>( killed );
   return killed;
}


std::size_t Milieu::forceExternalBehaviorSwitches( int count )
{
   if ( !bestioleFactory || count <= 0 || listeBestioles.empty() )
      return 0;

   const std::size_t switchCount = std::min<std::size_t>( static_cast<std::size_t>( count ), listeBestioles.size() );
   std::size_t switched = 0;

   for ( std::size_t i = 0; i < switchCount; ++i )
   {
      std::uniform_int_distribution<std::size_t> pick( 0, listeBestioles.size() - 1 );
      const std::size_t idx = pick( rng );
      // External events reuse the factory so behavior changes stay consistent with configuration.
      listeBestioles[ idx ]->setComportement( bestioleFactory->createBehaviorFromConfig() );
      ++switched;
   }

   externalEventStatsValue.behaviorSwitchesByExternalEvent += static_cast<int>( switched );
   return switched;
}


void Milieu::triggerFoodPulse( void )
{
   ++externalEventStatsValue.foodPulses;
   forceExternalBirths( 3 );
}


void Milieu::triggerPollution( void )
{
   ++externalEventStatsValue.pollutionEvents;
   const int deaths = static_cast<int>( std::max<std::size_t>( 1, listeBestioles.size() / 4 ) );
   forceExternalDeaths( deaths );
}


void Milieu::triggerDisturbance( void )
{
   ++externalEventStatsValue.disturbanceEvents;
   const int changes = static_cast<int>( std::max<std::size_t>( 1, listeBestioles.size() / 3 ) );
   forceExternalBehaviorSwitches( changes );
}


int Milieu::nbVoisins( const Bestiole & b ) const
{

   int         nb = 0;


   for ( std::vector<std::unique_ptr<Bestiole>>::const_iterator it = listeBestioles.begin() ; it != listeBestioles.end() ; ++it )
      if ( !(b == **it) && b.jeTeVois(**it) )
         ++nb;

   return nb;

}


const Bestiole * Milieu::plusProcheVoisine( const Bestiole & b ) const
{

   const Bestiole * plusProche = nullptr;
   double distanceMin = 0.0;


   for ( std::vector<std::unique_ptr<Bestiole>>::const_iterator it = listeBestioles.begin() ; it != listeBestioles.end() ; ++it )
   {
      if ( b == **it || !b.jeTeVois( **it ) )
         continue;

      const double d = b.distance( **it );
      if ( plusProche == nullptr || d < distanceMin )
      {
         plusProche = ( *it ).get();
         distanceMin = d;
      }
   }

   return plusProche;

}
