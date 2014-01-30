#ifndef __PreviewWalk_h_
#define __PreviewWalk_h_
#include "Module/Actuator/WalkTypeBase.h"
#include "PreviewWalkParam.h"
#include "PreBufferGenerator.h"
#include "PreviewController.h"
#include "Module/Common/SmartPtr.h"
 #include <fstream>
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
	void fillPreviewBuffer();
	void swingArms(int istep_, const PreviewWalkParam& walkParm_, int walkStaus_, WalkingEngineOutput& walkOutput_);
	void compensateHipRoll(int istep_, int supmod_, int walkStatus_, int miniStatus_, const PreviewWalkParam& walkParam, WalkingEngineOutput& walkOutput_);
	void binarySearchforYawRollPitch(const double& fRr, const double& fRl, Vector3<double>& equalAngles, double& rYawPitchAngle);
	void smoothJoints(WalkingEngineOutput& walkOutput_, const WalkingEngineOutput& lastWalkOutput_);
	void finishFrame();
private:
	PreviewWalkParam walkParam;
	PreBufferGenerator preBufferGenrator;
	PreviewController previewController;
	PreviewBuffer previewBuffer;
	WalkCmd executingCmd;
	WalkCmd pendingCmd;
	bool isCmdPending;
	int initialSlowCounter;
	NaoStructure naoStructTmp;		/**< A temporary Nao Structure Buffer, a copy of global naostructure*/
	int lastWalkStatus;
	Vector3<double> pRefCoMLast, vRefCoMLast; /**< Position, velocity and acceleration of Reference CoM*/
	//=====Debug related====
 	std::ofstream fout;
	std::ofstream foutJoints;

};
#endif