/**
* @file Representations/Infrastructure/JointData.h
*
* This file declares a class to represent the joint angles sent to the robot.
*
* @author <A href="mailto:Thomas.Roefer@dfki.de">Thomas Röfer</A>
*/

#ifndef __JointData_H__
#define __JointData_H__

#include "Tools/Streams/Streamable.h"
#include "Tools/Math/Common.h"
//#include "Platform/GTAssert.h"

/**
* @class JointData
* A class to represent the joint angles sent to the robot.
*/
class JointData : public Streamable
{
protected:
	virtual void serialize(In* in, Out* out)
	{
		//STREAM_REGISTER_BEGIN();
		STREAM_ARRAY(angles);
		STREAM(timeStamp);
		//STREAM_REGISTER_FINISH();
	}

public:
	enum Joint
	{
		NoneJoint = 0,
		HeadYaw ,
		HeadPitch ,
		RShoulderPitch ,
		RShoulderRoll ,  //should be Yaw, but to keep with the manual
		RElbowYaw ,//5
		RElbowRoll ,
		LShoulderPitch ,
		LShoulderRoll ,
		LElbowYaw ,
		LElbowRoll ,//10
		RHipYawPitch ,
		RHipRoll ,
		RHipPitch ,
		RKneePitch ,
		RAnklePitch ,//15
		RAnkleRoll ,
		LHipYawPitch ,
		LHipRoll ,
		LHipPitch ,
		LKneePitch ,//20
		LAnklePitch ,
		LAnkleRoll ,
		numOfJoints
	};


	// If you change those values be sure to change them in MofCompiler.cpp too. (Line ~280)
	enum
	{
		off    = 1000, /**< Special angle for switching off a joint. */
		ignore = 2000  /**< Special angle for not overwriting the previous setting. */
	};
	double angles[numOfJoints]; /**< The angles of all joints. */
	unsigned timeStamp; /**< The time when these angles were received. */

	/**
	* Default constructor.
	* Switches off all joints.
	*/
	JointData() : timeStamp(0)
	{
		for(int i = 0; i < numOfJoints; ++i)
			angles[i] = off;
	}
	JointData(int ijoint):timeStamp(0)
	{
		for(int i = 0; i < numOfJoints; ++i)
			angles[i] = ijoint;
	}
	/** Assignment operator
	*/
	JointData& operator= (const JointData& other)
	{
		for(int i = 0; i < numOfJoints; ++i)
			angles[i] = other.angles[i];
		timeStamp = other.timeStamp;
		return *this;
	}
	/**
	* The method returns the angle of the mirror (left/right) of the given joint.
	* @param joint The joint the mirror of which is returned.
	* @return The angle of the mirrored joint.
	*/
	double mirror(Joint joint) const
	{
		switch (joint)
		{
			// don't mirror an invalid join value (!)
		case HeadYaw: return angles[HeadYaw] == off || angles[HeadYaw] == ignore ? angles[HeadYaw]: -angles[HeadYaw];
		case RShoulderPitch: return angles[LShoulderPitch];
		case RShoulderRoll: return angles[LShoulderRoll];
		case RElbowYaw: return angles[LElbowYaw];
		case RElbowRoll: return angles[LElbowRoll];
		case LShoulderPitch: return angles[RShoulderPitch];
		case LShoulderRoll: return angles[RShoulderRoll];
		case LElbowYaw: return angles[RElbowYaw];
		case LElbowRoll: return angles[RElbowRoll];
		case RHipYawPitch: return angles[LHipYawPitch];
		case RHipRoll: return angles[LHipRoll];
		case RHipPitch: return angles[LHipPitch];
		case RKneePitch: return angles[LKneePitch];
		case RAnklePitch: return angles[LAnklePitch];
		case RAnkleRoll: return angles[LAnkleRoll];
		case LHipYawPitch: return angles[RHipYawPitch];
		case LHipRoll: return angles[RHipRoll];
		case LHipPitch: return angles[RHipPitch];
		case LKneePitch: return angles[RKneePitch];
		case LAnklePitch: return angles[RAnklePitch];
		case LAnkleRoll: return angles[RAnkleRoll];
		default: return angles[joint];
		}
	}

	/**
	* The method initializes the joint angles as a mirror of a set of other joint angles.
	* @param other The set of joint angles that are mirrored.
	*/
	void mirror(const JointData& other)
	{
		for(int i = 0; i < numOfJoints; ++i)
			angles[i] = other.mirror((Joint) i);
		timeStamp = other.timeStamp;
	}

	/**
	* The function returns the name of a joint.
	* @param joint The joint the name of which will be returned.
	* @return The name.
	*/
	static const char* getJointName(Joint joint)
	{
		switch(joint)
		{
		case HeadYaw        : return 	"HeadYaw";
		case HeadPitch      :  return "HeadPitch";
		case RShoulderPitch :  return "RShoulderPitch";
		case RShoulderRoll  :  return "RShoulderRoll";
		case RElbowYaw      :  return "RElbowYaw";
		case RElbowRoll     :  return "RElbowRoll";
		case LShoulderPitch :  return "LShoulderPitch";
		case LShoulderRoll  :  return "LShoulderRoll";
		case LElbowYaw      :  return "LElbowYaw";
		case LElbowRoll     :  return "LElbowRoll";
		case RHipYawPitch   : return 	"RHipYawPitch";
		case RHipRoll       : return 	"RHipRoll";
		case RHipPitch      : return 	"RHipPitch";
		case RKneePitch     : return 	"RKneePitch";
		case RAnklePitch    : return 	"RAnklePitch";
		case RAnkleRoll     : return 	"RAnkleRoll";
		case LHipYawPitch   :  return "LHipYawPitch";
		case LHipRoll       :  return "LHipRoll";
		case LHipPitch      :  return "LHipPitch";
		case LKneePitch     :  return "LKneePitch";
		case LAnklePitch    :  return "LAnklePitch";
		case LAnkleRoll     : return 	"LAnkleRoll";
		default:
			return "unknown";
		}
	}
};
#if 0
/**
* @class JointDataDeg
* A class that wraps joint data to be transmitted in degrees.
*/
class JointDataDeg : public JointData
{
private:
	virtual void serialize(In* in, Out* out)
	{
		STREAM_REGISTER_BEGIN();
		if(jointData)
		{
			ASSERT(out);
			for(int i = 0; i < JointData::numOfJoints; ++i)
				angles[i] = jointData->angles[i] == JointData::off ? JointData::off
				: floor(toDegrees(jointData->angles[i]) * 10.0 + 0.5) / 10.0;
			timeStamp = jointData->timeStamp;

			STREAM_ARRAY(angles);
			STREAM(timeStamp);
		}
		else
		{
			STREAM_BASE(JointData);
		}
		STREAM_REGISTER_FINISH();
	}

	JointData* jointData; /**< The joint data that is wrapped. */

public:
	/**
	* Default constructor.
	*/
	JointDataDeg() : jointData(0) {}

	/**
	* Constructor.
	* @param jointData The joint data that is wrapped.
	*/
	JointDataDeg(JointData& jointData) : jointData(&jointData) {}

	/**
	* Assignment operator.
	*/
	JointDataDeg& operator=(const JointDataDeg& other)
	{
		if(jointData)
			for(int i = 0; i < JointData::numOfJoints; ++i)
				jointData->angles[i] = other.angles[i] == JointData::off ? JointData::off
				: fromDegrees(other.angles[i]);
		else
			*((JointData*) this) = other;
		return *this;
	}
};
#endif
class JointCmd : public JointData {};
// class JointRequest: public JointData {};
class SensoryJointData : public JointData {};
//class MotionJointData :public JointData{};
class SafeJointData : public JointData {};
//class StandOutput : public JointRequest {};

#endif // __JointData_H__
