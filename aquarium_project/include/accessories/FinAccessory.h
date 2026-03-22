#ifndef _FIN_ACCESSORY_H_
#define _FIN_ACCESSORY_H_

#include "accessories/IAccessory.h"


class FinAccessory : public IAccessory
{
private:
   double speedCoeff;

public:
   explicit FinAccessory( double speedCoeffValue );

   std::unique_ptr<IAccessory> clone( void ) const override;
   double speedMultiplier( void ) const override { return speedCoeff; }
};


#endif
