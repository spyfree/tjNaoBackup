#include "GetSetting.h"
#include <windows.h>
GetSetting::GetSetting()
{
}
const char *GetSetting::GetTJDir()
{
	static char dir[MAX_PATH] = {0};
	    int len;
	if(!dir[0])
	{
		
			len = int(GetCurrentDirectory(sizeof(dir) , dir));
	}
	  return dir;
}
const char *GetSetting::GetConfigDir()
{
	
	static char dir[MAX_PATH] = {0};
	 int len=0;
		if(!dir[0])
		{
			
				len = int(GetCurrentDirectory(sizeof(dir) , dir));
		}
 	char str[16]="../../../Config";
	if(dir[len]!='\\'&&len)
	{
		dir[len] = '\\';
		len++;

		for(int i=0;i<15;i++)
		{
			dir[len++]=str[i];
		}
	//dir[len]='\0';
	}
	return dir;
	

}