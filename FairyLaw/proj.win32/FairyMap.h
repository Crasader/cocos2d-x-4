#ifndef _FAIRYMAP_H__
#define _FAIRYMAP_H__

#include "cocos2d.h"
#include"FairyRes.h"
#include "SimpleAudioEngine.h"
#include"FairyHero.h"
#include<vector>

class FairyMap : public cocos2d::CCLayer
{
public:

	virtual bool init();  

	void initMapObject(cocos2d::CCTMXTiledMap* map, cocos2d::CCTMXLayer* treeLayer, 
		cocos2d::CCTMXLayer* treeLayer2);  

	 static cocos2d::CCPoint mapLocationToGame(cocos2d::CCPoint point);

	 static std::vector<int> gameLocationToMap(cocos2d::CCPoint point, float mapY);

	 static cocos2d::CCPoint getLocationInMap(cocos2d::CCPoint point);

	 bool doScollMap();

	 float getMapY();

	 void moveHero(int direction, cocos2d::CCSprite* hero, int heroId);

	 CREATE_FUNC(FairyMap);

	 static float tileHeight;

	 static float tileWidth;

	 static float mapHeight;

	 static float mapWidth;

	 int mapObject[6][20];

};

#endif