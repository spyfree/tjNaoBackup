#ifndef LEDSimRequest_H
#define LEDSimRequest_H
#include "Platform/SystemCall.h"

class LEDSimRequest
{
public:
	static LEDSimRequest *getinstance();
	enum groupLedNames
	{
		AllLeds, 
		AllLedsBlue, 
		AllLedsGreen, 
		AllLedsRed, 
		ChestLeds, 
		EarLeds, 
		FaceLed0, 
		FaceLed1, 
		FaceLed2, 
		FaceLed3, 
		FaceLed4, 
		FaceLed5, 
		FaceLed6, 
		FaceLed7, 
		FaceLedLeft0, 
		FaceLedLeft1, 
		FaceLedLeft2, 
		FaceLedLeft3, 
		FaceLedLeft4, 
		FaceLedLeft5, 
		FaceLedLeft6, 
		FaceLedLeft7, 
		FaceLedRight0, 
		FaceLedRight1, 
		FaceLedRight2, 
		FaceLedRight3, 
		FaceLedRight4, 
		FaceLedRight5, 
		FaceLedRight6, 
		FaceLedRight7, 
		FaceLeds,
		FaceLedsBottom, 
		FaceLedsExternal, 
		FaceLedsInternal, 
		FaceLedsLeftBottom, 
		FaceLedsLeftExternal, 
		FaceLedsLeftInternal, 
		FaceLedsLeftTop, 
		FaceLedsRightBottom, 
		FaceLedsRightExternal, 
		FaceLedsRightInternal, 
		FaceLedsRightTop, 
		FaceLedsTop, 
		FeetLeds, 
		LeftEarLeds, 
		LeftEarLedsBack, 
		LeftEarLedsEven, 
		LeftEarLedsFront, 
		LeftEarLedsOdd, 
		LeftFaceLeds, 
		LeftFaceLedsBlue, 
		LeftFaceLedsGreen, 
		LeftFaceLedsRed, 
		LeftFootLeds, 
		RightEarLeds, 
		RightEarLedsBack, 
		RightEarLedsEven, 
		RightEarLedsFront, 
		RightEarLedsOdd, 
		RightFaceLeds, 
		RightFaceLedsBlue, 
		RightFaceLedsGreen, 
		RightFaceLedsRed, 
		RightFootLeds 
		};
	enum ledNames
	{
		ChestLedsBlue,
		ChestLedsGreen,
		ChestLedsRed,
		LeftFootLedsBlue,
		LeftFootLedsGreen,
		LeftFootLedsRed,
		RightFootLedsBlue,
		RightFootLedsGreen,
		RightFootLedsRed
	};

	LEDSimRequest();
	~LEDSimRequest();
	void led_on(groupLedNames led);
	void led_off(groupLedNames led);
	void led_set(groupLedNames led);
	void on(ledNames led);
	void off(ledNames led);

private:
	PROCESS_WIDE_STORAGE_STATIC LEDSimRequest *theled;


};
#endif//LEDRequest_H
