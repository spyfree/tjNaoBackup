#ifndef __GroundContactDetector_h_
#define __GroundContactDetector_h_
#include "Tools/Math/Range.h"
#include "Module/Common/GroundContactState.h"
class SensorData;
class FrameInfo;
class GroundContactDetector
{
public:
	GroundContactDetector();
	/**
	* Updates the GroundContactState representation .
	* @param groundContactState The ground contact representation which is updated by this module.
	*/
	void update(const SensorData& sensorData, const FrameInfo& theFrameInfo);

	GroundContactState contactStateLeft, contactStateRight;
private:
	/**
	* A collection of parameters for the walking engine.
	*/
	class Parameters
	{
	public:
		/** Default constructor. */
		Parameters() {}

		float minContactPercent;
		unsigned int minFootContactUnity;
		unsigned int minCycleContactUnity;
		int safeContactTime;
	};

	Parameters p;

	Range<float> leftRange;
	Range<float> rightRange;

	unsigned int framesWithContactLeft, framesWithContactRight;
	unsigned int framesWithoutContactLeft, framesWithoutContactRight;
	unsigned int contactStartTimeLeft, contactStartTimeRight;

	bool contact;
	bool contactLeft, contactRight;
	bool wasContacted;
	bool wasContactedLeft, wasContactedRight;

	void getMinAndMax(const float* data, float& min, float& max);
};
#endif