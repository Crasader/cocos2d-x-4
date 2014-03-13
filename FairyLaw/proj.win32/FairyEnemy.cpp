#include "FairyEnemy.h"

USING_NS_CC;
using namespace std;

FairyEnemy* FairyEnemy::create(int id){
	FairyEnemy *hero = new FairyEnemy();
	if(hero && hero->init()){
		hero->initHero(id);
		hero->autorelease();
		return hero;
	}
	CC_SAFE_DELETE(hero);
    return NULL;
}

void FairyEnemy::doAI(){
	selectSkill();
	switch(enemyType){
		case 0:
			doAI0();
			break;
		case 1:
			doAI1();
			break;
		case 2:
			doAI2();
			break;
	}
}

void FairyEnemy::selectSkill(){
	int SkillCount = (int)skillIds.size();
	int radomSkill = (int)CCRANDOM_0_1() * (SkillCount - 1);
	currentSkillId = skillIds.at(radomSkill);
}

void FairyEnemy::doAI0(){
	int targetX = -1;
	int targetY = -1;
	FairyMap* fairyMap = (FairyMap *)this->getParent();
	vector<int> heroPoint = FairyMap::gameLocationToMap(this->getPosition(), fairyMap->getMapY());
	for(int i = -(this->searchArea); i <= this->searchArea; i++){
		for(int j = -(this->searchArea); j <= this->searchArea; j++){
			int searchX = max(0,min(heroPoint.at(0) + i, (int)(fairyMap->mapWidth - 1)));
			int searchY = max(0,min(heroPoint.at(1) + j, (int)(fairyMap->mapHeight - 1)));
			if(fairyMap->mapObject[searchX][searchY] >= 0 && fairyMap->mapObject[searchX][searchY] < 2000
				&& fairyMap->mapObject[searchX][searchY] != this->heroId){
				targetX = searchX;
				targetY = searchY;
				if(searchId == fairyMap->mapObject[searchX][searchY]){
					break;
				}
			}
		}
	}
	if(targetX != -1 || targetY != -1){
		searchId = fairyMap->mapObject[targetX][targetY];
		if(targetX> heroPoint.at(0)){
			fairyMap->moveHero(3,this,this->heroId);
		}else if(targetX < heroPoint.at(0)){
			fairyMap->moveHero(0,this,this->heroId);
		}
		if(targetY > heroPoint.at(1)){
			fairyMap->moveHero(6,this,this->heroId);
		}else if(targetY < heroPoint.at(1)){
			fairyMap->moveHero(4,this,this->heroId);
		}
	}
}

void FairyEnemy::doAI1(){

}

void FairyEnemy::doAI2(){
}

bool FairyEnemy::initHero(int id){
	this->heroId = id;
	int resId = id / 10 *10;
	CCString* heroDataFile = CCString::createWithFormat("data/hero%d.xml",resId);
	XMLParser *heroData = XMLParser::parseWithFile(heroDataFile->getCString());
	enemyType = heroData->getString("enemyType")->intValue();
	searchArea = heroData->getString("searchArea")->intValue();
	return FairyHero::initHero(id);
	searchId = -1;
}
