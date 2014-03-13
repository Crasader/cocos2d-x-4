#ifndef _FAIRY_TITLE_PANEL_H_
#define _FAIRY_TITLE_PANEL_H_

#include "cocos2d.h"
#include"FairyRes.h"
#include "XMLParser.h"

class FairyTitlePanel : public cocos2d::CCLayer
{
public:

	virtual bool init();  

	void setTitleName(int section);

	void setMoveCount(int moveCount);

	void setResultCount(int resultCount);

	 CREATE_FUNC(FairyTitlePanel);

	 cocos2d::CCLabelTTF* titleNameText;

	 cocos2d::CCLabelTTF* moveCountText;

	 cocos2d::CCLabelTTF* resultCountText;

};

#endif
