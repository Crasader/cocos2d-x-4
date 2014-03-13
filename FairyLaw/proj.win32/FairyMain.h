#ifndef __FAIRY_MAIN_H__
#define __FAIRY_MAIN_H__

#pragma once

#include"cocos2d.h"
#include"FairyRes.h"
#include"FairyControlPanel.h"
#include"FairyHeroManager.h"
#include"SimpleAudioEngine.h"
#include"FairyArea.h"
#include"FairyMap.h"
#include"FairySkill.h"
#include"FairyTitlePanel.h"
#include"FairyTitle.h"
#include"FairyChat.h"
#include"AnimatePacker.h"

class FairyMain : public cocos2d::CCLayer
{
public:

	virtual bool init();  

	virtual void onEnter();

	virtual void onExit();

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouch, cocos2d::CCEvent *pEvent);
	
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouch, cocos2d::CCEvent *pEvent);
    
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouch, cocos2d::CCEvent *pEvent);

    static cocos2d::CCScene* scene(int section);
    
    void menuCloseCallback(CCObject* pSender);

	 void doClickButton(int tag);
	 
	 void doMoveHero(int tag);

	 void showMoveArea(FairyHero* selectHero);

	 void cleanMoveArea();

	 void updateFairyStatic(float t);

	 void doAttactEnemy();

	 void doAttactHero();

	 void doEnemyAI();

	 bool isCanMove();

	 void handleResult(bool isWin);

	 CREATE_FUNC(FairyMain);

	 FairyHeroManager* fairyHeroManager;

	 FairyHero* selectHero;

	 cocos2d::CCPoint selectPoint;

	 FairyTitlePanel* titlePanel;

	 FairyArea* areaLayer;

	 FairyMap* mapLayer;

	 bool isCanScoll;

	 static int currentStatic;

	 int enemyIndex;

	 int heroIndex;

	 int section;

	 int moveCount;
	 
	 int resultCount;
};

#endif  