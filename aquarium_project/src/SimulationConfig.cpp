#include "SimulationConfig.h"

#include <algorithm>


namespace
{
   double clampPercent( double value )
   {
      return std::max( 0.0, std::min( value, 100.0 ) );
   }
}


SimulationConfig::SimulationConfig( void )
   : initialPopulation( 20 ),
     gregariousPercent( 25.0 ),
     fearfulPercent( 25.0 ),
     kamikazePercent( 25.0 ),
     cautiousPercent( 25.0 ),
     spontaneousBirthRate( 0.01 ),
     cloningRate( 0.005 ),
     eyeSensorRate( 0.80 ),
     earSensorRate( 0.50 ),
     finAccessoryRate( 0.35 ),
     shellAccessoryRate( 0.20 ),
     camouflageAccessoryRate( 0.20 ),
     minLifespanTicks( 300 ),
     maxLifespanTicks( 1200 ),
     collisionDistance( 14.0 ),
     collisionDeathProbability( 0.20 ),
     delayMs( 30 ),
     maxTicks( -1 )
{
}


void SimulationConfig::setInitialPopulation( int value )
{
   initialPopulation = std::max( 0, value );
}


void SimulationConfig::setBehaviorPercentages( double gregarious, double fearful, double kamikaze, double cautious )
{
   const double g = clampPercent( gregarious );
   const double f = clampPercent( fearful );
   const double k = clampPercent( kamikaze );
   const double c = clampPercent( cautious );

   const double total = g + f + k + c;
   if ( total <= 0.0 )
   {
      gregariousPercent = 100.0;
      fearfulPercent = 0.0;
      kamikazePercent = 0.0;
      cautiousPercent = 0.0;
      return;
   }

   gregariousPercent = ( g / total ) * 100.0;
   fearfulPercent = ( f / total ) * 100.0;
   kamikazePercent = ( k / total ) * 100.0;
   cautiousPercent = ( c / total ) * 100.0;
}


void SimulationConfig::setSpontaneousBirthRate( double value )
{
   spontaneousBirthRate = std::max( 0.0, std::min( value, 1.0 ) );
}


void SimulationConfig::setCloningRate( double value )
{
   cloningRate = std::max( 0.0, std::min( value, 1.0 ) );
}


void SimulationConfig::setSensorRates( double eyeRate, double earRate )
{
   eyeSensorRate = std::max( 0.0, std::min( eyeRate, 1.0 ) );
   earSensorRate = std::max( 0.0, std::min( earRate, 1.0 ) );
}


void SimulationConfig::setAccessoryRates( double finRate, double shellRate, double camouflageRate )
{
   finAccessoryRate = std::max( 0.0, std::min( finRate, 1.0 ) );
   shellAccessoryRate = std::max( 0.0, std::min( shellRate, 1.0 ) );
   camouflageAccessoryRate = std::max( 0.0, std::min( camouflageRate, 1.0 ) );
}


void SimulationConfig::setLifespanTicks( int minTicks, int maxTicks )
{
   minLifespanTicks = std::max( 1, minTicks );
   maxLifespanTicks = std::max( minLifespanTicks, maxTicks );
}


void SimulationConfig::setCollisionParams( double distance, double deathProbability )
{
   collisionDistance = std::max( 0.0, distance );
   collisionDeathProbability = std::max( 0.0, std::min( deathProbability, 1.0 ) );
}


void SimulationConfig::setTiming( int delayMilliseconds, int maxTicksValue )
{
   delayMs = std::max( 0, delayMilliseconds );
   maxTicks = maxTicksValue;
}
