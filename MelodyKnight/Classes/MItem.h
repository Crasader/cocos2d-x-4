#ifndef _MITEM_H_
#define _MITEM_H_

#include"cocos2d.h"
#include"Utils.h"
#include"MActor.h"
#include"GameManager.h"

class MItem : public cocos2d::CCSprite,public MActor
{
public:
	std::string id;
	static MItem* create(std::string id, int x, int y);
	virtual bool initItem(std::string id, int x, int y);
	void moveTo(cocos2d::CCPoint point);
	void itemAI();
	void aiSkill();
	void dealManager();
	virtual void update();
	int x;
	int y;
	float speedx;
	float speedy;
	long initTime;
	cocos2d::CCPoint tempDes;
	std::string type;
	int propertis;
};

#endif