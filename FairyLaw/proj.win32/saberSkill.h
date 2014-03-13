#ifndef _SABER_SKILL_H_
#define _SABER_SKILL_H_

#include"FairySkill.h"
#include"cocos2d.h"
#include"FairyRes.h"

class saberSkill : public FairySkill
{
public:
	virtual void showEffect(FairyHero* hero, FairyHero* target);
};

#endif
