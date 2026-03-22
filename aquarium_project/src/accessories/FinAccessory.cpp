#include "accessories/FinAccessory.h"

#include <algorithm>
#include <memory>


FinAccessory::FinAccessory( double speedCoeffValue )
   : speedCoeff( std::max( 1.0, speedCoeffValue ) )
{
}


std::unique_ptr<IAccessory> FinAccessory::clone( void ) const
{
   return std::make_unique<FinAccessory>( *this );
}
