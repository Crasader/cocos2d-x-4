#ifndef _MainScene_SCENE_H_
#define _MainScene_SCENE_H_

#include "cocos2d.h"
#include "SneakyButton.h"
#include "SneakyButtonSkinnedBase.h"
#include "SneakyJoystick.h"
#include "SneakyJoystickSkinnedBase.h"
#include "AnimatePacker.h"
#include "MWorldMapScene.h"
#include "Utils.h"
#include "MRole.h"
#include "Menemy.h"
#include "MItem.h"
#include "HelloWorldScene.h"
#include "SkillManager.h"
#include "GameManager.h"

class MmainScene: public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
	void skillCallback(CCObject* sender);
	void controlSkillCallback(CCObject* sender);
	void returnBack(CCObject* sender);
	void menuCallback(CCObject* sender);
	void removeEnemy(CCNode* pSender,void* data);
	void initTouchPad();
	void initRole();
	void initMap();
	void initBuff();
	void loadMapInfo();
	void initArray();
	void initSkillSlot();
	void initHit();
	void updateGame(float t);
	void updateRole(float velocityX, float velocityY);
	void ShowItemSkill();
	void showShock();
	void showStandstill();
	void updateBuff();
	void updateSKillProgram(float dely);
	void checkGameCollision();
	void exitMap(CCNode* pSender,void* data);
	void switchPad();
	cocos2d::CCPoint getRoleBornPoint(int type);
	virtual void onEnter();
	virtual void onExit();
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	SneakyJoystick *joystick;
	cocos2d::CCSprite* hit;
	cocos2d::CCLabelTTF *comboCount;
	cocos2d::CCLabelTTF *moneyCount;
	cocos2d::CCLayerColor *shadow;
	cocos2d::CCSprite* headBar;
	MRole* role;
	cocos2d::CCLayer *spriteLayer;
	cocos2d::CCLayer *buffLayer;
	cocos2d::CCLayer *hitLayer;
	cocos2d::CCProgressTimer *mpBar;
	cocos2d::CCProgressTimer *hpBar;
	cocos2d::CCMenu* skillMenu;
	cocos2d::CCMenu* controlSkillMenu;
	std::vector<std::string> enemyInfo;
	std::vector<std::string> gateInfo;
	static int currentSection;
	static int roleBornPointType;
	long attickLastTime;
	int comBoCount;
	bool hasBigSkill;
	cocos2d::CCTMXTiledMap* map;
	cocos2d::CCTMXLayer* treeLayer;
	const static int SKILL_BIG_LAYER_TAG = 9;
	const static int SKILL_CONTROL_LAYER_TAG = 10;
	cocos2d::CCSize size;
	cocos2d::CCParticleFlower* particle;
	//cocos2d::CCArray* gateArray;
	SneakyJoystickSkinnedBase *joystickSkin;
    // implement the "static node()" method manually
    CREATE_FUNC(MmainScene);
	
};

#endif