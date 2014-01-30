/**
* @file FrameInfo.h
* The file declares a class that contains information on the current frame.
*/ 

#ifndef __FrameInfo_h_
#define __FrameInfo_h_

#include "Tools/Streams/Streamable.h"
#include "Platform/SystemCall.h"
/**
* @class FrameInfo
* A class that contains information on the current frame.
*/
class FrameInfo 
{

//private:
//	/**
//	* The method makes the object streamable.
//	* @param in The stream from which the object is read (if in != 0).
//	* @param out The stream to which the object is written (if out != 0).
//	*/
//	virtual void serialize(In* in, Out* out)
//	{  
//		//STREAM_REGISTER_BEGIN();
//		STREAM(time);
//		//STREAM_REGISTER_FINISH();
//	}
public:
	/*The singletion pointer*/
	static FrameInfo* theFrameInfo;

public:

	/**
	* The method returns the time difference between a given time stamp and the 
	* current frame time.
	* @param timeStamp A time stamp, usually in the past.
	* @return The number of ms passed since the given time stamp.
	*/
	FrameInfo() : time(0) {}
	int getTimeSince(unsigned timeStamp) const {return int(time - timeStamp);}

	unsigned time; /**< The time stamp of the data processed in the current frame in ms. */
};
/**
* @class ActuatorFrameInfo
* A class that contains information on the current Actuator frame.
* This representation is used to track whether joints data are
* received. 
*/
class ActuatorFrameInfo : public FrameInfo {};
#endif //__FrameInfo_h_
