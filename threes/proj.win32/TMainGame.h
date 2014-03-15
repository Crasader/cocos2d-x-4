#ifndef _MAIN_GAME_H_
#define _MAIN_GAME_H_

#include "cocos2d.h"
#include "TGameManager.h"
#include "TGameTitle.h"
#include "SimpleAudioEngine.h"

class TMainGame : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
	void initTable();
	void initMenu();
	void initLabel();
	cocos2d::CCSprite* getColor(int i);
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
	void updateGame(float t);
    virtual void onEnter();
	virtual void onExit();
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void menuCallback(CCObject* pSender);
    CREATE_FUNC(TMainGame);
	cocos2d::CCSize size;
	cocos2d::CCPoint startPoint;
	cocos2d::CCLayer* ballLayer;
	cocos2d::CCLabelTTF *bestPoint;
	cocos2d::CCLabelTTF *curPoint;
	int stauts;
	int bestCount;
	const static int vertical = 0;
	const static int standard = 1;
};

#endif  // __HELLOWORLD_SCENE_H__