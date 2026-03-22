#ifndef _TEST_DOMAIN_MODELS_H_
#define _TEST_DOMAIN_MODELS_H_

#include <algorithm>
#include <cmath>


namespace testsupport
{
   constexpr double PI = 3.14159265358979323846;

   inline double normalizeAngle( double angle )
   {
      while ( angle > PI ) angle -= 2.0 * PI;
      while ( angle < -PI ) angle += 2.0 * PI;
      return angle;
   }

   struct AgentState
   {
      double x = 0.0;
      double y = 0.0;
      double orientation = 0.0;
      double speed = 1.0;
      double camouflage = 0.0;
      double deathResistance = 1.0;
      bool alive = true;
      int age = 0;
      int maxAge = 100;
   };

   inline double distance( const AgentState & a, const AgentState & b )
   {
      const double dx = a.x - b.x;
      const double dy = a.y - b.y;
      return std::sqrt( dx * dx + dy * dy );
   }

   struct EyeSensor
   {
      double fieldOfView = PI / 2.0;
      double maxDistance = 50.0;
      double detection = 1.0;

      bool detects( const AgentState & observer, const AgentState & target ) const
      {
         if ( distance( observer, target ) > maxDistance ) return false;
         const double angleToTarget = std::atan2( target.y - observer.y, target.x - observer.x );
         const double diff = std::fabs( normalizeAngle( angleToTarget - observer.orientation ) );
         return diff <= fieldOfView * 0.5 && detection > target.camouflage;
      }
   };

   struct EarSensor
   {
      double maxDistance = 50.0;
      double detection = 1.0;

      bool detects( const AgentState & observer, const AgentState & target ) const
      {
         return distance( observer, target ) <= maxDistance && detection > target.camouflage;
      }
   };

   struct FinAccessory
   {
      double speedMultiplier = 1.2;

      void apply( AgentState & agent ) const
      {
         agent.speed *= speedMultiplier;
      }
   };

   struct ShellAccessory
   {
      double speedDivisor = 1.2;
      double resistanceMultiplier = 2.0;

      void apply( AgentState & agent ) const
      {
         agent.speed /= std::max( 1.0, speedDivisor );
         agent.deathResistance *= resistanceMultiplier;
      }
   };

   struct CamouflageAccessory
   {
      double camouflageValue = 0.4;

      void apply( AgentState & agent ) const
      {
         agent.camouflage = std::max( 0.0, std::min( camouflageValue, 1.0 ) );
      }
   };

   class CollisionManagerModel
   {
   public:
      static void resolve( AgentState & a, AgentState & b, double collisionDistance, double baseDeathProbability, double randomA, double randomB )
      {
         if ( !a.alive || !b.alive ) return;
         if ( distance( a, b ) > collisionDistance ) return;

         const double deathProbA = std::max( 0.0, std::min( baseDeathProbability / std::max( 1.0, a.deathResistance ), 1.0 ) );
         const double deathProbB = std::max( 0.0, std::min( baseDeathProbability / std::max( 1.0, b.deathResistance ), 1.0 ) );

         if ( randomA < deathProbA ) a.alive = false;
         else a.orientation = normalizeAngle( a.orientation + PI );

         if ( randomB < deathProbB ) b.alive = false;
         else b.orientation = normalizeAngle( b.orientation + PI );
      }
   };

   class LifecycleManagerModel
   {
   private:
      double spontaneousBirthRate;
      double cloningRate;

   public:
      LifecycleManagerModel( double birthRate, double cloneRate )
         : spontaneousBirthRate( std::max( 0.0, std::min( birthRate, 1.0 ) ) ),
           cloningRate( std::max( 0.0, std::min( cloneRate, 1.0 ) ) )
      {
      }

      void ageOneTick( AgentState & agent ) const
      {
         if ( !agent.alive ) return;
         ++agent.age;
         if ( agent.age >= agent.maxAge ) agent.alive = false;
      }

      bool shouldSpawn( double random01 ) const
      {
         return random01 < spontaneousBirthRate;
      }

      bool shouldClone( double random01 ) const
      {
         return random01 < cloningRate;
      }
   };
}


#endif
