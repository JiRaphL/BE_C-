#include "accessories/ShellAccessory.h"

#include <algorithm>
#include <memory>


ShellAccessory::ShellAccessory( double speedPenaltyValue, double resistanceCoeffValue )
   : speedPenalty( std::max( 1.0, speedPenaltyValue ) ),
     resistanceCoeff( std::max( 1.0, resistanceCoeffValue ) )
{
}


std::unique_ptr<IAccessory> ShellAccessory::clone( void ) const
{
   return std::make_unique<ShellAccessory>( *this );
}
