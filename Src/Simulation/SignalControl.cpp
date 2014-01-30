#include "SignalControl.h"
#include <iostream>

PROCESS_WIDE_STORAGE SignalControl* SignalControl::theSignalControl = NULL;
int SignalControl::visionSignal = SignalControl::VODE;
int SignalControl::locatorSignal = SignalControl::LODE;
int SignalControl::actuatorSignal = SignalControl::AODE;

SignalControl::SignalControl()
{
	SignalControl::theSignalControl = this;
}
SignalControl::~SignalControl()
{
	theSignalControl = NULL;
}

SignalControl* SignalControl::getInstance()
{
	if(theSignalControl == NULL)
	{
		new SignalControl();
	}
	return theSignalControl;
}