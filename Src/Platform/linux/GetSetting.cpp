#include "GetSetting.h"
#include<iostream>
GetSetting::GetSetting()
{
}
const char *GetSetting::GetTJDir()
{
	static char dir[FILENAME_MAX] = {0};
	    int len;
	if(!dir[0])
	{

			len = int(getcwd( dir,sizeof(dir) ));
	}
	  return dir;
}
const char *GetSetting::GetConfigDir()
{
	static char cfgDir[FILENAME_MAX] = "/home/nao/naoqi/Config/";
	return cfgDir;
}
