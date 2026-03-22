#include "app/StatisticsCollector.h"

#include "Bestiole.h"
#include "Milieu.h"
#include "behavior/IBehavior.h"

#include <fstream>
#include <numeric>
#include <sstream>


void StatisticsCollector::record( int tick, const Milieu & milieu )
{
   PopulationSnapshot snapshot;
   snapshot.tick = tick;
   snapshot.population = milieu.populationSize();
   snapshot.collisions = milieu.stepStats().collisions;
   snapshot.deathsByCollision = milieu.stepStats().deathsByCollision;
   snapshot.deathsByAge = milieu.stepStats().deathsByAge;
   snapshot.birthsByClone = milieu.stepStats().birthsByClone;
   snapshot.birthsSpontaneous = milieu.stepStats().birthsSpontaneous;

   double ageSum = 0.0;
   for ( std::vector<std::unique_ptr<Bestiole>>::const_iterator it = milieu.population().begin();
         it != milieu.population().end();
         ++it )
   {
      ageSum += static_cast<double>( ( *it )->getAge() );
      const IBehavior * behavior = ( *it )->getComportement();
      const std::string name = behavior ? behavior->name() : "Unknown";
      snapshot.behaviorDistribution[ name ] += 1;
   }

   if ( snapshot.population > 0 )
      snapshot.averageAge = ageSum / static_cast<double>( snapshot.population );

   snapshots.push_back( snapshot );
}


bool StatisticsCollector::exportCsv( const std::string & filePath ) const
{
   std::ofstream out( filePath );
   if ( !out.is_open() )
      return false;

   out << "tick,population,average_age,collisions,deaths_collision,deaths_age,births_clone,births_spontaneous\n";
   for ( std::vector<PopulationSnapshot>::const_iterator it = snapshots.begin(); it != snapshots.end(); ++it )
   {
      out << it->tick << ","
          << it->population << ","
          << it->averageAge << ","
          << it->collisions << ","
          << it->deathsByCollision << ","
          << it->deathsByAge << ","
          << it->birthsByClone << ","
          << it->birthsSpontaneous << "\n";
   }

   return true;
}


bool StatisticsCollector::exportJson( const std::string & filePath ) const
{
   std::ofstream out( filePath );
   if ( !out.is_open() )
      return false;

   out << "[\n";
   for ( std::size_t i = 0; i < snapshots.size(); ++i )
   {
      const PopulationSnapshot & s = snapshots[ i ];
      out << "  {\n";
      out << "    \"tick\": " << s.tick << ",\n";
      out << "    \"population\": " << s.population << ",\n";
      out << "    \"averageAge\": " << s.averageAge << ",\n";
      out << "    \"collisions\": " << s.collisions << ",\n";
      out << "    \"deathsByCollision\": " << s.deathsByCollision << ",\n";
      out << "    \"deathsByAge\": " << s.deathsByAge << ",\n";
      out << "    \"birthsByClone\": " << s.birthsByClone << ",\n";
      out << "    \"birthsSpontaneous\": " << s.birthsSpontaneous << ",\n";
      out << "    \"behaviorDistribution\": {";

      std::size_t k = 0;
      for ( std::map<std::string, std::size_t>::const_iterator it = s.behaviorDistribution.begin();
            it != s.behaviorDistribution.end();
            ++it, ++k )
      {
         out << "\"" << it->first << "\": " << it->second;
         if ( k + 1 < s.behaviorDistribution.size() ) out << ", ";
      }
      out << "}\n";
      out << "  }";
      if ( i + 1 < snapshots.size() ) out << ",";
      out << "\n";
   }
   out << "]\n";

   return true;
}


const PopulationSnapshot * StatisticsCollector::last( void ) const
{
   if ( snapshots.empty() )
      return nullptr;
   return &snapshots.back();
}
