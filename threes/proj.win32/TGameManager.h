#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "cocos2d.h"
#include<vector>

class TGameManager
{
public:
	TGameManager(void);
	static TGameManager* sharedGameManager();
	static TGameManager* instance;
	static int GameData[4][4];
	std::vector<int> calculatePosition(int Direction);
	void calculateData(int Direction);
	static int getRandomNum();
	int getStartRandomNum();
	void init();
	bool isMove(int i, int j);
	bool isOver();
	bool isAssign;
	static int newCount;
	static int point;
	const static int left = 0;
	const static int right = 1;
	const static int up = 2;
	const static int down = 3;
};

#endif
