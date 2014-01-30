#ifndef ClickButton_H
#define ClickButton_H

class ClickButton
{
public:
	ClickButton();
	~ClickButton();

	void updateFrame(float buttonValue);
	int getNumClicks()const { return numClicks;}
	void clearNumClicks();
	
private:
	void setNumClicks(int _numClicks);
public:
	static const int NO_CLICKS;
private:
	int buttonOnCounter;
	int buttonOffCounter;
	int lastButtonOnCounter;
	int lastButtonOffCounter;
	//Stores a running count of how many clicks we think we may have gotten:
	int buttonClicks;

	mutable int numClicks; //Stores the finalized 'public' number of clicks

	//These assume were running 50 fps
	static const int SINGLE_CLICK_ACTIVE_MIN;
	static const int SINGLE_CLICK_ACTIVE_MAX;
	static const int SINGLE_CLICK_INACTIVE_MIN;
	static const int SINGLE_CLICK_INACTIVE_MAX;
	static const float PUSHED;
	bool registeredClickThisTime;
	bool resetClicks;

};

#endif