#ifndef __CoPDetector_h_
#define __CoPDetector_h_
#include "Tools/Math/Vector3.h"
#include "Module/Common/CoP.h"
class SensorData;
class CoPDetector
{
public:
	CoPDetector();
	void update(const SensorData& sensorData);
	/** Return cop of Left or Right leg
	* @param left Left leg if true
	*/
	const CoP& getCoP(bool left){
		if(left)
			return copLeft;
		else
			return copRight;
	}
private:
	Vector3<double> locationLeft[4], locationRight[4];
	CoP copLeft, copRight;
};
#endif