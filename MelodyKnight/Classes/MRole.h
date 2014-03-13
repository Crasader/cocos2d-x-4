#ifndef _MROLE_H_
#define _MROLE_H_

#include"cocos2d.h"
#include"MActor.h"
#include"Utils.h"
#include "MSkillTarget.h"
#include "AnimatePacker.h"

class MRole : public cocos2d::CCSprite, public MActor,public MSkillTarget
{
public:
	static MRole* create();
	virtual bool initRole();
	virtual void showTargetEffet(int skillId);
	virtual void dealDamager(char* path, int attack);
	virtual void update();
	float getHp();
	float getMp();
	void showRoleEffect10000();
	void showRoleEffect10002();
	void showRoleEffect10004();
	void showSkillEffect(int type);
	int getEquipUp(int type);
	void showAni(float velocityX, float velocityY);
	void showDamagerNum(int num);
	void dealManager();
	static int getCurAttack();
	static int getCurDef();
	void resetNum(CCNode* pSender, void* data);
	cocos2d::CCSprite* roleEffect;
	long moveLatstTime;
	long lastHitTime;
	int roleAniType; 
	int roleMoveType;
	const static int SKILL_ID = 0; 
	cocos2d::CCLabelTTF* damagerNum;
	static long buffTime[6];
	static int attackPoint;
	static int defPoint;
	bool isFreeCost;
	static int equipIds[4];
};

#endif