#ifndef _ISENSOR_H_
#define _ISENSOR_H_

#include <memory>

class Bestiole;


class ISensor
{
public:
   virtual ~ISensor( void ) = default;

   virtual std::unique_ptr<ISensor> clone( void ) const = 0;
   virtual bool detects( const Bestiole & observer, const Bestiole & target ) const = 0;
   virtual double getDetectionCapacity( void ) const = 0;
};


#endif
