#ifndef _STATISTICS_COLLECTOR_H_
#define _STATISTICS_COLLECTOR_H_

#include <cstddef>
#include <map>
#include <string>
#include <vector>

class Milieu;


struct PopulationSnapshot
{
   int tick = 0;
   std::size_t population = 0;
   double averageAge = 0.0;
   int collisions = 0;
   int deathsByCollision = 0;
   int deathsByAge = 0;
   int birthsByClone = 0;
   int birthsSpontaneous = 0;
   std::map<std::string, std::size_t> behaviorDistribution;
};


class StatisticsCollector
{
private:
   std::vector<PopulationSnapshot> snapshots;

public:
   void record( int tick, const Milieu & milieu );
   bool exportCsv( const std::string & filePath ) const;
   bool exportJson( const std::string & filePath ) const;
   const std::vector<PopulationSnapshot> & history( void ) const { return snapshots; }
   const PopulationSnapshot * last( void ) const;
};


#endif
