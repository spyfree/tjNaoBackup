#ifndef GetSetting_H
#define GetSetting_H
class GetSetting
{
public:
	GetSetting();
	static const char* GetTJDir();
	static const char *GetConfigDir();
	static const char *CovertRoot();
};
#endif
