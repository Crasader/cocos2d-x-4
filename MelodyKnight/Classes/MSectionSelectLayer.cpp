#include "MSectionSelectLayer.h"
using namespace cocos2d;

bool MSectionSelectLayer::isShowToast = false;
// on "init" you need to initialize your instance
bool MSectionSelectLayer::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////
        CC_BREAK_IF(! CCLayer::init());
		CCSize size = CCDirector::sharedDirector()->getWinSize();

		sectionBack = CCSprite::create("worldMap/section1_back.png");
		sectionBack->setScale(2.0f);
		this->addChild(sectionBack);
		sectionBack->setPosition(ccp(size.width / 2, size.height / 2));
		
		CCSprite *shadow = CCSprite::create("ui/back_black.png");
		shadow->setScaleX(58.0f);
		shadow->setScaleY(31.6f);
		this->addChild(shadow);
		shadow->setPosition(ccp(size.width / 2, size.height / 2));
		shadow->setOpacity(100);

		sectionTitle = CCLabelTTF::create("text", "Arial", 50);
		ccColor3B color;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		sectionTitle->setColor(color);
		sectionTitle->setPosition(ccp(size.width / 2, size.height / 2 + 50));
		this->addChild(sectionTitle);

		CCSprite *frame = CCSprite::create("worldMap/section_frame.png");
		frame->setScale(2.0f);
		this->addChild(frame);
		frame->setPosition(ccp(size.width / 2, size.height / 2));

		initMenu();
        bRet = true;
    } while (0);

    return bRet;
}

void MSectionSelectLayer::initMenu(){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCMenu* typeMenu = CCMenu::create();
	CCMenuItemImage* easyItem = CCMenuItemImage::create("worldMap/section_button.png", "worldMap/section_button.png", this, menu_selector(MSectionSelectLayer::enterSection));
	easyItem->setTag(0);
	easyItem->setScale(2.0f);
	typeMenu->addChild(easyItem);
	easyItem->setPosition(-170, -100);
	CCLabelTTF* easyText = CCLabelTTF::create(Utils::getString("easy_mode"), "Arial", 20);
	easyText->setPosition(ccp(easyItem->getContentSize().width / 2, easyItem->getContentSize().height / 2));
	easyItem->addChild(easyText);

	CCMenuItemImage* normalItem = CCMenuItemImage::create("worldMap/section_button.png", "worldMap/section_button.png", this, menu_selector(MSectionSelectLayer::enterSection));
	normalItem->setTag(1);
	normalItem->setScale(2.0f);
	typeMenu->addChild(normalItem);
	normalItem->setPosition(0, -100);
	CCLabelTTF* normalText = CCLabelTTF::create(Utils::getString("normal_mode"), "Arial", 20);
	normalText->setPosition(ccp(normalItem->getContentSize().width / 2, normalItem->getContentSize().height / 2));
	normalItem->addChild(normalText);

	CCMenuItemImage* hardItem = CCMenuItemImage::create("worldMap/section_button.png", "worldMap/section_button.png", this, menu_selector(MSectionSelectLayer::enterSection));
	hardItem->setTag(2);
	hardItem->setScale(2.0f);
	typeMenu->addChild(hardItem);
	hardItem->setPosition(170, -100);
	CCLabelTTF* hardText = CCLabelTTF::create(Utils::getString("hard_mode"), "Arial", 20);
	hardText->setPosition(ccp(hardItem->getContentSize().width / 2, hardItem->getContentSize().height / 2));
	hardItem->addChild(hardText);

	this->addChild(typeMenu);
}

void MSectionSelectLayer::enterSection(CCObject* sender){
	CCMenuItemImage* selectItem = (CCMenuItemImage*)sender;
	GameManager::sectionHard = selectItem->getTag();
	this->removeFromParentAndCleanup(true);
	isShowToast = false;
	Utils::stopMusic();
	CCScene *pScene = MmainScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void MSectionSelectLayer::setBack(char *back){
	sectionBack->setTexture(CCTextureCache::sharedTextureCache()->addImage(back));
}

void MSectionSelectLayer::setTitleString(const char* text){
	this->setTag(TAG);
	isShowToast = true;
	sectionTitle->setString(text);
}

void MSectionSelectLayer::MTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
}

void MSectionSelectLayer::MTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
}

void MSectionSelectLayer::MTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	this->removeFromParentAndCleanup(true);
	isShowToast = false;
}
