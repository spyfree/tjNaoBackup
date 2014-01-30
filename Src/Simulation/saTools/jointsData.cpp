#include "jointsData.h"

int JointsData::jointsMaxArray[JointsData::numOfJoints] = {120, 45, 120,0, 120,  90, 120,95, 120,		 0,  0, 25,  25,130, 45, 45,  0, 45,  25,130, 45, 25};
int JointsData::jointsMinArray[JointsData::numOfJoints] = {-120,-45,-120, -95,-120,0,-120, 0,-120,-90,-90,-45,-100,  0,-75,-25,-90,-25,-100,  0,-75,-45};

int JointsData::JointMap[JointsData::numOfJoints] = {0,1,	6,7,8,9,	16,17,18,19,20,21,	10,11,12,13,14,15,	2,3,4,5};
int JointsData::MotionProxyToSaMap[JointsData::numOfJoints] = {0,1,	18,19,20,21,	 2,3,4,5,  12,13,14,15,16,17,  6,7,8,9,10,11};

// static const int JointMap[22]=	//Motion API  Joint  => JointsData::Joint
// {
// 	0,//HeadYaw
// 	1,//HeadPitch
// 	6,//LShoulderPitch
// 	7,//LShoulderRoll
// 	8,//LElbowYaw
// 	9,//LElbowRoll
// 	16,//LHipYawPitch
// 	17,//LHipRoll
// 	18,//LHipPitch
// 	19,//LKneePitch
// 	20,//LAnklePitch
// 	21,//LAnkleRoll
// 	10,//RHipYawPitch
// 	11,//RHipRoll
// 	12,//RHipPitch
// 	13,//RKneePitch
// 	14,//RAnklePitch
// 	15,//RAnkleRoll
// 	2,//RShoulderPitch
// 	3,//RShoulderRoll
// 	4,//RElbowYaw
// 	5,//RElbowRoll
// };

JointsData::JointsData()
{
	for (int i=0;i<numOfJoints;i++)
		jointsAngle[i]=0;
	numOfCycle = 0;
	timeOfAction = 0; //unit: msec
}