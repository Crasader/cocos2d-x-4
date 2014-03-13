#ifndef __WORLDMAP_SCENE_H__
#define __WORLDMAP_SCENE_H__

#include "cocos2d.h"
#include "MskillSelectLayer.h"
#include "MEquipSelectLayer.h"
#include "SimpleAudioEngine.h"
#include "MRole.h"
#include "MSectionSelectLayer.h"
#include "Utils.h"
#include "MToast.h"

class MWorldMapScene : public cocos2d::CCLayer
{
public:
    virtual bool init();
	void initScript();
	void initMap();
	void initRole();
	void initSign();
	void initButton();
	void initSection(CCNode* pSender);
	void gotoSection(CCObject* sender);
	void buttonSelect(cocos2d::CCSprite* sender);
	void showSkillSelect();
	void showeQuipSelect();
	void changeMap(int direction);
	void updateRole(float dely);
    static cocos2d::CCScene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	CREATE_FUNC(MWorldMapScene);
	cocos2d::CCSize size;
	cocos2d::CCPoint endPoint;
	MRole *role;
	const static int roleRate = 5;
	const static int cotainRect = 10;
	int mapIndex;
	std::vector<cocos2d::CCPoint> pointVec;
	std::vector<bool> pointIsBoss;
	std::vector<int> sectionIndex;
	cocos2d::CCSprite *skillSelect;
	cocos2d::CCSprite *equipSelect;
	cocos2d::CCSprite *worldMap;
	cocos2d::CCSprite *left;
	cocos2d::CCSprite *right;
	bool isShowMenu;
};

#endif 
