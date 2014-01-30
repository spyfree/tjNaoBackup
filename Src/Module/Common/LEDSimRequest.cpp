#include "LEDSimRequest.h"
#include <iostream>

PROCESS_WIDE_STORAGE LEDSimRequest* LEDSimRequest::theled = NULL; //wang

 static const char *GLedNames[] =
 {
 	//group names
	 "AllLeds",
	 "AllLedsBlue",
	 "AllLedsGreen",
	 "AllLedsRed",
	 "ChestLeds",
	 "EarLeds",
	 "FaceLed0",
	 "FaceLed1",
	 "FaceLed2",
	 "FaceLed3",
	 "FaceLed4",
	 "FaceLed5",
	 "FaceLed6",
	 "FaceLed7",
	 "FaceLedLeft0",
	 "FaceLedLeft1",
	 "FaceLedLeft2",
	 "FaceLedLeft3",
	 "FaceLedLeft4",
	 "FaceLedLeft5",
	 "FaceLedLeft6",
	 "FaceLedLeft7",
	 "FaceLedRight0",
	 "FaceLedRight1",
	 "FaceLedRight2",
	 "FaceLedRight3",
	 "FaceLedRight4",
	 "FaceLedRight5",
	 "FaceLedRight6",
	 "FaceLedRight7",
	 "FaceLeds",
	 "FaceLedsBottom",
	 "FaceLedsExternal",
	 "FaceLedsInternal",
	 "FaceLedsLeftBottom",
	 "FaceLedsLeftExternal",
	 "FaceLedsLeftInternal",
	 "FaceLedsLeftTop",
	 "FaceLedsRightBottom",
	 "FaceLedsRightExternal",
	 "FaceLedsRightInternal",
	 "FaceLedsRightTop",
	 "FaceLedsTop",
	 "FeetLeds",
	 "LeftEarLeds",
	 "LeftEarLedsBack",
	 "LeftEarLedsEven",
	 "LeftEarLedsFront",
	 "LeftEarLedsOdd",
	 "LeftFaceLeds",
	 "LeftFaceLedsBlue",
	 "LeftFaceLedsGreen",
	 "LeftFaceLedsRed",
	 "LeftFootLeds",
	 "RightEarLeds",
	 "RightEarLedsBack",
	 "RightEarLedsEven",
	 "RightEarLedsFront",
	 "RightEarLedsOdd",
	 "RightFaceLeds",
	 "RightFaceLedsBlue",
	 "RightFaceLedsGreen",
	 "RightFaceLedsRed",
	 "RightFootLeds"
 };

 static const char *LedNames[] =
 {
	 "ChestBoard/Led/Blue/Actuator/Value",
	 "ChestBoard/Led/Green/Actuator/Value",
	 "ChestBoard/Led/Red/Actuator/Value",
	 "LFoot/Led/Blue/Actuator/Value",
	 "LFoot/Led/Green/Actuator/Value",
	 "LFoot/Led/Red/Actuator/Value",
	 "RFoot/Led/Blue/Actuator/Value",
	 "RFoot/Led/Green/Actuator/Value",
	 "RFoot/Led/Red/Actuator/Value"
 };


LEDSimRequest::LEDSimRequest()
{
	theled=this;
    led_off(AllLeds);
}
LEDSimRequest::~LEDSimRequest()
{
    theled=NULL;
}
LEDSimRequest* LEDSimRequest::getinstance()
{
	if(theled == NULL)
	{

		new LEDSimRequest();
	}

	return theled;
}   //wang

void LEDSimRequest::led_on(groupLedNames led)
{
	
}

void LEDSimRequest::led_off(groupLedNames led)
{
	
}
void LEDSimRequest::led_set(groupLedNames led)
{
	
}

void LEDSimRequest::on(ledNames led)
{
	
}

void LEDSimRequest::off(ledNames led)
{
	
}
