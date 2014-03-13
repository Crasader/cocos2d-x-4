#ifndef __EQUIP_SELECT_LAYER_H__
#define __EQUIP_SELECT_LAYER_H__

#include "cocos2d.h"
#include "GameManager.h"
#include "MToast.h"

class MEquipSelectLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    void initSkillButton();
	void initBack();
	void initMenu();
	void initText();
	void setEquip();
	void buyEquip();
	void descripEquip();
	void buttonSelect(CCObject* sender);
	cocos2d::CCArray *buttonArr;
	cocos2d::CCArray *frameArr;
	cocos2d::CCLabelTTF* moneyCount;
	cocos2d::CCSprite *buyEquipButton;
	cocos2d::CCSprite *setEquipButton;
	cocos2d::CCSprite *descripEquipButton;
    virtual void onExit();
	virtual void onEnter();
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	CREATE_FUNC(MEquipSelectLayer);
	cocos2d::CCSize winSize;
	int oldIndex;
};

#endif 