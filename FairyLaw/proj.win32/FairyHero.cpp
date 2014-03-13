#include "FairyHero.h"

USING_NS_CC;
using namespace std;

FairyHero* FairyHero::create(int id){
	FairyHero *hero = new FairyHero();
	if(hero && hero->init()){
		hero->initHero(id);
		hero->autorelease();
		return hero;
	}
	CC_SAFE_DELETE(hero);
    return NULL;
}

bool FairyHero::initHero(int id){
	this->heroId = id;
	this->isDead = false;
	int resId = id / 10 *10;
	CCString* heroDataFile = CCString::createWithFormat("data/hero%d.xml",resId);
	XMLParser *heroData = XMLParser::parseWithFile(heroDataFile->getCString());
	vector<string> ids = Utils::split( heroData->getString("skillIds")->getCString(), NPARTITION);
	for(int i = 0; i < (int)ids.size(); i++){
		skillIds.push_back(atoi(ids.at(i).c_str()));
	}
	currentSkillId = skillIds.at(0);
	attick = heroData->getString("attick")->intValue();
	defense = heroData->getString("defense")->intValue();
	moveArea = heroData->getString("moveArea")->intValue();
	maxLife = heroData->getString("maxLife")->intValue();
	varLife = maxLife;
	this->initWithFile(heroData->getString("headIcon")->getCString());
	frame = CCSprite::create(heroFrame);
	frameSize = frame->getContentSize();
	setSeleted(false);
	frame->setPosition(ccp(frameSize.width / 4, frameSize.height / 2));
	this->addChild(frame,0,11);
	lifeBar = CCProgressTimer::create(CCSprite::create(heroLifeBar, CCRectMake(0,0,110,20)));
	lifeBar->setType(kCCProgressTimerTypeBar);
	lifeBar->setMidpoint(ccp(0,0));
	lifeBar->setBarChangeRate(ccp(1,0));
	lifeBar->setPercentage(100);
	barSize = lifeBar->getContentSize();
	lifeBar->setPosition(ccp(barSize.width / 2, barSize.height / 2));
	this->addChild(lifeBar);
	return true;
}

void FairyHero::updateHp(int attack){
	showHit();
	this->varLife = this->varLife - attack + this->defense;
	if(this->varLife > 0){
		lifeBar->setPercentage(40 + this->varLife * (100 - 40) / this->maxLife);
	}else{
		FairyMap* fairyMap = (FairyMap *)this->getParent();
		vector<int> heroPoint = FairyMap::gameLocationToMap(this->getPosition(), fairyMap->getMapY());
		fairyMap->mapObject[heroPoint.at(0)][heroPoint.at(1)] = -1;
		this->isDead = true;
		this->setOpacity(0);
		this->removeAllChildrenWithCleanup(true);
	}
}

void FairyHero::showHit(){
	CCActionInterval * blink = CCBlink::create(0.1f,1);
	this->runAction(blink);
}

void FairyHero::setSeleted(bool isSelect){
	if(isSelect){
		frame->setTextureRect(CCRect(0, 0 ,frameSize.width / 2, frameSize.height));
	}else{
		frame->setTextureRect(CCRect(frameSize.width / 2, 0, frameSize.width / 2, frameSize.height));
	}
}

void FairyHero::move(){
	CCActionInterval * moveBy = CCMoveBy::create(0.1f,ccp(0, 30));
	this->runAction(CCSequence::create(moveBy, moveBy->reverse(),NULL));
}
