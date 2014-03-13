#include "MEquipSelectLayer.h"

USING_NS_CC;

static const char* icon[3][4] = {{"equip/E_1_0.png", "equip/E_1_1.png", "equip/E_1_2.png", "equip/E_1_3.png"},
														  {"equip/E_2_0.png", "equip/E_2_1.png", "equip/E_2_2.png", "equip/E_2_3.png"},
													      {"equip/E_3_0.png", "equip/E_3_1.png", "equip/E_3_2.png", "equip/E_3_3.png"}};

// on "init" you need to initialize your instance
bool MEquipSelectLayer::init(){
    if(!CCLayer::init()){
        return false;
    }
	this->setTouchEnabled(true);
	winSize = GameManager::sharedGameManager()->getWindowSize();
	initBack();
	initSkillButton();
	initMenu();
	initText();
    return true;
}

void MEquipSelectLayer::initText(){
	ccColor3B color;
	CCString* str = CCString::createWithFormat("money : %d", GameManager::sharedGameManager()->getMoney());
	moneyCount = CCLabelTTF::create(str->getCString(), "Arial", 50);
	color.r = 255;
	color.g = 0;
	color.b = 0;
	moneyCount->setColor(color);
	this->addChild(moneyCount);
	moneyCount->setPosition(ccp(winSize.width - 300, winSize.height - 70));

}

void MEquipSelectLayer::initMenu(){
	CCMenu* menu = CCMenu::create();
	CCSprite *exit = CCSprite::create("ui/return.png");

	setEquipButton = CCSprite::create("worldMap/section_button.png");
	setEquipButton->setScale(2.0f);
	setEquipButton->setTag(1);
	setEquipButton->setPosition(ccp(winSize.width - 100, 100));
	CCLabelTTF* setEquipText = CCLabelTTF::create(Utils::getString("set_equip"), "Arial", 50);
	setEquipText->setPosition(ccp(winSize.width - 100, 100));
	this->addChild(setEquipButton);
	this->addChild(setEquipText);

	buyEquipButton = CCSprite::create("worldMap/section_button.png");
	buyEquipButton->setScale(2.0f);
	buyEquipButton->setTag(2);
	buyEquipButton->setPosition(ccp(winSize.width - 300, 100));
	CCLabelTTF* buyEquipText = CCLabelTTF::create(Utils::getString("buy_equip"), "Arial", 50);
	buyEquipText->setPosition(ccp(winSize.width - 300, 100));
	this->addChild(buyEquipButton);
	this->addChild(buyEquipText);
	
	descripEquipButton = CCSprite::create("worldMap/section_button.png");
	descripEquipButton->setScale(2.0f);
	descripEquipButton->setTag(3);
	descripEquipButton->setPosition(ccp(winSize.width - 500, 100));
	CCLabelTTF* descripEquipText = CCLabelTTF::create(Utils::getString("descrip_equip"), "Arial", 50);
	descripEquipText->setPosition(ccp(winSize.width - 500, 100));
	this->addChild(descripEquipButton);
	this->addChild(descripEquipText);

	CCMenuItemSprite *exitItem = CCMenuItemSprite::create(exit, exit, this, menu_selector(MEquipSelectLayer::buttonSelect));
	exitItem->setPosition(winSize.width / 2 - 100, winSize.height / 2 - 80);
	exitItem->setTag(0);
	menu->addChild(exitItem);
	this->addChild(menu);
}

void MEquipSelectLayer::buttonSelect(CCObject* sender){
	CCNode* itemSprite = (CCNode *)sender;
	switch(itemSprite->getTag()){
	case 0:
		this->removeFromParentAndCleanup(true);
		break;
	case 1:
		setEquip();
		break;
	case 2:
		buyEquip();
		break;
	case 3:
		descripEquip();
		break;
	}
}

void MEquipSelectLayer::descripEquip(){
	MToast* toast = MToast::create();
	CCString* str = CCString::createWithFormat("attack : %d \n def : %d \n maxHp : %d \n maxMp : %d", 10,10,10,10);
	toast->setToastString(str->getCString());
	this->addChild(toast);
}

void MEquipSelectLayer::setEquip(){
	int i = oldIndex / 10;
	int j = oldIndex % 10;
	CCSprite* lock = (CCSprite* )this->getChildByTag(300 + i * 10 +j);
	if(lock->getOpacity() != 0){
		return;
	}
	int oldSelect = GameManager::selectEquipIndex[j];
	GameManager::sharedGameManager()->setEquip(i,j);
	if(oldSelect >= 0){
		CCSprite* oldflag = (CCSprite* )this->getChildByTag(200 + oldSelect * 10 + j);
		oldflag->setOpacity(0);
	}
	CCSprite* flag = (CCSprite* )this->getChildByTag(200 + i * 10 +j);
	flag->setOpacity(255);
}

void MEquipSelectLayer::buyEquip(){
	int i = oldIndex / 10;
	int j = oldIndex % 10;
	CCString* data_string = CCString::createWithFormat("equip/equip%d.xml", oldIndex);
	int money = atoi(Utils::parseXML(data_string->getCString(), "money"));
	CCSprite* lock = (CCSprite* )this->getChildByTag(300 + i * 10 +j);
	if(lock->getOpacity() == 0){
		return;
	}
	if(GameManager::sharedGameManager()->getMoney() >= money){
		GameManager::sharedGameManager()->addMoney(-money);
		GameManager::sharedGameManager()->buyEquip(oldIndex);
		lock->setOpacity(0);
		CCString* str = CCString::createWithFormat("money : %d", GameManager::sharedGameManager()->getMoney());
		moneyCount->setString(str->getCString());
	}else{
		MToast* toast = MToast::create();
		toast->setToastString("money not enough");
		this->addChild(toast);
	}
}

void MEquipSelectLayer::initBack(){
	CCSprite *back = CCSprite::create("worldMap/map.png");
	this->addChild(back);
	back->setPosition(ccp(winSize.width / 2, winSize.height  /2));
	CCSprite *head = CCSprite::create("head/head1.png");
	this->addChild(head);
	head->setPosition(ccp(winSize.width / 4, winSize.height  /2));
}

void MEquipSelectLayer::initSkillButton(){
	buttonArr = CCArray::createWithCapacity(12);
	frameArr = CCArray::createWithCapacity(12);
	buttonArr->retain();
	frameArr->retain();
	for(int i  = 0; i < 3; i++){
		for(int j = 0; j < 4; j++){
			CCSprite* button = CCSprite::create(icon[i][j]);
			CCSprite* frame = CCSprite::create("ui/equip_frame.png");
			CCSprite* flag = CCSprite::create("worldMap/equip_set.png");
			CCSprite* back = CCSprite::create("equip/E_back.png");
			CCSprite* lock = CCSprite::create("worldMap/world_lock.png");
			CCSize frameSize = frame->getContentSize();
			int oldSelect = GameManager::selectEquipIndex[j];
			if(oldSelect != i){
				flag->setOpacity(0);
			}else{
				flag->setOpacity(255);
			}
			for ( int k = 0 ; k < 12 ; k++ ){
				if(GameManager::totolEquipBuyIds[k] == -1){
					break;
				}else if(GameManager::totolEquipBuyIds[k] == i * 10 + j){
					lock->setOpacity(0);
					break;
				}
			}
			if(i == 0 && j == 0){
				frame->setTextureRect(CCRect(0, 0 ,frameSize.width / 2, frameSize.height));
			}else{
				frame->setTextureRect(CCRect(frameSize.width / 2, 0, frameSize.width / 2, frameSize.height));
			}
			button->setTag(i * 10 + j);
			frame->setTag(100 + i * 10 + j);
			flag->setTag(200 + i * 10 + j);
			lock->setTag(300 + i * 10 + j);
			frameArr->addObject(frame);
			buttonArr->addObject(button);
			this->addChild(back);
			this->addChild(button);
			this->addChild(flag);
			this->addChild(lock);
			this->addChild(frame);
			float px = winSize.width / 2 + j * 150;
			float py = winSize.height  / 4 + 350 - i * 150;
			back->setPosition(ccp(px, py));
			button->setPosition(ccp(px, py));
			flag->setPosition(ccp(px + 20, py - 30));
			lock->setPosition(ccp(px , py));
			frame->setPosition(ccp(px, py));
		}
	}
	oldIndex = 0;
}

void MEquipSelectLayer::onExit(){
	CCLayer::onExit();
	buttonArr->release();
	frameArr->release();
}

void MEquipSelectLayer::onEnter(){
	CCLayer::onEnter();
}

void MEquipSelectLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCPoint point = ((CCTouch *)pTouches->anyObject())->getLocation();
	if(setEquipButton->boundingBox().containsPoint(point)){
		buttonSelect(setEquipButton);
	}else if(buyEquipButton->boundingBox().containsPoint(point)){
		buttonSelect(buyEquipButton);
	}else if(descripEquipButton->boundingBox().containsPoint(point)){
		buttonSelect(descripEquipButton);
	}
	for(int i  = 0; i < (int)buttonArr->count(); i++){
		CCSprite* button = static_cast<CCSprite *>(buttonArr->objectAtIndex(i));
		if(button->boundingBox().containsPoint(point)){
			int index = button->getTag();
			int i = index / 10;
			int j = index % 10;
			int oldI = oldIndex / 10;
			int oldJ = oldIndex % 10;
			CCSprite* newFrame = (CCSprite* )this->getChildByTag(100 + i * 10 +j);
			CCSize frameSize = newFrame->getContentSize();
			CCSprite* oldFrame = (CCSprite* )this->getChildByTag(100 + oldI * 10 + oldJ);
			oldFrame->setTextureRect(CCRect(frameSize.width, 0, frameSize.width, frameSize.height));
			newFrame->setTextureRect(CCRect(0, 0 ,frameSize.width, frameSize.height));
			oldIndex = index;
		}
	}
}

void MEquipSelectLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
}

void MEquipSelectLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	if(MToast::isShowToast){
		MToast* toast = (MToast* )this->getChildByTag(MToast::TAG);
		toast->MTouchesEnded(pTouches, pEvent);
	}
}