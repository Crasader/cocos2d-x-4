#pragma once
#ifndef _GAME_TITLE_H_
#define _GAME_TITLE_H_

#include "cocos2d.h"
#include "TMainGame.h"
#include "TGameHelp.h"

class TGameTitle : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    virtual void onEnter();
	virtual void onExit();
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

    CREATE_FUNC(TGameTitle);
	cocos2d::CCPoint startPoint;
	static int type;
};

#endif  