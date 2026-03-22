#ifndef _IACCESSORY_H_
#define _IACCESSORY_H_

#include <memory>


class IAccessory
{
public:
   virtual ~IAccessory( void ) = default;

   virtual std::unique_ptr<IAccessory> clone( void ) const = 0;

   virtual double speedMultiplier( void ) const { return 1.0; }
   virtual double speedDivisor( void ) const { return 1.0; }
   virtual double deathResistanceMultiplier( void ) const { return 1.0; }
   virtual double camouflageLevel( void ) const { return 0.0; }
};


#endif
