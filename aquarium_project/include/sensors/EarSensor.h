#ifndef _EAR_SENSOR_H_
#define _EAR_SENSOR_H_

#include "sensors/ISensor.h"


class EarSensor : public ISensor
{
private:
   double maxDistance;
   double detectionCapacity;

public:
   EarSensor( double maxDistanceValue, double detectionCapacityValue );

   std::unique_ptr<ISensor> clone( void ) const override;
   bool detects( const Bestiole & observer, const Bestiole & target ) const override;
   double getDetectionCapacity( void ) const override { return detectionCapacity; }
};


#endif
