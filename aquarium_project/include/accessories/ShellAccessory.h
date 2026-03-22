#ifndef _SHELL_ACCESSORY_H_
#define _SHELL_ACCESSORY_H_

#include "accessories/IAccessory.h"


class ShellAccessory : public IAccessory
{
private:
   double speedPenalty;
   double resistanceCoeff;

public:
   ShellAccessory( double speedPenaltyValue, double resistanceCoeffValue );

   std::unique_ptr<IAccessory> clone( void ) const override;
   double speedDivisor( void ) const override { return speedPenalty; }
   double deathResistanceMultiplier( void ) const override { return resistanceCoeff; }
};


#endif
