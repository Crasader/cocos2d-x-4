#ifndef FAIRYHERO_H_
#define FAIRYHERO_H_

#include"cocos2d.h"
#include"FairyRes.h"
#include "XMLParser.h"
#include "Utils.h"
#include"FairyMap.h"

class FairyHero : public cocos2d::CCSprite
{
public:
	static FairyHero* create(int id);
	virtual bool initHero(int id);
	void setSeleted(bool isSeleted);
	void updateHp(int attack);
	void move();
	void showHit();
	cocos2d::CCSprite* frame;
	cocos2d::CCProgressTimer* lifeBar;
	cocos2d::CCSize frameSize; 
	cocos2d::CCSize barSize;
	const static int heroWidth = 120;
	const static int heroHeight = 120;
	bool isDead;
	int mapStatic;
	int heroId;
	int varLife;
	const char* name;
	const char* headIcon;
	std::vector<int> skillIds;
	int currentSkillId;
	std::vector<int> buffeIds;
	int level;
	int attick;
	int defense;
	int moveArea;
	int maxLife;

};

#endif
