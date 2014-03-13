#include "FairyChat.h"

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;

CCScene* FairyChat::scene(int section, int status)
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        // 'layer' is an autorelease object
        FairyChat *layer = FairyChat::create();
		layer->section = section;
		layer->status = status;
        CC_BREAK_IF(! layer);
        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool FairyChat::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());
        CCSize size = CCDirector::sharedDirector()->getWinSize();
		chatParser = XMLParser::parseWithFile("data/chat.xml");
		chatParser->retain();
		back = CCSprite::create();
		back->setPosition(ccp(size.width / 2, size.height  / 2));
		this->addChild(back);
		dialogUp = CCSprite::create(dialogFrame);
		dialogDown = CCSprite::create(dialogFrame);
		this->addChild(dialogUp);
		dialogUp->setOpacity(0);
		this->addChild(dialogDown);
		dialogDown->setOpacity(0);
		dialogUp->setPosition(ccp(size.width / 2, size.height  - 150));
		dialogDown->setPosition(ccp(size.width / 2, 150));
        bRet = true;
    } while (0);

    return bRet;
}

void FairyChat::initCommands(){
	commandIndex = 0;
	CCString* scriptPath = CCString::createWithFormat("script/script%d%d.sc",section, status);
	string script = Utils::getDataFromFile(scriptPath->getCString());
	command = Utils::split(script, CPARTITION);
	doCommand(Utils::split(command.at(commandIndex), PPARTITION));
}

void FairyChat::doCommand(vector<string> commandParams){
	if(commandParams.at(1).compare("chat") == 0){
		dialogUp->removeAllChildrenWithCleanup(true);
		dialogUp->setOpacity(0);
		dialogDown->removeAllChildrenWithCleanup(true);
		dialogDown->setOpacity(0);
		vector<string> commandMessage = Utils::split(commandParams.at(2), MPARTITION);
		for(int i = 0; i < (int)commandMessage.size(); i = i+3){
			CCSprite* face = CCSprite::create(commandMessage.at(i + 1).c_str());
			CCSprite* faceFrame = CCSprite::create(dialogFaceFrame);
			CCSize faceSize = face->getContentSize();
			face->setPosition(ccp(faceSize.width / 2 + 15, faceSize.height / 2 + 15));
			faceFrame->setPosition(ccp(faceSize.width / 2 + 15, faceSize.height / 2 + 15));
			CCString* textString = chatParser->getString(commandMessage.at(i + 2).c_str());
			CCLabelTTF* text = CCLabelTTF::create(textString->getCString(), "Arial", 30);
			text->setDimensions(CCSizeMake(480,160));
			text->setAnchorPoint(ccp(0,1));
			text->setPosition(ccp(170,130));
			if(commandMessage.at(i).compare("up") == 0){
				dialogUp->setOpacity(255);
				dialogUp->addChild(face);
				dialogUp->addChild(faceFrame);
				dialogUp->addChild(text);
			}else{
				dialogDown->setOpacity(255);
				dialogDown->addChild(face);
				dialogDown->addChild(faceFrame);
				dialogDown->addChild(text);
			}
		}
	}else if(commandParams.at(1).compare("play") == 0){
		const char* musicPath = commandParams.at(2).c_str();
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(musicPath,true);
	}else if(commandParams.at(1).compare("back") == 0){
		const char* backPath = commandParams.at(2).c_str();
		back->initWithFile(backPath);
	}else if(commandParams.at(1).compare("exit") == 0){
		if(status == 0){
			CCScene* pScene = FairyMain::scene(this->section);
			CCDirector::sharedDirector()->replaceScene(pScene);
		}else{
			CCScene* pScene = FairyTitle::scene();
			CCDirector::sharedDirector()->replaceScene(pScene);
		}
	}
	if(commandIndex < (int)command.size() - 1){
		commandIndex++;
	}
	char* jump = const_cast<char*>(commandParams.at(0).c_str());
	Utils::trim(jump);
	if(*jump == 49){
		doCommand(Utils::split(command.at(commandIndex), PPARTITION));
	}
}

void FairyChat::onEnter()
{
	CCLayer::onEnter();
	initCommands();
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
}

void FairyChat::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void FairyChat::ccTouchesMoved(CCSet *pTouch, CCEvent *pEvent){
}

void FairyChat::ccTouchesEnded(CCSet *pTouch, CCEvent *pEvent){
	doCommand(Utils::split(command.at(commandIndex), PPARTITION));
}

void FairyChat::ccTouchesBegan(CCSet *pTouch, CCEvent *pEvent){
}

