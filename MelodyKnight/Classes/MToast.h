#ifndef __TOAST_H__
#define __TOAST_H__

#include "cocos2d.h"

class MToast : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    CREATE_FUNC(MToast);
	void setToastString(const char* text);
	void setToastBack(char *back);
	void setToastHead(char *head);
	void MTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void MTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void MTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	cocos2d::CCSprite *toastBack;
	cocos2d::CCSprite *toastHead;
	cocos2d::CCLabelTTF *toastText;
	static bool isShowToast;
	static const int TAG = 66;
};

#endif 