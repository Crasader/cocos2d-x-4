#include "FairyMap.h"

USING_NS_CC;
using namespace std;

float FairyMap::tileHeight = 0;

float FairyMap::tileWidth = 0;

float FairyMap::mapHeight = 0;

float FairyMap::mapWidth = 0;

bool FairyMap::init(){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCTMXTiledMap* map = CCTMXTiledMap::create(map0);
	CCTMXLayer* treeLayer = map->layerNamed("layer2");
	CCTMXLayer* treeLayer2 = map->layerNamed("layer3");
	tileHeight = map->getTileSize().height;
	tileWidth = map->getTileSize().width;
	mapHeight = map->getMapSize().height;
	mapWidth = map->getMapSize().width;
	initMapObject(map, treeLayer, treeLayer2);
	addChild(map, 0, 0);
	return true;
}

void FairyMap::initMapObject(CCTMXTiledMap* map, CCTMXLayer* treeLayer,  CCTMXLayer* treeLayer2){
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 20; j++){
			mapObject[i][j] = -1;
			int tileGid  = (int)treeLayer->tileGIDAt(ccp(i,j));
			if(tileGid != 0){
				CCDictionary* properties  = map->propertiesForGID(tileGid);
				if(properties){
					const CCString* type = properties->valueForKey("type");
					if(type->compare("tree") == 0){
						mapObject[i][(int)mapHeight - 1 - j] = -2;
					}
				}
			}
			int tileGid2  = (int)treeLayer2->tileGIDAt(ccp(i,j));
			if(tileGid2 != 0){
				CCDictionary* properties2  = map->propertiesForGID(tileGid2);
				if(properties2){
					const CCString* type2 = properties2->valueForKey("type");
					if(type2->compare("tree") == 0){
						mapObject[i][(int)mapHeight - 1 -j] = -2;
					}
				}
			}
		}
	}
}

float FairyMap::getMapY(){
	CCTMXTiledMap* map = (CCTMXTiledMap*)this->getChildByTag(0);
	return map->getPositionY();
}

vector<int> FairyMap::gameLocationToMap(CCPoint point, float mapY){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	vector<int> result; 
	result.push_back((int)((point.x - FairyHero::heroWidth / 2) / tileWidth));
	result.push_back((int)((point.y - FairyHero::heroHeight /  2 - mapY) / tileHeight));
	return result;
}

CCPoint FairyMap::mapLocationToGame(CCPoint point){
	CCPoint result = ccp(point.x * tileWidth, point.y * tileHeight);
	return result;
}

bool FairyMap::doScollMap(){
	CCTMXTiledMap* map = (CCTMXTiledMap*)this->getChildByTag(0);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	float mapX = map->getPositionX();
	float mapY = map->getPositionY() - tileHeight;
	if(mapHeight * tileHeight + mapY  >= size.height){
		map->setPosition(ccp(mapX, mapY));
		return true;
	}else{
		return false;
	}
}

CCPoint FairyMap::getLocationInMap(CCPoint point){
	CCPoint result = ccp((int)(point.x / tileWidth) , (int)(point.y / tileHeight));
	return ccp(result.x * tileWidth + FairyHero::heroWidth / 2, 
		result.y *tileHeight + FairyHero::heroHeight / 2);
}

void FairyMap::moveHero(int direction, CCSprite* hero, int heroId){
	CCTMXTiledMap* map = (CCTMXTiledMap*)this->getChildByTag(0);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	float mapY = map->getPositionY() - tileHeight;
	switch(direction){
		//hero向左走
		case 0:
			if(hero->getPosition().x > FairyHero::heroWidth){
				vector<int> oldPoint = gameLocationToMap(hero->getPosition(), this->getMapY());
				if(mapObject[oldPoint.at(0) - 1][oldPoint.at(1)] == -1){
					mapObject[oldPoint.at(0)][oldPoint.at(1)] = -1;
					hero->setPosition(ccp(hero->getPositionX() - FairyMap::tileWidth
						,hero->getPositionY()));
					mapObject[oldPoint.at(0) - 1][oldPoint.at(1)] = heroId;
				}
			}
			break;
		//hero向上走
		case 1:
			if(hero->getPosition().y <  CCDirector::sharedDirector()->getWinSize().height - FairyHero::heroHeight){
				vector<int> oldPoint = gameLocationToMap(hero->getPosition(), this->getMapY());
					if(mapObject[oldPoint.at(0)][oldPoint.at(1) + 1]  == -1){
						mapObject[oldPoint.at(0)][oldPoint.at(1)] = -1;
						if(! (mapHeight * tileHeight + mapY  >= size.height)){
							hero->setPosition(ccp(hero->getPositionX(), hero->getPositionY() + FairyMap::tileHeight));
						}
						mapObject[oldPoint.at(0)][oldPoint.at(1) + 1]=  heroId;
					}
			}
			break;
		//hero向右走
		case 3:
			if(hero->getPosition().x < FairyMap::tileWidth * FairyMap::mapWidth - FairyHero::heroWidth){
				vector<int> oldPoint = gameLocationToMap(hero->getPosition(), this->getMapY());
				if(mapObject[oldPoint.at(0) + 1][oldPoint.at(1)] == -1){
					mapObject[oldPoint.at(0)][oldPoint.at(1)] = -1;
					hero->setPosition(ccp(hero->getPositionX() + FairyMap::tileWidth, hero->getPositionY()));
					mapObject[oldPoint.at(0) + 1][oldPoint.at(1)] = heroId;
				}
			}
			break;
		//enemy向下走
		case 4:
			if(hero->getPosition().y <  CCDirector::sharedDirector()->getWinSize().height -
				FairyHero::heroHeight){
					vector<int> oldPoint = gameLocationToMap(hero->getPosition(), this->getMapY());
					if(mapObject[oldPoint.at(0)][oldPoint.at(1) - 1] == -1){
						mapObject[oldPoint.at(0)][oldPoint.at(1)] = -1;
						hero->setPosition(ccp(hero->getPositionX(), hero->getPositionY() - FairyMap::tileHeight));
						mapObject[oldPoint.at(0)][oldPoint.at(1) - 1] =  heroId;
					}
			}
			break;
		//enmey静止
		case 5:
			if(mapHeight * tileHeight + mapY  >= CCDirector::sharedDirector()->getWinSize().height){
					vector<int> oldPoint = gameLocationToMap(hero->getPosition(), this->getMapY());
					hero->setPosition(ccp(hero->getPositionX(), hero->getPositionY() - FairyMap::tileHeight));
			}
			break;
		//enmey向上走
		case 6:
			if(hero->getPosition().y <  CCDirector::sharedDirector()->getWinSize().height - FairyHero::heroHeight){
				vector<int> oldPoint = gameLocationToMap(hero->getPosition(), this->getMapY());
					if(mapObject[oldPoint.at(0)][oldPoint.at(1) + 1] == -1){
						mapObject[oldPoint.at(0)][oldPoint.at(1)] = -1;
						hero->setPosition(ccp(hero->getPositionX(), hero->getPositionY() + FairyMap::tileHeight));
						mapObject[oldPoint.at(0)][oldPoint.at(1) + 1] = heroId;
					}
			}
			break;
	}
}
