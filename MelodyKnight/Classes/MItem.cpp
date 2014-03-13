#include "MItem.h"
USING_NS_CC;
using namespace std;

MItem* MItem::create(string id, int x, int y){
	MItem *item = new MItem();
	if(item && item->init()){
		item->initItem(id, x, y);
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
    return NULL;
}

bool MItem::initItem(string id, int x, int y){
	this->id = id;
	vector<string> date = Utils::split(id,":");
	this->type = date.at(0);
	this->propertis = atoi(date.at(1).c_str());
	if(this->type.compare("skill") == 0){
		CCString* SkillDataFile = CCString::createWithFormat("normal_skill/skill%d.xml", atoi(date.at(1).c_str()));
		this->initWithFile(Utils::parseXML(SkillDataFile->getCString(), "skillIcon"));
	}else{
		this->initWithFile("map/coin.png");
	}
	this->x = x;
	this->y = y;
	speedx = 0;
	speedy = 0;
	initTime = Utils::millisecondNow();
	aiSkill();
	return true;
}

void MItem::update(){
	itemAI();
	if(isDestroy){
		GameManager::sharedGameManager()->getItemsArr()->removeObject(this);
		this->removeFromParentAndCleanup(true);
	}
}

void MItem::itemAI(){
	aiSkill();
}

void MItem::moveTo(CCPoint rolePoint){
	CCPoint startPosition= this->getPosition();
	float angle = Utils::getAngle(startPosition, rolePoint);
	this->speedx = Utils::getSpeedX(3, angle);
	this->speedy = Utils::getSpeedY(3, angle);
	if(this->getOpacity() > 0){
		this->x = this->x + speedx;
		this->y = this->y + speedy;
		this->setPosition(ccp(x, y));
	}
}

void MItem::aiSkill(){
	MRole* role = GameManager::sharedGameManager()->getRole();
	if(Utils::millisecondNow() - initTime >= 10000){
		this->isDestroy  = true;
	}else if(abs(this->getPositionX() - role->getPositionX()) < 10 && abs(this->getPositionY() - role->getPositionY()) < 10){
		dealManager();
	}else if(this->type.compare("skill") == 0 && abs(this->getPositionX() - tempDes.x) < 50 && abs(this->getPositionY() - tempDes.y) < 50){
		float x = CCRANDOM_0_1() * 1080;
		float y = CCRANDOM_0_1() * 720;
		tempDes = CCPointMake(x, y);
	}
	if(this->type.compare("skill") == 0){
		moveTo(tempDes);
	}else if(this->type.compare("money") == 0){
		this->setPosition(ccp(x, y));
	}
}

void MItem::dealManager(){
	isDestroy = true;
	Utils::playEffect("effect/buff.wav",false);
	if(this->type.compare("skill") == 0){
		GameManager::itemSkillId = this->propertis;
	}else if(this->type.compare("money") == 0){
		GameManager::sharedGameManager()->addMoney(this->propertis);
	}
}