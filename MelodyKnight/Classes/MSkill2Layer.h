#ifndef _MSKILL2_LAYER_H_
#define _MSKILL2_LAYER_H_

#include "cocos2d.h"
#include "MISkillLayer.h"
#include "GameManager.h"
#include<vector>

class MSkill2Layer : public MISkillLayer
{
public:
    static MSkill2Layer* create();
    virtual bool initLayer();  
	void initButton();
	void resetButton();
	void initSkillChain();
	void showSkillUp(float dely);
	void dealOutOfSkillUp(cocos2d::CCPoint point);
	void hideChain();
	void showChain(int count);
	virtual void onExit();
	virtual void MTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void MTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void MTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    // implement the "static node()" method manually
	cocos2d::CCSize size;
	cocos2d::CCArray* buttonArr;
	cocos2d::CCArray* skillChainArr;
	cocos2d::CCArray* skillArr;
	cocos2d::CCArray* skillIdArr;
	int a_totalSkillId[3][4];
	bool isSkillStart;
	bool isHideSkillUp;
	static int skillLevel;
	cocos2d::CCParticleFlower* particle;
};

#endif  // __HELLOWORLD_SCENE_H__