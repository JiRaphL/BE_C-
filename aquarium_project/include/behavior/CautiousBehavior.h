#ifndef _CAUTIOUS_BEHAVIOR_H_
#define _CAUTIOUS_BEHAVIOR_H_

#include "behavior/IBehavior.h"


class CautiousBehavior : public IBehavior
{
public:
   std::unique_ptr<IBehavior> clone( void ) const override;
   void apply( Bestiole & bestiole, Milieu & milieu ) const override;
   const char * name( void ) const override;
};


#endif
