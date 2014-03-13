#ifndef _FAIRY_MENU_H_
#define _FAIRY_MENU_H_

#include "cocos2d.h"
#include"FairyRes.h"
#include "FairyTitle.h"
#include"AnimatePacker.h"
#include "XMLParser.h"
#include "SimpleAudioEngine.h"

class FairyMenu : public cocos2d::CCLayer
{
public:

	virtual bool init();  

    static cocos2d::CCScene* scene();

	void fairyMenuCallback(CCObject* sender);

	CREATE_FUNC(FairyMenu);

};

#endif  
