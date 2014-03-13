#include "MToast.h"

using namespace cocos2d;

bool MToast::isShowToast = false;
// on "init" you need to initialize your instance
bool MToast::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////
        CC_BREAK_IF(! CCLayer::init());
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		toastBack = CCSprite::create("ui/chat_back.png");
		toastHead = CCSprite::create("head/head2.png");
		toastText = CCLabelTTF::create("text", "Arial", 30);
		ccColor3B color;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		toastText->setColor(color);
		this->addChild(toastHead);
		this->addChild(toastBack);
		toastBack->addChild(toastText);
		toastHead->setAnchorPoint(ccp(0.5, 0));
		toastBack->setAnchorPoint(ccp(0.5, 0));
		toastHead->setPosition(ccp(size.width / 2, 10));
		toastBack->setPosition(ccp(size.width / 2, 0));
		toastText->setPosition(ccp(toastBack->getContentSize().width / 2 - 10, toastBack->getContentSize().height / 2));
        bRet = true;
    } while (0);

    return bRet;
}

void MToast::setToastBack(char *back){
	toastBack->setTexture(CCTextureCache::sharedTextureCache()->addImage(back));
}

void MToast::setToastHead(char *head){
	toastHead->setTexture(CCTextureCache::sharedTextureCache()->addImage(head));
}

void MToast::setToastString(const char* text){
	this->setTag(TAG);
	isShowToast = true;
	toastText->setString(text);
}

void MToast::MTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
}

void MToast::MTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
}

void MToast::MTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	this->removeFromParentAndCleanup(true);
	isShowToast = false;
}
