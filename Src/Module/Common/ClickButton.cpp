#include "ClickButton.h"
#include <iostream>

const int ClickButton::SINGLE_CLICK_ACTIVE_MIN = 1;
const int ClickButton::SINGLE_CLICK_ACTIVE_MAX = 10;
const int ClickButton::SINGLE_CLICK_INACTIVE_MIN = 1;
const int ClickButton::SINGLE_CLICK_INACTIVE_MAX = 5;
const float ClickButton::PUSHED = 1.0f;

const int ClickButton::NO_CLICKS = -1;


ClickButton::ClickButton():
buttonOnCounter(0), buttonOffCounter(0),
lastButtonOnCounter(0),lastButtonOffCounter(0),
buttonClicks(0), numClicks(NO_CLICKS),
registeredClickThisTime(false),
resetClicks(false)
{

}

ClickButton::~ClickButton()
{

}

void ClickButton::clearNumClicks()
{
	numClicks = NO_CLICKS;
}

void ClickButton::setNumClicks(int _numClicks)
{
	numClicks = _numClicks;
}

void ClickButton::updateFrame(float buttonValue)
{


	if(buttonValue == PUSHED)
	{

		buttonOnCounter += 1;
		if(buttonOffCounter > 0)
		{
			lastButtonOffCounter = buttonOffCounter;
			buttonOffCounter  = 0;
		}
		registeredClickThisTime = false;
	}
	else
	{
		if(buttonOnCounter >0)
		{
			lastButtonOnCounter = buttonOnCounter;
			buttonOnCounter = 0;
		}
		buttonOffCounter +=1;
	}
	//std::cout<<"===========buttonOnCounter "<<buttonOnCounter<<std::endl;
	//std::cout<<"==========lastButtonOnCounter "<<lastButtonOnCounter<<std::endl;
	//std::cout<<"===========buttonOffCounter "<<buttonOffCounter<<std::endl;
	//std::cout<<"===========lastButtonOffCounter "<<lastButtonOffCounter<<std::endl;
	//detect if a click occured sometime in the past
	if( lastButtonOnCounter >= SINGLE_CLICK_ACTIVE_MIN &&
		lastButtonOnCounter <= SINGLE_CLICK_ACTIVE_MAX &&
		buttonOffCounter >= SINGLE_CLICK_INACTIVE_MIN &&
		buttonOffCounter <= SINGLE_CLICK_INACTIVE_MAX &&
		!registeredClickThisTime)
	{
			buttonClicks += 1;
			registeredClickThisTime = true;
	}
	//std::cout<<"===========buttonClicks "<<buttonClicks<<std::endl;
	if(resetClicks)
	{
		setNumClicks(0);
		resetClicks = false;
	}
	if( buttonOffCounter > SINGLE_CLICK_INACTIVE_MAX &&
		buttonClicks > 0)
	{
			setNumClicks(buttonClicks);
			buttonClicks = 0;
			resetClicks = true;
	}

}
