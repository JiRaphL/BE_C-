#ifndef _IBEHAVIOR_H_
#define _IBEHAVIOR_H_

#include <memory>

class Bestiole;
class Milieu;


class IBehavior
{
public:
   virtual ~IBehavior( void ) = default;

   virtual std::unique_ptr<IBehavior> clone( void ) const = 0;
   virtual void apply( Bestiole & bestiole, Milieu & milieu ) const = 0;
   virtual const char * name( void ) const = 0;
};


#endif
