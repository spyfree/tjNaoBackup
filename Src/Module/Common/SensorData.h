#ifndef _SensorData_H_
#define _SensorData_H_

#include "Tools/Streams/Streamable.h"
#include "JointData.h"

/**
* @class SensorData
* A class to represent the sensor datas received from the robot.
*/
class SensorData : public Streamable
{
protected:
	virtual void serialize(In* in, Out* out)
	{  
		//STREAM_REGISTER_BEGIN();
		STREAM_ARRAY(data);
		STREAM_ARRAY(loads);
		STREAM_ARRAY(temperatures);
		STREAM(timeStamp);
		//STREAM_REGISTER_FINISH();
	}
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
	enum{
		off = JointData::off
	};
	float data[numOfSensors]; /**< The data of all sensors. */
	float loads[JointData::numOfJoints]; /**< The load of all sensors. */  
	float temperatures[JointData::numOfJoints]; /**< The temperature of all sensors. */  
	unsigned timeStamp; /**< The time when the sensor data was received. */

	/**
	* Default constructor.
	*/
	SensorData() : timeStamp(0)
	{
		for(int i = 0; i < numOfSensors; ++i)
			if( i < fsrLFL || i > fsrRBR)
				data[i] = 0;
			else
				data[i] = doesNotExist;
		for(int i = 0; i < JointData::numOfJoints; ++i)
			loads[i] = temperatures[i] = 0;
	}

	/**
	* The function returns the name of a sensor.
	* @param sensor The sensor the name of which will be returned.
	* @return The name.
	*/
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
		case accX: return "Device/SubDeviceList/InertialSensor/AccX/Sensor/Value";
		case accY: return "Device/SubDeviceList/InertialSensor/AccY/Sensor/Value";
		case accZ: return "Device/SubDeviceList/InertialSensor/AccZ/Sensor/Value";	
		case angleX: return "Device/SubDeviceList/InertialSensor/AngleX/Sensor/Value";  //newly added for Torso Angle 
		case angleY: return "Device/SubDeviceList/InertialSensor/AngleY/Sensor/Value";
		case fsrLFL: return "Device/SubDeviceList/LFoot/FSR/FrontLeft/Sensor/Value";
		case fsrLFR: return "Device/SubDeviceList/LFoot/FSR/FrontRight/Sensor/Value";
		case fsrLBL: return "Device/SubDeviceList/LFoot/FSR/RearLeft/Sensor/Value";
		case fsrLBR: return "Device/SubDeviceList/LFoot/FSR/RearRight/Sensor/Value";
		case fsrRFL: return "Device/SubDeviceList/RFoot/FSR/FrontLeft/Sensor/Value";
		case fsrRFR: return "Device/SubDeviceList/RFoot/FSR/FrontRight/Sensor/Value";
		case fsrRBL: return "Device/SubDeviceList/RFoot/FSR/RearLeft/Sensor/Value";
		case fsrRBR: return "Device/SubDeviceList/RFoot/FSR/RearRight/Sensor/Value";
		default: return "unknown";
		}
	}
};

class SafeSensorData : public SensorData {};

#endif