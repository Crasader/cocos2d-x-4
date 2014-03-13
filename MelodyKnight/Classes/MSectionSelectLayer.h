#ifndef __M_SECTION_SELECT_H__
#define __M_SECTION_SELECT_H__

#include "cocos2d.h"
#include "MmainScene.h"

class MSectionSelectLayer : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
	void initMenu();
	void enterSection(CCObject* sender);
    CREATE_FUNC(MSectionSelectLayer);
	void setTitleString(const char* text);
	void setBack(char *back);
	void MTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void MTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void MTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	cocos2d::CCSprite *sectionBack;
	cocos2d::CCLabelTTF *sectionTitle;
	static bool isShowToast;
	static const int TAG = 67;
};

#endif 