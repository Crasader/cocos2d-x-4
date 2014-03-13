#ifndef _FAIRY_SKILL_H_
#define _FAIRY_SKILL_H_

#include"cocos2d.h"
#include"FairyRes.h"
#include "XMLParser.h"
#include "FairyHero.h"
#include "FairyMap.h"
#include "Utils.h"
#include "FairyHeroManager.h"
#include "AnimatePacker.h"

class FairySkill : public cocos2d::CCSprite
{
public:
	static FairySkill* create(int id);
	bool initSkill(int id);
	virtual void showEffect(FairyHero* hero, FairyMap* fairyMap);
	void showSaber1Effect();
	void showSaber2Effect();
	void showSaber3Effect();
	void removeEffect(CCNode* pSender);
	int skillId;
	std::vector<cocos2d::CCPoint> skillArea;
	int attact;
	int buffeId;
	int tagetType;
	const char* name;
	const char* decript;
	const char* skillIcon;
	FairyHero* hero;
	FairyHero* target;
	static bool isShowEffect;
};

#endif
