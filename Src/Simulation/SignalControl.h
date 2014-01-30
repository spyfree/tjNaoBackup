#ifndef SignalControl_H
#define SignalControl_H
#include "Platform/SystemCall.h"

class SignalControl
{
public:
	SignalControl();
	~SignalControl();
	static SignalControl* getInstance();
	enum Vision_Choose
	{
		VODE = 0,
		VOFFLINE = 1,
		NumberVisionNames
	};
	enum Locator_Choose
	{
		LODE = 0,
		LOFFLINE = 1,
		NumberLocatorNames
	};
	enum Actuator_Choose
	{
		AODE = 0,
		AONLINE = 1,
		NumberActuatorNames
	};
	static char* getVisionName(int visionName)
	{
		switch(visionName)
		{
		case VODE: return "ODE"; break;
		case VOFFLINE: return "OFFLINE"; break;
		default: return "Unknown VisionName";
		}
	}
	static char* getLocatorName(int locatorName)
	{
		switch(locatorName)
		{
		case LODE: return "ODE";break;
		case LOFFLINE: return "OFFLINE";break;
		default: return "Unknown LocatorName";
		}
	}
	static char* getActuatorName(int actuatorName)
	{
		switch(actuatorName)
		{
		case AODE: return "ODE";break;
		case AONLINE: return "ONLINE";break;
		default: return "Unknown ActuatorName";
		}
	}
	static int visionSignal;
	static int locatorSignal;
	static int actuatorSignal;
private:
	PROCESS_WIDE_STORAGE_STATIC SignalControl* theSignalControl;
};

#endif