#ifndef _SIMULATION_CONFIG_H_
#define _SIMULATION_CONFIG_H_


class SimulationConfig
{
private:
   int initialPopulation;
   double gregariousPercent;
   double fearfulPercent;
   double kamikazePercent;
   double cautiousPercent;
   double spontaneousBirthRate;
   double cloningRate;
   double eyeSensorRate;
   double earSensorRate;
   double finAccessoryRate;
   double shellAccessoryRate;
   double camouflageAccessoryRate;
   int minLifespanTicks;
   int maxLifespanTicks;
   double collisionDistance;
   double collisionDeathProbability;
   int delayMs;
   int maxTicks;

public:
   SimulationConfig( void );

   int getInitialPopulation( void ) const { return initialPopulation; }
   double getGregariousPercent( void ) const { return gregariousPercent; }
   double getFearfulPercent( void ) const { return fearfulPercent; }
   double getKamikazePercent( void ) const { return kamikazePercent; }
   double getCautiousPercent( void ) const { return cautiousPercent; }
   double getSpontaneousBirthRate( void ) const { return spontaneousBirthRate; }
   double getCloningRate( void ) const { return cloningRate; }
   double getEyeSensorRate( void ) const { return eyeSensorRate; }
   double getEarSensorRate( void ) const { return earSensorRate; }
   double getFinAccessoryRate( void ) const { return finAccessoryRate; }
   double getShellAccessoryRate( void ) const { return shellAccessoryRate; }
   double getCamouflageAccessoryRate( void ) const { return camouflageAccessoryRate; }
   int getMinLifespanTicks( void ) const { return minLifespanTicks; }
   int getMaxLifespanTicks( void ) const { return maxLifespanTicks; }
   double getCollisionDistance( void ) const { return collisionDistance; }
   double getCollisionDeathProbability( void ) const { return collisionDeathProbability; }
   int getDelayMs( void ) const { return delayMs; }
   int getMaxTicks( void ) const { return maxTicks; }

   void setInitialPopulation( int value );
   void setBehaviorPercentages( double gregarious, double fearful, double kamikaze, double cautious );
   void setSpontaneousBirthRate( double value );
   void setCloningRate( double value );
   void setSensorRates( double eyeRate, double earRate );
   void setAccessoryRates( double finRate, double shellRate, double camouflageRate );
   void setLifespanTicks( int minTicks, int maxTicks );
   void setCollisionParams( double distance, double deathProbability );
   void setTiming( int delayMilliseconds, int maxTicksValue );
};


#endif
