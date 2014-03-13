#include "Utils.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

#define M_PI_VALUE 3.14159f
map<string, string> Utils::stringMap = map<string, string>();

vector<string> Utils::split(string str,string pattern) 
{ 
	long pos; 
	vector<string> result; 
	str+=pattern; 
	long size=str.size(); 
	for( int i= 0; i<size; i++) { 
		pos=str.find(pattern,i); 
		if(pos<size) { 
			string s=str.substr(i,pos-i); 
			char* c = const_cast<char* >(s.c_str());
			trim(c);
			string a(c);
			result.push_back(a); 
			i=pos+pattern.size()- 1; 
		} 
	} 
	return result; 
} 

const char* Utils::parseXML(string path, string key)
{
	if(stringMap.find(path+":"+key) == stringMap.end()){
		XMLParser* data = XMLParser::parseWithFile(path.c_str());
		Utils::stringMap.insert(pair<string, string>(path+":"+key,  data->getString(key.c_str())->getCString())); 
	}
	return Utils::stringMap.find(path+":"+key)->second.c_str();
}

const char* Utils::getString(std::string key)
{
	return parseXML("string.xml", key);
}

string Utils::getDataFromFile(const char *path)
{
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(path);
    char* pBuffer = NULL;
    unsigned long bufferSize = 0;
	pBuffer = (char *)CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "r", &bufferSize);
	string d;
	d.assign(pBuffer,bufferSize);
	delete pBuffer;
	return d;
}

long Utils::millisecondNow()  
{ 
	struct cc_timeval now; 
	CCTime::gettimeofdayCocos2d(&now, NULL); 
	return (now.tv_sec * 1000 + now.tv_usec / 1000); 
} 

void Utils::trim(char *str)
{
    char *start = str - 1;
    char *end = str;
    char *p = str;
    while(*p)
    {
        switch(*p)
        {
        case ' ':
        case '\r':
        case '\n':
            {
                if(start + 1==p)
                    start = p;
            }
            break;
        default:
            break;
        }
        ++p;
    }
    //现在来到了字符串的尾部 反向向前
    --p;
    ++start;
    if(*start == 0)
    {
        //已经到字符串的末尾了
        *str = 0 ;
        return;
    }
    end = p + 1;
    while(p > start)
    {
        switch(*p)
        {
        case ' ':
        case '\r':
        case '\n':
            {
                if(end - 1 == p)
                    end = p;
            }
            break;
        default:
            break;
        }
        --p;
    }
    memmove(str,start,end-start);
    *(str + (int)end - (int)start) = 0;
}

float Utils:: getAngle(CCPoint beginPoint, CCPoint endPoint) { 
	double len_y = endPoint.y - beginPoint.y; 
    double len_x = endPoint.x - beginPoint.x; 
	if(len_y == 0) { 
		if(len_x < 0) { 
			return 270; 
		}else if(len_x > 0) { 
			return 90; 
		} 
		return 0; 
	} 
	if(len_x == 0) { 
		if(len_y >= 0) { 
			return 0; 
		}else if(len_y < 0){ 
			return 180; 
		} 
	} 
	return atan2f(len_x,len_y)*180/M_PI_VALUE;
} 

float Utils::getSpeedY(float speedOrigin, float moveAngle)
{
 speedOrigin = abs(speedOrigin);
 if(moveAngle == 0)
 {
  return speedOrigin;
 }
 if(moveAngle == 90)
 {
  return 0;
 }
 if(moveAngle == 180)
 {
  return -speedOrigin;
 }
 if(moveAngle == 270)
 {
  return 0;
 }
 return getCosValue(moveAngle)*speedOrigin;
}

float Utils::getSpeedX(float speedOrigin,float moveAngle)
{
 speedOrigin = abs(speedOrigin);
 if(moveAngle == 0)
 {
  return  0;
 }
 if(moveAngle == 90)
 {
  return speedOrigin;
 }
 if(moveAngle == 180)
 {
  return 0;
 }
 if(moveAngle == 270)
 {
  return -speedOrigin;
 }
 return getSinValue(moveAngle)*speedOrigin;
}

float Utils::getSinValue(float angle)
{
 return sin(CC_DEGREES_TO_RADIANS(angle));
}

float Utils::getCosValue(float angle)
{
 return cos(CC_DEGREES_TO_RADIANS(angle));
}

void Utils::showVibrator(int vibratorTime){
	/*JniMethodInfo minfo;  
    if (JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "Vibrator", "(I)V"))  
    {  
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, vibratorTime);  
        minfo.env->DeleteLocalRef(minfo.classID);  
    } */
}

void Utils::playMusic(char* path, bool isLoop){
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(path,isLoop);
}

void Utils::playEffect(char* path, bool isLoop){
	SimpleAudioEngine::sharedEngine()->playEffect(path, isLoop);
}

bool Utils::isPlaying(){
	return SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
}

void Utils::stopMusic(){
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}
