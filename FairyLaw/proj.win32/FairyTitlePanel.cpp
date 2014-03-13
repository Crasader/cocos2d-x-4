#include "FairyTitlePanel.h"

using namespace cocos2d;

// on "init" you need to initialize your instance
bool FairyTitlePanel::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());
        CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCSprite *title = CCSprite::create(uiTitleName);
		title->setPosition(ccp(260,size.height - 80));
		this->addChild(title);
		titleNameText = CCLabelTTF::create("", "Arial", 30);
		title->addChild(titleNameText);
		CCSize titleNameSize = title->getContentSize();
		titleNameText->setPosition(ccp(titleNameSize.width / 2, titleNameSize.height / 2));

		CCSprite *moveCount = CCSprite::create(uiMoveCount);
		moveCount->setPosition(ccp(size.width - 60,size.height - 130));
		this->addChild(moveCount);
		moveCountText = CCLabelTTF::create("", "Arial", 28);
		moveCount->addChild(moveCountText);
		CCSize moveCountSize = moveCount->getContentSize();
		moveCountText->setPosition(ccp(moveCountSize.width / 2, moveCountSize.height / 2));

		CCSprite *resultCount = CCSprite::create(uiAttackCount);
		resultCount->setPosition(ccp(size.width - 160,size.height - 80));
		this->addChild(resultCount);
		resultCountText = CCLabelTTF::create("", "Arial", 30);
		resultCount->addChild(resultCountText);
		CCSize resultCountSize = resultCount->getContentSize();
		resultCountText->setPosition(ccp(resultCountSize.width / 2, resultCountSize.height / 5));
        bRet = true;
    } while (0);

    return bRet;
}

void FairyTitlePanel::setTitleName(int section){
	CCString* sceneNameFile = CCString::createWithFormat("data/scene%d.xml",section);
	XMLParser *sceneData = XMLParser::parseWithFile(sceneNameFile->getCString());
	CCString* name = sceneData->getString("name");
	titleNameText->setString(name->getCString());
}

void FairyTitlePanel::setMoveCount(int moveCount){
	char moveCountstr[64] = {0};  
	sprintf(moveCountstr, "%d",  moveCount);      
	moveCountText->setString(moveCountstr);
}

void FairyTitlePanel::setResultCount(int resultCount){
	XMLParser *scene = XMLParser::parseWithFile("data/common.xml");
	CCString* string = scene->getString("fight_emeny_count");
	char result[64] = {0};  
	sprintf(result, "%d%s",  resultCount, string->getCString());
	resultCountText->setString(result);
}


