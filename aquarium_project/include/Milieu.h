#ifndef _MILIEU_H_
#define _MILIEU_H_


#include "UImg.h"
#include "Bestiole.h"

#include <iostream>
#include <cstddef>
#include <memory>
#include <random>
#include <vector>

class BestioleFactory;
class CollisionManager;
class LifecycleManager;
class SimulationConfig;

struct RuntimeStepStats
{
   int collisions = 0;
   int deathsByCollision = 0;
   int deathsByAge = 0;
   int birthsByClone = 0;
   int birthsSpontaneous = 0;
};

struct ExternalEventStats
{
   int birthsByExternalEvent = 0;
   int deathsByExternalEvent = 0;
   int behaviorSwitchesByExternalEvent = 0;
   int foodPulses = 0;
   int pollutionEvents = 0;
   int disturbanceEvents = 0;
};


class Milieu : public UImg
{

private :
   static const T          white[];

   int                     width, height;
   std::vector<std::unique_ptr<Bestiole>>   listeBestioles;
   std::unique_ptr<CollisionManager> collisionManager;
   std::unique_ptr<LifecycleManager> lifecycleManager;
   BestioleFactory * bestioleFactory;
   RuntimeStepStats lastStepStats;
   ExternalEventStats externalEventStatsValue;
   mutable std::mt19937 rng;

public :
   Milieu( int _width, int _height );
   ~Milieu( void );

   int getWidth( void ) const { return width; };
   int getHeight( void ) const { return height; };

   void step( void );
   void configureRuntime( BestioleFactory & factory, const SimulationConfig & config );

   void addMember( std::unique_ptr<Bestiole> b );
   std::size_t forceExternalBirths( int count );
   std::size_t forceExternalDeaths( int count );
   std::size_t forceExternalBehaviorSwitches( int count );
   void triggerFoodPulse( void );
   void triggerPollution( void );
   void triggerDisturbance( void );
   std::size_t populationSize( void ) const { return listeBestioles.size(); }
   const std::vector<std::unique_ptr<Bestiole>> & population( void ) const { return listeBestioles; }
   const RuntimeStepStats & stepStats( void ) const { return lastStepStats; }
   const ExternalEventStats & externalEventStats( void ) const { return externalEventStatsValue; }
   int nbVoisins( const Bestiole & b ) const;
   const Bestiole * plusProcheVoisine( const Bestiole & b ) const;

};


#endif
