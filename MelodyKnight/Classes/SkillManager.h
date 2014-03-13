#ifndef __SKILL_MANAGER_H__
#define __SKILL_MANAGER_H__
#include "Utils.h"
#include "cocos2d.h"
#include "MISkillLayer.h"
#include "MSkill1Layer.h"
#include "MSkill2Layer.h"

class SkillManager
{
public:
	static const int BIG_SKILL_STATU = 1; 
	static const int CONTROL_SKILL_STATU = 2; 
	static int skillStatu;
	static long currentTime;
	static long maxTime;
	static int comboCount;
	static int getSkillPercentage(int height);
	static SkillManager* sharedSkillManager();
	MISkillLayer* getBigSkillLayer();
	MISkillLayer* getSkillControlLayer();
	void showButton();
	void MTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void MTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void MTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	MISkillLayer* skillbigLayer;
	MISkillLayer* skillcontrolLayer;
};

#endif  // __HELLOWORLD_SCENE_H__

