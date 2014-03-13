#ifndef  _UTILS_H_
#define _UTILS_H_

#include"cocos2d.h"
#include<vector>
#include <string>
#include "XMLParser.h"
#include"FairyRes.h"

class Utils
{
public:
	static std::string getDataFromFile(const char *path);
	static long millisecondNow();
	static void trim(char *str);
	static std::vector<std::string> split(std::string str, std::string pattern);
	static cocos2d::CCString* getString(const char *str);
	static float getAngle(cocos2d::CCPoint& beginPoint, cocos2d::CCPoint& endPoint);
};

#endif