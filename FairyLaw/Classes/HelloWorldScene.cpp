#include "HelloWorldScene.h"
#include "FairyHero.h"
#include "XMLParser.h"

using namespace cocos2d;

CCScene* HelloWorld::scene(int section)
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());
        CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCTMXTiledMap* map = CCTMXTiledMap::create(map0);
		addChild(map, 0, 0);
		FairyHero *hero = FairyHero::create(0);
		this->addChild(hero);
		CCPoint heroPoint  = ccpAdd(mapLocationToGame(ccp(2,2)), ccp(map->getTileSize().width / 2, 
			map->getTileSize().height / 2));
		hero->setPosition(heroPoint);
		initHero();
		addChild(FairyControlPanel::create());
        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::initHero(){
	XMLParser *pXmlParser = XMLParser::parseWithFile("data/strings.xml");
	CCString *pValue1 = pXmlParser->getString("hello");
	CCString *pValue2 = pXmlParser->getString("name");
	CCString *pValue = CCString::createWithFormat("%s%s%s%s", "XML: ", pValue1->getCString(), ",", pValue2->getCString());
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
	CCTMXTiledMap* map = (CCTMXTiledMap*)this->getChildByTag(0);
}

void HelloWorld::doClickButton(int tag){
	switch(tag){
		case FairyControlPanel::leftDrection:
			break;
		case FairyControlPanel::upDrection:
			doScollMap();
			break;
		case FairyControlPanel::holdDrection:
			break;
		case FairyControlPanel::rightDrection:
			break;
	}
}

void HelloWorld::doScollMap(){
	CCTMXTiledMap* map = (CCTMXTiledMap*)this->getChildByTag(0);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	float mapX = map->getPositionX();
	float mapY = map->getPositionY() - map->getTileSize().height;
	if(map->getMapSize().height * map->getTileSize().height + mapY  >= size.height){
		map->setPosition(ccp(mapX, mapY));
	}
}

CCPoint HelloWorld::mapLocationToGame(CCPoint point){
	CCTMXTiledMap* map = (CCTMXTiledMap*)this->getChildByTag(0);
	CCSize size = map->getTileSize();
	CCPoint result = ccp(point.x * size.width, point.y * size.height);
	return result;
}

CCPoint HelloWorld::gameLocationToMap(CCPoint point){
	CCTMXTiledMap* map = (CCTMXTiledMap*)this->getChildByTag(0);
	CCSize size = map->getTileSize();
	CCPoint result = ccp(point.x / size.width, point.y / size.height);
	return result;
}

