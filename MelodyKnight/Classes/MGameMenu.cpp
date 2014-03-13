#include "MGameMenu.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* MGameMenu::scene()
{
    CCScene * scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        MGameMenu *layer = MGameMenu::create();
        CC_BREAK_IF(! layer);
        scene->addChild(layer);
    } while (0);
    return scene;
}

bool MGameMenu::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());
		AnimatePacker::getInstance()->loadAnimations("menu/loading.xml"); 
		Utils::playMusic("music/menu.mp3", true);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCSprite *fairyMenuBack = CCSprite::create("menu/menu_back.png");
		this->addChild(fairyMenuBack);
		fairyMenuBack->setPosition(ccp(size.width / 2, size.height / 2));
		CCSprite *fairyMenuMenu = CCSprite::create("menu/menu_menu.png");
		this->addChild(fairyMenuMenu);
		fairyMenuMenu->setPosition(ccp(size.width / 2, size.height / 2));
		CCSprite *menuLoading=CCSprite::create();
		this->addChild(menuLoading);
		menuLoading->setPosition(ccp(size.width/2, size.height/2 + 160));
		menuLoading->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("loading")));
		int itemPosition[3][2]  = {{5, 10 }, {5, -105}, {5, 0 - 210}};
		CCMenu* menu = CCMenu::create();
		for(int i =0; i < 3; i++){
			CCSprite* fairyMenuButton = CCSprite::create("menu/menu_item.png");
			CCMenuItemSprite* fairyMenuButtonItem = CCMenuItemSprite::create(fairyMenuButton, fairyMenuButton, this, menu_selector(MGameMenu::menuCallback));
			fairyMenuButtonItem->setPosition(ccp(itemPosition[i][0] ,itemPosition[i][1]));
			fairyMenuButtonItem->setTag(i + 1);
			menu->addChild(fairyMenuButtonItem);
		}
		this->addChild(menu);
		CCLabelTTF* menuStart = CCLabelTTF::create(Utils::getString("menu_start"), "Arial", 30);
		this->addChild(menuStart);
		menuStart->setPosition(ccp(size.width/2, size.height/2 + 10));
		CCLabelTTF* menuIntro = CCLabelTTF::create(Utils::getString("menu_intro"), "Arial", 30);
		this->addChild(menuIntro);
		menuIntro->setPosition(ccp(size.width/2, size.height/2 - 104));
		CCLabelTTF* menuExit = CCLabelTTF::create(Utils::getString("menu_exit"), "Arial", 30);
		this->addChild(menuExit);
		menuExit->setPosition(ccp(size.width/2, size.height/2 - 210));
        bRet = true;
    } while (0);

    return bRet;
}

void MGameMenu::menuCallback(CCObject* sender){
	CCMenuItemSprite* itemSprite = (CCMenuItemSprite *)sender;
	int itemTag = itemSprite->getTag();
	SimpleAudioEngine::sharedEngine()->end();
	CCScene* pScene = MWorldMapScene::scene();
	switch(itemTag){
	case 1:
		CCDirector::sharedDirector()->replaceScene(pScene);
		break;
	case 3:
		CCDirector::sharedDirector()->end();
		break;
	}
}