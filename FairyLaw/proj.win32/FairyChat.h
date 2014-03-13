#ifndef _FAIRY_CHAT_H_
#define _FAIRY_CHAT_H_

#include "Utils.h"
#include "cocos2d.h"
#include"FairyRes.h"
#include"FairyMain.h"
#include "SimpleAudioEngine.h"

class FairyChat : public cocos2d::CCLayer
{
public:

	virtual bool init();  

	virtual void onEnter();

	virtual void onExit();

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouch, cocos2d::CCEvent *pEvent);
	
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouch, cocos2d::CCEvent *pEvent);
    
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouch, cocos2d::CCEvent *pEvent);

    static cocos2d::CCScene* scene(int section, int status);

	 CREATE_FUNC(FairyChat);

	 void initCommands();

	 void doCommand(std::vector<std::string> commandParams);

	 cocos2d::CCSprite* back;

	 cocos2d::CCSprite* dialogUp;

	 cocos2d::CCSprite* dialogDown;

	 int commandIndex;

	 std::vector<std::string> command;

	 XMLParser *chatParser;

	int section;

	int status;
};

#endif  