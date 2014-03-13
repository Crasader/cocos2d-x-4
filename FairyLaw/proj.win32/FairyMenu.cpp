#include "FairyMenu.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* FairyMenu::scene()
{
    CCScene * scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        FairyMenu *layer = FairyMenu::create();
        CC_BREAK_IF(! layer);
        scene->addChild(layer);
    } while (0);
    return scene;
}

bool FairyMenu::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());
		AnimatePacker::getInstance()->loadAnimations("skill/loading.xml"); 
		CCString *fileName = CCString::createWithFormat(BattleMusic, 2);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(fileName->getCString(), true);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCSprite *fairyMenuBack = CCSprite::create(menuBack);
		this->addChild(fairyMenuBack);
		fairyMenuBack->setPosition(ccp(size.width / 2, size.height / 2));
		CCSprite *fairyMenuMenu = CCSprite::create(menuMenu);
		this->addChild(fairyMenuMenu);
		fairyMenuMenu->setPosition(ccp(size.width / 2, size.height / 2 - 100));
		CCSprite *menuLoading=CCSprite::create();
		this->addChild(menuLoading);
		menuLoading->setPosition(ccp(size.width/2, size.height/2 + 60));
		menuLoading->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("loading")));
		int itemPosition[3][2]  = {{5, 0 - 90 }, {5, -205}, {5, 0 - 310}};
		XMLParser *scene = XMLParser::parseWithFile("data/common.xml");
		CCString* menuStart = scene->getString("menu_start");
		CCString* menuIntro = scene->getString("menu_intro");
		CCString* menuExit = scene->getString("menu_exit");
		CCMenu* menu = CCMenu::create();
		for(int i =0; i < 3; i++){
			CCSprite* fairyMenuButton = CCSprite::create(menuItem);
			CCMenuItemSprite* fairyMenuButtonItem = CCMenuItemSprite::create(fairyMenuButton, fairyMenuButton, this, menu_selector(FairyMenu::fairyMenuCallback));
			fairyMenuButtonItem->setPosition(ccp(itemPosition[i][0] ,itemPosition[i][1]));
			fairyMenuButtonItem->setTag(i + 1);
			menu->addChild(fairyMenuButtonItem);
		}
		this->addChild(menu);
		CCLabelTTF* fairyMenuStart = CCLabelTTF::create(menuStart->getCString(), "Arial", 30);
		this->addChild(fairyMenuStart);
		fairyMenuStart->setPosition(ccp(size.width/2, size.height/2 - 90));
		CCLabelTTF* fairyMenuIntro = CCLabelTTF::create(menuIntro->getCString(), "Arial", 30);
		this->addChild(fairyMenuIntro);
		fairyMenuIntro->setPosition(ccp(size.width/2, size.height/2 - 204));
		CCLabelTTF* fairyMenuExit = CCLabelTTF::create(menuExit->getCString(), "Arial", 30);
		this->addChild(fairyMenuExit);
		fairyMenuExit->setPosition(ccp(size.width/2, size.height/2 - 310));
        bRet = true;
    } while (0);

    return bRet;
}

void FairyMenu::fairyMenuCallback(CCObject* sender){
	CCMenuItemSprite* itemSprite = (CCMenuItemSprite *)sender;
	int itemTag = itemSprite->getTag();
	SimpleAudioEngine::sharedEngine()->end();
	CCScene* pScene = FairyTitle::scene();
	switch(itemTag){
	case 1:
		CCDirector::sharedDirector()->replaceScene(pScene);
		break;
	case 3:
		CCDirector::sharedDirector()->end();
		break;
	}
}