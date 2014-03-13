#include "MEffect.h"

USING_NS_CC;
using namespace std;

MEffect* MEffect::create(int id, int x, int y,  int speedx, int speedy, int distance, int time, int updateTime, int skillType, int attackPoint){
	MEffect *effect = new MEffect();
	if(effect && effect->init()){
		effect->initEffect(id, x, y, speedx, speedy,distance, time, updateTime, skillType, attackPoint);
		effect->autorelease();
		return effect;
	}
	CC_SAFE_DELETE(effect);
    return NULL;
}

bool MEffect::initEffect(int skillId, int x, int y, int speedx, int speedy, int distance, int time, int updateTime, int skillType,  int attackPoint){
	this->init();
	this->id = skillId;
	this->distance = distance;
	this->time = time;
	this->updateTime = updateTime;
	this->initTime = Utils::millisecondNow();
	this->initPoint = ccp(x, y);
	this->skillType = skillType;
	this->attackPoint = attackPoint;
	skillEffect = CCSprite::create();
	this->addChild(skillEffect);
	switch(skillId){
	case 10000:
		isPenetrate = false;
		isDelay = false;
		isHook = false;
		skillEffect->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("holy_wave")));
		break;
	case 10001:
		isPenetrate = false;
		isDelay = true;
		isHook = false;
		skillEffect->setScale(2.5f);
		skillEffect->runAction( CCSequence::create(AnimatePacker::getInstance()->getAnimate("water_wave"), CCFadeOut::create(0.2f),NULL));
		break;
	case 10002:
		isPenetrate = true;
		isDelay = false;
		isHook = false;
		skillEffect->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("dark_wave")));
		break;
	case 10003:
		isPenetrate = false;
		isDelay = true;
		isHook = false;
		skillEffect->runAction( CCSequence::create(AnimatePacker::getInstance()->getAnimate("fire_wave"), CCFadeOut::create(0.2f),NULL));
		break;
	case 10005:
		isPenetrate = true;
		isDelay = false;
		isHook = true;
		skillEffect->runAction(AnimatePacker::getInstance()->getAnimate("water_hook"));
		break;
	case 10007:
		isPenetrate = true;
		isDelay = false;
		isHook = true;
		skillEffect->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("fire_hook")));
		break;
	case 10050:
		isPenetrate = false;
		isDelay = false;
		isHook = false;
		skillEffect->initWithFile("normal_skill/attick6.png");
		break;
	case 10051:
		isPenetrate = false;
		isDelay = false;
		isHook = false;
		skillEffect->initWithFile("normal_skill/attick7.png");
		break;
	}
	this->x = x;
	this->y = y;
	this->speedx = speedx;
	this->speedy = speedy;
	return true;
}

void MEffect::update(){
	move();
	dealManager();
	if(isDestroy){
		if(isDelay && this->getOpacity() != 0){
			return;
		}
		GameManager::sharedGameManager()->getEffectsArr()->removeObject(this);
		this->removeFromParentAndCleanup(true);
	}
}

void MEffect::dealManager(){
	CCArray* enemysArr = GameManager::sharedGameManager()->getEnemysArr();
	CCObject* objItem;
	if(Utils::millisecondNow() - initTime >= this->time * 1000
		|| abs(this->x - initPoint.x) + abs(this->y - initPoint.y) >= this->distance){
			isDestroy = true;
	}
	bool isHit = false;
	if(skillType == MSkill::ROLE_TYPE){
		CCARRAY_FOREACH(enemysArr, objItem){
			Menemy* enemy = static_cast<Menemy*>(objItem);
			if(!isDestroy && this->boundingBox().intersectsRect(enemy->boundingBox())){
				isHit = true;
				GameManager::lastComboCheckTime = Utils::millisecondNow();
				GameManager::comboCount++;
				enemy->dealDamager("damage/damage1.res");
			}
		}
	}else{
		MRole* role = GameManager::sharedGameManager()->getRole();
		if(!isDestroy && this->boundingBox().intersectsRect(role->boundingBox())){
			isHit = true;
			role->dealDamager("damage/damage1.res", this->attackPoint);
		}
	}
	if(!isPenetrate && isHit){
		isDestroy = true;
	}
}

void MEffect::move(){
	if(this->getOpacity() > 0){
		this->x = this->x + speedx;
		this->y = this->y + speedy;
		this->setPosition(ccp(x, y));
	}
	CCSize mapSize = GameManager::sharedGameManager()->getMapSize();
	if(this->getPositionY() > mapSize.height || this->getPositionY() < 0||
				this->getPositionX() >(1080 + mapSize.width) / 2 || this->getPositionX() <  (1080 - mapSize.width) / 2){
		this->isDestroy = true;
	}
}
