#ifndef __PreviewWalk_h_
#define __PreviewWalk_h_
#include "Module/Actuator/WalkTypeBase.h"
#include "PreviewWalkParam.h"
#include "PreBufferGenerator.h"
#include "PreviewController.h"
#include "Module/Common/SmartPtr.h"
#include "Learner/LearnerAgent.h"
#include "SPController.h"
 #include <fstream>
//class MatlabNN;
class PreviewWalk : public WalkTypeBase
{
public:
	PreviewWalk();
	~PreviewWalk();
	void goNextFrame(WalkingEngineOutput& lastWalkOoutput, WalkingEngineOutput& walkingEngineOutput, const NaoStructure& naoStructure);
	void reset();
	void updateWalkCmd(const WalkCmd& newCmd);
	//void setSensorData(SafeSensorData& safeSensorData);
	//void updateWalkParam(const PreviewWalkParam& walkParam);
private:
	bool isCmdChangePossible();
	void binarySearchforYawRollPitch(const double& fRr, const double& fRl, Vector3<double>& equalAngles, double& rYawPitchAngle);
	void smoothJoints(WalkingEngineOutput& walkOutput_, const WalkingEngineOutput& lastWalkOutput_);
	void finishFrame();
	void compensateHipRoll(int istep_, int supmod_, const PreviewWalkParam& walkParam, WalkingEngineOutput& walkOutput_);
	void compensateHipRollindependent(int istep_, int supmod_, const PreviewWalkParam& walkParam, WalkingEngineOutput& walkOutput_, WalkCmd& walkcmd_);
private:
	PreviewWalkParam walkParam;
	PreBufferGenerator preBufferGenrator;
	PreviewController previewController;
	PreviewState lastState;
	WalkCmd executingCmd;
	WalkCmd pendingCmd;
	bool isCmdPending;
	int initialSlowCounter;
	NaoStructure naoStructTmp;		/**< A temporary Nao Structure Buffer, a copy of global naostructure*/
	int lastWalkStatus;
	int lastwalktype;
	Vector3<double> pRefCoMLast, vRefCoMLast; /**< Position, velocity and acceleration of Reference CoM*/
	Vector3<double> lastCoMW;
	//===
	SPController spController;
	Vector3<double> errorZMP;
	int nErrors;
	int onSeeSupMode;
	Vector3<double> sumZMPErrorTotal;
	int numZMPErrorAdded;
	bool lastUpdateModelFlag;
	LearnerAgent learnerAgent;
	int updateCounter;
	Vector3<double> sumStepModify;
	int numStepModify;
	//===NNReflectWalk===
	//MatlabNN* pMatlabNN;
	void plotJointAngles(const JointData& joints, const std::string& prefix);

};
#endif