#ifndef _FAIRY_CONTROL_H_
#define _FAIRY_CONTROL_H_

#pragma once

#include "cocos2d.h"
#include"FairyRes.h"
#include "FairyMain.h"
#include"FairyHero.h"

class FairyControlPanel : public cocos2d::CCLayer
{
public:

	virtual bool init();  

	void initButton();

	void initHero(FairyHero* selectHero);

	void fairyMenuCallback(CCObject* sender);

	void finishDoClick(CCNode* pSender);

	void setInvisiable();

	void setVisiable();

	void playAni();

	void changeControlPanel(FairyHero* selectHero);

	CREATE_FUNC(FairyControlPanel);

	static bool isDoClick;

	const static int leftDrection = 0;

	const static int upDrection = 1;

	const static int holdDrection = 2;

	const static int rightDrection = 3;

	int controlType;

	cocos2d::CCSprite* panelBack;

};

#endif  