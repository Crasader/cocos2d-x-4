#include "MSkill.h"

USING_NS_CC;
using namespace std;

MSkill* MSkill::create(MSkillTarget* target, int fileData, int skillType, int speedx, int speedy, int x, int y, int attackPoint){
	MSkill *skill = new MSkill();
	if(skill && skill->init()){
		skill->initSkill(target, fileData, skillType, speedx, speedy, x, y, attackPoint);
		skill->autorelease();
		return skill;
	}
	CC_SAFE_DELETE(skill);
    return NULL;
}

bool MSkill::initSkill(MSkillTarget* target, int fileData, int skillType, int speedx, int speedy, int x, int y, int attackPoint){
	CCString* SkillDataFile = CCString::createWithFormat("normal_skill/skill%d.xml",fileData);
	this->id = atoi(Utils::parseXML(SkillDataFile->getCString(), "id"));
	this->mp = atoi(Utils::parseXML(SkillDataFile->getCString(), "mp"));
	this->distance = atoi(Utils::parseXML(SkillDataFile->getCString(), "distance"));
	this->time =  atoi(Utils::parseXML(SkillDataFile->getCString(), "time"));
	this->updateTime =  atoi(Utils::parseXML(SkillDataFile->getCString(), "updateTime"));
	this->skillType = skillType;
	this->target = target;
	this->x = x;
	this->y = y;
	this->speedx = speedx;
	this->speedy = speedy;
	this->attackPoint =attackPoint;
	this->initTime = Utils::millisecondNow();
	this->init();
	return true;
}

void MSkill::addBuff(int type){
	CCArray* buffArray = GameManager::sharedGameManager()->buffArray;
	GameManager::buffHasChange = true;
	MRole::buffTime[type] = Utils::millisecondNow();
	for(int i = 0; i < (int)buffArray->count(); i ++){
		int buffType = static_cast<CCInteger*>(buffArray->objectAtIndex(i))->getValue();
		if(buffType == type){
			GameManager::sharedGameManager()->buffArray->removeObjectAtIndex(i);
		}
	}
	GameManager::sharedGameManager()->buffArray->addObject(CCInteger::create(type));
	Utils::playEffect("effect/buff.wav",false);
}

void MSkill::update(){
	switch(id){
	case 10000:
		showEffect10000();
		break;
	case 10001:
		showEffect10001();
		break;
	case 10002:
		showEffect10002();
		break;
	case 10003:
		showEffect10003();
		break;
	case 10004:
	case 10008:
	case 10009:
	case 10010:
	case 10011:
		Utils::playEffect("effect/attack1.wav",false);
		break;
	case 10005:
		showEffect10005();
		break;
	case 10007:
		showEffect10007();
		break;
	case 10012:
		addBuff(0);
		break;
	case 10013:
		addBuff(1);
		break;
	case 10014:
		addBuff(2);
		break;
	case 10015:
		addBuff(3);
	case 10050:
		showEffect10050();
		break;
	case 10051:
		showEffect10051();
		break;
	}
}

void MSkill::showEffect10000(){
	MRole* role = GameManager::sharedGameManager()->getRole();
	int speedX = 0;
	int speedY = 0;
	int rotation = 0;
	switch(role->roleAniType){
	case 0:
	case 4:
		speedX = -20;
		rotation = 180;
		break;
	case 1:
	case 5:
		speedX = 20;
		break;
	case 2:
	case 6:
		speedY = 20;
		rotation = -90;
		break;
	case 3:
	case 7:
		speedY = -20;
		rotation = 90;
		break;
	}
	MEffect* effect = MEffect::create(id, role->getPositionX(), role->getPositionY(), speedX, speedY, this->distance, this->time, this->updateTime, skillType);
	effect->isShow = false;
	effect->isDestroy = false;
	effect->setRotation(rotation);
	effect->setPosition(ccp(effect->x, effect->y));
	GameManager::sharedGameManager()->getEffectsArr()->addObject(effect);
	Utils::playEffect("effect/attack1.wav",false);
}

void MSkill::showEffect10001(){
	MRole* role = GameManager::sharedGameManager()->getRole();
	MEffect* effect = MEffect::create(id, role->getPositionX(), role->getPositionY() - 20, 0, 0, this->distance, this->time, this->updateTime, skillType);
	effect->isShow = false;
	effect->isDestroy = false;
	effect->setPosition(ccp(effect->x, effect->y));
	GameManager::sharedGameManager()->getEffectsArr()->addObject(effect);
	Utils::playEffect("effect/attack1.wav",false);
}

void MSkill::showEffect10002(){
	MRole* role = GameManager::sharedGameManager()->getRole();
	int speedX = 0;
	int speedY = 0;
	int roleOffsetX = 0;
	int roleOffsetY = 0;
	int rotation = 0;
	switch(role->roleAniType){
	case 0:
	case 4:
		speedX = -40;
		rotation = 180;
		break;
	case 1:
	case 5:
		speedX = 40;
		break;
	case 2:
	case 6:
		speedY = 40;
		rotation = -90;
		break;
	case 3:
	case 7:
		speedY = -40;
		rotation = 90;
		break;
	}
	MEffect* effect = MEffect::create(id, role->getPositionX(), role->getPositionY(), speedX, speedY, this->distance, this->time, this->updateTime, skillType);
	effect->isShow = false;
	effect->isDestroy = false;
	effect->setRotation(rotation);
	effect->setPosition(ccp(effect->x, effect->y));
	GameManager::sharedGameManager()->getEffectsArr()->addObject(effect);
	Utils::playEffect("effect/attack1.wav",false);
}

void MSkill::showEffect10003(){
	MRole* role = GameManager::sharedGameManager()->getRole();
	int pos[4][2] = {{200,0},{-200,0},{0,200},{0,-200}};
	for(int i = 0; i < 4; i++){
		MEffect* effect = MEffect::create(id, role->getPositionX()+pos[i][0], role->getPositionY() - 20 + +pos[i][1], 0, 0, this->distance, this->time, this->updateTime, skillType);
		effect->isShow = false;
		effect->isDestroy = false;
		effect->setPosition(ccp(effect->x, effect->y));
		GameManager::sharedGameManager()->getEffectsArr()->addObject(effect);
	}
	Utils::playEffect("effect/attack1.wav",false);
}

void MSkill::showEffect10005(){
	MRole* role = GameManager::sharedGameManager()->getRole();
	MEffect* effect = MEffect::create(id, role->getPositionX(), role->getPositionY() - 20, 0, 0, this->distance, this->time, this->updateTime, skillType);
	effect->isShow = false;
	effect->isDestroy = false;
	effect->setPosition(ccp(effect->x, effect->y));
	GameManager::sharedGameManager()->getEffectsArr()->addObject(effect);
	Utils::playEffect("effect/attack1.wav",false);
}

void MSkill::showEffect10007(){
	MRole* role = GameManager::sharedGameManager()->getRole();
	MEffect* effect = MEffect::create(id, role->getPositionX(), role->getPositionY() - 20, 0, 0, this->distance, this->time, this->updateTime, skillType);
	effect->isShow = false;
	effect->isDestroy = false;
	effect->setPosition(ccp(effect->x, effect->y));
	GameManager::sharedGameManager()->getEffectsArr()->addObject(effect);
	Utils::playEffect("effect/attack1.wav",false);
}

void MSkill::showEffect10050(){
	MEffect* effect = MEffect::create(id, x, y, speedx, speedy, this->distance, this->time, this->updateTime, skillType, this->attackPoint);
	effect->isShow = false;
	effect->isDestroy = false;
	effect->setPosition(ccp(effect->x, effect->y));
	if(speedx > 0){
		effect->setRotation(90);
	}else if(speedx < 0){
		effect->setRotation(-90);
	}
	if(speedy > 0){
		effect->setRotation(0);
	}else if(speedy < 0){
		effect->setRotation(180);
	}
	GameManager::sharedGameManager()->getEffectsArr()->addObject(effect);
	Utils::playEffect("effect/attack1.wav",false);
}

void MSkill::showEffect10051(){
	int speed[4][2] = {{speedx, 0},{-speedx, 0},{0, speedy},{0, -speedy}};
	for(int i = 0; i < 4; i++){
		MEffect* effect = MEffect::create(id, x, y, speed[i][0], speed[i][1], this->distance, this->time, this->updateTime, skillType, this->attackPoint);
		effect->isShow = false;
		effect->isDestroy = false;
		effect->setPosition(ccp(effect->x, effect->y));
		GameManager::sharedGameManager()->getEffectsArr()->addObject(effect);
	}
	Utils::playEffect("effect/attack1.wav",false);
}
