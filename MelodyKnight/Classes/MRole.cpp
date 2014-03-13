#include "MRole.h"

USING_NS_CC;
using namespace std;
long MRole::buffTime[6] = { 0, 0, 0, 0, 0, 0};
int MRole::equipIds[4]= {-1, -1, -1, -1};
int MRole::attackPoint = 20;
int MRole::defPoint = 20;

MRole* MRole::create(){
	MRole *role = new MRole();
	if(role && role->init()){
		role->initRole();
		role->autorelease();
		return role;
	}
	CC_SAFE_DELETE(role);
    return NULL;
}

void MRole::showSkillEffect(int type){
	switch(type){
	case 0:
		roleEffect->runAction(AnimatePacker::getInstance()->getAnimate("roleEffectf"));
		break;
	case 1:
		roleEffect->runAction(AnimatePacker::getInstance()->getAnimate("roleEffectw"));
		break;
	case 2:
		roleEffect->runAction(AnimatePacker::getInstance()->getAnimate("roleEffecth"));
		break;
	case 3:
		roleEffect->runAction(AnimatePacker::getInstance()->getAnimate("roleEffectd"));
		break;
	}
}

void MRole::showRoleEffect10000(){
	showSkillEffect(2);
	int roleOffsetX = 0;
	int roleOffsetY = 0;
	switch(this->roleAniType){
	case 0:
	case 4:
		roleOffsetX = -5;
		break;
	case 1:
	case 5:
		roleOffsetX = 5;
		break;
	case 2:
	case 6:
		roleOffsetY = 5;
		break;
	case 3:
	case 7:
		roleOffsetX = -5;
		break;
	}
	this->setPosition(ccpAdd(this->getPosition(), ccp(roleOffsetX, roleOffsetY)));
}

void MRole::showRoleEffect10002(){
	showSkillEffect(3);
	int roleOffsetX = 0;
	int roleOffsetY = 0;
	switch(this->roleAniType){
	case 0:
	case 4:
		roleOffsetX = -5;
		break;
	case 1:
	case 5:
		roleOffsetX = 5;
		break;
	case 2:
	case 6:
		roleOffsetY = 5;
		break;
	case 3:
	case 7:
		roleOffsetY = -5;
		break;
	}
	this->setPosition(ccpAdd(this->getPosition(), ccp(roleOffsetX, roleOffsetY)));
}

void MRole::showRoleEffect10004(){
	showSkillEffect(3);
	int roleOffsetX = 0;
	int roleOffsetY = 0;
	switch(this->roleAniType){
	case 0:
	case 4:
		roleOffsetX = -200;
		break;
	case 1:
	case 5:
		roleOffsetX = 200;
		break;
	case 2:
	case 6:
		roleOffsetY = 200;
		break;
	case 3:
	case 7:
		roleOffsetY = -200;
		break;
	}
	CCPoint movePoint = ccpAdd(this->getPosition(), ccp(roleOffsetX, roleOffsetY));
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	//movePoint.x = min(max((int)movePoint.x, 80), (int)(size.width - 80));
	//movePoint.y = min(max((int)movePoint.y, 80), (int)(size.height - 80));
	this->setPosition(movePoint);
}

bool MRole::initRole(){
	isFreeCost = false;
	curHp = maxHp = atoi(Utils::parseXML("role/role_data.xml", "maxHp")) + getEquipUp(0);
	maxMp = atoi(Utils::parseXML("role/role_data.xml", "maxMp")) + getEquipUp(1);
	attackPoint =  atoi(Utils::parseXML("role/role_data.xml", "attackPoint")) + getEquipUp(2);
	defPoint =  atoi(Utils::parseXML("role/role_data.xml", "defPoint")) + getEquipUp(3);
	curMp = 0;
	moveLatstTime = Utils::millisecondNow();
	lastHitTime = Utils::millisecondNow();
	roleAniType = 0; 
	roleMoveType = 0;
	roleEffect = CCSprite::create();
	this->addChild(roleEffect);
	roleEffect->setScale(0.5f);
	roleEffect->setPosition(ccp(60, 60));
	this->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("role_stand_right")));
	this->setScale(1.5f);
	damagerNum = CCLabelTTF::create("", "Arial", 30);
	this->addChild(damagerNum);
	damagerNum->setPosition(ccp(60, 80));
	return true;
}

float MRole::getHp(){
	return this->curHp * 100.0f / this->maxHp;
}

float MRole::getMp(){
	return this->curMp * 100.0f / this->maxMp;
}

int MRole::getEquipUp(int type){
	int total[4] = {0, 0, 0, 0};
	for(int i = 0; i < 4; i++){
		if(equipIds[i] != -1){
			CCString* data_string = CCString::createWithFormat("equip/equip%d.xml", equipIds[i]);
			int maxHp = atoi(Utils::parseXML(data_string->getCString(), "maxHp"));
			int maxMp = atoi(Utils::parseXML(data_string->getCString(), "maxMp"));
			int attackPoint = atoi(Utils::parseXML(data_string->getCString(), "attackPoint"));
			int defPoint = atoi(Utils::parseXML(data_string->getCString(), "defPoint"));
			total[0] +=  maxHp;
			total[1] += maxMp;
			total[2] += attackPoint;
			total[3] += defPoint;
		}
	}
	return total[type];
}

int MRole::getCurAttack(){
	int curActtack = attackPoint;
	if(MRole::buffTime[0] != 0){
		curActtack = curActtack + 5;
	}
	return curActtack;
}

int MRole::getCurDef(){
	int curDef = defPoint;
	if(MRole::buffTime[0] != 0){
		curDef = curDef  + 5;
	}
	return curDef;
}

void MRole::update(){
	if(buffTime[4] != 0){
		this->curHp = min(this->curHp + 1, this->maxHp);
	}
	if(buffTime[3] != 0){
		this->curHp = max(this->curHp - 0.05, 0);
	}
}

void MRole::showTargetEffet(int skillId){
	switch(skillId){
	case 10000:
		showRoleEffect10000();
		break;
	case 10001:
		showSkillEffect(1);
		break;
	case 10002:
		showRoleEffect10002();
		showSkillEffect(3);
		break;
	case 10003:
		showSkillEffect(0);
		break;
	case 10004:
		showRoleEffect10004();
		break;
	case 10005:
		showSkillEffect(1);
		break;
	case 10007:
		showSkillEffect(0);
		break;
	case 10006:
		showSkillEffect(2);
		isFreeCost = true;
		break;
	case 10008:
		showSkillEffect(0);
		MRole::buffTime[4] = Utils::millisecondNow();
		break;
	case 10009:
		showSkillEffect(2);
		this->curHp = min(this->maxHp, this->curHp *( 1.5f));
		break;
	case 10010:
		showSkillEffect(1);
		MRole::buffTime[5] = Utils::millisecondNow();
		break;
	case 10011:
		showSkillEffect(3);
		this->curHp = min(this->curHp - 10, this->maxHp);
		this->curMp = min(this->curMp + 40, this->maxMp);
		break;
	case 10012:
		showSkillEffect(0);
		break;
	case 10013:
		showSkillEffect(1);
		break;
	case 10014:
		showSkillEffect(2);
		break;
	case 10015:
		showSkillEffect(3);
		break;
	}
}

void MRole::dealDamager(char* path, int attack){
	if(buffTime[5] != 0){
		return;
	}
	if(Utils::millisecondNow() - lastHitTime > 500){
		lastHitTime = Utils::millisecondNow();
		string script = Utils::getDataFromFile(path);
		vector<string> v_damage = Utils::split(script, "#");
		for(int i = 0; i < (int)v_damage.size(); i++){
			if(v_damage.at(i).compare("dead") == 0){
				isDestroy = true;
			}else if(v_damage.at(i).compare("hpSub") == 0){
				i++;
				int num = -((int)atoi(v_damage.at(i).c_str()) * (1 - MRole::getCurDef() / 100.0f + attack / 100.0f));
				showDamagerNum(num);
				this->curHp  += num;
				if(this->curHp <= 0){
					this->isDestroy = true;
				}else{
					Utils::playEffect("effect/damage.wav", false);
				}
			}
		}
	}
}

void MRole::showAni(float velocityX, float velocityY){
	int newType = roleAniType;
	if(velocityX == 0 && velocityY == 0){
		if(roleAniType == 0 ){
			newType = 4;
		}else if(roleAniType == 1){
			newType = 5;
		}else if(roleAniType == 2){
			newType = 6;
		}else if(roleAniType == 3){
			newType = 7;
		}
	}else{
		if(velocityX <= 0 && abs(velocityX) >= abs(velocityY)){
			newType = 0;
		}else if(velocityX > 0 && abs(velocityX) >= abs(velocityY)){
			newType = 1;
		}else if(velocityY >= 0 && abs(velocityX) < abs(velocityY)){
			newType = 2;
		}else if(velocityY < 0 && abs(velocityX) < abs(velocityY)){
			newType = 3;
		}
	}
	if(roleAniType == 4 || roleAniType == 5 || roleAniType == 6 || roleAniType == 7){
		moveLatstTime = Utils::millisecondNow();
	}
	if(newType != roleAniType){
		roleAniType = newType;
		this->stopAllActions();
		switch(roleAniType){
		case 0:
			this->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("role_walk_left")));
			break;
		case 1:
			this->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("role_walk_right")));
			break;
		case 2:
			this->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("role_walk_top")));
			break;
		case 3:
			this->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("role_walk_down")));
			break;
		case 4:
			this->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("role_stand_left")));
			break;
		case 5:
			this->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("role_stand_right")));
			break;
		case 6:
			this->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("role_stand_top")));
			break;
		case 7:
			this->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("role_stand_down")));
			break;
		}
	}
}

void MRole::showDamagerNum(int num){
	if(strcmp(damagerNum->getString(),"") !=0 ){
		return;
	}
	CCString* str = CCString::createWithFormat("%d",num);
	ccColor3B color;
	color.r = 255;
	color.g = 0;
	color.b = 0;
	damagerNum->setColor(color);
	damagerNum->setString(str->getCString());
	damagerNum->runAction( CCSequence::create(CCMoveTo::create(0.3f, ccp(damagerNum->getPositionX(), damagerNum->getPositionY() + 20)), 
		CCCallFuncND::create(this,callfuncND_selector(MRole::resetNum), 0),NULL));
}

void MRole::resetNum(CCNode* pSender, void* data){
	damagerNum->setString("");
	damagerNum->setPosition(ccp(60, 80));
}
