#include "sensors/EyeSensor.h"

#include "Bestiole.h"

#include <algorithm>
#include <cmath>
#include <memory>


namespace
{
   constexpr double PI = 3.14159265358979323846;

   double normalizeAngle( double angle )
   {
      while ( angle > PI ) angle -= 2.0 * PI;
      while ( angle < -PI ) angle += 2.0 * PI;
      return angle;
   }
}


EyeSensor::EyeSensor( double angularFieldRad, double maxDistanceValue, double detectionCapacityValue )
   : angularField( std::max( 0.0, std::min( angularFieldRad, 2.0 * PI ) ) ),
     maxDistance( std::max( 0.0, maxDistanceValue ) ),
     detectionCapacity( std::max( 0.0, std::min( detectionCapacityValue, 1.0 ) ) )
{
}


std::unique_ptr<ISensor> EyeSensor::clone( void ) const
{
   return std::make_unique<EyeSensor>( *this );
}


bool EyeSensor::detects( const Bestiole & observer, const Bestiole & target ) const
{
   const double d = observer.distance( target );
   if ( d > maxDistance ) return false;

   const double dx = static_cast<double>( target.getX() - observer.getX() );
   const double dy = static_cast<double>( target.getY() - observer.getY() );
   const double angleToTarget = std::atan2( -dy, dx );
   const double angleDiff = std::fabs( normalizeAngle( angleToTarget - observer.getOrientation() ) );
   return angleDiff <= ( angularField / 2.0 );
}
