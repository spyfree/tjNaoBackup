#ifndef JOINTSDATA_H
#define JOINTSDATA_H

class JointsData
{
public:
	JointsData();

public:
	enum 
	{
		angle=1,
		stiffness=2
	};	

	enum Joint
	{
		HeadYaw = 0,	//0
		HeadPitch ,
		RShoulderPitch ,
		RShoulderRoll , 
		RElbowYaw ,		//4
		RElbowRoll ,
		LShoulderPitch ,
		LShoulderRoll ,
		LElbowYaw ,
		LElbowRoll ,//9
		RHipYawPitch ,
		RHipRoll ,
		RHipPitch ,
		RKneePitch ,
		RAnklePitch ,//14
		RAnkleRoll ,
		LHipYawPitch ,
		LHipRoll ,
		LHipPitch ,
		LKneePitch ,//19
		LAnklePitch ,
		LAnkleRoll ,	//21
		numOfJoints		//22
	};

	static const char* getJointName(Joint joint)	//static ??
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

	static int jointsMaxArray[JointsData::numOfJoints];
	static int jointsMinArray[JointsData::numOfJoints];

	static int JointMap[JointsData::numOfJoints];
	static int MotionProxyToSaMap[JointsData::numOfJoints];
	
public:
	double jointsAngle[JointsData::numOfJoints];
	int  numOfCycle;
	int  timeOfAction;
};

#endif	//JOINTSDATA_H