#ifndef _FAIRY_AREA_H__
#define _FAIRY_AREA_H__

#include "cocos2d.h"
#include "FairyHero.h"
#include "FairyHeroManager.h"
#include"FairyRes.h"
#include"FairyMap.h"
#include"FairySkill.h"

class FairyArea : public cocos2d::CCLayer
{
public:

	 void showMoveArea(FairyHero* selectHero);

	 void showAttactArea(FairyHero* selectHero);

	 void cleanMoveArea();

	 bool contains(cocos2d::CCPoint point,  FairyHeroManager* fairyHeroManager);

	 CREATE_FUNC(FairyArea);

	 int areaWidth;

	 int areaHeight;

	 int areaX;

	 int areaY;
};

#endif 
