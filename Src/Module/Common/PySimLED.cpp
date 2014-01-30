#include "PySimLED.h"
#include <iostream>
static LEDSimRequest* ledRequest_reference=0;
LEDSimModule::LEDSimModule()
{
	ledRequest = ledRequest_reference;
}
LEDSimModule::~LEDSimModule()
{


}

void LEDSimModule::onFindOppGoal()
{
	ledRequest->led_on(LEDSimRequest::RightFaceLedsGreen);
	ledRequest->led_on(LEDSimRequest::RightFaceLedsRed);
}

void LEDSimModule::onFindOwnGoal()
{
	ledRequest->led_on(LEDSimRequest::RightFaceLedsBlue);
}

void LEDSimModule::offFindOppGoal()
{
	ledRequest->led_off(LEDSimRequest::RightFaceLedsGreen);
	ledRequest->led_off(LEDSimRequest::RightFaceLedsRed);
}

void LEDSimModule::offFindOwnGoal()
{
	ledRequest->led_off(LEDSimRequest::RightFaceLedsBlue);
}

void set_led_reference(LEDSimRequest* _ledRequest)
{
	ledRequest_reference = _ledRequest;
}


BOOST_PYTHON_MODULE(PyLED)
{

	class_<LEDSimModule>("led")
		.def("onOppGoal",&LEDSimModule::onFindOppGoal)
		.def("onOwnGoal",&LEDSimModule::onFindOwnGoal)
		.def("offOppGoal",&LEDSimModule::offFindOppGoal)
		.def("offOwnGoal",&LEDSimModule::offFindOwnGoal)
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



















