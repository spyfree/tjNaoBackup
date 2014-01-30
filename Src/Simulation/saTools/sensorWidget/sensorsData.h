#ifndef SENSORSDATA_H
#define SENSORSDATA_H


class SensorsData
{

public:
	enum Sensor
	{
		gyroX,
		gyroY,
		gyroZ,
		accX,
		accY,
		accZ,
		angleX,    //newly added for Torso Angle 
		angleY,
		compassX,
		compassY,
		voltage,
		fsrLFL,     //the feetsensors of the Nao-Robot
		fsrLFR,
		fsrLBL,
		fsrLBR,
		fsrRFL,
		fsrRFR,
		fsrRBL,
		fsrRBR,
		us,
		numOfSensors   //22
	};

	//special value to indicate that the sensor does not exist
	enum
	{
		doesNotExist=-9999999
	};

	static const char* getSensorUnit(Sensor sensor)
	{
		switch(sensor)
		{
		case gyroX: return "rad/s";
		case gyroY: return "rad/s";
		case gyroZ: return "rad/s";
		case accY: return "m/s^2";
		case accZ: return "m/s^2";
		case accX: return "m/s^2";
		case angleX: return "rad";  //newly added for Torso Angle 
		case angleY: return "rad";
		case compassX: return "m";
		case compassY: return "m";
		case voltage: return "%";
		case fsrLFL: return "N";
		case fsrLFR: return "N";
		case fsrLBL: return "N";
		case fsrLBR: return "N";
		case fsrRFL: return "N";
		case fsrRFR: return "N";
		case fsrRBL: return "N";
		case fsrRBR: return "N";
		case us: return " ";
		default: return "unknown";
		}
	}

	static const char* getSensorName(Sensor sensor)
	{
		switch(sensor)
		{
		case gyroX: return "gyroX";
		case gyroY: return "gyroY";
		case gyroZ: return "gyroZ";
		case accY: return "accY";
		case accZ: return "accZ";
		case accX: return "accX";
		case angleX: return "angleX";  //newly added for Torso Angle 
		case angleY: return "angleY";
		case compassX: return "compassX";
		case compassY: return "compassY";
		case voltage: return "voltage";
		case fsrLFL: return "fsrLFL";
		case fsrLFR: return "fsrLFR";
		case fsrLBL: return "fsrLBL";
		case fsrLBR: return "fsrLBR";
		case fsrRFL: return "fsrRFL";
		case fsrRFR: return "fsrRFR";
		case fsrRBL: return "fsrRBL";
		case fsrRBR: return "fsrRBR";
		case us: return "us";
		default: return "unknown";
		}
	}

	static const char* getNaoSensorName(Sensor sensor)
	{
		switch(sensor)
		{
		case gyroX: return "Device/SubDeviceList/InertialSensor/GyrX/Sensor/Value";
		case gyroY: return "Device/SubDeviceList/InertialSensor/GyrY/Sensor/Value";
		case gyroZ: return "Device/SubDeviceList/InertialSensor/GyrY/Sensor/Value";
		case accY: return "Device/SubDeviceList/InertialSensor/AccY/Sensor/Value";
		case accZ: return "Device/SubDeviceList/InertialSensor/AccZ/Sensor/Value";
		case accX: return "Device/SubDeviceList/InertialSensor/AccX/Sensor/Value";
		case angleX: return "Device/SubDeviceList/InertialSensor/AngleX/Sensor/Value";  //newly added for Torso Angle 
		case angleY: return "Device/SubDeviceList/InertialSensor/AngleY/Sensor/Value";
		case compassX: return "Device/SubDeviceList/InertialSensor/AccZ/Sensor/Value";
		case compassY: return "Device/SubDeviceList/InertialSensor/AccZ/Sensor/Value";
		case voltage: return "Device/SubDeviceList/InertialSensor/AccZ/Sensor/Value";
		case fsrLFL: return "Device/SubDeviceList/LFoot/FSR/FrontLeft/Sensor/Value";
		case fsrLFR: return "Device/SubDeviceList/LFoot/FSR/FrontRight/Sensor/Value";
		case fsrLBL: return "Device/SubDeviceList/LFoot/FSR/RearLeft/Sensor/Value";
		case fsrLBR: return "Device/SubDeviceList/LFoot/FSR/RearRight/Sensor/Value";
		case fsrRFL: return "Device/SubDeviceList/RFoot/FSR/FrontLeft/Sensor/Value";
		case fsrRFR: return "Device/SubDeviceList/RFoot/FSR/FrontRight/Sensor/Value";
		case fsrRBL: return "Device/SubDeviceList/RFoot/FSR/RearLeft/Sensor/Value";
		case fsrRBR: return "Device/SubDeviceList/RFoot/FSR/RearRight/Sensor/Value";
		case us: return "US";
		default: return "unknown";
			// 		"RFoot/Bumper/Right",
			// 		"RFoot/Bumper/Left",
			// 		"LFoot/Bumper/Right",
			// 		"LFoot/Bumper/Left",
			//"ChestBoard/Button"
		}
	}
};

#endif		//SENSORSDATA_H