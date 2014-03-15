#include "TGameTitle.h"

using namespace cocos2d;
int TGameTitle::type = 0;

CCScene* TGameTitle::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        TGameTitle *layer = TGameTitle::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TGameTitle::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CCSprite* back = CCSprite::create("title.png");
		CCSprite* logo;
		if(type == 0){
			logo = CCSprite::create("logo.png");
			logo->setScale(1.5f);
		}else{
			logo = CCSprite::create("over.png");
		}
        CC_BREAK_IF(! back);
        back->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(back, 0);
		logo->setPosition(ccp(size.width/2 - 140, size.height/2 + 100));
        this->addChild(logo, 0);
        bRet = true;
    } while (0);

    return bRet;
}

void TGameTitle::onEnter(){
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
}

void TGameTitle::onExit(){
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
}

void TGameTitle::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	startPoint = ((CCTouch *)pTouches->anyObject())->getLocation();
}

void TGameTitle::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	CCPoint endPoint = ((CCTouch *)pTouches->anyObject())->getLocation();
	if(endPoint.x - startPoint.x > 50){
		CCScene *pScene = TMainGame::scene();
		CCTransitionCrossFade *transitionScene = CCTransitionCrossFade::create(0.5f, pScene);
		CCDirector::sharedDirector()->replaceScene(transitionScene);
	}
}

void TGameTitle::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	
}