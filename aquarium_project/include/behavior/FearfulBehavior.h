#ifndef _FEARFUL_BEHAVIOR_H_
#define _FEARFUL_BEHAVIOR_H_

#include "behavior/IBehavior.h"


class FearfulBehavior : public IBehavior
{
public:
   std::unique_ptr<IBehavior> clone( void ) const override;
   void apply( Bestiole & bestiole, Milieu & milieu ) const override;
   const char * name( void ) const override;
};


#endif
