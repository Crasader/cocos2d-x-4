#ifndef _FAIRY_HERO_MANAGER_H__
#define _FAIRY_HERO_MANAGER_H__

#include "cocos2d.h"
#include"FairyRes.h"
#include"FairyHero.h"
#include"FairyEnemy.h"
#include"FairyMap.h"
#include"Utils.h"
#include "XMLParser.h"


class FairyHeroManager : public cocos2d::CCLayer
{
public:

	virtual bool init();  

	void setHero(FairyMap *mapLayer, int section);

	static FairyHero* containsHero(cocos2d::CCPoint point);

	static FairyHero* containsEnemy(cocos2d::CCPoint point);

	static FairyHero* getHeroById(int id);

	static FairyHero* getEnemyById(int id);

	static cocos2d::CCArray* getHeroArr();

	static cocos2d::CCArray* getEnemyArr();

	static int getEnemyCount();

	static int getHeroCount();

	CREATE_FUNC(FairyHeroManager);

	static cocos2d::CCArray* fairyHeroArr;

	static cocos2d::CCArray* fairyEnemyArr;
};

#endif  