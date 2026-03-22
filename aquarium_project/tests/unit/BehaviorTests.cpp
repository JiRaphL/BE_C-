#include "framework/TestFramework.h"

#include "Bestiole.h"
#include "Milieu.h"
#include "behavior/CautiousBehavior.h"
#include "behavior/FearfulBehavior.h"
#include "behavior/GregariousBehavior.h"
#include "behavior/KamikazeBehavior.h"
#include "sensors/EarSensor.h"

#include <cmath>
#include <memory>


namespace
{
   double normalizeAngle( double angle )
   {
      constexpr double pi = 3.14159265358979323846;
      while ( angle > pi ) angle -= 2.0 * pi;
      while ( angle < -pi ) angle += 2.0 * pi;
      return angle;
   }
}


TEST_CASE( gregarious_behavior_turns_toward_neighbor )
{
   Milieu milieu( 200, 200 );
   auto actor = std::make_unique<Bestiole>( std::make_unique<GregariousBehavior>() );
   auto neighbor = std::make_unique<Bestiole>( std::make_unique<GregariousBehavior>() );

   Bestiole * actorPtr = actor.get();
   Bestiole * neighborPtr = neighbor.get();
   milieu.addMember( std::move( actor ) );
   milieu.addMember( std::move( neighbor ) );

   actorPtr->setCoords( 100, 100 );
   neighborPtr->setCoords( 120, 110 );
   actorPtr->setOrientation( 0.0 );
   actorPtr->addSensor( std::make_unique<EarSensor>( 100.0, 1.0 ) );

   GregariousBehavior behavior;
   behavior.apply( *actorPtr, milieu );

   EXPECT_TRUE( actorPtr->getOrientation() < 0.0 );
}


TEST_CASE( fearful_behavior_accelerates_when_crowded )
{
   Milieu milieu( 200, 200 );
   auto actor = std::make_unique<Bestiole>( std::make_unique<FearfulBehavior>() );
   Bestiole * actorPtr = actor.get();
   milieu.addMember( std::move( actor ) );

   for ( int i = 0; i < 3; ++i )
   {
      auto neighbor = std::make_unique<Bestiole>( std::make_unique<GregariousBehavior>() );
      Bestiole * neighborPtr = neighbor.get();
      milieu.addMember( std::move( neighbor ) );
      neighborPtr->setCoords( 105 + i, 100 + i );
   }

   actorPtr->setCoords( 100, 100 );
   actorPtr->setOrientation( 0.0 );
   const double baseSpeed = actorPtr->getVitesseBase();

   FearfulBehavior behavior;
   behavior.apply( *actorPtr, milieu );

   EXPECT_TRUE( actorPtr->getVitesse() >= baseSpeed );
   EXPECT_NEAR( std::fabs( normalizeAngle( actorPtr->getOrientation() ) ), 3.14159265358979323846, 1e-6 );
}


TEST_CASE( kamikaze_behavior_targets_closest_neighbor )
{
   Milieu milieu( 200, 200 );
   auto actor = std::make_unique<Bestiole>( std::make_unique<KamikazeBehavior>() );
   auto neighbor = std::make_unique<Bestiole>( std::make_unique<GregariousBehavior>() );

   Bestiole * actorPtr = actor.get();
   Bestiole * neighborPtr = neighbor.get();
   milieu.addMember( std::move( actor ) );
   milieu.addMember( std::move( neighbor ) );

   actorPtr->setCoords( 100, 100 );
   neighborPtr->setCoords( 120, 90 );
   actorPtr->setOrientation( 0.0 );
   actorPtr->addSensor( std::make_unique<EarSensor>( 100.0, 1.0 ) );
   const double baseSpeed = actorPtr->getVitesseBase();

   KamikazeBehavior behavior;
   behavior.apply( *actorPtr, milieu );

   EXPECT_TRUE( actorPtr->getOrientation() > 0.0 );
   EXPECT_TRUE( actorPtr->getVitesse() >= baseSpeed );
}


TEST_CASE( cautious_behavior_avoids_close_neighbor )
{
   Milieu milieu( 200, 200 );
   auto actor = std::make_unique<Bestiole>( std::make_unique<CautiousBehavior>() );
   auto neighbor = std::make_unique<Bestiole>( std::make_unique<GregariousBehavior>() );

   Bestiole * actorPtr = actor.get();
   Bestiole * neighborPtr = neighbor.get();
   milieu.addMember( std::move( actor ) );
   milieu.addMember( std::move( neighbor ) );

   actorPtr->setCoords( 100, 100 );
   neighborPtr->setCoords( 120, 90 );
   actorPtr->setOrientation( 0.0 );
   actorPtr->addSensor( std::make_unique<EarSensor>( 100.0, 1.0 ) );
   const double baseSpeed = actorPtr->getVitesseBase();

   CautiousBehavior behavior;
   behavior.apply( *actorPtr, milieu );

   EXPECT_TRUE( actorPtr->getOrientation() < 0.0 );
   EXPECT_TRUE( actorPtr->getVitesse() <= baseSpeed );
}
