#ifndef __SKILL_SELECT_LAYER_H__
#define __SKILL_SELECT_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameManager.h"

class MskillSelectLayer : public cocos2d::CCLayer, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
public:
    virtual bool init();  
    void initSkillButton();
	void initSkillTableView();
	void initBack();
	void initMenu();
	void resetButton();
	void exitSkillSelect(CCObject* sender);
	cocos2d::CCArray *buttonArr;
    virtual void onExit();
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	virtual void onEnter();
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	CREATE_FUNC(MskillSelectLayer);
	int iconIndex;
	int selectButtonIndex;
	int skillLevel;
	cocos2d::CCSize winSize;

};

#endif 