#ifndef __Sensor_h_
#define __Sensor_h_
#include "Module/Representations/KS.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Common/NaoConfig.h"
#include "Module/Common/JointData.h"
#include "Module/Common/NaoStructure.h"
#include "Module/Common/SensorData.h"
#include "Module/Common/FallDownState.h"
#include "Module/Common/WalkingEngineOutput.h"
#include "Module/Common/RingBufferWithSum.h"


DEFINE_KS(Sensor)
USES(NaoConfig)
USES(JointCmd)
REQUIRES(SafeJointData)
REQUIRES(SafeSensorData)
PROVIDES(NaoStructure)
PROVIDES(FallDownState)
END_KS

class Sensor : public SensorBase,BaseProvider
{
public:
	Sensor(){}
	~Sensor(){}
	/************************************************************************/
	/*		Implement functions derived from Base                                        */
	/************************************************************************/
private:
	void update(NaoStructure* theNaoStructure);
	void update(FallDownState* theFallDownState);
	void outFallDownState(){}
	void inSafeJointData();
	void inSafeSensorData(){}
	void outNaoStructure();

private:
  /** Indices for buffers of sensor data */
	enum BufferEntries{accX = 0, accY, accZ, numOfBuffers};

  /** Buffers for averaging sensor data */
  RingBufferWithSum<double,5> buffers[numOfBuffers];
};
#endif //__Sensor_h_
