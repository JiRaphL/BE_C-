#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

#include <memory>
#include <random>
#include <vector>

class Bestiole;

struct CollisionStats
{
   int collisions = 0;
   int deaths = 0;
};


class CollisionManager
{
private:
   double collisionDistance;
   double baseDeathProbability;
   mutable std::mt19937 rng;

public:
   CollisionManager( double collisionDistanceValue, double baseDeathProbabilityValue );

   CollisionStats resolve( std::vector<std::unique_ptr<Bestiole>> & population ) const;
};


#endif
