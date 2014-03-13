#include "FairyMain.h"
#include "FairyHero.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

int FairyMain::currentStatic = 1;

CCScene* FairyMain::scene(int section)
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        // 'layer' is an autorelease object
        FairyMain *layer = FairyMain::create();
        CC_BREAK_IF(! layer);
		layer->section = section;
        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool FairyMain::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());
		isCanScoll = true;
		enemyIndex = 0;
		heroIndex = 0;
		srand((unsigned) time(NULL)); 
		int random = rand() % 3;
		CCString *fileName = CCString::createWithFormat(BattleMusic, random);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(fileName->getCString(), true);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
		mapLayer = FairyMap::create();
		addChild(mapLayer);
		fairyHeroManager = FairyHeroManager::create();
		addChild(fairyHeroManager);
		selectHero = NULL;
		areaLayer = FairyArea::create();
		addChild(areaLayer);
		FairyControlPanel* controlLayer = FairyControlPanel::create();
		addChild(controlLayer, 0, 0);
		titlePanel = FairyTitlePanel::create();
		addChild(titlePanel, 0, 0);
        bRet = true;
    } while (0);
    return bRet;
}

void FairyMain::handleResult(bool isWin){
	if(isWin){
		currentStatic = HEROMOVE;
		CCUserDefault::sharedUserDefault()->setIntegerForKey(keySection,this->section + 1);
		CCUserDefault::sharedUserDefault()->flush();
		CCScene *pScene =  FairyChat::scene(this->section, 1);
		CCDirector::sharedDirector()->replaceScene(pScene);
	}else{

	}
}

void FairyMain::updateFairyStatic(float t){
	int heroCount = 0;
	switch(FairyMain::currentStatic){
		case HEROMOVE:
			heroCount = fairyHeroManager->getHeroCount();
			if(heroCount == 0){
				handleResult(false);
			}
			titlePanel->setMoveCount(moveCount);
			break;
		case HEROATTACT:
			doAttactEnemy();
			break;
		case ENEMYMOVE:
			resultCount = fairyHeroManager->getEnemyCount();
			if(resultCount == 0){
				handleResult(true);
			}else{
				titlePanel->setResultCount(resultCount);
				doEnemyAI();
			}
			break;
		case ENEMYATTACT:
			doAttactHero();
			break;
	}
}

void FairyMain::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
	CCTMXTiledMap* map = (CCTMXTiledMap*)this->getChildByTag(0);
}

void FairyMain::doClickButton(int tag){
	if(selectHero != NULL){
		selectHero->currentSkillId = selectHero->skillIds.at(tag);
	}else{
		doMoveHero(tag);
	}
}

void FairyMain::doEnemyAI(){
	CCArray* EnemyArr = fairyHeroManager->getEnemyArr();
	CCObject *obj;
	CCARRAY_FOREACH(EnemyArr, obj){
		FairyEnemy* fairyEnemy = dynamic_cast<FairyEnemy* >(obj);
		if(!fairyEnemy->isDead){
			fairyEnemy->doAI();
		}
	}
	currentStatic = ENEMYATTACT;
}

void FairyMain::doMoveHero(int tag){
	if(currentStatic == HEROMOVE){
		CCArray* heroArr = fairyHeroManager->getHeroArr();
		CCArray* enemyArr = fairyHeroManager->getEnemyArr();
		CCObject *obj;
		switch(tag){
            case FairyControlPanel::leftDrection:
				CCARRAY_FOREACH(heroArr, obj){
					FairyHero* fairyHero = dynamic_cast<FairyHero* >(obj);
					mapLayer->moveHero(FairyControlPanel::leftDrection, fairyHero, fairyHero->heroId);
					fairyHero->move();
				}
				break;
			case FairyControlPanel::upDrection:
				if(isCanMove()){
					CCARRAY_FOREACH(heroArr, obj){
						FairyHero* fairyHero = dynamic_cast<FairyHero* >(obj);
						mapLayer->moveHero(FairyControlPanel::upDrection, fairyHero, fairyHero->heroId);
						fairyHero->move();
					}
					isCanScoll = mapLayer->doScollMap();
					if(isCanScoll){
						CCARRAY_FOREACH(enemyArr, obj){
							FairyEnemy* fairyEnemy = dynamic_cast<FairyEnemy* >(obj);
							mapLayer->moveHero(5, fairyEnemy, fairyEnemy->heroId);
						}
					}
				}
				break;
			case FairyControlPanel::holdDrection:
				CCARRAY_FOREACH(heroArr, obj){
					FairyHero* fairyHero = dynamic_cast<FairyHero* >(obj);
					fairyHero->move();
				}
				break;
			case FairyControlPanel::rightDrection:
				CCARRAY_FOREACH(heroArr, obj){
					FairyHero* fairyHero = dynamic_cast<FairyHero* >(obj);
					mapLayer->moveHero(FairyControlPanel::rightDrection, fairyHero, fairyHero->heroId);
					fairyHero->move();
				}
				break;
		}
		FairyControlPanel* controlLayer = (FairyControlPanel*)this->getChildByTag(0);
		controlLayer->setInvisiable();
		currentStatic = HEROATTACT;
		moveCount = fairyHeroManager->getHeroArr()->count();
	}
}

bool FairyMain::isCanMove(){
	bool isCanMove = true;
	CCArray* heroArr = fairyHeroManager->getHeroArr();
	CCArray* enemyArr = fairyHeroManager->getEnemyArr();
	CCObject *obj;
	CCARRAY_FOREACH(enemyArr, obj){
		FairyEnemy* fairyEnemy = dynamic_cast<FairyEnemy* >(obj);
		if(!fairyEnemy->isDead && fairyEnemy->getPositionY() <= FairyMap::tileHeight / 2){
			isCanMove = false;
		}
	}
	CCARRAY_FOREACH(heroArr, obj){
		FairyHero* fairyHero = dynamic_cast<FairyHero* >(obj);
		fairyHero->move();
		vector<int> oldPoint = mapLayer->gameLocationToMap(fairyHero->getPosition(), mapLayer->getMapY());
		if(!fairyHero->isDead && mapLayer->mapObject[oldPoint.at(0)][oldPoint.at(1) + 1]  != -1){
			isCanMove = false;
		}
	}
	return isCanMove;
}

void FairyMain::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
	this->schedule(schedule_selector(FairyMain::updateFairyStatic), 0.05f);
	fairyHeroManager->setHero(mapLayer, this->section);
	moveCount = fairyHeroManager->getHeroArr()->count();
	resultCount = fairyHeroManager->getEnemyArr()->count();
	titlePanel->setTitleName(this->section);
	titlePanel->setMoveCount(moveCount);
	titlePanel->setResultCount(resultCount);
	AnimatePacker::getInstance()->loadAnimations("skill/effect10011.xml"); 
}

void FairyMain::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void FairyMain::ccTouchesMoved(CCSet *pTouch, CCEvent *pEvent){
	if(currentStatic == HEROMOVE){
		CCPoint point = ((CCTouch *)pTouch->anyObject())->getLocation();
		if(selectHero != NULL){
			selectHero->setPosition(point);
		}
	}
}

void FairyMain::ccTouchesEnded(CCSet *pTouch, CCEvent *pEvent){
	if(currentStatic == HEROMOVE){
		CCPoint point = ((CCTouch *)pTouch->anyObject())->getLocation();
		if(selectHero != NULL){
			selectHero->setSeleted(true);
			if(areaLayer->contains(point, fairyHeroManager)){
				vector<int> newPoint = FairyMap::gameLocationToMap(FairyMap::getLocationInMap(point), mapLayer->getMapY());
				if(mapLayer->mapObject[newPoint.at(0)][newPoint.at(1)] == -1){
					vector<int> oldPoint = FairyMap::gameLocationToMap(selectPoint, mapLayer->getMapY());
					mapLayer->mapObject[oldPoint.at(0)][oldPoint.at(1)] = -1;
					selectHero->setPosition(FairyMap::getLocationInMap(point));
					selectPoint = selectHero->getPosition();
					mapLayer->mapObject[newPoint.at(0)][newPoint.at(1)]=  selectHero->heroId;
					moveCount--;
					titlePanel->setMoveCount(moveCount);
					if(moveCount == 0){
						FairyControlPanel* controlLayer = (FairyControlPanel*)this->getChildByTag(0);
						controlLayer->setInvisiable();
						currentStatic = HEROATTACT;
						moveCount = fairyHeroManager->getHeroArr()->count();
					}
				}else{
					selectHero->setPosition(selectPoint);
				}
			}else{
				selectHero->setPosition(selectPoint);
			}
			areaLayer->cleanMoveArea();
			areaLayer->showAttactArea(selectHero);
		}
		FairyControlPanel* controlLayer = (FairyControlPanel*)this->getChildByTag(0);
		controlLayer->changeControlPanel(selectHero);
	}
}

void FairyMain::ccTouchesBegan(CCSet *pTouch, CCEvent *pEvent){
	if(currentStatic == HEROMOVE){
		CCPoint point = ((CCTouch *)pTouch->anyObject())->getLocation();
		if(selectHero != NULL){
			selectHero->setSeleted(false);
			areaLayer->cleanMoveArea();
			selectHero = NULL;
		}
		selectHero = fairyHeroManager->containsHero(point);
		if(selectHero != NULL){
			selectPoint = selectHero->getPosition();
			areaLayer->showMoveArea(selectHero);
		}
	}
}

void FairyMain::doAttactEnemy(){
	if(FairySkill::isShowEffect || FairyControlPanel::isDoClick){
		return;
	}
	CCArray* heroArr = fairyHeroManager->getHeroArr();
	if(heroIndex < (int)heroArr->count()){
		FairyHero* fairyHero = dynamic_cast<FairyHero* >(heroArr->objectAtIndex(heroIndex));
		if(!fairyHero->isDead){
			FairySkill* fairySkill = FairySkill::create(fairyHero->currentSkillId);
			fairySkill->showEffect(fairyHero, mapLayer);
		}
		heroIndex++;
	}else{
		heroIndex = 0;
		currentStatic = ENEMYMOVE;
	}
}

void FairyMain::doAttactHero(){
	if(FairySkill::isShowEffect || FairyControlPanel::isDoClick){
		return;
	}
	CCArray* enemyArr = fairyHeroManager->getEnemyArr();
	if(enemyIndex < (int)enemyArr->count()){
		FairyEnemy* fairyEnemy = dynamic_cast<FairyEnemy* >(enemyArr->objectAtIndex(enemyIndex));
		if(!fairyEnemy->isDead){
			FairySkill* fairySkill = FairySkill::create(fairyEnemy->currentSkillId);
			fairySkill->showEffect(fairyEnemy, mapLayer);
		}
		enemyIndex++;
	}else{
		enemyIndex = 0;
		FairyControlPanel* controlLayer = (FairyControlPanel*)this->getChildByTag(0);
		controlLayer->setVisiable();
		currentStatic = HEROMOVE;
	}
}
