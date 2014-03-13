#ifndef _MACTOR_H_
#define _MACTOR_H_
#include "cocos2d.h"

class MActor
{
public:
	bool isShow;
	bool isDestroy;
	virtual void update();
};

#endif

