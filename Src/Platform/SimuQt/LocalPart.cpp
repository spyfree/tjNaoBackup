#include "LocalPart.h"
#include "Simulation/ODESimulation/SimulationCore/Simulation/APIDatatypes.h"
#include "Platform/SystemCall.h"
#include <iostream>
#include <fstream>
#include <map>
#include "Module/Actuator/MatlabHelp/MatlabHelp.h"//x


LocalPart::LocalPart(SimObject* obj)
{
	ctrl = RoboCupCtrl::getController();

	std::vector<std::string> parts;
	parts.resize(2);
	parts[0] = obj->getFullName();
	//initialize sensor
	parts[1] = "camera";
	spCamera = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "acceleration";//"accelerometer";
	spAcc = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "gyroscope";
	spGyro = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "RFsrFL";
	spRFsrFL = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "RFsrFR";
	spRFsrFR = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "RFsrBL";
	spRFsrBL = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "RFsrBR";
	spRFsrBR = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "LFsrFL";
	spLFsrFL = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "LFsrFR";
	spLFsrFR = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "LFsrBL";
	spLFsrBL = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "LFsrBR";
	spLFsrBR = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "myRFsrFL";
	myFsr = ctrl->getSensorportIdByPartOfName(parts);
}

LocalPart::~LocalPart()
{

}


void LocalPart::update()
{
	// Acceleration sensors
	if(spAcc!=-1){
	
	double* acceleration;
	ctrl->getSensorValue(spAcc, acceleration);
	sensorData.data[SensorData::accX] = float(acceleration[0] / 9.81);
	sensorData.data[SensorData::accY] = float(acceleration[1] / 9.81);
	sensorData.data[SensorData::accZ] = float(acceleration[2] / 9.81);
	
	}
	//MATLABHELP_PLOT(sensorData.data[SensorData::accX],"myAccX");  //x
	//MATLABHELP_PLOT(sensorData.data[SensorData::accY],"myAccY");  //x
	//MATLABHELP_PLOT(sensorData.data[SensorData::accZ],"myAccZ");  //x


	
	// Gyros		//@todo: compare signs and absolute values with real robot
	if(spGyro!=-1){

	double* rotationSpeed;
	ctrl->getSensorValue(spGyro, rotationSpeed);
	sensorData.data[SensorData::gyroX] = float(rotationSpeed[0]);
	sensorData.data[SensorData::gyroY] = float(rotationSpeed[1]);
	sensorData.data[SensorData::gyroZ] = float(rotationSpeed[2]);
	}
	MATLABHELP_PLOT(sensorData.data[SensorData::gyroX],"myGyroX");  //x
	MATLABHELP_PLOT(sensorData.data[SensorData::gyroY],"myGyroY");  //x
	MATLABHELP_PLOT(sensorData.data[SensorData::gyroZ],"myGyroZ");  //x

	// Fsr Sensors
	double fsr[8];
	if(spRFsrFL != -1){
		ctrl->getSensorValue(spRFsrFL, fsr[0]);
		sensorData.data[SensorData::fsrRFL] = float(fsr[0]);
	}
	if(spRFsrFR != -1){
		ctrl->getSensorValue(spRFsrFR, fsr[1]);sensorData.data[SensorData::fsrRFR] = float(fsr[1]);
	}
	if(spRFsrBL != -1){
		ctrl->getSensorValue(spRFsrBL, fsr[2]);sensorData.data[SensorData::fsrRBL] = float(fsr[2]);
	}
	if(spRFsrBR != -1){
		ctrl->getSensorValue(spRFsrBR, fsr[3]);sensorData.data[SensorData::fsrRBR] = float(fsr[3]);
	}
	if(spLFsrFL != -1){
		ctrl->getSensorValue(spLFsrFL, fsr[4]);sensorData.data[SensorData::fsrLFL] = float(fsr[4]);
	}
	if(spLFsrFR != -1){
		ctrl->getSensorValue(spLFsrFR, fsr[5]);sensorData.data[SensorData::fsrLFR] = float(fsr[5]);
	}
	if(spLFsrBL != -1){
		ctrl->getSensorValue(spLFsrBL, fsr[6]);sensorData.data[SensorData::fsrLBL] = float(fsr[6]);
	}
	if(spLFsrBL != -1){
		ctrl->getSensorValue(spLFsrBR, fsr[7]);
		sensorData.data[SensorData::fsrLBR] = float(fsr[7]);
	}
}

SensorData LocalPart::getSensorData()
{
	return sensorData;
}