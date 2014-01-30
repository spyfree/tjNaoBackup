#ifndef KeyRequest_H
#define KeyRequest_H
#include "Platform/SystemCall.h"
#include "Module/Common/KeyStates.h"

class KeyRequest
{
public:
	KeyRequest();
	~KeyRequest();
	void PressOnce(KeyStates::Key key);
	void PressTwice(KeyStates::Key key);
private:
	unsigned int currentTime;
	unsigned int lastTime;
};

#endif