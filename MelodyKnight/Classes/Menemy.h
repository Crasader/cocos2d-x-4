#ifndef _MENEMY_H_
#define _MENEMY_H_

#include"cocos2d.h"
#include"MActor.h"
#include"Utils.h"
#include "MSkillTarget.h"
#include "AnimatePacker.h"
#include "GameManager.h"


class Menemy : public cocos2d::CCSprite, public MActor,public MSkillTarget
{
public:
	int id;
	static Menemy* create(int id, int x, int y, int speedx, int speedy, std::string itemDescript);
	virtual bool initEnemy(int id, int x, int y, int speedx, int speedy, std::string itemDescript);
	virtual void showTargetEffet(int skillId);
	virtual void dealDamager(char* path);
	virtual void update();
	void moveTo(cocos2d::CCPoint rolePoint);
	void moveXY(cocos2d::CCPoint rolePoint);
	void stand();
	void showAni(int speedx, int speedy);
	void showDamagerNum(int num);
	void dealManager(cocos2d::CCNode* pSender,void* data);
	void enemyAI();
	void ai10001();
	void ai10002();
	void ai10003();
	void fight1001();
	void fight1002();
	void showSkill(int type);
	void showSkill10050();
	void showSkill10051();
	void checkStatus10001();
	void resetNum(CCNode* pSender, void* data);
	int x;
	int y;
	int speedx;
	int speedy;
	int rate;
	int searchDis;
	int hitDistanceX;
	int hitDistanceY;
	bool isRadomAttack;
	int roleAniType;
	long lastAttackTime;
	long lastHitTime;
	cocos2d::CCLabelTTF* damagerNum;
	int aiStatus;
	cocos2d::CCPoint tempDes;
	int attackPoint;
    int defPoint;
	std::string itemDescript;
};

#endif
