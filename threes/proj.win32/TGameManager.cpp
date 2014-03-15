#include "TGameManager.h"
USING_NS_CC;
using namespace std;

TGameManager* TGameManager::instance;
int TGameManager::point = 0;
int TGameManager::newCount = 0;
int TGameManager::GameData[4][4] = {{0,1,1,2},
                                                                 {0,3,3,0},
                                                                 {2,0,2,0},
                                                                 {0,0,2,1}}; 

TGameManager::TGameManager(void)
{
	isAssign = false;
}

void TGameManager::init(){
	point = 0;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			GameData[i][j] = getStartRandomNum();
		}
	}
	newCount = getRandomNum();
}


TGameManager* TGameManager::sharedGameManager(){
	if(!instance){
		instance = new TGameManager();
	}
	return instance;
}

bool TGameManager::isOver(){
	for(int j = 0; j < 4; j++){
		for(int i = 0; i < 3; i++){
			if(isMove(GameData[i][j], GameData[i + 1][j])){
				return false;
			}
		}
	}
	for(int j = 0; j < 4; j++){
		for(int i = 3; i > 0; i--){
			if(isMove(GameData[i][j], GameData[i - 1][j])){
				return false;
			}
		}
	}
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(isMove(GameData[i][j],GameData[i][j + 1])){
				return false;
			}
		}
	}
	for(int i = 0; i < 4; i++){
		for(int j = 3; j > 0; j--){
			if(isMove(GameData[i][j], GameData[i][j - 1])){
				return false;
			}
		}
	}
	return true;
}

std::vector<int> TGameManager::calculatePosition(int Direction){
	vector<int> result;
	int tags[4] = {-1,-1,-1,-1};
	switch(Direction){
	case left:
		for(int j = 0; j < 4; j++){
			for(int i = 0; i < 3; i++){
				if(isMove(GameData[i][j], GameData[i + 1][j])){
					tags[j] = i * 10 +j;
					break;
				}
			}
		}
		break;
	case right:
		for(int j = 0; j < 4; j++){
			for(int i = 3; i > 0; i--){
				if(isMove(GameData[i][j], GameData[i - 1][j])){
					tags[j] = i * 10 +j;
					break;
				}
			}
		}
		break;
	case up:
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				if(isMove(GameData[i][j],GameData[i][j + 1])){
					tags[i] = i * 10 +j;
					break;
				}
			}
		}
		break;
	case down:
		for(int i = 0; i < 4; i++){
			for(int j = 3; j > 0; j--){
				if(isMove(GameData[i][j], GameData[i][j - 1])){
					tags[i] = i * 10 +j;
					break;
				}
			}
		}
		break;
	}
	for(int i = 0 ; i < 4; i++){
		result.push_back(tags[i]);
	}
	return result;
}

void  TGameManager::calculateData(int Direction){
	bool isChange = false;
	switch(Direction){
	case left:
		for(int j = 0; j < 4; j++){
			for(int i = 0; i < 3; i++){
				if(isMove(GameData[i][j],GameData[i + 1][j])){
					isChange = true;
					GameData[i][j] = GameData[i][j] +  GameData[i + 1][j];
					for(int k = i + 1; k < 3; k++){
						GameData[k][j] = GameData[k+1][j];
					}
					if(!isAssign){
						GameData[3][j] = newCount;
						isAssign = true;
					}else{
						GameData[3][j] = 0;
					}
					break;
				}
			}
		}
		break;
	case right:
		for(int j = 0; j < 4; j++){
			for(int i = 3; i > 0; i--){
				if(isMove(GameData[i][j],GameData[i - 1][j])){
					isChange = true;
					GameData[i][j] = GameData[i][j] +  GameData[i - 1][j];
					for(int k = i - 1; k > 0; k--){
						GameData[k][j] = GameData[k - 1][j];
					}
					if(!isAssign){
						GameData[0][j] = newCount;
						isAssign = true;
					}else{
						GameData[0][j] = 0;
					}
					break;
				}
			}
		}
		break;
	case up:
		for(int i = 0; i < 4; i++){
			for(int j = 0 ; j < 3; j++){
				if(isMove(GameData[i][j],GameData[i][j + 1])){
					isChange = true;
					GameData[i][j] = GameData[i][j] +  GameData[i][j + 1];
					for(int k = j + 1; k < 4 ; k++){
						GameData[i][k] = GameData[i][k + 1];
					}
					if(!isAssign){
						GameData[i][3] = newCount;
						isAssign = true;
					}else{
						GameData[i][3] = 0;
					}
					break;
				}
			}
		}
		break;
	case down:
		for(int i = 0; i < 4; i++){
			for(int j = 3; j > 0; j--){
				if(isMove(GameData[i][j], GameData[i][j - 1])){
					isChange = true;
					GameData[i][j] = GameData[i][j] +  GameData[i][j - 1];
					for(int k = j - 1; k > 0; k--){
						GameData[i][k] = GameData[i][k - 1];
					}
					if(!isAssign){
						GameData[i][0] = newCount;
						isAssign = true;
					}else{
						GameData[i][0] = 0;
					}
					break;
				}
			}
		}
		break;
	}
	isAssign = false;
	if(isChange){
		newCount = getRandomNum();
	}
	point = 0;
	for(int j = 0; j < 4; j++){
		for(int i = 0; i < 4; i++){
			point += GameData[i][j];
		}
	}
}

int TGameManager::getRandomNum(){
	float type =rand()/(RAND_MAX+1.0);
	if(type <= 0.4){
		return 1;
	}else if(type <= 0.8){
		return 2;
	}else if(type <= 0.9){
		return 3;
	}else if(type <= 0.96){
		return 6;
	}else if(type <= 0.98){
		return 12;
	}else if(type <= 0.99){
		return 24;
	}else{
		return 48;
	}
}

int TGameManager::getStartRandomNum(){
	float type =rand()/(RAND_MAX+1.0);
	if(type <= 0.5){
		return 0;
	}else if(type <= 0.7){
		return 1;
	}else if(type <= 0.9){
		return 2;
	}else{
		return 3;
	}
}

bool TGameManager::isMove(int i, int j){
	bool canMove = false;
	if(i == 0 && j != 0){
		canMove = true;
	}else if(i * j == 2){
		canMove = true;
	}else if(i > 2 && j >2 && i==j){
		canMove = true;
	}
	return canMove;
}
