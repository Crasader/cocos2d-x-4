#ifndef _MEFFECT_H_
#define _MEFFECT_H_

#include"cocos2d.h"
#include"MActor.h"
#include "AnimatePacker.h"
#include "GameManager.h"

class MEffect : public cocos2d::CCSprite, public MActor
{
public:
	static MEffect* create(int id, int x, int y, int speedx, int speedy, int distance, int time, int updateTime, int skillType, int attackPoint = 0);
	virtual void update();
	virtual bool initEffect(int id, int x, int y, int speedx, int speedy, int distance, int time, int updateTime, int skillType, int attackPoint);
	void dealManager();
	void move();
	int x;
	int y;
	int speedx;
	int speedy;
	int attackPoint;
	int id;
	int distance;
	int time;
	int updateTime;
	bool isPenetrate;
	bool isDelay;
	bool isHook;
	int skillType;
	cocos2d::CCPoint initPoint;
	long initTime;
	cocos2d::CCSprite* skillEffect;
};

#endif