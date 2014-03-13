#ifndef _FAIRY_TITLE_H_
#define _FAIRY_TITLE_H_

#include "cocos2d.h"
#include"FairyRes.h"
#include "FairyMain.h"
#include "FairyChat.h"
#include "SimpleAudioEngine.h"

class FairyTitle : public cocos2d::CCLayer
{
public:

	virtual bool init();  

    static cocos2d::CCScene* scene();

	void initSection(cocos2d::CCSize size, cocos2d::CCTexture2D* sectionTexture);

	int getCurSection(int section);

	void fairyMenuCallback(CCObject* sender);

	CREATE_FUNC(FairyTitle);

	int defSection;

};

#endif  
