#include "accessories/CamouflageAccessory.h"

#include <algorithm>
#include <memory>


CamouflageAccessory::CamouflageAccessory( double camouflageCoeffValue )
   : camouflageCoeff( std::max( 0.0, std::min( camouflageCoeffValue, 1.0 ) ) )
{
}


std::unique_ptr<IAccessory> CamouflageAccessory::clone( void ) const
{
   return std::make_unique<CamouflageAccessory>( *this );
}
