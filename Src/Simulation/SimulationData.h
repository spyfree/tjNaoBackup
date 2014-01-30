#ifndef SimulationData_H
#define SimulationData_H
#include <Module/Common/JointData.h>

class SimulationData
{
public:
	SimulationData();
	static SimulationData* getInstance();
	void SetJointData(int portID,double value);
	double GetJointData(int portID);
//	SensoryJointData getSensoryJointData();  //added by qiang
//	SimuNaoProvider *simuNaoProvider;
	JointCmd  theMotionJointData;
	int portID;
protected:
	static SimulationData* theSimulationData;
private:
	SensoryJointData theSensoryJointData;
	
};
#endif