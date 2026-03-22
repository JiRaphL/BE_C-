#ifndef _LIFECYCLE_MANAGER_H_
#define _LIFECYCLE_MANAGER_H_

#include <memory>
#include <random>
#include <vector>

class Bestiole;
class BestioleFactory;

struct LifecycleStats
{
   int deathsByAge = 0;
   int birthsByClone = 0;
   int birthsSpontaneous = 0;
};


class LifecycleManager
{
private:
   int minLifespan;
   int maxLifespan;
   mutable std::mt19937 rng;

private:
   int sampleLifespan( void ) const;

public:
   LifecycleManager( int minLifespanValue, int maxLifespanValue );

   void initializeNewborn( Bestiole & bestiole ) const;
   LifecycleStats ageAndCull( std::vector<std::unique_ptr<Bestiole>> & population ) const;
   LifecycleStats processBirthsAndClones( std::vector<std::unique_ptr<Bestiole>> & population,
                                          BestioleFactory & factory,
                                          int width,
                                          int height ) const;
};


#endif
