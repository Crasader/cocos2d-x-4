#include "FairyControlPanel.h"

USING_NS_CC;

bool FairyControlPanel::isDoClick = false;

bool FairyControlPanel::init()
{
    bool bRet = false;
    do 
    {
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		panelBack = CCSprite::create(uiPanel);
		addChild(panelBack,0,0);
		panelBack->setPosition(ccp(size.width / 2,panelBack->getContentSize().height / 2 + 30));
		initButton();
        bRet = true;
    } while (0);

    return bRet;
}

void FairyControlPanel::initButton(){
	controlType = 1;
	panelBack->removeAllChildrenWithCleanup(true);
	CCSize size = panelBack->getContentSize();
	CCSprite* leftNormal = CCSprite::create(uiLeftNormal);
	CCSprite* leftClick = CCSprite::create(uiLeftClick);
	CCSprite* holdNormal = CCSprite::create(uiHoldNormal);
	CCSprite* holdClick = CCSprite::create(uiHoldClick);
	CCSprite* rightNormal = CCSprite::create(uiRightNormal);
	CCSprite* rightClick = CCSprite::create(uiRightClick);
	CCSprite* upNormal = CCSprite::create(uiUpNormal);
	CCSprite* upClick = CCSprite::create(uiUpClick);
	CCMenuItemSprite* leftButton = CCMenuItemSprite::create(leftNormal, leftClick, this, menu_selector(FairyControlPanel::fairyMenuCallback));
	leftButton->setTag(leftDrection);
	CCMenuItemSprite* holdButton = CCMenuItemSprite::create(holdNormal, holdClick, this, menu_selector(FairyControlPanel::fairyMenuCallback));
	holdButton->setTag(holdDrection);
	CCMenuItemSprite* rightButton = CCMenuItemSprite::create(rightNormal, rightClick, this, menu_selector(FairyControlPanel::fairyMenuCallback));
	rightButton->setTag(rightDrection);
	CCMenuItemSprite* upButton = CCMenuItemSprite::create(upNormal, upClick, this, menu_selector(FairyControlPanel::fairyMenuCallback));
	upButton->setTag(upDrection);
	CCMenu* menu = CCMenu::create(leftButton, holdButton, rightButton, upButton, NULL);
	leftButton->setPosition(ccp(size.width / 5, size.height / 2 + 30));
	upButton->setPosition(ccp(2 * size.width / 5, size.height / 2+ 30));
	holdButton->setPosition(ccp(3 * size.width / 5, size.height / 2 + 30));
	rightButton->setPosition(ccp(4 * size.width / 5, size.height / 2 + 30));
	panelBack->addChild(menu);
	menu->setPosition(ccp(0, - 30));
}

void FairyControlPanel::initHero(FairyHero* selectHero){
	controlType = 2;
	panelBack->removeAllChildrenWithCleanup(true);
	CCSize size = panelBack->getContentSize();
	int ids[4] = {-1, -1, -1, -1};
	for(int i = 0; i < (int)selectHero->skillIds.size(); i++){
		ids[i] = selectHero->skillIds.at(i);
	}
	CCString* SkillDataFile;
	XMLParser *skillData;
	CCMenu* menu = CCMenu::create();
	if(ids[0] != -1){
		SkillDataFile = CCString::createWithFormat("data/skill%d.xml",ids[0]);
		skillData = XMLParser::parseWithFile(SkillDataFile->getCString());
		CCSprite* skill1 = CCSprite::create(skillData->getString("skillIcon")->getCString());
		CCMenuItemSprite*skill1Button = CCMenuItemSprite::create(skill1, skill1, this, menu_selector(FairyControlPanel::fairyMenuCallback));
		skill1Button->setTag(leftDrection);
		skill1Button->setPosition(ccp(size.width / 8, size.height / 2 + 30));
		menu->addChild(skill1Button);
	}
	if(ids[1] != -1){
		SkillDataFile = CCString::createWithFormat("data/skill%d.xml",ids[1]);
		skillData = XMLParser::parseWithFile(SkillDataFile->getCString());
		CCSprite* skill1 = CCSprite::create(skillData->getString("skillIcon")->getCString());
		CCSprite* skill2 = CCSprite::create(uiLeftNormal);
		CCMenuItemSprite* skill2Button = CCMenuItemSprite::create(skill2, skill2, this, menu_selector(FairyControlPanel::fairyMenuCallback));
		skill2Button->setTag(holdDrection);
		skill2Button->setPosition(ccp(2 * size.width / 8, size.height / 2+ 30));
		menu->addChild(skill2Button);
	}
	if(ids[2] != -1){
		SkillDataFile = CCString::createWithFormat("data/skill%d.xml",ids[2]);
		skillData = XMLParser::parseWithFile(SkillDataFile->getCString());
		CCSprite* skill1 = CCSprite::create(skillData->getString("skillIcon")->getCString());
		CCSprite* skill3 = CCSprite::create(uiLeftNormal);
		CCMenuItemSprite* skill3Button = CCMenuItemSprite::create(skill3, skill3, this, menu_selector(FairyControlPanel::fairyMenuCallback));
		skill3Button->setTag(rightDrection);
		skill3Button->setPosition(ccp(3 * size.width / 8, size.height / 2 + 30));
		menu->addChild(skill3Button);
	}
	if(ids[3] != -1){
		SkillDataFile = CCString::createWithFormat("data/skill%d.xml",ids[3]);
		skillData = XMLParser::parseWithFile(SkillDataFile->getCString());
		CCSprite* skill4 = CCSprite::create(skillData->getString("skillIcon")->getCString());
		CCMenuItemSprite* skill4Button = CCMenuItemSprite::create(skill4, skill4, this, menu_selector(FairyControlPanel::fairyMenuCallback));
		skill4Button->setTag(upDrection);
		skill4Button->setPosition(ccp(4 * size.width / 8, size.height / 2 + 30));
		menu->addChild(skill4Button);
	}
	panelBack->addChild(menu);
	menu->setPosition(ccp(0, - 30));
	CCString* heroDataFile = CCString::createWithFormat("data/hero%d.xml",selectHero->heroId / 10 * 10);
	XMLParser *heroData = XMLParser::parseWithFile(heroDataFile->getCString());
	CCLabelTTF* heroName = CCLabelTTF::create(heroData->getString("name")->getCString(), "Arial", 50);
	heroName->setColor(ccc3(0x00, 0x00, 0x00));
	heroName->setAnchorPoint(ccp(0,1));
	heroName->setPosition(ccp( 5 * size.width / 8, size.height / 2 + 30));
	panelBack->addChild(heroName);
}

void FairyControlPanel::changeControlPanel(FairyHero* selectHero){
	if(selectHero == NULL && controlType != 1){
		playAni();
		initButton();
	}else if(selectHero != NULL && controlType != 2){
		playAni();
		initHero(selectHero);
	}
}

void FairyControlPanel::fairyMenuCallback(CCObject* sender){
	CCMenuItemSprite* buttonSprite = (CCMenuItemSprite *)sender;
	int buttonTag = buttonSprite->getTag();
	FairyMain* parent = static_cast<FairyMain *>(this->getParent());
	parent->doClickButton(buttonTag);
}

void FairyControlPanel::playAni(){
	if(!isDoClick){
		isDoClick = true;
		CCActionInterval * moveBy = CCMoveBy::create(0.3f,ccp(0,
			-panelBack->getContentSize().height - 30));
		panelBack->runAction(CCSequence::create(moveBy, moveBy->reverse(), 
			CCCallFuncN::create(this,callfuncN_selector(FairyControlPanel::finishDoClick)),NULL));
	}
}

void FairyControlPanel::setInvisiable(){
	if(!isDoClick){
		isDoClick = true;
		CCActionInterval * moveBy = CCMoveBy::create(0.3f,ccp(0,
			-panelBack->getContentSize().height - 30));
		panelBack->runAction(CCSequence::create(moveBy, 
			CCCallFuncN::create(this,callfuncN_selector(FairyControlPanel::finishDoClick)),NULL));
	}
}

void FairyControlPanel::setVisiable(){
	if(!isDoClick){
		CCActionInterval * moveBy = CCMoveBy::create(0.3f,ccp(0,
			-panelBack->getContentSize().height - 30));
		panelBack->runAction(CCSequence::create(moveBy->reverse(), 
			CCCallFuncN::create(this,callfuncN_selector(FairyControlPanel::finishDoClick)),NULL));
	}
}

void FairyControlPanel::finishDoClick(CCNode* pSender){
	isDoClick = false;
}