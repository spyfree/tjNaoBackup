#include "PyLED.h"
#include <iostream>
static LEDRequest* ledRequest_reference=0;
LEDModule::LEDModule()
{
	ledRequest = ledRequest_reference;
}
LEDModule::~LEDModule()
{


}

void LEDModule::onFindOppGoal()
{
	ledRequest->led_on(LEDRequest::RightFaceLedsGreen);
	ledRequest->led_on(LEDRequest::RightFaceLedsRed);
}

void LEDModule::onFindOwnGoal()
{
	ledRequest->led_on(LEDRequest::RightFaceLedsBlue);
}

void LEDModule::offFindOppGoal()
{
	ledRequest->led_off(LEDRequest::RightFaceLedsGreen);
	ledRequest->led_off(LEDRequest::RightFaceLedsRed);
}

void LEDModule::offFindOwnGoal()
{
	ledRequest->led_off(LEDRequest::RightFaceLedsBlue);
}

void set_led_reference(LEDRequest* _ledRequest)
{
	ledRequest_reference = _ledRequest;
}


BOOST_PYTHON_MODULE(PyLED)
{

	class_<LEDModule>("led")
		.def("onOppGoal",&LEDModule::onFindOppGoal)
		.def("onOwnGoal",&LEDModule::onFindOwnGoal)
		.def("offOppGoal",&LEDModule::offFindOppGoal)
		.def("offOwnGoal",&LEDModule::offFindOwnGoal)
		;
}
void c_init_led()
{
	if (!Py_IsInitialized())
	{
		Py_Initialize();
	}
	try {
		initPyLED();
	} catch (error_already_set) {
		PyErr_Print();
	}
}



















