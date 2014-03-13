#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include"FairyRes.h"
#include"FairyControlPanel.h"
#include "SimpleAudioEngine.h"

class HelloWorld : public cocos2d::CCLayer
{
public:

	virtual bool init();  

    static cocos2d::CCScene* scene(int section);
    
    void menuCloseCallback(CCObject* pSender);

	 void doClickButton(int tag);

	 void doScollMap();

	 void initHero();

	 cocos2d::CCPoint mapLocationToGame(cocos2d::CCPoint point);

	 cocos2d::CCPoint gameLocationToMap(cocos2d::CCPoint point);

	 CREATE_FUNC(HelloWorld);
};

#endif  // __HELLOWORLD_SCENE_H__