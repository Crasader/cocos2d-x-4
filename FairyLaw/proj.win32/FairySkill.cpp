#include "FairySkill.h"

USING_NS_CC;
using namespace std;

bool FairySkill::isShowEffect = false;

FairySkill* FairySkill::create(int id){
	FairySkill *skill = new FairySkill();
	if(skill && skill->init()){
		skill->initSkill(id);
		skill->autorelease();
		return skill;
	}
	CC_SAFE_DELETE(skill);
    return NULL;
}

bool FairySkill::initSkill(int id){
	this->skillId = id;
	CCString* SkillDataFile = CCString::createWithFormat("data/skill%d.xml",id);
	XMLParser *skillData = XMLParser::parseWithFile(SkillDataFile->getCString());
	buffeId = skillData->getString("buffeId")->intValue();
	attact = skillData->getString("attact")->intValue();
	tagetType = skillData->getString("tagetType")->intValue();
	vector<string> area = Utils::split( skillData->getString("skillArea")->getCString(), NPARTITION);
	for(int i = 0; i < (int)area.size(); i = i + 2){
		CCPoint point = ccp(atoi(area.at(i).c_str()), 
			atoi(area.at(i+1).c_str()));
		skillArea.push_back(point);
	}
	return true;
}

void FairySkill::showEffect(FairyHero* hero, FairyMap* fairyMap){
	this->hero = hero;
	vector<int> targets;
	vector<int> heroPoint = FairyMap::gameLocationToMap(hero->getPosition(), fairyMap->getMapY());
	for(int i = 0; i < (int)skillArea.size(); i++){
	int searchX = max(0,min(heroPoint.at(0) + (int)(skillArea.at(i).x), (int)(fairyMap->mapWidth - 1)));
		int searchY = max(0,min(heroPoint.at(1) + (int)(skillArea.at(i).y), (int)(fairyMap->mapHeight - 1)));
		bool hasTarget = false;
		if(hero->heroId > 2000){
			if(fairyMap->mapObject[searchX][searchY] >= 0 && fairyMap->mapObject[searchX][searchY] <2000 
				&& fairyMap->mapObject[searchX][searchY] != hero->heroId){
				targets.push_back(fairyMap->mapObject[searchX][searchY]);
			}
		}else if(hero->heroId > 0){
			if(fairyMap->mapObject[searchX][searchY] >= 2000 
				&& fairyMap->mapObject[searchX][searchY] != hero->heroId){
				targets.push_back(fairyMap->mapObject[searchX][searchY]);
			}
		}
	}
	if(targets.size() > 0){
		if(hero->heroId > 2000){
			FairyHero* fairyHero = FairyHeroManager::getHeroById(targets.at(0));
			if(fairyHero){
				this->target = fairyHero;
			}
		}else if(hero->heroId < 2000){
			FairyHero* fairyHero = FairyHeroManager::getEnemyById(targets.at(0));
			if(fairyHero){
				this->target = fairyHero;
			}
		}
		if(skillId == 0){
			showSaber1Effect();
		}else if(skillId == 1){
			showSaber2Effect();
		}else if(skillId == 2){
			showSaber3Effect();
		}
		/*if(hero->heroId > 2000){
			for(int i = 0; i < (int)targets.size(); i++){
				FairyHero* fairyHero = FairyHeroManager::getHeroById(targets.at(i));
				if(fairyHero){
					fairyHero->updateHp(this->hero->attick + this->attact);
				}
			}
		}else if(hero->heroId < 2000){
			for(int i = 0; i < (int)targets.size(); i++){
				FairyHero* fairyHero = FairyHeroManager::getEnemyById(targets.at(i));
				if(fairyHero){
					fairyHero->updateHp(this->hero->attick + this->attact);
				}
			}
		}*/
	}
};

void FairySkill::showSaber1Effect(){
	isShowEffect = true;
	CCSprite *effect = CCSprite::create(skill0);
	hero->getParent()->addChild(effect,0,10);
	effect->setAnchorPoint(ccp(1, 0.5));
	CCSize size = effect->getContentSize();
	effect->setPosition(hero->getPosition());
	CCActionInterval * rotateTo = CCRotateBy::create(0.5f, 360);
	effect->runAction(CCSequence::create(rotateTo, 
			CCCallFuncN::create(this,callfuncN_selector(FairySkill::removeEffect)),NULL));
};

void FairySkill::showSaber2Effect(){
	isShowEffect = true;
	CCSprite *effect = CCSprite::create(skill0);
	effect->setOpacity(0);
	target->getParent()->addChild(effect,0,10);
	CCSize size = effect->getContentSize();
	CCPoint beginPoint = hero->getPosition();
	CCPoint endPoint = target->getPosition();
	effect->setRotation(Utils::getAngle(beginPoint, endPoint) + 90);
	effect->setPosition(beginPoint);
	CCActionInterval * fadeTo = CCFadeIn::create(0.5f); 
	CCActionInterval * moveTo = CCMoveTo::create(0.5f,endPoint);
	effect->runAction(CCSequence::create(fadeTo, moveTo,
			CCCallFuncN::create(this,callfuncN_selector(FairySkill::removeEffect)),NULL));
};

void FairySkill::showSaber3Effect(){
	isShowEffect = true;
	CCSprite *effect = CCSprite::create(skill0);
	effect->setOpacity(0);
	target->getParent()->addChild(effect,0,10);
	CCSize size = effect->getContentSize();
	CCPoint endPoint = ccpAdd(target->getPosition(), ccp(0, 20));
	effect->setRotation(270);
	effect->setPosition(ccpAdd(endPoint, ccp(0, 100)));
	CCActionInterval * fadeTo = CCFadeIn::create(0.5f); 
	CCActionInterval * moveTo = CCMoveTo::create(0.5f,endPoint);
	effect->runAction(CCSequence::create(fadeTo, moveTo,
			CCCallFuncN::create(this,callfuncN_selector(FairySkill::removeEffect)),NULL));
};

void FairySkill::removeEffect(CCNode* pSender){
	this->hero->getParent()->removeChildByTag(10,true);
	this->hero->getParent()->removeChildByTag(11,true);
	target->updateHp(this->hero->attick + this->attact);
	isShowEffect = false;
}