#include "GameManager.h"
USING_NS_CC;
using namespace std;

bool GameManager::buffHasChange = false;
bool GameManager::monenyHasChange = false;
bool GameManager::isReleaseCurrentSkill = true;
int GameManager::sectionHard = 0;
int GameManager::curSection = 0;
int GameManager::rate = normalRate;
GameManager* GameManager::instance = new GameManager();
int GameManager::comboCount = 0;
long GameManager::lastComboCheckTime = Utils::millisecondNow();
int GameManager::totalSkillPad[3][5] = {{-1, -1,-1,-1,99},{-1, -1,-1,-1,99},{-1, -1,-1,-1,99}}; 
int GameManager::selectEquipIndex[4] = { -1, -1, -1, -1};
int GameManager::totolEquipBuyIds[12] = { -1, -1, -1, -1,-1, -1, -1, -1,-1, -1, -1, -1};
char GameManager::keySection[] = "key_section";
char GameManager::keyMoney[] = "key_money";
int GameManager::itemSkillId = -1;

GameManager::GameManager(){
	enemysArr = CCArray::createWithCapacity(50);
	skillsArr = CCArray::createWithCapacity(20);
	effectsArr = CCArray::createWithCapacity(50);
	itemsArr = CCArray::createWithCapacity(50);
	buffArray = CCArray::createWithCapacity(4);
	enemysArr->retain();
	buffArray->retain();
	skillsArr->retain();
	itemsArr->retain();
	effectsArr->retain();
	skillCDTime = -1;
	string localSkillId = CCUserDefault::sharedUserDefault()->getStringForKey("skillPad");
	if(localSkillId.compare("") != 0){
		vector<string> skillPadIds = Utils::split(localSkillId,"|");
		int k = 0;
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 4; j++){
				GameManager::totalSkillPad[i][j] = atoi(skillPadIds.at(k).c_str());
				k++;
			}
		}
	}

	string equipIndex = CCUserDefault::sharedUserDefault()->getStringForKey("equipIndex");
	if(equipIndex.compare("") != 0){
		vector<string> equipIndexs = Utils::split(equipIndex,"|");
		int k = 0;
		for(int i = 0; i < 4; i++){
			GameManager::selectEquipIndex[i] = atoi(equipIndexs.at(k).c_str());
			MRole::equipIds[i] =  atoi(equipIndexs.at(k).c_str()) * 10 + i;
			k++;
		}
	}

	string equipBuy = CCUserDefault::sharedUserDefault()->getStringForKey("equipBuy");
	if(equipBuy.compare("") != 0){
		vector<string> equipBuyIds = Utils::split(equipBuy,"|");
		int k = equipBuyIds.size();
		for(int i = 0; i < 12; i++){
			if(i < k){
				GameManager::totolEquipBuyIds[i] = atoi(equipBuyIds.at(i).c_str());
			}
		}
	}
}

void GameManager::setMap(CCTMXTiledMap* map){
	this->map = map;
	this->treeLayer = map->layerNamed("layer2");
	this->gateLayer = map->layerNamed("layer3");
}
int GameManager::isGate(CCPoint point){
	int gateType = -1;
	if(!map || !treeLayer){
		return -1;
	}
	int tileGid  = (int)gateLayer->tileGIDAt(point);
	if(tileGid != 0){
		CCDictionary* properties  = map->propertiesForGID(tileGid);
		if(properties){
			const CCString* type = properties->valueForKey("type");
			if(type->compare("left") == 0){
				gateType =  0;
			}else if(type->compare("right") == 0){
				gateType =  2;
			}else if(type->compare("up") == 0){
				gateType =  1;
			}else if(type->compare("bottom") == 0){
				gateType =  3;
			}
		}
	}
	return gateType;
}

bool GameManager::isObstruct(CCPoint point){
	if(!map || !treeLayer){
		return false;
	}
	int tileGid  = (int)treeLayer->tileGIDAt(point);
	if(tileGid != 0){
		CCDictionary* properties  = map->propertiesForGID(tileGid);
		if(properties){
			const CCString* type = properties->valueForKey("type");
			if(type->compare("tree") == 0){
				return true;
			}
		}
	}
	return false;
}

void GameManager::buyEquip(int index){
	string locaEquipIds;
	boolean isBuy = false;
	for(int i = 0; i < 12; i++){
		if(totolEquipBuyIds[i] == -1 && !isBuy){
			totolEquipBuyIds[i] = index;
			isBuy = true;
		}
		char c[7];
		itoa(GameManager::totolEquipBuyIds[i],c,10);
		locaEquipIds.append(c).append("|");
	}
	CCUserDefault::sharedUserDefault()->setStringForKey("equipBuy",locaEquipIds);
}

void GameManager::setSkillPad(int x, int y, int id){
	if(x < 3 && y < 4){
		GameManager::totalSkillPad[x][y] = id;
		string localSkillId;
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 4; j++){
				char c[7];
				itoa(GameManager::totalSkillPad[i][j],c,10);
				localSkillId.append(c).append("|");
			}
		}
		CCUserDefault::sharedUserDefault()->setStringForKey("skillPad",localSkillId);
	}
}

void GameManager::setEquip(int x, int y){
	if(x < 3 && y < 4){
		GameManager::selectEquipIndex[y] = x;
		string locaEquipSelect;
		for(int i = 0; i < 4; i++){
			char c[7];
			itoa(GameManager::selectEquipIndex[i],c,10);
			locaEquipSelect.append(c).append("|");
		}
		CCUserDefault::sharedUserDefault()->setStringForKey("equipIndex",locaEquipSelect);
	}
}

void GameManager::setRole(MRole *role){
	this->role = role;
}

MRole* GameManager::getRole(){
	return this->role;
}

int GameManager::getMoney(){
	int money = CCUserDefault::sharedUserDefault()->getIntegerForKey("keyMoney");
	return money;
}

void GameManager::addMoney(int money){
	int oldMoney = CCUserDefault::sharedUserDefault()->getIntegerForKey("keyMoney");
	CCUserDefault::sharedUserDefault()->setIntegerForKey("keyMoney",oldMoney + money);
	monenyHasChange = true;
}

GameManager* GameManager::sharedGameManager(){
	if(!instance){
		instance = new GameManager();
	}
	return instance;
}

CCArray* GameManager::getEnemysArr(){
	if(!enemysArr){
		enemysArr = CCArray::createWithCapacity(50);
		enemysArr->retain();
	}
	return enemysArr;
}

CCArray* GameManager::getEffectsArr(){
	if(!effectsArr){
		effectsArr = CCArray::createWithCapacity(50);
		effectsArr->retain();
	}
	return effectsArr;
}

CCArray* GameManager::getSkillsArr(){
	if(!skillsArr){
		skillsArr = CCArray::createWithCapacity(20);
		skillsArr->retain();
	}
	return skillsArr;
}

CCArray* GameManager::getItemsArr(){
	if(!itemsArr){
		itemsArr = CCArray::createWithCapacity(20);
		itemsArr->retain();
	}
	return itemsArr;
}

void GameManager::setSkills(CCArray* skillArr){
	CCObject *objItem;
	CCARRAY_FOREACH(skillArr, objItem){
		MSkill* skill = static_cast<MSkill*>(objItem);
		getSkillsArr()->addObject(skill);
	}
}

void GameManager::checkCombo(){
	if(Utils::millisecondNow() - lastComboCheckTime > 1000){
		lastComboCheckTime = Utils::millisecondNow();
		GameManager::comboCount = 0;
	}
}

void GameManager::checkRate(){
	if(Utils::millisecondNow() - role->moveLatstTime > 3000){
		rate = maxRate;
	}else{
		rate = normalRate;
	}
}

void GameManager::checkBuff(){
	bool isChange = false;
	for(int i = 0; i < 6; i++){
		if(MRole::buffTime[i] != 0){
			if(Utils::millisecondNow() - MRole::buffTime[i] > 10000){
				if(i < 4){
					for(int j = 0; j < (int)buffArray->count(); j ++){
						int type = static_cast<CCInteger*>(buffArray->objectAtIndex(j))->getValue();
						if(type == i){
							buffArray->removeObjectAtIndex(j);
						}
					}
					isChange = true;
				}
				MRole::buffTime[i] = 0;
			}
		}
	}
	buffHasChange = isChange;
}

void GameManager::update(){
	checkCombo();
	checkRate();
	checkBuff();
	if(skillsArr->count() > 0){
		MSkill *skill = static_cast<MSkill* >(getSkillsArr()->lastObject());
		if(skillCDTime == -1 || Utils::millisecondNow() - lastSkillTime >skillCDTime * 1000){
			MSkill* skill = static_cast<MSkill*>(skillsArr->objectAtIndex(0));
			if(MRole::buffTime[3] != 0){
				skill->mp = (int)skill->mp * (1 - 0.2);
			}
			if(role->isFreeCost && skill->skillType == MSkill::ROLE_TYPE){
				skill->mp = 0;
				role->isFreeCost = false;
			}
			if(role->curMp >= skill->mp || skill->skillType != MSkill::ROLE_TYPE){
				skillCDTime = skill->updateTime;
				lastSkillTime = Utils::millisecondNow();
				skill->update();
				if(role->isFreeCost && skill->skillType == MSkill::ROLE_TYPE){
					role->curMp -= skill->mp;
				}
				skill->target->showTargetEffet(skill->id);
			}
			skillsArr->removeObject(skill);
		}
	}

	for(int i = 0; i < (int)effectsArr->count(); i++){
		MEffect* effect = static_cast<MEffect*>(effectsArr->objectAtIndex(i));
		effect->update();
	}
	for(int i = 0; i < (int)enemysArr->count(); i++){
		Menemy* enemy = static_cast<Menemy*>(enemysArr->objectAtIndex(i));
		enemy->update();
	}
	for(int i = 0; i < (int)itemsArr->count(); i++){
		MItem* item = static_cast<MItem*>(itemsArr->objectAtIndex(i));
		item->update();
	}
}

void GameManager::release(){
	enemysArr->removeAllObjects();
	skillsArr->removeAllObjects();
	effectsArr->removeAllObjects();
	buffArray->removeAllObjects();
	itemsArr->removeAllObjects();
}

CCSize GameManager::getWindowSize(){
	return CCDirector::sharedDirector()->getWinSize();
}

CCSize GameManager::getMapSize(){
	return mapSize;
}

void GameManager::setMapSize(CCSize size){
	mapSize = size;
}
