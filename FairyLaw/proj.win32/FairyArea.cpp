#include "FairyArea.h"

USING_NS_CC;

void FairyArea::showMoveArea(FairyHero* selectHero){
	for(int i = -(selectHero->moveArea); i <= selectHero->moveArea; i++){
		for(int j = -(selectHero->moveArea); j <= selectHero->moveArea; j++){
			if(i != 0 || j != 0){
				CCSprite* areaSprite = CCSprite::create(moveArea);
				areaSprite->setPosition(ccpAdd(selectHero->getPosition(), ccp(i * 120, j * 120)));
				this->addChild(areaSprite);
			}
		}
	}
	areaX = -(selectHero->moveArea) * 120 + selectHero->getPosition().x - selectHero->getContentSize().width / 2;
	areaY = -(selectHero->moveArea) * 120 + selectHero->getPosition().y - selectHero->getContentSize().height / 2;
	areaWidth = selectHero->moveArea * 240 + selectHero->getContentSize().width;
	areaHeight = selectHero->moveArea * 240 + selectHero->getContentSize().height;
}

void FairyArea::showAttactArea(FairyHero* selectHero){
	FairySkill* skill = FairySkill::create(selectHero->currentSkillId);
	for(int i = 0; i < (int)skill->skillArea.size(); i++){
		CCSprite* areaSprite = CCSprite::create(attackArea);
		areaSprite->setPosition(ccpAdd(selectHero->getPosition(), 
			ccp(skill->skillArea.at(i).x * FairyMap::tileWidth, skill->skillArea.at(i).y * FairyMap::tileHeight)));
		this->addChild(areaSprite);
	}
}

bool FairyArea::contains(CCPoint point, FairyHeroManager* fairyHeroManager){
	CCRect rect = CCRectMake(areaX, areaY, areaWidth, areaHeight);
	if(rect.containsPoint(point)){
		return true;
	}else{
		return false;
	}
}

void FairyArea::cleanMoveArea(){
	this->removeAllChildrenWithCleanup(true);
}
