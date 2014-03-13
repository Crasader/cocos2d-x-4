#include "MmainScene.h"

using namespace cocos2d;
using namespace std;
int MmainScene::currentSection = 0;
int MmainScene::roleBornPointType = 2;

CCScene* MmainScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        MmainScene *layer = MmainScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MmainScene::init()
{
    bool bRet = false;
	this->setTouchEnabled(true);
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////
        CC_BREAK_IF(! CCLayer::init());
        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////
		AnimatePacker::getInstance()->loadAnimations("role/role_walk.xml"); 
		AnimatePacker::getInstance()->loadAnimations("enemy/boomA.xml"); 
		AnimatePacker::getInstance()->loadAnimations("role/roleEffect.xml"); 
		AnimatePacker::getInstance()->loadAnimations("enemy/enemy1.xml"); 
		AnimatePacker::getInstance()->loadAnimations("enemy/enemy2.xml"); 
		AnimatePacker::getInstance()->loadAnimations("enemy/enemy3.xml"); 
		AnimatePacker::getInstance()->loadAnimations("enemy/enemy4.xml"); 
		AnimatePacker::getInstance()->loadAnimations("map/gate.xml"); 
		AnimatePacker::getInstance()->loadAnimations("normal_skill/hit.xml"); 
		AnimatePacker::getInstance()->loadAnimations("normal_skill/attick.xml"); 
		AnimatePacker::getInstance()->loadAnimations("normal_skill/attick2.xml"); 
		AnimatePacker::getInstance()->loadAnimations("normal_skill/attick3.xml"); 
		AnimatePacker::getInstance()->loadAnimations("normal_skill/attick4.xml"); 
		AnimatePacker::getInstance()->loadAnimations("normal_skill/attick5.xml"); 
		size = CCDirector::sharedDirector()->getWinSize();
		comBoCount = 0;
		attickLastTime = Utils::millisecondNow();
		spriteLayer = CCLayer::create();
		this->addChild(spriteLayer);
		initMap();
		initArray();
		initRole();
		initTouchPad();
		initSkillSlot();
		initHit();
		switchPad();
		initBuff();
		shadow = CCLayerColor::create(ccc4(0,0,0,0));
		this->addChild(shadow,11,11);
		shadow->runAction(CCFadeOut::create(0.5f));
		this->schedule(schedule_selector(MmainScene::updateGame), 0.05f);
		hasBigSkill = false;
        bRet = true;
    } while (0);

    return bRet;
}

void MmainScene::initBuff(){
	buffLayer = CCLayer::create();
	this->addChild(buffLayer);
}

void MmainScene::initHit(){
	hitLayer = CCLayer::create();
	this->addChild(hitLayer);
	hit = CCSprite::create();
	hit->setOpacity(0);
	ccColor3B color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	comboCount = CCLabelTTF::create("", "Arial", 50);
	comboCount->setColor(color);
	CCString* str = CCString::createWithFormat("money : %d", GameManager::sharedGameManager()->getMoney());
	moneyCount = CCLabelTTF::create(str->getCString(), "Arial", 50);
	color.r = 255;
	color.g = 0;
	color.b = 0;
	moneyCount->setColor(color);
	hitLayer->addChild(hit);
	hitLayer->addChild(comboCount);
	hitLayer->addChild(moneyCount);
	hit->setPosition(ccp(size.width - 100, size.height - 200));
	comboCount->setPosition(ccp(size.width - 100, size.height - 200));
	moneyCount->setPosition(ccp(size.width - 500, size.height - 100));
}

void MmainScene::initSkillSlot(){
	controlSkillMenu = CCMenu::create();
	CCMenuItemImage* controlSkillItem = CCMenuItemImage::create("ui/skill_sscene.png", "ui/skill_sscene.png", this, menu_selector(MmainScene::controlSkillCallback));
	controlSkillMenu->addChild(controlSkillItem);
	controlSkillItem->setPosition(-50, 100);

	CCMenuItemImage* returnlItem = CCMenuItemImage::create("icon/return.png", "icon/return.png", this, menu_selector(MmainScene::returnBack));
	returnlItem->setScale(1.5f);
	controlSkillMenu->addChild(returnlItem);
	returnlItem->setPosition(-50, 200);

	this->addChild(controlSkillMenu,0,0);
	controlSkillMenu->setPosition(size.width - 50, size.height - 300);
}

void MmainScene::initArray(){
	int count = enemyInfo.size();
	for(int i = 0; i < count; i++){
		    int id = atoi(Utils::split(enemyInfo.at(i), "|").at(0).c_str());
			int positionX = atoi(Utils::split(enemyInfo.at(i), "|").at(1).c_str());
			int positionY = atoi(Utils::split(enemyInfo.at(i), "|").at(2).c_str());
			string itemDescript = Utils::split(enemyInfo.at(i), "|").at(3);
			Menemy* enemy = Menemy::create(id, positionX, positionY, 2, 0, itemDescript);
			enemy->isShow = false;
			enemy->isDestroy = false;
			GameManager::sharedGameManager()->getEnemysArr()->addObject(enemy);
	}
}

void MmainScene::skillCallback(CCObject* sender){
	CCMenuItemImage* skill = static_cast<CCMenuItemImage*>(sender);
	if(SkillManager::skillStatu == SkillManager::BIG_SKILL_STATU){
		Utils::playMusic("music/bgm.mp3",true);
		SkillManager::skillStatu = SkillManager::CONTROL_SKILL_STATU;
	}else{
		if(mpBar->getPercentage() < 100){
			return;
		}
		role->curMp = 0;
		mpBar->setPercentage(role->getMp());
		SkillManager::skillStatu = SkillManager::BIG_SKILL_STATU;
	}
	switchPad();
}

void MmainScene::controlSkillCallback(CCObject* sender){
	if(GameManager::itemSkillId >= 0){
		ShowItemSkill();
		GameManager::itemSkillId = -1;
	}
}

void MmainScene::menuCallback(CCObject* sender){
	CCMenuItemFont* item = (CCMenuItemFont *)sender;
	switch(item->getTag()){
	case 10:
		this->shadow->setOpacity(0);
		this->removeChildByTag(33);
		break;
	case 11:
		CCScene *pScene = MWorldMapScene::scene();
		Utils::stopMusic();
		currentSection = 0;
		roleBornPointType = 2;
		CCDirector::sharedDirector()->replaceScene(pScene);
		this->unschedule(schedule_selector(MmainScene::updateGame));
		break;
	}
}

void MmainScene::returnBack(CCObject* sender){
	this->shadow->setOpacity(100);
	CCMenuItemFont::setFontSize(100);  
	CCMenuItemFont::setFontName("Arial");
	CCMenu* returnMenu = CCMenu::create();
    CCMenuItemFont *continueItem= CCMenuItemFont::create("continue", this, menu_selector(MmainScene::menuCallback));
	continueItem->setPosition(0, 100);
	continueItem->setTag(10);
	returnMenu->addChild(continueItem);
	CCMenuItemFont *quiteItem= CCMenuItemFont::create("quite", this, menu_selector(MmainScene::menuCallback));
	quiteItem->setPosition(0, -100);
	quiteItem->setTag(11);
	returnMenu->addChild(quiteItem);
	returnMenu->setPosition(size.width / 2, size.height / 2);
	this->addChild(returnMenu,0,33);
}

void MmainScene::switchPad(){
	this->removeChildByTag(SKILL_BIG_LAYER_TAG);
	this->removeChildByTag(SKILL_CONTROL_LAYER_TAG);
	if(SkillManager::skillStatu == SkillManager::BIG_SKILL_STATU){
		this->addChild(SkillManager::sharedSkillManager()->getBigSkillLayer(), 0, SKILL_BIG_LAYER_TAG);
		SkillManager::sharedSkillManager()->showButton();
	}else if(SkillManager::skillStatu == SkillManager::CONTROL_SKILL_STATU){
		this->addChild(SkillManager::sharedSkillManager()->getSkillControlLayer(), 0, SKILL_CONTROL_LAYER_TAG);
	}
}

void MmainScene::initRole(){
	role=MRole::create();
	spriteLayer->addChild(role);
	role->setPosition(getRoleBornPoint(roleBornPointType));
	GameManager::sharedGameManager()->setRole(role);

	skillMenu = CCMenu::create();
	CCMenuItemImage* roleHead = CCMenuItemImage::create("ui/role_head.png", "ui/role_head.png", this, menu_selector(MmainScene::skillCallback));
	skillMenu->addChild(roleHead);
	this->addChild(skillMenu,0,0);
	skillMenu->setPosition(ccp(90, size.height - 100));

	headBar = CCSprite::create("ui/headbar.png");
	this->addChild(headBar);
	headBar->setPosition(ccp(170, size.height - 100));

	hpBar = CCProgressTimer::create(CCSprite::create("ui/hp_bar.png"));
	hpBar->setScale(1.5f);
	hpBar->setPosition(ccp(223, size.height - 98));
	hpBar->setType(kCCProgressTimerTypeBar);
	hpBar->setMidpoint(ccp(0,0));
	hpBar->setBarChangeRate(ccp(1, 0));
	hpBar->setPercentage(100);
	this->addChild(hpBar);

	mpBar = CCProgressTimer::create(CCSprite::create("ui/mp_bar.png"));
	mpBar->setScale(1.5f);
	mpBar->setPosition(ccp(205, size.height - 118));
	mpBar->setType(kCCProgressTimerTypeBar);
	mpBar->setMidpoint(ccp(0,0));
	mpBar->setBarChangeRate(ccp(1, 0));
	mpBar->setPercentage(role->getMp());
	this->addChild(mpBar);
}

void MmainScene::initTouchPad(){
	float joystickRadius=100;
	joystick=new SneakyJoystick();
	joystick->autorelease();
	joystick->initWithRect(CCRectZero);
	joystick->setAutoCenter(true);
	joystick->setHasDeadzone(true);
	joystick->setDeadRadius(10);
	joystickSkin=new SneakyJoystickSkinnedBase();
	joystickSkin->autorelease();
	joystickSkin->init();
	joystickSkin->setBackgroundSprite(CCSprite::create("ui/pad.png"));
	joystickSkin->setThumbSprite(CCSprite::create("ui/ball.png"));
	joystickSkin->setJoystick(joystick);
	joystickSkin->setPosition(ccp(-100,-100));
	this->addChild(joystickSkin);
	
}

void MmainScene::updateBuff(){
	if(GameManager::buffHasChange){
		GameManager::buffHasChange = false;
		buffLayer->removeAllChildren();
		int space = 70;
		CCArray* buffArray = GameManager::sharedGameManager()->buffArray;
		for(int i = 0; i < (int)buffArray->count(); i ++){
			int type = static_cast<CCInteger*>(buffArray->objectAtIndex(i))->getValue();
			CCString* name = CCString::createWithFormat("normal_skill/buff%d.png",type);
			CCSprite* buffIcon = CCSprite::create(name->getCString());
			buffLayer->addChild(buffIcon);
			buffIcon->setPosition(ccp(70 + space * i, size.height - 180));
		}
	}
}

void MmainScene::updateGame(float t){
	if(GameManager::monenyHasChange){
		CCString* str = CCString::createWithFormat("money : %d", GameManager::sharedGameManager()->getMoney());
		moneyCount->setString(str->getCString());
		GameManager::monenyHasChange = false;
	}
	updateBuff();
	updateSKillProgram(t);
	CCPoint velocity=joystick->getVelocity();  
	if(!Utils::isPlaying()){
		Utils::playMusic("music/bgm.mp3",true);
		SkillManager::skillStatu = SkillManager::CONTROL_SKILL_STATU;
		switchPad();
	}
	updateRole(velocity.x, velocity.y);
	hpBar->setPercentage(role->getHp());
	checkGameCollision();
	GameManager::sharedGameManager()->update();
	if(GameManager::comboCount == 0){
		comboCount->setString("");
		hit->stopAllActions();
		hit->setOpacity(0);
	}else{
		CCString* str = CCString::createWithFormat("%d", GameManager::comboCount);
		comboCount->setString(str->getCString());
		if(hit->getOpacity() == 0){
			hit->setOpacity(255);
			hit->runAction(CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate("hit")));
		}
	}
}

void MmainScene::exitMap(CCNode* pSender, void* data){
	SkillManager::skillStatu = SkillManager::CONTROL_SKILL_STATU;
	CCDirector::sharedDirector()->getRunningScene()->removeChild(this);
	MmainScene *layer = MmainScene::create();
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
}

void MmainScene::checkGameCollision(){
	int gateType = GameManager::sharedGameManager()->isGate(ccp((int)((role->getPositionX() + 60) / 120), 
		(int)((720 - role->getPositionY()) / 120)));
	if(gateType != -1){
		for(int i = 0; i < (int)gateInfo.size(); i++){
			vector<string> gateInfos = Utils::split(gateInfo.at(i), "|");
			int gatePositionType = atoi(gateInfos.at(0).c_str());
			int sceneId = atoi(gateInfos.at(1).c_str());
			if(gatePositionType == gateType){
				currentSection = sceneId;
				roleBornPointType = gatePositionType;
			}
		}
		this->unschedule(schedule_selector(MmainScene::updateGame));
		this->removeChildByTag(SKILL_BIG_LAYER_TAG);
		shadow->runAction(CCFadeIn::create(0.5f));
		shadow->runAction(CCSequence::create(CCFadeIn::create(0.5f),
			CCCallFuncND::create(this,callfuncND_selector(MmainScene::exitMap), 0),NULL));
	}
	
	spriteLayer->reorderChild(role, -role->getPositionY());
	
	CCObject *objItem;
	CCObject *objEnemy;
	CCObject *objEffect;

	CCArray* effectsArray = GameManager::sharedGameManager()->getEffectsArr();
	CCARRAY_FOREACH(effectsArray, objEffect)
	{
		MEffect* effect = dynamic_cast<MEffect*>(objEffect);
		if(!effect->isShow){
			effect->isShow = true;
			spriteLayer->addChild(effect);
		}
		if(	effect->isHook){
			effect->setZOrder(-9998);
		}else{
			spriteLayer->reorderChild(effect, -effect->getPositionY());
		}
	}

	CCArray* enemysArray = GameManager::sharedGameManager()->getEnemysArr();
	CCARRAY_FOREACH(enemysArray, objEnemy)
	{
		Menemy* enemy = dynamic_cast<Menemy*>(objEnemy);
		if(!enemy->isShow){
			enemy->isShow = true;
			spriteLayer->addChild(enemy);
		}
		spriteLayer->reorderChild(enemy, -enemy->getPositionY());
	}

	CCArray* itemsArray = GameManager::sharedGameManager()->getItemsArr();
	CCARRAY_FOREACH(itemsArray, objItem)
	{
		MItem* item = dynamic_cast<MItem*>(objItem);
		if(!item->isShow){
			item->isShow = true;
			spriteLayer->addChild(item);
		}
		spriteLayer->reorderChild(item, -item->getPositionY());
	}
}

void MmainScene::updateSKillProgram(float dely){
	if(role->curMp >= role->maxMp){
		if(!hasBigSkill){
			hasBigSkill  =true;
			particle = CCParticleFlower::create();
			particle->setTotalParticles(40);
			particle->setTexture( CCTextureCache::sharedTextureCache()->addImage("ui/stars.png") );
			particle->setPosition(ccp(headBar->getContentSize().width - 220, headBar->getContentSize().height / 2));
			headBar->addChild(particle);
		}
	}else{
		if(hasBigSkill){
			headBar->removeAllChildrenWithCleanup(true);
			hasBigSkill = false;
		}
		float num = 1.0f;
		if(MRole::buffTime[0] != 0){
			num = num * (1 + 0.2);
		}
		role->curMp += num;
		mpBar->setPercentage(role->getMp());
	}
}

void MmainScene::ShowItemSkill(){
	Utils::showVibrator(100);
	MSkill* skill = MSkill::create(role, GameManager::itemSkillId, MSkill::ROLE_TYPE);
	CCArray* skillArr = CCArray::createWithCapacity(1);
	skillArr->addObject(skill);
	GameManager::sharedGameManager()->setSkills(skillArr);
}

void MmainScene::initMap(){
	loadMapInfo();
}

void MmainScene::loadMapInfo(){
	CCString* data = CCString::createWithFormat("map/scene%d.res", GameManager::curSection);
	string script = Utils::getDataFromFile(data->getCString());
	vector<string> mapInfo = Utils::split(script, "#");
	vector<string> currentMapInfo = Utils::split(mapInfo.at(currentSection), "@");
	char* back = const_cast<char*>(currentMapInfo.at(0).c_str());
	Utils::trim(back);
	map = CCTMXTiledMap::create(back);
	spriteLayer->addChild(map,-9999,0);
	map->setPosition(ccp(-60, 0));
	GameManager::sharedGameManager()->setMap(map);
	GameManager::sharedGameManager()->setMapSize(map->getContentSize());
	gateInfo = Utils::split(currentMapInfo.at(1), "&");
	enemyInfo = Utils::split(currentMapInfo.at(2), "&");
}

CCPoint MmainScene::getRoleBornPoint(int type){
	switch (type)
	{
	case 0://right
		spriteLayer->setPosition(ccp(size.width - 600 - size.width / 2, spriteLayer->getPositionY()));
		return ccp(size.width - 200 - spriteLayer->getPositionX(), size.height / 2 + 100);
	case 1://bottom
		return ccp(size.width / 2 + 40, 150);  
	case 2://left
		spriteLayer->setPosition(ccp(600 - size.width / 2, spriteLayer->getPositionY()));
		return ccp(200 - spriteLayer->getPositionX(), size.height / 2 + 100);
	case 3://up
		return ccp(size.width / 2 + 40, size.height - 150);
	default:
		return CCPointZero;
		break;
	}
}

void MmainScene::updateRole(float velocityX, float velocityY){
	role->update();
	role->showAni(velocityX, velocityY);
	CCPoint movePoint = ccpAdd(role->getPosition(),ccp(velocityX * GameManager::rate, velocityY * GameManager::rate));
	if(GameManager::sharedGameManager()->isObstruct(ccp((int)((movePoint.x + 60) / 120), 
		(int)((720 - movePoint.y) / 120)))){
		return;
	}
	role->roleMoveType = 0;
	if((role->roleAniType == 1)&& spriteLayer->getPositionX() > -size.width / 2 + 490 
		&& spriteLayer->getPositionX() + role->getPositionX() > size.width / 2){
		role->roleMoveType = 1;
	}
	if((role->roleAniType ==0) && spriteLayer->getPositionX() < size.width / 2 - 490 
		&& spriteLayer->getPositionX() + role->getPositionX() < size.width / 2){
		role->roleMoveType = 1;
	}
	switch(role->roleMoveType){
	case 0:
		movePoint.x = min(max((int)movePoint.x, (int)(80 - spriteLayer->getPositionX())), (int)(size.width - 80 - spriteLayer->getPositionX()));
		movePoint.y = min(max((int)movePoint.y, 80), (int)(size.height - 80));
		role->setPosition(movePoint);
		break;
	case 1:
		spriteLayer->setPosition(ccpSub(spriteLayer->getPosition(), ccp(velocityX * GameManager::rate, 0)));
		role->setPosition(ccp(role->getPositionX() +  velocityX * GameManager::rate,  min(max((int)movePoint.y, 80), (int)(size.height - 80))));
		break;
	}
}

void MmainScene::showShock(){
	CCJumpTo* mJumpTo =CCJumpTo::create(0.5f, spriteLayer->getPosition(), 30.0f, 4);
	spriteLayer->runAction(mJumpTo);
}

void MmainScene::showStandstill(){
	CCDirector::sharedDirector()->pause();
	Sleep(500);
	CCDirector::sharedDirector()->resume();
}

void MmainScene::onEnter(){
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
}

void MmainScene::onExit(){
	CCLayer::onExit();
	GameManager::sharedGameManager()->release();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	this->unschedule(schedule_selector(MmainScene::updateGame));
}

void MmainScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	SkillManager::sharedSkillManager()->MTouchesBegan(pTouches, pEvent);
}

void MmainScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	SkillManager::sharedSkillManager()->MTouchesMoved(pTouches, pEvent);
}

void MmainScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	SkillManager::sharedSkillManager()->MTouchesEnded(pTouches, pEvent);
}

