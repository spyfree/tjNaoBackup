#include "LEDRequest.h"
#include <iostream>

extern ALPtr<ALBroker>test_pBroker;
PROCESS_WIDE_STORAGE LEDRequest* LEDRequest::theled = NULL; //wang

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


LEDRequest::LEDRequest()
{
	theled=this;

	try
	{
		ledproxy=ALPtr<ALLedsProxy>(new ALLedsProxy(test_pBroker));
	}
	catch(AL::ALError &e){
		std::cout<<"Failed to initialize LEDproxy" << std::endl;
	}
    led_off(AllLeds);
}
LEDRequest::~LEDRequest()
{
    theled=NULL;
}
LEDRequest* LEDRequest::getinstance()
{
	if(theled == NULL)
	{

		new LEDRequest();
	}

	return theled;
}   //wang

void LEDRequest::led_on(groupLedNames led)
{
	ledproxy->on(GLedNames[led]);
}

void LEDRequest::led_off(groupLedNames led)
{
	ledproxy->off(GLedNames[led]);
}
void LEDRequest::led_set(groupLedNames led)
{
	/*led_off(1);*/
	ledproxy->setIntensity(GLedNames[led],1.0f);
}

void LEDRequest::on(ledNames led)
{
	ledproxy->on(LedNames[led]);
}

void LEDRequest::off(ledNames led)
{
	ledproxy->off(LedNames[led]);
}
