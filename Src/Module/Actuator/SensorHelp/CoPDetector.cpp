#include "CoPDetector.h"
#include "Module/Common/SensorData.h"
// TODO: move this to NaoConfig
const double FSR_LOCATION_LEFT[][2] = { {0.071, 0.03},
																{0.071, -0.023},
																{-0.0304, 0.03},
																{-0.0298, -0.019} };
CoPDetector::CoPDetector()
{
	for(int i = 0; i < 4; ++i)
	{
		locationLeft[i].x = FSR_LOCATION_LEFT[i][0];
		locationLeft[i].y = FSR_LOCATION_LEFT[i][1];
		locationRight[i].x = FSR_LOCATION_LEFT[i][0];
		locationRight[i].y = -FSR_LOCATION_LEFT[i][1];
	}
}

void CoPDetector::update(const SensorData& sensorData)
{
	Vector3<double> copSumLeft, copSumRight;
	double lpressure =0, rpressure = 0;
	for(int i = 0; i < 4; ++i)
	{
		copSumLeft += locationLeft[i] * static_cast<double>(sensorData.data[SensorData::fsrLFL + i]);
		lpressure += sensorData.data[SensorData::fsrLFL + i];
		copSumRight += locationRight[i] * static_cast<double>(sensorData.data[SensorData::fsrRFL + i]);
		rpressure += sensorData.data[SensorData::fsrRFL + i];
	}
	copLeft.pressure = lpressure;
	copRight.pressure = rpressure;
	copLeft.p = copSumLeft / lpressure;
	copRight.p = copSumRight / rpressure;
}