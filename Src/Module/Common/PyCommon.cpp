#include "PyCommon.h"
const static OutBehaviorData* outBehaviorData_reference=0;
CommonModule::CommonModule()
{
	outBehaviorData = outBehaviorData_reference;
}
CommonModule::~CommonModule()
{

}
float CommonModule::getLeftFootX()
{
	return outBehaviorData->leftx;
}
float CommonModule::getLeftFootY()
{
	return outBehaviorData->lefty;
}
float CommonModule::getRightFootX()
{
	return outBehaviorData->rightx;
}
float CommonModule::getRightFootY()
{
	return outBehaviorData->righty;
}

int CommonModule::getFallState()
{
    return outBehaviorData->intData[0];
}

int CommonModule::getBumperRight()
{
    return outBehaviorData->intData[1];
}

int CommonModule::getBumperLeft()
{
    return outBehaviorData->intData[2];
}

int CommonModule::getChestButton()
{
    return outBehaviorData->intData[3];
}

float CommonModule::getHeadYaw()
{
    return outBehaviorData->jointData[0];
}

float CommonModule::getHeadPitch()
{
    return outBehaviorData->jointData[1];
}
float CommonModule::getRShoulderPitch()
{
    return outBehaviorData->jointData[2];
}
float CommonModule::getRShoulderRoll()
{
    return outBehaviorData->jointData[3];
}
float CommonModule::getRElbowYaw()
{
    return outBehaviorData->jointData[4];
}
float CommonModule::getRElbowRoll()
{
    return outBehaviorData->jointData[5];
}
float CommonModule::getLShoulderPitch()
{
    return outBehaviorData->jointData[6];
}
float CommonModule::getLShoulderRoll()
{
    return outBehaviorData->jointData[7];
}
float CommonModule::getLElbowYaw()
{
    return outBehaviorData->jointData[8];
}
float CommonModule::getLElbowRoll()
{
    return outBehaviorData->jointData[9];
}
float CommonModule::getRHipYawPitch()
{
    return outBehaviorData->jointData[10];
}
float CommonModule::getRHipRoll()
{
    return outBehaviorData->jointData[11];
}
float CommonModule::getRHipPitch()
{
    return outBehaviorData->jointData[12];
}
float CommonModule::getRKneePitch()
{
    return outBehaviorData->jointData[13];
}
float CommonModule::getRAnklePitch()
{
    return outBehaviorData->jointData[14];
}

float CommonModule::getRAnkleRoll()
{
    return outBehaviorData->jointData[15];
}

float CommonModule::getLHipYawPitch()
{
    return outBehaviorData->jointData[16];
}

float CommonModule::getLHipRoll()
{
    return outBehaviorData->jointData[17];
}

float CommonModule::getLHipPitch()
{
    return outBehaviorData->jointData[18];
}

float CommonModule::getLKneePitch()
{
    return outBehaviorData->jointData[19];
}

float CommonModule::getLAnklePitch()
{
    return outBehaviorData->jointData[20];
}

float CommonModule::getLAnkleRoll()
{
    return outBehaviorData->jointData[21];
}


void set_common_reference(const OutBehaviorData* _outBehaviorData)
{
	outBehaviorData_reference = _outBehaviorData;
}


BOOST_PYTHON_MODULE(PyCommon)
{

	class_<CommonModule>("common")
        .def("fallState",&CommonModule::getFallState)
        .def("bumpRight",&CommonModule::getBumperRight)
        .def("bumpLeft",&CommonModule::getBumperLeft)
        .def("chestButton",&CommonModule::getChestButton)
		.def("leftX",&CommonModule::getLeftFootX)
		.def("leftY",&CommonModule::getLeftFootY)
		.def("rightX",&CommonModule::getRightFootX)
		.def("rightY",&CommonModule::getRightFootY)
		.def("headYaw",&CommonModule::getHeadYaw)
        .def("headPitch",&CommonModule::getHeadPitch)
        .def("rShoulderPitch",&CommonModule::getRShoulderPitch)
        .def("rShoulderRoll",&CommonModule::getRShoulderRoll)
        .def("rElbowYaw",&CommonModule::getRElbowYaw)
        .def("rElbowRoll",&CommonModule::getRElbowRoll)
        .def("lShoulderPitch",&CommonModule::getLShoulderPitch)
        .def("lShoulderRoll",&CommonModule::getLShoulderRoll)
        .def("lElbowYaw",&CommonModule::getLElbowYaw)
        .def("lElbowRoll",&CommonModule::getLElbowRoll)
        .def("rHipYawPitch",&CommonModule::getRHipYawPitch)
        .def("rHipRoll",&CommonModule::getRHipRoll)
        .def("rHipPitch",&CommonModule::getRHipPitch)
        .def("rKneePitch",&CommonModule::getRKneePitch)
        .def("rAnklePitch",&CommonModule::getRAnklePitch)
        .def("rAnkleRoll",&CommonModule::getRAnkleRoll)
        .def("lHipYawPitch",&CommonModule::getLHipYawPitch)
        .def("lHipRoll",&CommonModule::getLHipRoll)
        .def("lHipPitch",&CommonModule::getLHipPitch)
        .def("lKneePitch",&CommonModule::getLKneePitch)
        .def("lAnklePitch",&CommonModule::getLAnklePitch)
        .def("lAnkleRoll",&CommonModule::getLAnkleRoll)
		;


}
void c_init_common()
{
	if (!Py_IsInitialized())
		Py_Initialize();
	try {
		initPyCommon();
	} catch (error_already_set) {
		PyErr_Print();
	}
}
