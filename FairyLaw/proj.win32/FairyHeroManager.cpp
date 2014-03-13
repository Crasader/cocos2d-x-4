#include "FairyHeroManager.h"

USING_NS_CC;
using namespace std;

cocos2d::CCArray* FairyHeroManager::fairyHeroArr = NULL;
cocos2d::CCArray* FairyHeroManager::fairyEnemyArr = NULL;

bool FairyHeroManager::init(){
	fairyHeroArr = CCArray::create();
	fairyHeroArr->retain();
	fairyEnemyArr = CCArray::create();
	fairyEnemyArr->retain();
	return true;
}

void FairyHeroManager::setHero(FairyMap *mapLayer, int section){
	CCString* sceneDataFile = CCString::createWithFormat("data/scene%d.xml",section);
	XMLParser *sceneData = XMLParser::parseWithFile(sceneDataFile->getCString());
	int pointX = 0;
	int pointY = 0;
	vector<string> heroData = Utils::split( sceneData->getString("hero")->getCString(), MPARTITION);
	for(int i = 0; i < (int)heroData.size(); i++){
		vector<string> heroDataProperty = Utils::split( heroData.at(i), PPARTITION);
		int heroId = atoi(heroDataProperty.at(0).c_str());
		pointX = atoi(heroDataProperty.at(1).c_str());
		pointY = atoi(heroDataProperty.at(2).c_str());
		FairyHero *hero = FairyHero::create(heroId);
		fairyHeroArr->addObject(hero);
		CCPoint heroPoint = ccp(FairyHero::heroWidth / 2, FairyHero::heroHeight / 2);
		hero->setPosition(ccpAdd(heroPoint,FairyMap::mapLocationToGame(ccp(pointX, pointY))));
		mapLayer->mapObject[pointX][pointY] = hero->heroId;
		mapLayer->addChild(hero);
	}

	vector<string> enemyData = Utils::split( sceneData->getString("enemy")->getCString(), MPARTITION);
	for(int i = 0; i < (int)enemyData.size(); i++){
		vector<string> enemyDataProperty = Utils::split( enemyData.at(i), PPARTITION);
		int enemyId = atoi(enemyDataProperty.at(0).c_str());
		pointX = atoi(enemyDataProperty.at(1).c_str());
		pointY = atoi(enemyDataProperty.at(2).c_str());
		FairyEnemy *enemy = FairyEnemy::create(enemyId);
		fairyEnemyArr->addObject(enemy);
		CCPoint enemyPoint = ccp(FairyHero::heroWidth / 2, FairyHero::heroHeight / 2);
		enemy->setPosition(ccpAdd(enemyPoint,FairyMap::mapLocationToGame(ccp(pointX, pointY))));
		mapLayer->mapObject[pointX][pointY] = enemy->heroId;
		mapLayer->addChild(enemy);
	}
}

CCArray* FairyHeroManager::getHeroArr(){
	return fairyHeroArr;
}

CCArray* FairyHeroManager::getEnemyArr(){
	return fairyEnemyArr;
}

int FairyHeroManager::getEnemyCount(){
	int count = fairyEnemyArr->count();
	CCObject *obj;
	CCARRAY_FOREACH(fairyEnemyArr, obj)
	{
		FairyHero* fairyHero = (FairyHero*)obj;
		if(fairyHero->isDead){
			count--;
		}
	}
	return count;
}

int FairyHeroManager::getHeroCount(){
	int count = fairyHeroArr->count();
	CCObject *obj;
	CCARRAY_FOREACH(fairyHeroArr, obj)
	{
		FairyHero* fairyHero = (FairyHero*)obj;
		if(fairyHero->isDead){
			count--;
		}
	}
	return count;
}

FairyHero* FairyHeroManager::containsHero(cocos2d::CCPoint point){
	CCObject *obj;
	CCARRAY_FOREACH(fairyHeroArr, obj)
	{
		FairyHero* fairyHero = (FairyHero*)obj;
		if(fairyHero->boundingBox().containsPoint(point)){
			return fairyHero;
		}
	}
	return NULL;
}

FairyHero* FairyHeroManager::containsEnemy(cocos2d::CCPoint point){
	CCObject *obj;
	CCARRAY_FOREACH(fairyEnemyArr, obj)
	{
		FairyHero* fairyHero = (FairyHero*)obj;
		if(fairyHero->boundingBox().containsPoint(point)){
			return fairyHero;
		}
	}
	return NULL;
}

FairyHero* FairyHeroManager::getHeroById(int id){
	CCObject *obj;
	CCARRAY_FOREACH(fairyHeroArr, obj)
	{
		FairyHero* fairyHero = (FairyHero*)obj;
		if(fairyHero->heroId == id && !fairyHero->isDead){
			return fairyHero;
		}
	}
	return NULL;
}

FairyHero* FairyHeroManager::getEnemyById(int id){
	CCObject *obj;
	CCARRAY_FOREACH(fairyEnemyArr, obj)
	{
		FairyHero* fairyHero = (FairyHero*)obj;
		if(fairyHero->heroId == id && !fairyHero->isDead){
			return fairyHero;
		}
	}
	return NULL;
}

