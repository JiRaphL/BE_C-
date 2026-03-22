#include "sensors/EarSensor.h"

#include "Bestiole.h"

#include <algorithm>
#include <memory>


EarSensor::EarSensor( double maxDistanceValue, double detectionCapacityValue )
   : maxDistance( std::max( 0.0, maxDistanceValue ) ),
     detectionCapacity( std::max( 0.0, std::min( detectionCapacityValue, 1.0 ) ) )
{
}


std::unique_ptr<ISensor> EarSensor::clone( void ) const
{
   return std::make_unique<EarSensor>( *this );
}


bool EarSensor::detects( const Bestiole & observer, const Bestiole & target ) const
{
   return observer.distance( target ) <= maxDistance;
}
