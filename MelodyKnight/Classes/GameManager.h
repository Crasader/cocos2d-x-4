#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "cocos2d.h"
#include "Menemy.h"
#include "MSkill.h"
#include "MEffect.h"
#include "MRole.h"
#include "MItem.h"
#include "Utils.h"

class GameManager
{
public:
	GameManager();
	static GameManager* sharedGameManager();
	static GameManager* instance;
	static int comboCount;
	cocos2d::CCSize getMapSize();
	void setMapSize(cocos2d::CCSize size);
	void setRole(MRole *role);
	MRole* getRole();
	cocos2d::CCArray* getEnemysArr();
	cocos2d::CCArray* getEffectsArr();
	cocos2d::CCArray* getSkillsArr();
	cocos2d::CCArray* getItemsArr();
	void setSkills(cocos2d::CCArray* skillArr);
	void setMap(cocos2d::CCTMXTiledMap* map);
	void checkCombo();
	void checkRate();
	void checkBuff();
	void update();
	void release();
    void setSkillPad(int x, int y, int id);
	void setEquip(int x, int y);
	void buyEquip(int index);
	int getMoney();
	void addMoney(int money);
	bool isObstruct(cocos2d::CCPoint point);
	int isGate(cocos2d::CCPoint point);
	MRole* role;
	cocos2d::CCSize mapSize;
	cocos2d::CCTMXTiledMap* map;
	cocos2d::CCTMXLayer* treeLayer;
	cocos2d::CCTMXLayer* gateLayer;
	cocos2d::CCSize getWindowSize();
	cocos2d::CCArray* effectsArr;
	cocos2d::CCArray* enemysArr;
	cocos2d::CCArray* skillsArr;
	cocos2d::CCArray* itemsArr;
	cocos2d::CCArray* buffArray;
	static bool buffHasChange;
	static bool monenyHasChange;
	static long lastComboCheckTime;
	static bool isReleaseCurrentSkill;
	static int totalSkillPad[3][5];
	static int sectionHard;
	static int curSection;
	static int selectEquipIndex[4];
	static int totolEquipBuyIds[12];
	static int rate;
	const static int maxRate = 20;
	const static int normalRate = 10;
	static char keySection[20];
	static char keyMoney[20];
	static int itemSkillId;
	int skillCDTime;
	long lastSkillTime;
};

#endif