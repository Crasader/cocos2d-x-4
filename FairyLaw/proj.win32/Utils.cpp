#include "Utils.h"

USING_NS_CC;
using namespace std;
#define M_PI_VALUE 3.14159f

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
			result.push_back(s); 
			i=pos+pattern.size()- 1; 
		} 
	} 
	return result; 
} 

string Utils::getDataFromFile(const char *path)
{
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(path);
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

CCString* Utils::getString(const char *str){
	XMLParser *stringData = XMLParser::parseWithFile(stringPath);
	return (stringData->getString(str));
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

float Utils:: getAngle(CCPoint& beginPoint, CCPoint& endPoint) { 
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

