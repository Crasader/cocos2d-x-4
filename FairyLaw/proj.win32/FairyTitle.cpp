#include "FairyTitle.h"

using namespace cocos2d;
using namespace CocosDenshion;

static const int sectionPosition[7][2] = {
	{-140,-150},{170,200},{160,-60},{20,350},{-140,200},{140,-280},{100,50}
};

static const int sectionLock[7] = {3, 4, 5, 5, 6, 4, 3};

CCScene* FairyTitle::scene()
{
    CCScene * scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        FairyTitle *layer = FairyTitle::create();
        CC_BREAK_IF(! layer);
        scene->addChild(layer);
    } while (0);
    return scene;
}

bool FairyTitle::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(titleMusic,true);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCLayerColor* layer = CCLayerColor::create(ccc4(0xd3, 0xff, 0xfd, 0xff), 720, 1080);
		CCSprite *worldMapBack = CCSprite::create(worldMap);
		layer->addChild(worldMapBack);
		worldMapBack->setPosition(ccp(size.width / 2, size.height / 2));
		addChild(layer,0,0);
		CCTexture2D* sectionTexture = CCTextureCache::sharedTextureCache()->addImage(worldSection);
		defSection = CCUserDefault::sharedUserDefault()->getIntegerForKey(keySection,1);
		initSection(size,sectionTexture);
        bRet = true;
    } while (0);

    return bRet;
}

void FairyTitle::initSection(CCSize size , CCTexture2D* sectionTexture){
	CCMenu* menu = CCMenu::create();
	for(int i = 0; i < 7; i++){
			CCSprite* section = CCSprite::createWithTexture(sectionTexture,CCRectMake(
				 i % 2 * sectionTexture->getPixelsWide()  / 2, i / 2 * sectionTexture->getPixelsHigh() / 4
				,sectionTexture->getPixelsWide()  / 2, sectionTexture->getPixelsHigh()  / 4));
			int curSection = getCurSection(defSection);
			CCSize sectionSize = section->getContentSize();
			if(curSection <  i){
				CCSprite* lock = CCSprite::create(worldLock);
				section->addChild(lock);
				lock->setPosition(ccp(sectionSize.width / 2, sectionSize.height / 2));
			}else{
				int curNum = defSection;
				if(i < curSection){
					curNum = sectionLock[i];
				}else{
					for(int j = 0; j < curSection; j++){
						curNum -= sectionLock[j];
					}
				}
				CCString* textString = CCString::createWithFormat("%d / %d", curNum, sectionLock[i]);
				CCLabelTTF* text = CCLabelTTF::create(textString->getCString(), "Arial", 30);
				section->addChild(text);
				text->setPosition(ccp(sectionSize.width / 2, sectionSize.height / 2));
			}
			CCMenuItemSprite* sectionItem = CCMenuItemSprite::create(section, section, this, menu_selector(FairyTitle::fairyMenuCallback));
			sectionItem->setPosition(ccp(sectionPosition[i][0] ,sectionPosition[i][1]));
			sectionItem->setTag(i + 1);
			menu->addChild(sectionItem);
	}
	addChild(menu,1,0);
}

int FairyTitle::getCurSection(int section){
	int count = sectionLock[0];
	for(int i = 0; i < 7; i++){
		if(section > count){
			count+=sectionLock[i];
		}else{
			return i;
		}
	}
	return -1;
}

void FairyTitle::fairyMenuCallback(CCObject* sender){
	CCMenuItemSprite* itemSprite = (CCMenuItemSprite *)sender;
	int sectionTag = itemSprite->getTag();
	if(defSection  > sectionTag){
		SimpleAudioEngine::sharedEngine()->end();
		CCScene* pScene = FairyChat::scene(sectionTag,0);
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}