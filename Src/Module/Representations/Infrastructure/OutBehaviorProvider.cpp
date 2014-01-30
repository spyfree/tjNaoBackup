#include "OutBehaviorProvider.h"
#include "Module/Common/NaoConfig.h"
#include "Tools/Math/Pose3D.h"
#include <iostream>

OutBehaviorProvider::OutBehaviorProvider()
{

}

OutBehaviorProvider::~OutBehaviorProvider()
{

}

void OutBehaviorProvider::update(OutBehaviorData *theOutBehaviorData)
{
    outOutBehaviorData();
}

void OutBehaviorProvider::outOutBehaviorData()
{
    setBehaviorData();
	theOutBehaviorData->intData = outBehaviorData.intData;
	theOutBehaviorData->jointData = outBehaviorData.jointData;
	theOutBehaviorData->leftx = outBehaviorData.leftx;
	theOutBehaviorData->lefty = outBehaviorData.lefty;
	theOutBehaviorData->rightx = outBehaviorData.rightx;
	theOutBehaviorData->righty = outBehaviorData.righty;
}

void OutBehaviorProvider::setBehaviorData()
{
   /* int i = 0;
    int numSet = 0;
    int fallState = FallDownState::upright;
    bool bumperRight = false, bumperLeft = false, chestButton = false;
    if(theKeyStates != NULL){
        bumperRight = theKeyStates->pressed[KeyStates::RBumperRight] || theKeyStates->pressed[KeyStates::RBumperLeft];
        bumperLeft  = theKeyStates->pressed[KeyStates::LBumperRight] || theKeyStates->pressed[KeyStates::LBumperLeft];
        chestButton = theKeyStates->pressed[KeyStates::ChestButton];
    }
    if(theFallDownState != NULL){
        fallState = theFallDownState->state;
    }
    i = 0;
    outBehaviorData.intData.clear();
    outBehaviorData.intData.resize(OutBehaviorData::NUM_INT_DATA);
    outBehaviorData.intData[i++] = fallState;//fallen state
    outBehaviorData.intData[i++] = bumperRight;//bumper right
    outBehaviorData.intData[i++] = bumperLeft;//bumper left
    outBehaviorData.intData[i++] = chestButton;//chest button*/

	int i = 0;
	int numSet = 0;
	int fallState = FallDownState::upright;
	/*int bumperRight = 0, bumperLeft = false, chestButton = false;
	if(theKeyStates != NULL){
	bumperRight = theKeyStates->pressed[KeyStates::RBumperRight] || theKeyStates->pressed[KeyStates::RBumperLeft];
	bumperLeft  = theKeyStates->pressed[KeyStates::LBumperRight] || theKeyStates->pressed[KeyStates::LBumperLeft];
	chestButton = theKeyStates->pressed[KeyStates::ChestButton];
	}*/
	if(theFallDownState != NULL){
		fallState = theFallDownState->state;
	}
	i = 0;
	outBehaviorData.intData.clear();
	outBehaviorData.intData.resize(OutBehaviorData::NUM_INT_DATA);
	outBehaviorData.intData[i++] = fallState;//fallen state
	outBehaviorData.intData[i++] = theKeyStates->numRightClick;//bumper right
	outBehaviorData.intData[i++] = theKeyStates->numLeftClick;//bumper left
	outBehaviorData.intData[i++] = theKeyStates->numChestClick;//chest button

//    numSet = i;
//    if(numSet == OutBehaviorData::NUM_INT_DATA)
//        std::cout<<"Num of Data Set missmatch with NUM_INT_DATA: NumSet: "<<numSet<<", NUM_INT_DATA: "<<OutBehaviorData::NUM_INT_DATA<<std::endl;
//    printf("%d Fallen and Button Data Set to :\n", outBehaviorData.intData.size());
//    for(i = 0; i < numSet; i++)
//    {
//        printf("the %d number is ", i);
//        std::cout<<outBehaviorData.intData[i]<<std::endl;
//    }

	//========for joint data
	i = 0;
	outBehaviorData.jointData.clear();
    outBehaviorData.jointData.resize(OutBehaviorData::NUM_JOINT_DATA);
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::HeadYaw];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::HeadPitch];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::RShoulderPitch];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::RShoulderRoll];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::RElbowYaw];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::RElbowRoll];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::LShoulderPitch];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::LShoulderRoll];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::LElbowYaw];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::LElbowRoll];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::RHipYawPitch];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::RHipRoll];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::RHipPitch];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::RKneePitch];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::RAnklePitch];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::RAnkleRoll];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::LHipYawPitch];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::LHipRoll];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::LHipPitch];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::LKneePitch];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::LAnklePitch];
	outBehaviorData.jointData[i++] =(float)theSafeJointData->angles[JointData::LAnkleRoll];

	RotationMatrix Rsupport;
	Rsupport = theNaoStructure->uLink[NaoStructure::bodyLink].R.invert();
	Pose3D Tsupport;
	Tsupport.rotation= Rsupport;
	Tsupport.translate(theNaoStructure->uLink[NaoStructure::bodyLink].p*(-1));
	if (theNaoStructure->supportingMode ==NaoConfig::SUPPORT_MODE_LEFT || theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		outBehaviorData.leftx = Tsupport.translation.x*1000;
		outBehaviorData.lefty = Tsupport.translation.y*1000;
		Pose3D Tswing;
		Tswing.rotation = Rsupport;
		Tswing.translate(theNaoStructure->uLink[NaoStructure::rFootLink].p-theNaoStructure->uLink[NaoStructure::bodyLink].p);
		outBehaviorData.rightx = Tswing.translation.x*1000;
		outBehaviorData.righty = Tswing.translation.y*1000;
	}
	else
	{
		outBehaviorData.rightx = Tsupport.translation.x*1000;
		outBehaviorData.righty = Tsupport.translation.y*1000;
		Pose3D Tswing;
		Tswing.rotation = Rsupport;
		Tswing.translate(theNaoStructure->uLink[NaoStructure::lFootLink].p-theNaoStructure->uLink[NaoStructure::bodyLink].p);
		outBehaviorData.leftx = Tswing.translation.x*1000;
		outBehaviorData.lefty = Tswing.translation.y*1000;

	}
//	numSet = i;
//    if(numSet == OutBehaviorData::NUM_JOINT_DATA)
//        std::cout<<"Num of Data Set missmatch with NUM_JOINT_DATA: NumSet: "<<numSet<<", NUM_JOINT_DATA: "<<OutBehaviorData::NUM_JOINT_DATA<<std::endl;
//    printf("%d joint data Set to :\n", outBehaviorData.jointData.size());
//    for(i = 0; i < numSet; i++)
//    {
//        printf("the %d number is ", i);
//        std::cout<<outBehaviorData.jointData[i]<<std::endl;
//    }

}

MAKE_KS(OutBehaviorProvider)

