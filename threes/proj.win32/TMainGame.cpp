#include "TMainGame.h"
using namespace cocos2d;
using namespace CocosDenshion;

CCScene* TMainGame::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        TMainGame *layer = TMainGame::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TMainGame::init()
{
    bool bRet = false;
    do 
    {
      stauts = -1;
       CC_BREAK_IF(! CCLayer::init());
       size = CCDirector::sharedDirector()->getWinSize();
	   CCSprite* pSprite = CCSprite::create("back.png");
	   pSprite->setPosition(ccp(size.width/2, size.height/2));
	   this->addChild(pSprite, 0);
	   ballLayer = CCLayer::create();
	   this->addChild(ballLayer);
	   ballLayer->setPosition(ccp(40,130));
	   TGameManager::sharedGameManager()->init();
	   bestCount = 0;
	   bestCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("bestCount");
	   initLabel();
	   initTable();
	   initMenu();
	   srand( (unsigned)time( NULL ) ); 
       bRet = true;
    } while (0);

    return bRet;
}

void TMainGame::initMenu(){
	CCMenu* gameMenu = CCMenu::create();
	CCMenuItemImage* musicItem = CCMenuItemImage::create("music_button.png", "music_button.png", this, menu_selector(TMainGame::menuCallback));
	musicItem->setTag(0);
	musicItem->setScale(2.0f);
	gameMenu->addChild(musicItem);
	musicItem->setPosition(-50, 200);

	CCMenuItemImage* startItem = CCMenuItemImage::create("start_button.png", "start_button.png", this, menu_selector(TMainGame::menuCallback));
	startItem->setTag(1);
	startItem->setScale(2.0f);
	gameMenu->addChild(startItem);
	startItem->setPosition(-50, 80);

	this->addChild(gameMenu,0,0);
	gameMenu->setPosition(size.width - 50, size.height - 300);
}

void TMainGame::initLabel(){
	ccColor3B color;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	char string[6];
	sprintf(string, "%d", bestCount);
	bestPoint = CCLabelTTF::create(string, "Arial", 50);
	curPoint = CCLabelTTF::create("0", "Arial", 50);
	bestPoint->setColor(color);
	curPoint->setColor(color);
	this->addChild(bestPoint);
	this->addChild(curPoint);
	bestPoint->setPosition(ccp(300,size.height - 80));
	curPoint->setPosition(ccp(300,size.height - 170));
}

void TMainGame::initTable(){
	CCSprite* nextball;
	char string[6];
	sprintf(string, "%d", TGameManager::point);
	curPoint->setString(string);
	if(TGameManager::sharedGameManager()->isOver()){
		CCUserDefault::sharedUserDefault()->setIntegerForKey("bestCount", max(TGameManager::point, bestCount));
		TGameTitle::type = 1;
		CCScene *pScene = TGameTitle::scene();
		CCTransitionCrossFade *transitionScene = CCTransitionCrossFade::create(0.5f, pScene);
		CCDirector::sharedDirector()->replaceScene(transitionScene);
	}
	nextball = getColor(TGameManager::newCount);
	ballLayer->addChild(nextball, 0);
	nextball->setPosition(ccp(300, 670));
	nextball->setScale(0.5f);
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			CCSprite* ball = getColor(TGameManager::GameData[i][j]);
			ballLayer->addChild(ball);
			ball->setAnchorPoint(ccp(0,0));
			ball->setTag(i * 10 + j);
			ball->setPosition(ccp(i *150, 450 - j * 150));
			char buffer[10];
			sprintf(buffer,"%d", TGameManager::GameData[i][j]);
			CCLabelTTF* text = CCLabelTTF::create(buffer, "Arial", 50);
			ball->addChild(text);
			CCSize ballSize = ball->getContentSize();
			text->setPosition(ccp(ballSize.width / 2, ballSize.height / 2));
			if(TGameManager::GameData[i][j] == 0){
				ball->setOpacity(0);
				text->setOpacity(0);
			}
		}
	}
}

CCSprite* TMainGame::getColor(int i){
	char* name;
	if(i == 1){
		name = "ball2.png";
	}else if(i == 2){
		name = "ball3.png";
	}else{
		name = "ball1.png";
	}
	CCSprite* ball = CCSprite::create(name);
	if(i == 0){
		ball->setOpacity(0);
	}else{
		ball->setOpacity(255);
	}
	return ball;
}

void TMainGame::onEnter(){
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm.mp3",true);
	this->schedule(schedule_selector(TMainGame::updateGame), 0.05f);
}

void TMainGame::updateGame(float t){

}

void TMainGame::onExit(){
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}
}

void TMainGame::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	startPoint = ((CCTouch *)pTouches->anyObject())->getLocation();
}

void TMainGame::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	CCPoint curPoint = ((CCTouch *)pTouches->anyObject())->getLocation();
	if((abs(startPoint.x - curPoint.x) > abs(startPoint.y - curPoint.y)  &&  ccpDistance(startPoint, curPoint) > 10 && stauts == -1) || stauts == standard){
		stauts = standard;
		if(startPoint.x > curPoint.x){
			std::vector<int> result = TGameManager::sharedGameManager()->calculatePosition(TGameManager::left);
			for(int i = 0; i < result.size(); i++){
				if(result.at(i) != -1){
					int staicX = result.at(i) / 10;
					int staicY = result.at(i) % 10;
					for(int j = staicX + 1; j < 4; j++){
						CCSprite* ball = static_cast<CCSprite*>(ballLayer->getChildByTag(j * 10 + staicY));
						ball->setPosition(ccp(j * 150 - min(150,(int)(startPoint.x - curPoint.x)), 450 - staicY * 150));
					}
				}
			}
		}else if(startPoint.x < curPoint.x){
			std::vector<int> result = TGameManager::sharedGameManager()->calculatePosition(TGameManager::right);
			for(int i = 0; i < result.size(); i++){
				if(result.at(i) != -1){
					int staicX = result.at(i) / 10;
					int staicY = result.at(i) % 10;
					for(int j = staicX - 1; j >= 0; j--){
						CCSprite* ball = static_cast<CCSprite*>(ballLayer->getChildByTag(j * 10 + staicY));
						ball->setPosition(ccp(j * 150 + min(150,(int)(curPoint.x - startPoint.x)), 450 - staicY * 150));
					}
				}
			}
		}
	}else if((abs(startPoint.x - curPoint.x) < abs(startPoint.y - curPoint.y) && ccpDistance(startPoint, curPoint) > 10 && stauts == -1) || stauts == vertical){
		stauts = vertical;
		if(startPoint.y > curPoint.y){
			std::vector<int> result = TGameManager::sharedGameManager()->calculatePosition(TGameManager::down);
			for(int i = 0; i < result.size(); i++){
				if(result.at(i) != -1){
					int staicX = result.at(i) / 10;
					int staicY = result.at(i) % 10;
					for(int j = staicY - 1; j >= 0; j--){
						CCSprite* ball = static_cast<CCSprite*>(ballLayer->getChildByTag(staicX * 10 + j));
						ball->setPosition(ccp(staicX * 150, 450 - j * 150 - min(150,(int)(startPoint.y - curPoint.y))));
					}
				}
			}
		}else if(startPoint.y < curPoint.y){
			std::vector<int> result = TGameManager::sharedGameManager()->calculatePosition(TGameManager::up);
			for(int i = 0; i < result.size(); i++){
				if(result.at(i) != -1){
					int staicX = result.at(i) / 10;
					int staicY = result.at(i) % 10;
					for(int j = staicY + 1; j < 4; j++){
						CCSprite* ball = static_cast<CCSprite*>(ballLayer->getChildByTag(staicX * 10 + j));
						ball->setPosition(ccp(staicX * 150, 450 - j * 150 + min(150,(int)(curPoint.y - startPoint.y))));
					}
				}
			}
		}
	}
}

void TMainGame::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	CCPoint curPoint = ((CCTouch *)pTouches->anyObject())->getLocation();
	if(abs(startPoint.x - curPoint.x) > 75 || abs(startPoint.y - curPoint.y) > 75){
		if((abs(startPoint.x - curPoint.x) > abs(startPoint.y - curPoint.y) && ccpDistance(startPoint, curPoint) > 10 && stauts == -1) || stauts == standard){
			if(startPoint.x > curPoint.x){
				TGameManager::sharedGameManager()->calculateData(TGameManager::left);
			}else if(startPoint.x < curPoint.x){
				TGameManager::sharedGameManager()->calculateData(TGameManager::right);
			}
		}else if((abs(startPoint.x - curPoint.x) < abs(startPoint.y - curPoint.y)  && ccpDistance(startPoint, curPoint) > 10 && stauts == -1) || stauts == vertical){
			if(startPoint.y > curPoint.y){
				TGameManager::sharedGameManager()->calculateData(TGameManager::down);
			}else if(startPoint.y < curPoint.y){
				TGameManager::sharedGameManager()->calculateData(TGameManager::up);
			}
		}
	}
	ballLayer->removeAllChildren();
	initTable();
	stauts = -1;
}

void TMainGame::menuCallback(CCObject* pSender)
{
	int tag = static_cast<CCMenuItemImage*>(pSender)->getTag();
	switch(tag){
	case 0:
		if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		}else{
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm.mp3");
		}
		break;
	case 1:
		TGameManager::sharedGameManager()->init();
		ballLayer->removeAllChildren();
		initTable();
		stauts = -1;
		bestCount = 0;
	    bestCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("bestCount");
		char string[6];
		sprintf(string, "%d", bestCount);
		bestPoint->setString(string);
		break;
	}
}