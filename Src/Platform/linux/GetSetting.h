#ifndef GetSetting_H
#define GetSetting_H
#include<stdio.h>
#include <unistd.h>
class GetSetting
{
public:
	GetSetting();
	static const char* GetTJDir();
	static const char *GetConfigDir();
};
#endif
