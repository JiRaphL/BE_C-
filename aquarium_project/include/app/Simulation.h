#ifndef _SIMULATION_H_
#define _SIMULATION_H_

class Milieu;
class Renderer;
class StatisticsCollector;


class Simulation
{
private:
   Milieu & milieuRef;
   Renderer & rendererRef;
   StatisticsCollector & statsRef;
   int delayMs;
   int tick;
   int maxTicks;

public:
   Simulation( Milieu & milieu, Renderer & renderer, StatisticsCollector & collector, int delayMilliseconds, int maxTicksValue = -1 );

   void run( void );
   int currentTick( void ) const { return tick; }
};


#endif
