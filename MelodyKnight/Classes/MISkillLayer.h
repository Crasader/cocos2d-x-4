#ifndef __MISKILL_LAYER_H__
#define __MISKILL_LAYER_H__

#include "cocos2d.h"

class MISkillLayer : public cocos2d::CCLayer
{
public:
	virtual void showButton();
	virtual void MTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void MTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void MTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
};

#endif  // __HELLOWORLD_SCENE_H__