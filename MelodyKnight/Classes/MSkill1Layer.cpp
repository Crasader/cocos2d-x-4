#include "MSkill1Layer.h"

using namespace cocos2d;
using namespace std;
// on "init" you need to initialize your instance

MSkill1Layer* MSkill1Layer::create(){
	MSkill1Layer *mSkill1Layer = new MSkill1Layer();
	if(mSkill1Layer && mSkill1Layer->init()){
		mSkill1Layer->initLayer();
		mSkill1Layer->autorelease();
		return mSkill1Layer;
	}
	CC_SAFE_DELETE(mSkill1Layer);
    return NULL;
}

bool MSkill1Layer::initLayer()
{
    bool bRet = false;
    do 
    {
		size = CCDirector::sharedDirector()->getWinSize();
		initBar();
		initButton();
		skillArray = CCArray::createWithCapacity(10);
		skillArray->retain();
		initSkill();
        bRet = true;
    } while (0);

    return bRet;
}

void MSkill1Layer::initButton(){
	skillButton = CCSprite::create("scene_skill/skill_button.png");
	skillButton->setScale(1.5f);
	skillButton->setPosition(ccp(size.width * 4 / 5, size.height / 4));
	this->addChild(skillButton);
	skillButton->setOpacity(0);
}

void MSkill1Layer::onExit(){
	skillArray->release();
}

void MSkill1Layer::initBar(){
	comboBar = CCSprite::create("scene_skill/combo_bar.png");
	comboBar->setPosition(ccp(size.width / 2, size.height / 5));
	this->addChild(comboBar);
	starsArray = CCArray::createWithCapacity(50);
	starsArray->retain();
	comboSuccess = CCSprite::create("scene_skill/combo_success.png");
	comboFail = CCSprite::create("scene_skill/combo_fail.png");
	comboSuccess->setScale(1.0f);
	comboSuccess->setOpacity(0);
	this->addChild(comboSuccess);
	comboFail->setScale(1.0f);
	comboFail->setOpacity(0);
	this->addChild(comboFail);
	comboSuccess->setPosition(ccp(size.width - 250, size.height / 5));
	comboFail->setPosition(ccp(size.width - 250, size.height / 5));
}

void MSkill1Layer::initSkill(){
	this->unschedule(schedule_selector(MSkill1Layer::showCombo));
	initDescript();
	lastTime = Utils::millisecondNow();
	comboFail->setOpacity(0);
	descriptIndex = 0;
	isLongTouch = false;
	skillArray->removeAllObjects();

}

void MSkill1Layer::showButton(){
	Utils::playMusic("music/HolyKnight.mp3",false);
	this->target = GameManager::sharedGameManager()->getRole();
	initSkill();
	skillButton->setOpacity(255);
	SkillManager::currentTime = Utils::millisecondNow();
	this->schedule(schedule_selector(MSkill1Layer::showCombo), 0.2f);
}

void MSkill1Layer::skilldownCallback(){
	isLongTouch = false;
	if(starsArray->count() > 0){
		CCSprite *star =  static_cast<CCSprite *> (starsArray->objectAtIndex(0));
		if(star->getPositionX() > size.width - 350 && star->getPositionX() < size.width - 250){
			if(star->getTag() == 1){
				isLongTouch = true;
			}
			resetStar(star);
			showComboSuccess(comboSuccess);
			MSkill* skill = MSkill::create(GameManager::sharedGameManager()->getRole(), SKILL_ID, MSkill::ROLE_TYPE);
			CCArray* skillArr = CCArray::createWithCapacity(1);
			skillArr->addObject(skill);
			GameManager::sharedGameManager()->setSkills(skillArr);
		}else{
			showComboSuccess(comboFail);
		}
	}
}

void MSkill1Layer::skillupCallback(){
	if(starsArray->count() > 0){
		CCSprite *star =  static_cast<CCSprite *> (starsArray->objectAtIndex(0));
		if(star->getPositionX() > size.width - 350 && star->getPositionY() < size.width - 250 && isLongTouch){
			resetStar(star);
			showComboSuccess(comboSuccess);
			MSkill* skill = MSkill::create(this->target, SKILL_ID,  MSkill::ROLE_TYPE);
			CCArray* skillArr = CCArray::createWithCapacity(1);
			skillArr->addObject(skill);
			GameManager::sharedGameManager()->setSkills(skillArr);
		}
	}
	isLongTouch = false;
}

void MSkill1Layer::showCombo(float dt){
	if(Utils::millisecondNow() - lastTime > dely){
		lastTime = Utils::millisecondNow();
		CCSprite* comboStar = CCSprite::create("scene_skill/combo_star.png");
		comboStar->setTag(atoi(Utils::split(descript.at(descriptIndex), "||").at(1).c_str()));
		starsArray->addObject(comboStar);
		this->addChild(comboStar);
		comboStar->setPosition(ccp(size.width - 850, size.height / 5));
		comboStar->runAction(CCSequence::create(CCMoveTo::create(atoi(Utils::split(descript.at(descriptIndex), "||").at(2).c_str()), ccp(size.width - 250, size.height  / 5)) ,
			CCCallFuncN::create(this,callfuncN_selector(MSkill1Layer::missStar)), NULL));
		descriptIndex++;
		string type = descript.at(descriptIndex);
		if( type.compare("stop") == 0){
			comboFail->setOpacity(0);
			this->unschedule(schedule_selector(MSkill1Layer::showCombo));
		}
		string des = Utils::split(type, "||").at(0);
		char* jump = const_cast<char*>(des.c_str());
		Utils::trim(jump);
		dely = atoi(jump);
	}
}


void MSkill1Layer::showComboSuccess(CCSprite *pSender){
	pSender->setScale(1.0f);
	pSender->setOpacity(0);
	CCFiniteTimeAction* span = CCSpawn::create(CCScaleTo::create(0.2f, 2, 2), CCFadeIn::create(0.2f) , NULL);
	pSender->runAction(CCSequence::create(span, CCFadeOut::create(0.1f), NULL));
}

void MSkill1Layer::resetStar(CCNode* pSender){
	CCSprite *star =  static_cast<CCSprite *> (pSender);
	if(star->getOpacity() != 0){
		star->setOpacity(0);
		star->removeFromParentAndCleanup(true);
		starsArray->removeObject(star,true);
	}
}

void MSkill1Layer::missStar(CCNode* pSender){
	isLongTouch = false;
	if(comboFail->getOpacity() != 255){
		comboFail->setScale(1.0f);
		comboFail->setOpacity(0);
		CCFiniteTimeAction* span = CCSpawn::create(CCScaleTo::create(0.2f, 2, 2), CCFadeIn::create(0.2f) , NULL);
		comboFail->runAction(CCSequence::create(span, CCFadeOut::create(0.1f), NULL));
	}
	resetStar(pSender);
}

void MSkill1Layer::MTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCPoint point = ((CCTouch *)pTouches->anyObject())->getLocation();
	if(skillButton->getOpacity() != 0 && skillButton->boundingBox().containsPoint(point)){
		Utils::showVibrator(100);
		skilldownCallback();
	}
}

void MSkill1Layer::MTouchesMoved(CCSet *pTouches, CCEvent *pEvent){

}

void MSkill1Layer::MTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	CCPoint point = ((CCTouch *)pTouches->anyObject())->getLocation();
	if(skillButton->getOpacity() != 0 && skillButton->boundingBox().containsPoint(point)){
		skillupCallback();
	}
}

void MSkill1Layer::initDescript(){
	string script = Utils::getDataFromFile("scene_skill/HolyKnight.res");
	std::vector<std::string> song = Utils::split(script, "@");
	SkillManager::maxTime = atoi(song.at(0).c_str());
	descript = Utils::split(song.at(1), "&&");

}

