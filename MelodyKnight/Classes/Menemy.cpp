#include "Menemy.h"

USING_NS_CC;
using namespace std;
const static float warp[3] ={0, 10.0f, 20.0f};

Menemy* Menemy::create(int id, int x, int y,  int speedx, int speedy, string itemDescript){
	Menemy *enemy = new Menemy();
	if(enemy && enemy->init()){
		enemy->initEnemy(id, x, y, speedx, speedy, itemDescript);
		enemy->autorelease();
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
    return NULL;
}

bool Menemy::initEnemy(int id, int x, int y, int speedx, int speedy, string itemDescript){
	this->id = id;
	CCString* data_string = CCString::createWithFormat("enemy/enemy_data_%d.xml", id);
	curHp = maxHp = atoi(Utils::parseXML(data_string->getCString(), "maxHp")) * (1 + warp[GameManager::sectionHard]);
	attackPoint = atoi(Utils::parseXML(data_string->getCString(), "attackPoint")) * (1 + warp[GameManager::sectionHard]);
	defPoint = atoi(Utils::parseXML(data_string->getCString(), "defPoint")) * (1 + warp[GameManager::sectionHard]);
	rate = atoi(Utils::parseXML(data_string->getCString(), "rate"));
	isRadomAttack = atoi(Utils::parseXML(data_string->getCString(), "isRadomAttack")) == 1;
	hitDistanceX = atoi(Utils::parseXML(data_string->getCString(), "hitDistanceX"));
	hitDistanceY =  atoi(Utils::parseXML(data_string->getCString(), "hitDistanceY"));
	searchDis =  atoi(Utils::parseXML(data_string->getCString(), "searchDis"));
	CCString* string = CCString::createWithFormat("enemy_walk_right_%d", id);
	this->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate(string->getCString())));
	this->x = x;
	this->y = y;
	this->speedx = speedx;
	this->speedy = speedy;
	this->itemDescript = itemDescript;
	this->setScale(1.5f);
	roleAniType = 0;
	tempDes = CCPointMake(CCRANDOM_0_1() * 840 + 120, CCRANDOM_0_1() * 480 + 120);
	lastHitTime = Utils::millisecondNow();
	lastAttackTime = Utils::millisecondNow();
	damagerNum = CCLabelTTF::create("", "Arial", 30);
	this->addChild(damagerNum);
	damagerNum->setPosition(ccp(60, 80));
	switch(id){
	case 10001:
	case 10002:
	case 10003:
	case 10004:
		aiStatus = 1;
		break;
	}
	return true;
}

void Menemy::update(){
	enemyAI();
	if(isDestroy){
		Utils::playEffect("effect/boom.wav",false);
		GameManager::sharedGameManager()->getEnemysArr()->removeObject(this);
		this->removeFromParentAndCleanup(true);
	}
}

void Menemy::moveTo(CCPoint rolePoint){
	MRole* role = GameManager::sharedGameManager()->getRole();
	if(this->getOpacity() == 255){
		showAni(this->speedx, this->speedy);
	}
	CCPoint startPosition= this->getPosition();
	float angle = Utils::getAngle(startPosition, rolePoint);
	this->speedx = Utils::getSpeedX(rate, angle);
	this->speedy = Utils::getSpeedY(rate, angle);
	if(GameManager::sharedGameManager()->isObstruct(ccp((int)((this->x + speedx + 60) / 120), 
		(int)((720 - this->y - speedy ) / 120)))){
		return;
	}
	if(this->getOpacity() > 0){
		this->x = this->x + speedx;
		this->y = this->y + speedy;
		this->setPosition(ccp(x, y));
	}
}

void Menemy::moveXY(CCPoint rolePoint){
	if(this->getOpacity() == 255){
		showAni(this->speedx, this->speedy);
	}
	CCPoint startPosition= this->getPosition();
	if(abs(startPosition.x - rolePoint.x) < abs(startPosition.y - rolePoint.y)){
		if(abs(startPosition.x - rolePoint.x) < 50){
			return;
		}
		if(startPosition.x > rolePoint.x){
			speedx = -rate;
		}else if(startPosition.x <  rolePoint.x){
			speedx = rate;
		}
		speedy = 0;
	}else{
		if(abs(startPosition.y - rolePoint.y) < 50){
			return;
		}
		if(startPosition.y > rolePoint.y){
			speedy = -rate;
		}else{
			speedy = rate;
		}
		speedx = 0;
	}
	if(GameManager::sharedGameManager()->isObstruct(ccp((int)((this->x + speedx + 60) / 120), 
		(int)((720 - this->y - speedy) / 120)))){
		return;
	}
	if(this->getOpacity() > 0){
		this->x = this->x + speedx;
		this->y = this->y + speedy;
		this->setPosition(ccp(x, y));
	}
}

void Menemy::stand(){
	if(roleAniType != 0){
		roleAniType = 0;
		this->stopAllActions();
		CCString* string = CCString::createWithFormat("enemy_walk_right_%d", id);
		this->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate(string->getCString())));
		speedx = 0;
		speedy = 0;
		if(this->getOpacity() > 0){
			this->x = this->x + speedx;
			this->y = this->y + speedy;
			this->setPosition(ccp(x, y));
		}
	}
}

void Menemy::enemyAI(){
	switch(this->id){
	case 10001:
	case 10002:
	case 10003:
		ai10001();
		break;
	case 10004:
		ai10003();
		break;
	}
}

void Menemy::ai10003(){
	speedx = 0;
	speedy = 0;
	if(this->getOpacity() > 0){
		this->x = this->x + speedx;
		this->y = this->y + speedy;
		this->setPosition(ccp(x, y));
	}
	if(this->aiStatus == -1){
		return;
	}
	switch(this->aiStatus){
	case 0:
	case 1:
	case 2:
		fight1002();
		break;
	}
}

void Menemy::ai10002(){
	if(this->aiStatus == -1){
		return;
	}
	MRole* role = GameManager::sharedGameManager()->getRole();
	switch(this->aiStatus){
	case 0:
		moveXY(GameManager::sharedGameManager()->getRole()->getPosition());
		break;
	case 1:
		moveTo(tempDes);
		break;
	case 2:
		if(isRadomAttack || (abs(this->getPositionX() - role->getPositionX()) < hitDistanceX && abs(this->getPositionY() - role->getPositionY()) < 50)
			|| (abs(this->getPositionX() - role->getPositionX()) < 50 && abs(this->getPositionY() - role->getPositionY()) < hitDistanceY)){
			fight1001();
		}else{
			checkStatus10001();
		}
		break;
	}
	if((this->aiStatus == 0) && 
		(abs(this->getPositionX() - role->getPositionX()) < hitDistanceX && abs(this->getPositionY() - role->getPositionY()) < 50)
		|| (abs(this->getPositionX() - role->getPositionX()) < 50 && abs(this->getPositionY() - role->getPositionY()) < hitDistanceY)){
		this->aiStatus = 2;
	}else if(this->aiStatus == 1 && abs(this->getPositionX() - tempDes.x) < 50 && abs(this->getPositionY() - tempDes.y) < 50){
		checkStatus10001();
	}
}

void Menemy::ai10001(){
	if(this->aiStatus == -1){
		return;
	}
	MRole* role = GameManager::sharedGameManager()->getRole();
	switch(this->aiStatus){
	case 0:
		moveTo(GameManager::sharedGameManager()->getRole()->getPosition());
		break;
	case 1:
		moveTo(tempDes);
		break;
	case 2:
		if(isRadomAttack || abs(this->getPositionX() - role->getPositionX()) < hitDistanceX && abs(this->getPositionY() - role->getPositionY()) < hitDistanceY){
			fight1001();
		}else{
			checkStatus10001();
		}
		break;
	}
	if((this->aiStatus == 0) && abs(this->getPositionX() - role->getPositionX()) < hitDistanceX && abs(this->getPositionY() - role->getPositionY()) < hitDistanceY){
		this->aiStatus = 2;
	}else if(this->aiStatus == 1 && abs(this->getPositionX() - tempDes.x) < 50 && abs(this->getPositionY() - tempDes.y) < 50){
		checkStatus10001();
	}
}

void Menemy::fight1001(){
	if(Utils::millisecondNow() - lastAttackTime > 1000){
		lastAttackTime = Utils::millisecondNow();
		this->aiStatus = -1;
		MRole* role = GameManager::sharedGameManager()->getRole();
		this->stopAllActions();
		CCString* string;
		if(this->getPositionX() >= role->getPositionX() && abs(this->getPositionX() - role->getPositionX()) >=
			abs(this->getPositionY() - role->getPositionY())){
			roleAniType  = 4;
			string = CCString::createWithFormat("enemy_fight_left_%d", id);
		}else if(this->getPositionX() < role->getPositionX() && abs(this->getPositionX() - role->getPositionX()) >=
			abs(this->getPositionY() - role->getPositionY())){
			roleAniType  = 5;
			string = CCString::createWithFormat("enemy_fight_right_%d", id);
		}else if(this->getPositionY() >= role->getPositionY() && abs(this->getPositionX() - role->getPositionX()) <
			abs(this->getPositionY() - role->getPositionY())){
			roleAniType  = 6;
			string = CCString::createWithFormat("enemy_fight_bottom_%d", id);
		}else if(this->getPositionY() < role->getPositionY() && abs(this->getPositionX() - role->getPositionX()) <
			abs(this->getPositionY() - role->getPositionY())){
			roleAniType  = 7;
			string = CCString::createWithFormat("enemy_fight_top_%d", id);
		}else{
			string = CCString::createWithFormat("stand");
		}
		if(string->compare("stand") != 0){
			this->runAction(CCSequence::create(AnimatePacker::getInstance()->getAnimate(string->getCString()),
					CCCallFuncND::create(this,callfuncND_selector(Menemy::dealManager), 0),NULL));
			GameManager::sharedGameManager()->getRole()->dealDamager("damage/damage1.res", attackPoint);
		}
		showSkill(10050);
	}else{
		checkStatus10001();
	}
}

void Menemy::fight1002(){
	if(Utils::millisecondNow() - lastAttackTime > 1000){
		lastAttackTime = Utils::millisecondNow();
		this->aiStatus = -1;
		roleAniType = 1;
		this->stopAllActions();
		CCString* string = CCString::createWithFormat("enemy_fight_right_%d", id);
		this->runAction(CCSequence::create(AnimatePacker::getInstance()->getAnimate(string->getCString()),
					CCCallFuncND::create(this,callfuncND_selector(Menemy::dealManager), 0),NULL));
		showSkill(10051);
	}
}

void Menemy::dealManager(CCNode* pSender,void* data){
	checkStatus10001();
}

void Menemy::checkStatus10001(){
	MRole* role = GameManager::sharedGameManager()->getRole();
	int temp =(int) (CCRANDOM_0_1() * 3);
	if(temp == 0){
		if(searchDis < abs(this->getPositionX() - role->getPositionX()) +  abs(this->getPositionY() - role->getPositionY())){
			temp = 1;
		}
	}else if(temp == 1){
		float x = CCRANDOM_0_1() * 840 + 120;
		float y = CCRANDOM_0_1() * 480 + 120;
		tempDes = CCPointMake(x, y);
	}else if(temp == 3){
		temp = 0;
	}
	aiStatus = temp;
}

void Menemy::showAni(int speedx, int speedy){
	int newType = roleAniType;
	if(speedx != 0 || speedy != 0){
		if(speedx <= 0 && abs(speedx) >= abs(speedy)){
			newType = 0;
		}else if(speedx > 0 && abs(speedx) > abs(speedy)){
			newType = 1;
		}else if(speedy >= 0 && abs(speedx) <= abs(speedy)){
			newType = 2;
		}else if(speedy < 0 && abs(speedx) < abs(speedy)){
			newType = 3;
		}
	}
	if(newType != roleAniType){
		roleAniType = newType;
		this->stopAllActions();
		CCString* string;
		switch(roleAniType){
		case 0:
			string = CCString::createWithFormat("enemy_walk_left_%d", id);
			break;
		case 1:
			string = CCString::createWithFormat("enemy_walk_right_%d", id);
			break;
		case 2:
			string = CCString::createWithFormat("enemy_walk_top_%d", id);
			break;
		case 3:
			string = CCString::createWithFormat("enemy_walk_bottom_%d", id);
			break;
		default:
			string = CCString::createWithFormat("stand");
			break;
		}
		if(string->compare("stand") != 0){
			this->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate(string->getCString())));
		}
	}
}

void Menemy::showTargetEffet(int skillId){
}

void Menemy::dealDamager(char* path){
	if(Utils::millisecondNow() - lastHitTime > 500){
		lastHitTime = Utils::millisecondNow();
		string script = Utils::getDataFromFile(path);
		vector<string> v_damage = Utils::split(script, "#");
		for(int i = 0; i < (int)v_damage.size(); i++){
			if(v_damage.at(i).compare("dead") == 0){
				isDestroy = true;
			}else if(v_damage.at(i).compare("hpSub") == 0){
				i++;
				int num = -atoi(v_damage.at(i).c_str());
				num = (int)num * (1 + MRole::getCurAttack() / 100.0f - defPoint / 100.0f);
				showDamagerNum(num);
				this->curHp  += num;
				if(this->curHp <= 0){
					this->isDestroy = true;
					vector<string> items = Utils::split(itemDescript, "-");
					for(int i = 0; i < items.size(); i++){
						MItem* item = MItem::create(items.at(i), this->getPositionX(), this->getPositionY());
						item->isShow = false;
						item->isDestroy = false;
						GameManager::sharedGameManager()->getItemsArr()->addObject(item);
					}
				}
				Utils::playEffect("effect/boom.wav",false);
			}
		}
	}
}

void Menemy::showSkill(int type){
	switch(type){
	case 10050:
		showSkill10050();
		break;
	case 10051:
		showSkill10051();
		break;
	}
}

void Menemy::showSkill10050(){
	MRole* role = GameManager::sharedGameManager()->getRole();
	int speedx = 0;
	int speedy = 0;
	if(this->getPositionX() >= role->getPositionX() && abs(this->getPositionX() - role->getPositionX()) >=
			abs(this->getPositionY() - role->getPositionY())){
		speedx = -20;
	}else if(this->getPositionX() < role->getPositionX() && abs(this->getPositionX() - role->getPositionX()) >=
			abs(this->getPositionY() - role->getPositionY())){
		speedx = 20;
	}else if(this->getPositionY() >= role->getPositionY() && abs(this->getPositionX() - role->getPositionX()) <
			abs(this->getPositionY() - role->getPositionY())){
		speedy = -20;
	}else if(this->getPositionY() < role->getPositionY() && abs(this->getPositionX() - role->getPositionX()) <
			abs(this->getPositionY() - role->getPositionY())){
		speedy = 20;
	}
	MSkill* skill = MSkill::create(this,  50,  MSkill::ENEMY_TYPE, speedx, speedy, this->x, this->y, this->attackPoint);
	CCArray* skillArr = CCArray::createWithCapacity(1);
	skillArr->addObject(skill);
	GameManager::sharedGameManager()->setSkills(skillArr);
}

void Menemy::showSkill10051(){
	MSkill* skill = MSkill::create(this,  51,  MSkill::ENEMY_TYPE, 20, 20, this->x, this->y, this->attackPoint);
	CCArray* skillArr = CCArray::createWithCapacity(1);
	skillArr->addObject(skill);
	GameManager::sharedGameManager()->setSkills(skillArr);
}

void Menemy::showDamagerNum(int num){
	CCString* str = CCString::createWithFormat("%d",num);
	ccColor3B color;
	color.r = 255;
	color.g = 0;
	color.b = 0;
	damagerNum->setColor(color);
	damagerNum->setString(str->getCString());
	damagerNum->runAction( CCSequence::create(CCMoveTo::create(0.3f, ccp(damagerNum->getPositionX(), damagerNum->getPositionY() + 20)), 
		CCCallFuncND::create(this,callfuncND_selector(Menemy::resetNum), 0),NULL));
}

void Menemy::resetNum(CCNode* pSender, void* data){
	damagerNum->setString("");
	damagerNum->setPosition(ccp(60, 80));
}
