#include "MskillSelectLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

static const char* defaultIcon[] = {"icon/defaut.png", "icon/defaut.png", "icon/defaut.png", "icon/defaut.png", "icon/change.png"};
static const int buttonPosition[5][2] = {{200, 250},{200, -50},{50, 100},{350, 100},{200,100}}; 

// on "init" you need to initialize your instance
bool MskillSelectLayer::init(){
    if(!CCLayer::init()){
        return false;
    }
	this->setTouchEnabled(true);
	iconIndex = 0;
	selectButtonIndex = - 1;
	skillLevel = 0;
	winSize = GameManager::sharedGameManager()->getWindowSize();
	initBack();
	initSkillTableView();
	initSkillButton();
	initMenu();
    return true;
}

void MskillSelectLayer::initMenu(){
	CCMenu* menu = CCMenu::create();
	CCSprite *exit = CCSprite::create("ui/return.png");
	CCMenuItemSprite *exitItem = CCMenuItemSprite::create(exit, exit, this, menu_selector(MskillSelectLayer::exitSkillSelect));
	exitItem->setPosition(winSize.width / 2 - 100, winSize.height / 2 - 100);
	exitItem->setTag(0);
	menu->addChild(exitItem);
	this->addChild(menu);
}

void MskillSelectLayer::exitSkillSelect(CCObject* sender){
	CCMenuItemSprite* itemSprite = (CCMenuItemSprite *)sender;
	switch(itemSprite->getTag()){
	case 0:
		this->removeFromParentAndCleanup(true);
		break;
	}
}

void MskillSelectLayer::initBack(){
	CCSprite *back = CCSprite::create("worldMap/map.png");
	this->addChild(back);
	back->setPosition(ccp(winSize.width / 2, winSize.height  /2));
	CCSprite *head = CCSprite::create("head/head3.png");
	this->addChild(head);
	head->setPosition(ccp(winSize.width / 4, winSize.height  /2));
	CCSprite *skillBack = CCSprite::create("ui/back_black.png");
	this->addChild(skillBack);
	skillBack->setScaleX(108);
	skillBack->setScaleY(24);
	skillBack->setPosition(ccp(winSize.width / 2, 80));
	skillBack->setOpacity(100);
}

void MskillSelectLayer::initSkillTableView(){
	CCTableView* tableView = CCTableView::create(this, CCSizeMake(1080, 300));
    tableView->setDirection(kCCScrollViewDirectionHorizontal);
	tableView->setAnchorPoint(ccp(0,0));
	tableView->setPosition(ccp(0, 0));
    tableView->setDelegate(this);
    this->addChild(tableView);
    tableView->reloadData();
}

void MskillSelectLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	if(selectButtonIndex >= 0){
		int curSection = CCUserDefault::sharedUserDefault()->getIntegerForKey(GameManager::keySection,0);
		if(curSection / 2 < cell->getIdx()){
			return;
		}
		CCSprite* button = static_cast<CCSprite *>(buttonArr->objectAtIndex(selectButtonIndex));
		CCString* SkillDataFile = CCString::createWithFormat("normal_skill/skill%d.xml",cell->getIdx());
		button->setTexture(CCTextureCache::sharedTextureCache()->addImage(Utils::parseXML(SkillDataFile->getCString(), "skillIcon")));
		GameManager::sharedGameManager()->setSkillPad(skillLevel, selectButtonIndex, cell->getIdx());
	}
    CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize MskillSelectLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return CCSizeMake(200, 300);
}

CCTableViewCell* MskillSelectLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
	CCSprite *lock = CCSprite::create("worldMap/world_lock.png");
	lock->setTag(3);
	int curSection = CCUserDefault::sharedUserDefault()->getIntegerForKey(GameManager::keySection,0);
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
		CCString* SkillDataFile = CCString::createWithFormat("normal_skill/skill%d.xml",iconIndex);
		CCSprite *skill = CCSprite::create(Utils::parseXML(SkillDataFile->getCString(), "skillIcon"));
		skill->setScale(2.0f);
		skill->setAnchorPoint(CCPointZero);
		skill->setPosition(ccp(50,50));
		skill->setTag(1);
		cell->addChild(skill);
		if(curSection / 2 <iconIndex){
			lock->setPosition(ccp(110,110));
			cell->addChild(lock);
		}
		iconIndex++;
		CCLabelTTF *label = CCLabelTTF::create(Utils::parseXML(SkillDataFile->getCString(), "name"), "Helvetica", 40.0f);
		label->setTag(2);
		label->setPosition(ccp(100,30));
		cell->addChild(label);
    }else{
		cell->removeChildByTag(3);
		CCString* SkillDataFile = CCString::createWithFormat("normal_skill/skill%d.xml",idx);
		CCSprite *skill = (CCSprite*)cell->getChildByTag(1);
		CCLabelTTF *label = (CCLabelTTF*)cell->getChildByTag(2);
		label->setString(Utils::parseXML(SkillDataFile->getCString(), "name"));
		skill->setTexture(CCTextureCache::sharedTextureCache()->addImage(Utils::parseXML(SkillDataFile->getCString(), "skillIcon")));
		if(curSection / 2 <idx){
			lock->setPosition(ccp(110,110));
			cell->addChild(lock);
		}
	}
    return cell;
}

unsigned int MskillSelectLayer::numberOfCellsInTableView(CCTableView *table)
{
    return 16;
}

void MskillSelectLayer::initSkillButton(){
	buttonArr = CCArray::createWithCapacity(5);
	buttonArr->retain();
	for(int i  = 0; i < 5; i++){
		CCSprite* button = CCSprite::create(defaultIcon[i]);
		if(GameManager::totalSkillPad[skillLevel][i] >= 0 && i != 4){
			CCString* SkillDataFile = CCString::createWithFormat("normal_skill/skill%d.xml",GameManager::totalSkillPad[skillLevel][i]);
			button->setTexture(CCTextureCache::sharedTextureCache()->addImage(Utils::parseXML(SkillDataFile->getCString(), "skillIcon")));
		}else{
			button->setTexture(CCTextureCache::sharedTextureCache()->addImage(defaultIcon[i]));
		}
		button->setTag(i);
		buttonArr->addObject(button);
		this->addChild(button);
		button->setScale(2.0f);
		float px = winSize.width / 2 + buttonPosition[i][0];
		float py = winSize.height  / 2 +buttonPosition[i][1];
		button->setPosition(ccp(px, py));
	}
}

void MskillSelectLayer::onExit(){
	CCLayer::onExit();
	buttonArr->release();
}

void MskillSelectLayer::onEnter(){
	CCLayer::onEnter();
}

void MskillSelectLayer::resetButton(){
	skillLevel++;
	if(skillLevel > 2){
		skillLevel = 0;
	}
	for(int i  = 0; i < (int)buttonArr->count() - 1; i++){
		CCSprite* button = static_cast<CCSprite *>(buttonArr->objectAtIndex(i));
		button->setTexture(CCTextureCache::sharedTextureCache()->addImage("icon/defaut.png"));
		if(GameManager::totalSkillPad[skillLevel][i] >= 0 && i != buttonArr->count() - 1){
			CCString* SkillDataFile = CCString::createWithFormat("normal_skill/skill%d.xml",GameManager::totalSkillPad[skillLevel][i]);
			button->setTexture(CCTextureCache::sharedTextureCache()->addImage(Utils::parseXML(SkillDataFile->getCString(), "skillIcon")));
		}else{
			button->setTexture(CCTextureCache::sharedTextureCache()->addImage(defaultIcon[i]));
		}
	}

}

void MskillSelectLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCPoint point = ((CCTouch *)pTouches->anyObject())->getLocation();
	for(int i  = 0; i < (int)buttonArr->count(); i++){
		CCSprite* button = static_cast<CCSprite *>(buttonArr->objectAtIndex(i));
		if(button->boundingBox().containsPoint(point)){
			if(i == 4){
				resetButton();
			}else{
				selectButtonIndex = i;
			}
			 CCLOG("button touched at index: %i", i);
		}
	}
}

void MskillSelectLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
}

void MskillSelectLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
}