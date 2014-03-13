#ifndef _MSKILL_H_
#define _MSKILL_H_

#include"cocos2d.h"
#include"MActor.h"
#include "AnimatePacker.h"
#include "MSkillTarget.h"
#include "MEffect.h"
#include "MRole.h"

class MSkill : public cocos2d::CCSprite, public MActor
{
public:
	int id;
	static MSkill* create(MSkillTarget* target, int id, int skillType, int speedx = 0, int speedy = 0, int x = 0, int y = 0, int attackPoint = 0);
	virtual void update();
	virtual bool initSkill(MSkillTarget* target, int id, int skillType, int speedx, int speedy, int x, int y, int attackPoint);
	void addBuff(int type);
	void showEffect10000();
	void showEffect10001();
	void showEffect10002();
	void showEffect10003();
	void showEffect10005();
	void showEffect10007();
	void showEffect10050();
	void showEffect10051();
	MSkillTarget* target;
	int skillType;
	int mp;
	int distance;
	int time;
	int updateTime;
	long initTime;
	int speedx;
	int speedy;
	int x;
	int y;
	int attackPoint;
	const static int ROLE_TYPE = 0;
	const static int ENEMY_TYPE = 1;
};

#endif
