#include "SimulationData.h"
SimulationData *SimulationData::theSimulationData=0;
SimulationData::SimulationData()
{
	theSimulationData=this;
	for(int i=0;i<JointData::numOfJoints;i++)
	{
		//theSensoryJointData->angles[i] = JointData::ignore;
		theMotionJointData.angles[i] = JointData::ignore;
	}
}
SimulationData*SimulationData::getInstance()
{

if(!theSimulationData)
{
	theSimulationData = new SimulationData();
}
return theSimulationData;
}
void SimulationData::SetJointData(int portID_, double value)
{
	theMotionJointData.angles[portID_]=value;
	portID = portID_;
}
double SimulationData::GetJointData(int portID)
{
	return theMotionJointData.angles[portID];
}
