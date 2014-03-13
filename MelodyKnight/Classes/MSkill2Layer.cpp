#include "MSkill2Layer.h"

using namespace cocos2d;
using namespace std;

static const char* defaultIcon[] = {"icon/defaut.png", "icon/defaut.png", "icon/defaut.png", "icon/defaut.png", "icon/change.png"};
int MSkill2Layer::skillLevel = 0;
// up, down, left, right, center
static const int buttonPosition[5][2] = {{-200, 300},{-200, 100},{-300, 200},{-100, 200},{-200,200}}; 
// up, down, left, right, leftUp, rightUp, leftDown, rightDown
//static const int skillChainPosition[8][2] = {{-200, 250},{-200, 150},{-250, 200},{-150, 200},
//                                                                    {-250,250},{-150, 250},{-250, 150},{-150, 150}}; 
// up, down, left, right, leftUp, rightUp, leftDown, rightDown
//static const float skillChainRotate[8] = { 90, 90, 0 , 0, 135, 45, 215, 315};

//static const int totalSkillId[3][4] = {{10001, 10001,10001,10001},{10002, 10001,10001,10001},{10001, 10001,10001,10001}}; 

MSkill2Layer* MSkill2Layer::create(){
	MSkill2Layer *mSkill2Layer = new MSkill2Layer();
	if(mSkill2Layer && mSkill2Layer->init()){
		mSkill2Layer->initLayer();
		mSkill2Layer->autorelease();
		return mSkill2Layer;
	}
	CC_SAFE_DELETE(mSkill2Layer);
    return NULL;
}

bool MSkill2Layer::initLayer()
{
    bool bRet = false;
    do 
    {
		size = CCDirector::sharedDirector()->getWinSize();
		//initSkillChain();
		initButton();
		skillArr = CCArray::createWithCapacity(10);
		skillArr->retain();
		skillIdArr = CCArray::createWithCapacity(10);
		skillIdArr->retain();
		particle = CCParticleFlower::create();
		particle->setTotalParticles(100);
		particle->setTexture( CCTextureCache::sharedTextureCache()->addImage("ui/stars.png") );
		particle->setPosition(ccp(buttonPosition[4][0], buttonPosition[4][1]));
		this->addChild(particle);
		isSkillStart = false;
        bRet = true;
    } while (0);

    return bRet;
}

//void MSkill2Layer::initSkillChain(){
//	skillChainArr = CCArray::createWithCapacity(8);
//	skillChainArr->retain();
//	for(int i = 0; i < 8; i++){
//		CCSprite* skillChain = CCSprite::create("ui/skill_chain.png");
//		skillChain->setTag(i);
//		skillChainArr->addObject(skillChain);
//		this->addChild(skillChain);
//		float px = size.width + skillChainPosition[i][0];
//		float py = skillChainPosition[i][1];
//		skillChain->setPosition(ccp(px, py));
//		skillChain->setRotation(skillChainRotate[i]);
//		skillChain->setOpacity(0);
//	}
//}

void MSkill2Layer::initButton(){
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
		button->setScale(1.5f);
		float px = size.width + buttonPosition[i][0];
		float py = buttonPosition[i][1];
		button->setPosition(ccp(px, py));
	}
}

void MSkill2Layer::onExit(){
	skillArr->release();
	buttonArr->release();
}

void MSkill2Layer::resetButton(){
	skillLevel++;
	if(skillLevel > 2){
		skillLevel =0;
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

void MSkill2Layer::MTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCPoint point = ((CCTouch *)pTouches->anyObject())->getLocation();
	particle->setPosition(point);
	for(int i  = 0; i < (int)buttonArr->count(); i++){
		CCSprite* button = static_cast<CCSprite *>(buttonArr->objectAtIndex(i));
		if(button->boundingBox().containsPoint(point) && GameManager::totalSkillPad[skillLevel][button->getTag()] >= 0){
			skillIdArr->addObject(CCInteger::create(button->getTag()));
			if(button->getTag() != 4){
				Utils::showVibrator(100);
				MSkill* skill = MSkill::create(GameManager::sharedGameManager()->getRole(), GameManager::totalSkillPad[skillLevel][button->getTag()],  MSkill::ROLE_TYPE);
				skillArr->addObject(skill);
				isSkillStart = true;
			}else{
				resetButton();
			}
			break;
		}
	}
}

void MSkill2Layer::MTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	CCPoint point = ((CCTouch *)pTouches->anyObject())->getLocation();
	particle->setPosition(point);
	dealOutOfSkillUp(point);
	for(int i  = 0; i < (int)buttonArr->count(); i++){
		CCSprite* button = static_cast<CCSprite *>(buttonArr->objectAtIndex(i));
		bool isHide = true;
		if(skillIdArr->count() > 0){
			CCInteger*  lastId = static_cast<CCInteger*>(skillIdArr->lastObject());
			isHide = button->getTag() != lastId->getValue();
		}
		if(isSkillStart && button->boundingBox().containsPoint(point) && isHide
			&& GameManager::totalSkillPad[skillLevel][button->getTag()] >= 0){
			skillIdArr->addObject(CCInteger::create(button->getTag()));
			//showChain(skillIdArr->count());
			if(button->getTag() != 4){
				Utils::showVibrator(100);
				MSkill* skill = MSkill::create(GameManager::sharedGameManager()->getRole(),  GameManager::totalSkillPad[skillLevel][button->getTag()],  MSkill::ROLE_TYPE);
				skillArr->addObject(skill);
				isSkillStart = true;
			}else{
				if(!isHideSkillUp){
					this->scheduleOnce(schedule_selector(MSkill2Layer::showSkillUp),0.3f);
				}
				isHideSkillUp = true;
			}
			break;
		}
	}
}

void MSkill2Layer::dealOutOfSkillUp(CCPoint point){
	if(isHideSkillUp){
		CCSprite* button = static_cast<CCSprite *>(buttonArr->objectAtIndex(4));
		if(!button->boundingBox().containsPoint(point)){
			isHideSkillUp = false;
			this->unschedule(schedule_selector(MSkill2Layer::showSkillUp));
		}
	}
}

void MSkill2Layer::showSkillUp(float dely){
	Utils::showVibrator(100);
	resetButton();
}

void MSkill2Layer::MTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	if(skillArr->count() > 0){
		GameManager::sharedGameManager()->setSkills(skillArr);
	}
	isSkillStart = false;
	particle->setPosition(ccp(buttonPosition[4][0], buttonPosition[4][1]));
	//hideChain();
	skillArr->removeAllObjects();
}

//void MSkill2Layer::hideChain(){
//	for(int i = 0; i < (int)skillChainArr->count(); i++){
//		CCSprite* skillChain = static_cast<CCSprite *>(skillChainArr->objectAtIndex(i));
//		skillChain->setOpacity(0);
//		skillIdArr->removeAllObjects();
//	}
//}

// up, down, left, right, center
// up, down, left, right, leftUp, rightUp, leftDown, rightDown

//void MSkill2Layer::showChain(int count){
//	if(count > 1){
//		int lastSkillId = (static_cast<CCInteger *>(skillIdArr->objectAtIndex(count - 1)))->getValue();
//		int secondSkillId = (static_cast<CCInteger *>(skillIdArr->objectAtIndex(count - 2)))->getValue();
//		int type = min(lastSkillId, secondSkillId) * 10 +max(lastSkillId, secondSkillId) ;
//		switch(type){
//		case 01:
//			(static_cast<CCSprite *>(skillChainArr->objectAtIndex(0)))->setOpacity(255);
//			(static_cast<CCSprite *>(skillChainArr->objectAtIndex(1)))->setOpacity(255);
//			break;
//		case 02:
//			(static_cast<CCSprite *>(skillChainArr->objectAtIndex(4)))->setOpacity(255);
//			break;
//		case 03:
//			(static_cast<CCSprite *>(skillChainArr->objectAtIndex(5)))->setOpacity(255);
//			break;
//		case 04:
//			(static_cast<CCSprite *>(skillChainArr->objectAtIndex(0)))->setOpacity(255);
//			break;
//		case 12:
//			(static_cast<CCSprite *>(skillChainArr->objectAtIndex(6)))->setOpacity(255);
//			break;
//		case 13:
//			(static_cast<CCSprite *>(skillChainArr->objectAtIndex(7)))->setOpacity(255);
//			break;
//		case 14:
//			(static_cast<CCSprite *>(skillChainArr->objectAtIndex(1)))->setOpacity(255);
//			break;
//		case 23:
//			(static_cast<CCSprite *>(skillChainArr->objectAtIndex(2)))->setOpacity(255);
//			(static_cast<CCSprite *>(skillChainArr->objectAtIndex(3)))->setOpacity(255);
//			break;
//		case 24:
//			(static_cast<CCSprite *>(skillChainArr->objectAtIndex(2)))->setOpacity(255);
//			break;
//		case 34:
//			(static_cast<CCSprite *>(skillChainArr->objectAtIndex(3)))->setOpacity(255);
//			break;
//		}
//	}
//}