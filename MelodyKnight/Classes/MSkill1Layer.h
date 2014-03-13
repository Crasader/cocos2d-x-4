#ifndef __MSKILL1_LAYER_H__
#define __MSKILL1_LAYER_H__

#include "cocos2d.h"
#include "Utils.h"
#include "SkillManager.h"
#include "MISkillLayer.h"
#include "MSkillTarget.h"
#include "MSkill.h"
#include "GameManager.h"

class MSkill1Layer : public MISkillLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	static MSkill1Layer* create();
    virtual bool initLayer();  
	virtual void onExit();
	void skilldownCallback();
	void skillupCallback();
	void initButton();
	void initBar();
	void showCombo(float dt);
	void showComboSuccess(cocos2d::CCSprite *pSender);
	void resetStar(CCNode* pSender);
	void missStar(CCNode* pSender);
	void initDescript();
	void initSkill();
	virtual void showButton();
	virtual void MTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void MTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void MTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	MSkillTarget* target;
	cocos2d::CCSprite* comboBar;
	cocos2d::CCSprite* skillButton;
	cocos2d::CCSprite* comboSuccess;
	cocos2d::CCSprite* comboFail;
	bool isLongTouch;
	long lastTime;
	cocos2d::CCSize size;
	cocos2d::CCArray* starsArray;
	cocos2d::CCArray* skillArray;
	std::vector<std::string> descript;
	int descriptIndex;
	int dely;
	const static int SKILL_ID = 0; 
    // implement the "static node()" method manually
};

#endif  // __HELLOWORLD_SCENE_H__