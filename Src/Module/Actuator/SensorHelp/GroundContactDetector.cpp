#include "GroundContactDetector.h"
#include "Module/Common/SensorData.h"
#include "Module/Common/FrameInfo.h"
//#include "Module/Actuator/MatlabHelp/MatlabHelp.h"
#include <cfloat>
GroundContactDetector::GroundContactDetector():
leftRange(FLT_MAX, -FLT_MAX), rightRange(FLT_MAX, -FLT_MAX), framesWithoutContactLeft(0),  framesWithoutContactRight(0),
framesWithContactLeft(0), framesWithContactRight(0), contactStartTimeLeft(0), contactStartTimeRight(0), 
contact(false), wasContacted(false), wasContactedLeft(false), wasContactedRight(false),
contactLeft(false), contactRight(false)
{
	p.minContactPercent = 0.25f;
	p.minFootContactUnity = 2;
	p.minCycleContactUnity = 10;
	p.safeContactTime = 3300;
// 	InConfigFile stream("groundContact.cfg");
// 	if(stream.exists())
// 		stream >> p;
}

void GroundContactDetector::getMinAndMax(const float* data, float& min, float& max)
{
	max = min = *(data++);
	if(*data < min) min = *data;
	if(*data > max) max = *data;
	++data;
	if(*data < min) min = *data;
	if(*data > max) max = *data;
	++data;
	if(*data < min) min = *data;
	if(*data > max) max = *data;
}
void GroundContactDetector::update(const SensorData& sensorData, const FrameInfo& theFrameInfo)
{

	bool lastContactLeft = contactStateLeft.contact;
	bool lastContactRight = contactStateRight.contact;
	if(sensorData.data[SensorData::fsrLFL] != SensorData::off && sensorData.data[SensorData::fsrRFL] != SensorData::off)
	{
		float min, max;
		getMinAndMax(&sensorData.data[SensorData::fsrLFL], min, max);
		if(min > leftRange.max) leftRange.max = min;
		if(max < leftRange.min) leftRange.min = max;
		getMinAndMax(&sensorData.data[SensorData::fsrRFL], min, max);
		if(min > rightRange.max) rightRange.max = min;
		if(max < rightRange.min) rightRange.min = max;

		unsigned int leftContact = 0;
		float size = leftRange.getSize();
		if(size > 0.f)
			for(int i = SensorData::fsrLFL; i <= SensorData::fsrLBR; ++i)
				if((sensorData.data[i] - leftRange.min) / size > p.minContactPercent)
					leftContact++;
		unsigned int rightContact = 0;
		size = rightRange.getSize();
		if(size > 0.f)
			for(int i = SensorData::fsrRFL; i <= SensorData::fsrRBR; ++i)
				if((sensorData.data[i] - rightRange.min) / size > p.minContactPercent)
					rightContact++;
		//  Left Contact
		if(leftContact >= p.minFootContactUnity) // contact 
		{
			framesWithContactLeft++;
			framesWithoutContactLeft = 0;
		}
		else
		{
			framesWithContactLeft = 0;
			framesWithoutContactLeft++;
		}
		if(contactLeft)
			contactLeft = framesWithoutContactLeft <= p.minCycleContactUnity;
		else
			contactLeft = framesWithContactLeft > p.minCycleContactUnity;
		if(contactLeft)
			wasContactedLeft = true;
		contactStateLeft.contact = contactLeft || !wasContactedLeft;
		//  Right Contact
		if(rightContact >= p.minFootContactUnity) // contact 
		{
			framesWithContactRight++;
			framesWithoutContactRight = 0;
		}
		else
		{
			framesWithContactRight = 0;
			framesWithoutContactRight++;
		}
		if(contactRight)
			contactRight = framesWithoutContactRight <= p.minCycleContactUnity;
		else
			contactRight = framesWithContactRight > p.minCycleContactUnity;
		if(contactRight)
			wasContactedRight = true;
		contactStateRight.contact = contactRight || !wasContactedRight;

		contact = contactLeft || contactRight;
		wasContacted = wasContactedLeft || wasContactedRight;
		//justContactLeft = contact || !wasContacted /*|| theMotionRequest.motion == MotionRequest::specialAction|| theMotionInfo.executedMotionRequest.motion == MotionRequest::specialAction*/ ;
	}
	else{
		contactStateLeft.contact = true;
		contactStateRight.contact = true;
	}

	if((contactStateLeft.contact && !lastContactLeft) || (contactStateLeft.contact && contactStartTimeLeft == 0))
		contactStartTimeLeft = theFrameInfo.time;
	if((contactStateRight.contact && !lastContactRight) || (contactStateRight.contact && contactStartTimeRight == 0))
		contactStartTimeRight = theFrameInfo.time;
	contactStateLeft.contactSafe = (contactStateLeft.contact && theFrameInfo.getTimeSince(contactStartTimeLeft) >= p.safeContactTime);
	contactStateRight.contactSafe = (contactStateRight.contact && theFrameInfo.getTimeSince(contactStartTimeRight) >= p.safeContactTime);

// 	MATLABHELP_PLOT(sensorData.data[SensorData::fsrLFL], "fsrLFL");
// 	MATLABHELP_PLOT(sensorData.data[SensorData::fsrLFR], "fsrLFR");
// 	MATLABHELP_PLOT(sensorData.data[SensorData::fsrLBL], "fsrLBL");
// 	MATLABHELP_PLOT(sensorData.data[SensorData::fsrLBR], "fsrLBR");
// 	MATLABHELP_PLOT(sensorData.data[SensorData::fsrRFL], "fsrRFL");
// 	MATLABHELP_PLOT(sensorData.data[SensorData::fsrRFR], "fsrRFR");
// 	MATLABHELP_PLOT(sensorData.data[SensorData::fsrRBL], "fsrRBL");
// 	MATLABHELP_PLOT(sensorData.data[SensorData::fsrRBR], "fsrRBR");
// 	MATLABHELP_PLOT(leftRange.min, "leftRangeMin");
// 	MATLABHELP_PLOT(leftRange.max, "leftRangeMax");
// 	MATLABHELP_PLOT(rightRange.min, "rightRangeMin");
// 	MATLABHELP_PLOT(rightRange.max, "rightRangeMax");
// 	MATLABHELP_PLOT(framesWithContactRight,"framesWithContactRight");
// 	MATLABHELP_PLOT(framesWithoutContactRight,"framesWithoutContactRight");
// 	MATLABHELP_PLOT(framesWithContactLeft,"framesWithContactLeft");
// 	MATLABHELP_PLOT(framesWithoutContactLeft,"framesWithoutContactLeft");
	//MATLABHELP_PLOT(contactLeft?0.75:0.25, "contactLeft");
	//MATLABHELP_PLOT(contactRight?0.75:0.25, "contactRight");
}
