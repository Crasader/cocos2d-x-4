#include "MWorldMapScene.h"

using namespace cocos2d;
using namespace std;

CCScene* MWorldMapScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        MWorldMapScene *layer = MWorldMapScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MWorldMapScene::init()
{
    bool bRet = false;
	this->setTouchEnabled(true);
    do 
    {
		CC_BREAK_IF(! CCLayer::init());
		size = CCDirector::sharedDirector()->getWinSize();
		AnimatePacker::getInstance()->loadAnimations("role/role_walk.xml"); 
		AnimatePacker::getInstance()->loadAnimations("role/roleEffect.xml"); 
		AnimatePacker::getInstance()->loadAnimations("worldMap/flag.xml"); 
		mapIndex = 0;
		initScript();
		initMap();
		initSign();
		initRole();
		initButton();
		MToast* toast = MToast::create();
		toast->setToastString("welcome to the game");
		this->addChild(toast);
		isShowMenu = true;
        bRet = true;
    } while (0);

    return bRet;
}

void MWorldMapScene::initScript(){
	CCString* data = CCString::createWithFormat("worldMap/section%d.res", mapIndex);
	string script = Utils::getDataFromFile(data->getCString());
	vector<string> sectionInfo = Utils::split(script, "#");
	pointVec.clear();
	pointIsBoss.clear();
	sectionIndex.clear();
	for(int i = 0; i < (int)sectionInfo.size(); i++){
		int sectionX = atoi(Utils::split(sectionInfo.at(i),"@").at(0).c_str());
		int sectionY = atoi(Utils::split(sectionInfo.at(i),"@").at(1).c_str());
		pointVec.push_back(ccp(sectionX, sectionY));
		pointIsBoss.push_back(Utils::split(sectionInfo.at(i),"@").at(2).compare("boss") == 0);
		sectionIndex.push_back(atoi(Utils::split(sectionInfo.at(i),"@").at(3).c_str()));
	}
}

void MWorldMapScene::initButton(){
	skillSelect = CCSprite::create("worldMap/skill_selector.png");
	skillSelect->setScale(1.5f);
	skillSelect->setPosition(ccp(100, size.height - 100));
	skillSelect->setTag(10);
	CCLabelTTF* skillSelectText = CCLabelTTF::create(Utils::getString("select_skill"), "Arial", 30);
	skillSelectText->setTag(12);
	skillSelectText->setPosition(ccp(100, size.height - 145));

	equipSelect = CCSprite::create("worldMap/equip_selector.png");
	equipSelect->setScale(1.5f);
	equipSelect->setPosition(ccp(100, size.height - 250));
	equipSelect->setTag(11);
	CCLabelTTF* equipSelectText = CCLabelTTF::create(Utils::getString("select_equip"), "Arial", 30);
	equipSelectText->setPosition(ccp(100, size.height - 285));
	equipSelectText->setTag(13);

	left = CCSprite::create("worldMap/left.png");
	left->setPosition(ccp(size.width - 200, size.height - 50));
	left->setTag(18);
	right = CCSprite::create("worldMap/right.png");
	right->setPosition(ccp(size.width - 100, size.height - 50));
	right->setTag(19);
	this->addChild(skillSelect);
	this->addChild(equipSelect);
	this->addChild(skillSelectText);
	this->addChild(equipSelectText);
	this->addChild(left);
	this->addChild(right);
}

void MWorldMapScene::initMap(){
	worldMap = CCSprite::create("worldMap/map.png");
	worldMap->setScale(2.0f);
	this->addChild(worldMap);
	worldMap->setPosition(ccp(size.width, 0));
}

void MWorldMapScene::initRole(){
	int defSection = CCUserDefault::sharedUserDefault()->getIntegerForKey(GameManager::keySection,0);
	role=MRole::create();
	role->setTag(21);
	this->addChild(role);
	role->setPosition(ccpAdd(pointVec.at(defSection), ccp(size.width / 2 + 10, size.height / 2 + 70)));
	endPoint = role->getPosition();
	role->showAni(0, 0);
}

void MWorldMapScene::initSign(){
	CCLayer* setionLayer = CCLayer::create();
	setionLayer->setTag(22);
	this->addChild(setionLayer);
	CCMenu* menu = CCMenu::create();
	for(int i = 0; i < (int)pointVec.size(); i++){
		CCSprite* section = CCSprite::create("worldMap/sign_normal.png");
		if(pointIsBoss.at(i)){
			section->setTexture(CCTextureCache::sharedTextureCache()->addImage("worldMap/sign_boss.png"));
		}
		section->setScale(1.5f);
		CCMenuItemSprite* sectionItem = CCMenuItemSprite::create(section, section, this, menu_selector(MWorldMapScene::gotoSection));
		sectionItem->setPosition(pointVec.at(i));
		sectionItem->setTag(sectionIndex.at(i));
		menu->addChild(sectionItem);
	}
	setionLayer->addChild(menu);
	int defSection = CCUserDefault::sharedUserDefault()->getIntegerForKey(GameManager::keySection,0);
	for(int i = 0; i < (int)pointVec.size(); i++){
		if(defSection == i){
			continue;
		}
		CCSprite* flag = CCSprite::create("worldMap/flag.png");
		CCPoint flagPoint;
		flagPoint.x = pointVec.at(i).x + size.width / 2 + 10;
		flagPoint.y = pointVec.at(i).y + size.height / 2 + 60;
		flag->setPosition(flagPoint);
		setionLayer->addChild(flag);
		flag->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("flag")));
	}

	/*CCSpriteBatchNode *spriteParent = CCSpriteBatchNode::create("worldMap/sign_small.png"); 
	this->addChild(spriteParent);
	for(int i = 0; i < 1; i++){
		float sx = pointVec.at(i).x + size.width / 2;
		float sy = pointVec.at(i).y + size.height / 2;
		float ex = pointVec.at(i + 1).x+ size.width / 2;
		float ey = pointVec.at(i + 1).y + size.height / 2;
		float angle = Utils::getAngle(ccp(sx, sy),ccp(ex, ey));
		int speedx = Utils::getSpeedX(roleRate, angle);
		int speedy = Utils::getSpeedY(roleRate, angle);
		for(int x = sx, y = sy; x < ex && y < ey; x = x+speedx *20, y = y+ speedy *20){
			CCSprite *sd = CCSprite::create("worldMap/sign_small.png");
			spriteParent->addChild(sd);
			sd->setPosition(ccp(x, y));
		}
	}*/
}

void MWorldMapScene::gotoSection(CCObject* sender){
	this->removeChildByTag(10);
	this->removeChildByTag(11);
	this->removeChildByTag(12);
	this->removeChildByTag(13);
	this->removeChildByTag(18);
	this->removeChildByTag(19);
	isShowMenu = false;
	CCMenuItemSprite* itemSprite = (CCMenuItemSprite *)sender;
	GameManager::curSection = itemSprite->getTag();
	endPoint = ccpAdd(itemSprite->getPosition(), ccp(size.width / 2, size.height / 2 + 50));
	this->unschedule(schedule_selector(MWorldMapScene::updateRole));
	this->schedule(schedule_selector(MWorldMapScene::updateRole), 0.05f);
}

void MWorldMapScene::buttonSelect(CCSprite* itemSprite){
	switch(itemSprite->getTag()){
	case 10:
		showSkillSelect();
		break;
	case 11:
		showeQuipSelect();
	case 18:
		changeMap(-1);
		break;
	case 19:
		changeMap(1);
		break;
	}
}

void MWorldMapScene::changeMap(int direction){
	if(mapIndex + direction< 4 && mapIndex + direction>=0){
		mapIndex+=direction;
		initScript();
		this->removeChildByTag(21);
		this->removeChildByTag(22);
		this->removeChildByTag(10);
		this->removeChildByTag(11);
		this->removeChildByTag(12);
		this->removeChildByTag(13);
		this->removeChildByTag(18);
		this->removeChildByTag(19);
		isShowMenu = false;
		CCMoveTo* move;
		switch(mapIndex){
		case 0:
			move = CCMoveTo::create(1.0f, ccp(size.width,0));
			break;
		case 1:
			move = CCMoveTo::create(1.0f, ccp(0,0));
			break;
		case 2:
			move = CCMoveTo::create(1.0f, ccp(size.width, size.height));
			break;
		case 3:
			move = CCMoveTo::create(1.0f, ccp(0,size.height));
			break;
		}
		worldMap->runAction(CCSequence::create(move, CCCallFuncN::create(this,callfuncN_selector(MWorldMapScene::initSection)),NULL));
	}
}

void MWorldMapScene::initSection(CCNode* pSender){
	initSign();
	initRole();
	initButton();
	isShowMenu = true;
}

void MWorldMapScene::showSkillSelect(){
	this->removeChildByTag(0);
	MskillSelectLayer *selectLayer = MskillSelectLayer::create();
	this->addChild(selectLayer,0,0);
}

void MWorldMapScene::showeQuipSelect(){
	this->removeChildByTag(1);
	MEquipSelectLayer *selectLayer = MEquipSelectLayer::create();
	this->addChild(selectLayer,0,1);
}

void MWorldMapScene::updateRole(float dely){
	CCPoint startPosition= role->getPosition();
	if(ccpDistance(startPosition, endPoint) > cotainRect){
		float angle = Utils::getAngle(startPosition, endPoint);
		int speedx = Utils::getSpeedX(roleRate, angle);
		int speedy = Utils::getSpeedY(roleRate, angle);
		role->setPosition(ccp(role->getPositionX() + speedx, role->getPositionY() + speedy));
		role->showAni(speedx, speedy);
	}else{
		this->unschedule(schedule_selector(MWorldMapScene::updateRole));
		this->removeChildByTag(21);
		this->removeChildByTag(22);
		initSection(this);
		MSectionSelectLayer* sectionSelect = MSectionSelectLayer::create();
		CCString* string =CCString::createWithFormat("section_name%d",GameManager::curSection);
		sectionSelect->setTitleString(Utils::getString(string->getCString()));
		this->addChild(sectionSelect);
	}
}

void MWorldMapScene::onEnter(){
	CCLayer::onEnter();
	Utils::playMusic("music/worldmap.mp3",true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
}

void MWorldMapScene::onExit(){
	CCLayer::onExit();
	pointVec.swap(vector<CCPoint>());
	pointIsBoss.swap(vector<bool>());
	sectionIndex.swap(vector<int>());
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void MWorldMapScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	if(!isShowMenu){
		return;
	}
	CCPoint point = ((CCTouch *)pTouches->anyObject())->getLocation();
	if(skillSelect->boundingBox().containsPoint(point)){
		buttonSelect(skillSelect);
	}else if(equipSelect->boundingBox().containsPoint(point)){
		buttonSelect(equipSelect);
	}else if(left->boundingBox().containsPoint(point)){
		buttonSelect(left);
	}else if(right->boundingBox().containsPoint(point)){
		buttonSelect(right);
	}
}

void MWorldMapScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
}

void MWorldMapScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	if(MToast::isShowToast){
		MToast* toast = (MToast* )this->getChildByTag(MToast::TAG);
		toast->MTouchesEnded(pTouches, pEvent);
	}
	if(MSectionSelectLayer::isShowToast){
		MSectionSelectLayer* sectionSelect = (MSectionSelectLayer* )this->getChildByTag(MSectionSelectLayer::TAG);
		sectionSelect->MTouchesEnded(pTouches, pEvent);
	}
}