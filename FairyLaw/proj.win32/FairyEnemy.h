#ifndef _FAIRY_ENEMY_H_
#define _FAIRY_ENEMY_H_

#include"FairyHero.h"
#include"FairyMap.h"
#include"FairyHeroManager.h"

class FairyEnemy : public FairyHero
{
public:
	static FairyEnemy* create(int id);
	virtual bool initHero(int id);
	void doAI();
	void selectSkill();
	void doAI0();
	void doAI1();
	void doAI2();
	int enemyType; 
	int searchId;
	int searchArea;

};

#endif
