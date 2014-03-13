#ifndef _MSKILL_TARGET_H_
#define _MSKILL_TARGET_H_
#include "cocos2d.h"

class MSkillTarget
{
public:
	virtual void showTargetEffet(int skillId);
	virtual void dealDamager(char* path);
	int maxHp;
	int curHp;
	int maxMp;
	int curMp;
};

#endif


