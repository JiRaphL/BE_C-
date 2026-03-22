#ifndef _BESTIOLE_FACTORY_H_
#define _BESTIOLE_FACTORY_H_

#include "Bestiole.h"
#include "SimulationConfig.h"
#include "behavior/IBehavior.h"

#include <memory>
#include <random>
#include <vector>


enum class BehaviorKind
{
   Gregarious,
   Fearful,
   Kamikaze,
   Cautious
};


class BestioleFactory
{
private:
   const SimulationConfig & config;
   mutable std::mt19937 rng;

private:
   BehaviorKind pickBehaviorKind( void );
   std::unique_ptr<IBehavior> createBehavior( BehaviorKind kind ) const;
   std::unique_ptr<Bestiole> createBestiole( BehaviorKind kind );
   void equipSensors( Bestiole & bestiole );
   void equipAccessories( Bestiole & bestiole );

public:
   explicit BestioleFactory( const SimulationConfig & configuration );

   std::unique_ptr<Bestiole> createFromConfig( void );
   std::unique_ptr<IBehavior> createBehaviorFromConfig( void );
   std::vector<std::unique_ptr<Bestiole>> createInitialPopulation( void );

   std::unique_ptr<Bestiole> tryCreateSpontaneousBirth( void );
   std::unique_ptr<Bestiole> tryCreateClone( const Bestiole & parent );
};


#endif
