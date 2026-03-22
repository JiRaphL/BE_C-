#ifndef _EYE_SENSOR_H_
#define _EYE_SENSOR_H_

#include "sensors/ISensor.h"


class EyeSensor : public ISensor
{
private:
   double angularField;
   double maxDistance;
   double detectionCapacity;

public:
   EyeSensor( double angularFieldRad, double maxDistanceValue, double detectionCapacityValue );

   std::unique_ptr<ISensor> clone( void ) const override;
   bool detects( const Bestiole & observer, const Bestiole & target ) const override;
   double getDetectionCapacity( void ) const override { return detectionCapacity; }
};


#endif
