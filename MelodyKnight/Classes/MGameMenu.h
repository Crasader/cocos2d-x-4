#ifndef _GAME_MENU_H_
#define _GAME_MENU_H_

#include "cocos2d.h"
#include"AnimatePacker.h"
#include "XMLParser.h"
#include "MWorldMapScene.h"
#include "Utils.h"

class MGameMenu : public cocos2d::CCLayer
{
public:

	virtual bool init();  

    static cocos2d::CCScene* scene();

	void menuCallback(CCObject* sender);

	CREATE_FUNC(MGameMenu);

};

#endif  