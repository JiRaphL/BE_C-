#ifndef _CAMOUFLAGE_ACCESSORY_H_
#define _CAMOUFLAGE_ACCESSORY_H_

#include "accessories/IAccessory.h"


class CamouflageAccessory : public IAccessory
{
private:
   double camouflageCoeff;

public:
   explicit CamouflageAccessory( double camouflageCoeffValue );

   std::unique_ptr<IAccessory> clone( void ) const override;
   double camouflageLevel( void ) const override { return camouflageCoeff; }
};


#endif
