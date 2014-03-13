#include "SkillManager.h"

int SkillManager::skillStatu = CONTROL_SKILL_STATU;
long SkillManager::maxTime = 0;
long SkillManager::currentTime = Utils::millisecondNow();
int SkillManager::comboCount = 0;

static SkillManager *s_SharedSkillManager  = NULL;

int SkillManager::getSkillPercentage(int height){
	int percentage;
	percentage = (int)(maxTime - Utils::millisecondNow() + currentTime) * height / maxTime;
	return percentage;
}

MISkillLayer* SkillManager::getBigSkillLayer(){
	/*if(skillLayer){
		return skillLayer;
	}else{*/
		skillbigLayer = MSkill1Layer::create();
		skillbigLayer->retain();
		return skillbigLayer;
	//}
}

MISkillLayer* SkillManager::getSkillControlLayer(){
	/*if(skillLayer){
		return skillLayer;
	}else{*/
		skillcontrolLayer = MSkill2Layer::create();
		skillcontrolLayer->retain();
		return skillcontrolLayer;
	//}
}

SkillManager* SkillManager::sharedSkillManager(){
	 if (!s_SharedSkillManager){
        s_SharedSkillManager = new SkillManager();
    }
	 return s_SharedSkillManager;
}

void SkillManager::showButton(){
	skillbigLayer->showButton();
}

void SkillManager::MTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	if(SkillManager::skillStatu == SkillManager::BIG_SKILL_STATU){
		skillbigLayer->MTouchesMoved(pTouches, pEvent);
	}else{
		skillcontrolLayer->MTouchesMoved(pTouches, pEvent);
	}
}

void SkillManager::MTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	if(SkillManager::skillStatu == SkillManager::BIG_SKILL_STATU){
		skillbigLayer->MTouchesEnded(pTouches, pEvent);
	}else{
		skillcontrolLayer->MTouchesEnded(pTouches, pEvent);
	}
}

void SkillManager::MTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	if(SkillManager::skillStatu == SkillManager::BIG_SKILL_STATU){
		skillbigLayer->MTouchesBegan(pTouches, pEvent);
	}else{
		skillcontrolLayer->MTouchesBegan(pTouches, pEvent);
	}
}