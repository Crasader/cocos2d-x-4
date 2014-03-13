#ifndef  _UTILS_H_
#define _UTILS_H_

#include"cocos2d.h"
#include "SimpleAudioEngine.h"
#include "XMLParser.h"
#include<vector>
#include<map>
#include <string>
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)   
#include "platform/android/jni/JniHelper.h"
#endif   

class Utils
{
public:
	static std::string getDataFromFile(const char *path);
	static long millisecondNow();
	static void trim(char *str);
	static std::vector<std::string> split(std::string str, std::string pattern);
	static float getAngle(cocos2d::CCPoint beginPoint, cocos2d::CCPoint endPoint);
	static float getSpeedX(float speedOrigin,float moveAngle);
	static float getSpeedY(float speedOrigin,float moveAngle);
	static float getSinValue(float angle);
	static float getCosValue(float angle);
	static void showVibrator(int vibratorTime);
	static void playMusic(char* path, bool isLoop);
	static void playEffect(char* path, bool isLoop);
	static bool isPlaying();
	static void stopMusic();
	static const char* parseXML(std::string path, std::string key);
	static const char* getString(std::string key);
	static std::map<std::string, std::string> stringMap; 
};

#endif