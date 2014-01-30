#include "PreviewWalk.h"
#include "Tools/Streams/OutStreams.h"
#include "Module/Common/Kinematics.h"
#include "Module/Actuator/MotionDebugging.h"
#include "Platform/GTAssert.h"
#include "Platform/GetSetting.h"
//#include "../MatlabHelp/MatlabHelp.h"
PreviewWalk::PreviewWalk()
:isCmdPending(false), initialSlowCounter(50), nErrors(-1), onSeeSupMode(-1), 
lastUpdateModelFlag(false), numZMPErrorAdded(0), updateCounter(0),
numStepModify(0)
{
	//Load Walking Params from Config File
	std::string path = std::string(GetSetting::GetConfigDir()) +"/WalkCfg/PreviewWalkParam.cfg";
	if(!walkParam.load(path))
		walkParam.setDefaultParams();
	previewController.resetCoefficient(walkParam.PG, walkParam.Zc, walkParam.dt, walkParam.g);
	preBufferGenrator.setWalkParam(walkParam);
	SPController::Gain gainx(Vector3<double>(-0.0349, -0.0665, -0.0153),  0.0523);//(-0.0349, -0.0665, -0.0153),  0.0523);
	//(0.0238, 0.0118, 0.0105), 0.1348//(-0.0112, -0.0463, -0.0239), 0.0052)
	SPController::Gain gainy(Vector3<double>(0.0230, -0.0242, -0.0191), 0.0086); //(0.0230, -0.0242, -0.0191), 0.0086
	//(0.0101, 0.0170, -0.0490),0.0088//(0.0379, 0.0076, -0.0228),0.0274)
	spController.setGainX(gainx);
	spController.setGainY(gainy);
	learnerAgent.initPGM(spController);
}
PreviewWalk::~PreviewWalk()
{

}
static const bool Use_Smooth_Filter = true;
static const bool Open_Step_Learning = false;
static const bool Open_Step_Modify = false;//true;
void PreviewWalk::goNextFrame(WalkingEngineOutput& lastWalkOoutput, WalkingEngineOutput& walkingEngineOutput, const NaoStructure& naoStructure)
{
	lastWalkStatus = walkStatus;
	// Copy from global naoStructure, as we will temporary modify this buffer for some calculation
	naoStructTmp = naoStructure;
	//first check for cmd change
	if(isCmdPending && isCmdChangePossible() && !(lastWalkStatus==WalkTypeBase::standing&&pendingCmd.speed==Pose2D(0,0,0)))
	{
		//std::cout<<"---isCmdPending shoule be true:  "<<pendingCmd.speed.translation.x<<"  "<<pendingCmd.speed.translation.y<<"  "<<pendingCmd.speed.rotation<<std::endl;
		executingCmd = pendingCmd;
		isCmdPending = false;
		// TODO: when we switch from other walk type , we need to reset the generator
		if(!preBufferGenrator.hasInitialized() 
			|| (executingCmd.walkType != lastwalktype/*lastWalkStatus*/ && lastWalkStatus == WalkTypeBase::standing))
		{
			//std::cout<<"------previewwalk reset---"<<std::endl;
			preBufferGenrator.reset(executingCmd,naoStructTmp);
			preBufferGenrator.changeSpeed(executingCmd, naoStructTmp);
			initialSlowCounter = walkParam.PG;
			// TODO: use measured COM Vector(p, vel, acc)
			double cposx = naoStructure.pCOM.x;
			double cposy = naoStructure.pCOM.y - (naoStructure.uLink[NaoStructure::lFootLink].p.y + naoStructure.uLink[NaoStructure::rFootLink].p.y)/2 ;
			previewController.resetController(Vector3<double>(cposx,0,0), Vector3<double>(cposy,0,0));
			pRefCoMLast = naoStructure.pCOM;
			
		}else
		{
			preBufferGenrator.changeSpeed(executingCmd, naoStructTmp);
			if (lastWalkStatus == WalkTypeBase::standing)
			{
				double cposx = naoStructure.pCOM.x;
				double cposy = naoStructure.pCOM.y - (naoStructure.uLink[NaoStructure::lFootLink].p.y + naoStructure.uLink[NaoStructure::rFootLink].p.y)/2 ;
				previewController.resetController(Vector3<double>(cposx,0,0), Vector3<double>(cposy,0,0));
				pRefCoMLast = naoStructure.pCOM;
			}
			//std::cout<<"------previewwalk changeSpeed---"<<std::endl;
		}
		lastwalktype = executingCmd.walkType;
	}
	

	if(!preBufferGenrator.hasInitialized())
		return;
	//=======
	PreviewState state;
	PreviewBuffer& previewBuffer = preBufferGenrator.getPreviewBuffer();
	if(!previewBuffer.empty())
		state= previewBuffer.front();
	if(state.walkStatus == WalkTypeBase::standing){
		walkStatus = WalkTypeBase::standing;
		//std::cout<<"------------standing standing standing ! ! !---------"<<std::endl;
		walkingEngineOutput.isLeavingPossible = true;
		return;
	}
	else{
		walkingEngineOutput.isLeavingPossible = false;
	}
	int lastSupMode = state.supmod;
	// lastWalkStatus = state.walkStatus
	// preBufferGenrator go Next and generate new knots and state
	preBufferGenrator.generateNext();
	if(!previewBuffer.empty())
		state = previewBuffer.front();
	sup_mod = state.supmod;
	walkStatus = state.walkStatus;
	naoStructTmp.supportingMode = sup_mod;
	//std::cout<<"------------WalkTypeBase: "<<walkStatus<<std::endl;  //start 1; ending 2; running 3; standing 4;  
	if (walkStatus!=WalkTypeBase::ending&&lastWalkStatus==WalkTypeBase::ending)
	{
		preBufferGenrator.finishEnding();
	}
	/*if(walkStatus == WalkTypeBase::standing){
		std::cout<<"------------standing standing standing ! ! !---------"<<std::endl;
		smoothJoints(walkingEngineOutput, lastWalkOoutput);
		lastWalkOoutput = walkingEngineOutput;
		walkingEngineOutput.isLeavingPossible = true;
		preBufferGenrator.generateNext();
		return;
	}*/
	// Foot positioning modification according to zmp error and last walk status
// 	if(state.walkStatus == WalkTypeBase::running && previewBuffer[1].supmod != sup_mod &&
// 		(sup_mod == NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT || sup_mod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT) )
// 	{
// 		Vector3<double> zmpm = RotationMatrix(0, 0, lastState.rSupFootZMP.z) *pRobotState->mZMP + lastState.pSupFootZMP;
// 		Vector3<double> zmperror = state.pZMP - zmpm;
// 		preBufferGenrator.doLandingEvent(zmperror, Vector3<double>(0,0,0));
// 	}
	// TODO: If we use Sensor to detect supporting mode, the code bellow should be modified!!
	// When Supporting Leg Changed, modify pCOM to value relative to current supporting Foot
	if(lastSupMode != sup_mod &&
		((sup_mod == NaoConfig::SUPPORT_MODE_LEFT || sup_mod == NaoConfig::SUPPORT_MODE_RIGHT) ||
		(lastWalkStatus != walkStatus && lastWalkStatus == WalkTypeBase::standing)))
	{ 
		naoStructTmp.uLink[NaoStructure::bodyLink].p = Vector3<double>(0,0,0);
		naoStructTmp.uLink[NaoStructure::bodyLink].R = RotationMatrix();
		Kinematics::execForwardKinematics(naoStructTmp, NaoStructure::bodyLink);
		Kinematics::recalcBodyPos2SupFoot(naoStructTmp, Vector3<double>(0,0,0), naoStructTmp.supportingMode);
		Kinematics::execForwardKinematics(naoStructTmp, NaoStructure::bodyLink);
		naoStructTmp.pCOM = Kinematics::calcCOM(naoStructTmp);
		//In case we get NAN or IND num.... print some thing and halt execution when run on robot!!
		ASSERT(naoStructTmp.pCOM == naoStructTmp.pCOM);
	}
	//===== Check Landing Step Error=============================
	if(false && state.walkStatus == WalkTypeBase::running && lastSupMode != sup_mod &&
		(sup_mod == NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT || sup_mod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT) )
	{
		Vector3<double> pRealLandFootW,rRealLandFootW;
		Vector3<double> pRealLandFoot2Sup;
		Vector3<double> pRefLandFootW, rRefLandFootW;
		const PreBufferGenerator::Knot* pKnot = 0;
		int landFoot = 0;
		if(sup_mod == NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT)
			landFoot = NaoStructure::lFootLink;
		else if(sup_mod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
			landFoot = NaoStructure::rFootLink;

		pRealLandFoot2Sup = naoStructTmp.getPosition(landFoot, NaoStructure::Space_Supprot_Leg);
		pRealLandFoot2Sup.z = 0;
		pRealLandFootW = state.pSupFootZMP + RotationMatrix::getRotationZ(state.rSupFootZMP.z) * pRealLandFoot2Sup;
		rRealLandFootW = state.rSupFootZMP;
		rRealLandFootW.z += naoStructTmp.getRotAngles(landFoot, NaoStructure::Space_Supprot_Leg).z;

		pKnot = preBufferGenrator.findNextKnot();	
		ASSERT(pKnot);
		pRefLandFootW = pKnot->pSupFootZMP;
		rRefLandFootW = pKnot->rSupFootZMP;
		Vector3<double> pError = (pRefLandFootW - pRealLandFootW), rError(rRefLandFootW - rRealLandFootW);
		preBufferGenrator.doLandingEvent(pError, rError);
		// TODO: check here?
		preBufferGenrator.generateNext();
		if(!previewBuffer.empty())
			state = previewBuffer.front();
		sup_mod = state.supmod;
		walkStatus = state.walkStatus;
		naoStructTmp.supportingMode = sup_mod;
	}
	// ==== Get Swing Foot Infomation
	int iswgleg = NaoConfig::LegIDLeft;
	if(sup_mod == NaoConfig::SUPPORT_MODE_LEFT || sup_mod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
		iswgleg= NaoConfig::LegIDRight;
	Vector3<double> pSwgFootW, rSwgFootW;
	preBufferGenrator.getCurrentSwgF(pSwgFootW, rSwgFootW, iswgleg);
	pSwgFootW += Vector3<double>(0,0, NaoConfig::getInstance()->FootHeight);// TODO: muliply foot rotation
	Vector3<double> pSwgFoot2Sup, rSwgFoot2Sup;
	pSwgFoot2Sup = RotationMatrix::getRotationZ(-state.rSupFootZMP.z) * (pSwgFootW - state.pSupFootZMP);
	rSwgFoot2Sup = rSwgFootW - state.rSupFootZMP;
	// ====== CoM Control=====================
	double bRoll = pSensorData->data[SensorData::angleX];
	double bTilt = pSensorData->data[SensorData::angleY];
	RotationMatrix Rbw = RotationMatrix().rotateX(bRoll).rotateY(bTilt);
	RotationMatrix Rfw;//rotation of supporting foot in world cooridnate
	if(naoStructTmp.supportingMode == NaoConfig::SUPPORT_MODE_LEFT || naoStructTmp.supportingMode == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		Rfw = naoStructTmp.uLink[NaoStructure::lFootLink].R * naoStructTmp.uLink[NaoStructure::bodyLink].R.invert();
	}else
	{
		Rfw =  naoStructTmp.uLink[NaoStructure::rFootLink].R * naoStructTmp.uLink[NaoStructure::bodyLink].R.invert();
	}
	Rfw = RotationMatrix::getRotationZ(state.rSupFootZMP.z) * Rfw * Rbw;
		// check world com state
	Vector3<double> comW = Rfw * naoStructure.getCoM(NaoStructure::Space_Supprot_Leg)+ lastState.pSupFootZMP;
	Vector3<double> comWv =  (comW - lastCoMW) / walkParam.dt;
	Vector3<double> comWa = Rbw * Vector3<double>(pSensorData->data[SensorData::accX], pSensorData->data[SensorData::accY], pSensorData->data[SensorData::accZ]);
	PrviewControllerState inputState;
	inputState.p = comW; inputState.v = comWv; inputState.a = comWa;
	Vector3<double> zmpW = RotationMatrix(0, 0, lastState.rSupFootZMP.z) *pRobotState->mZMP + lastState.pSupFootZMP;
	//=======Compute step modification according to  ZMP error and CoM information=========
	Vector3<double> ezmp =pRobotState->mZMP - RotationMatrix(0, 0, -state.rSupFootZMP.z)* (state.pZMP - state.pSupFootZMP);// state.pZMP - zmpW;
	if(state.supmod == NaoConfig::SUPPORT_MODE_RIGHT || state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT)
			ezmp.y = - ezmp.y;
	//======Learning Step Modification Process=====
	if(Open_Step_Learning)
	{
		if(state.walkStatus == WalkTypeBase::running){
			sumZMPErrorTotal += Vector3<double>(fabs(ezmp.x), fabs(ezmp.y), fabs(ezmp.z));
			numZMPErrorAdded ++;
			updateCounter ++;
		}
		if((lastUpdateModelFlag != pRobotState->fallState.updateModel && pRobotState->fallState.updateModel == true) 
			|| (state.walkStatus == WalkTypeBase::running && updateCounter%600 == 0))
		{
			double critia1 = (sumZMPErrorTotal.x * 0.3+ sumZMPErrorTotal.y * 0.7)/ numZMPErrorAdded;//if we fall, the zmp error will also be significant
			double performance = critia1;
			double critia2 =  (pi - atan(static_cast<double>(numZMPErrorAdded) * 2 / walkParam.PG -3)) /2;
			performance = performance + performance * critia2;//walk longer, we walk better
			double critia3 = 0;
			if(numStepModify > 0)
				critia3 = (sumStepModify.x / numStepModify * 0.5 + sumStepModify.y / numStepModify) * 0.5;
			performance += critia3;
			learnerAgent.updatePerform(performance);
			learnerAgent.getPendingParam(spController);
			//std::cout<<"===>LearnCritia: ("<<numZMPErrorAdded<<", "<<sumZMPErrorTotal.x<<", "<<sumZMPErrorTotal.y<<"), "<<critia1<<", "<<critia2<<", "<<critia3<<std::endl;
			sumZMPErrorTotal = Vector3<double>(0,0,0);
			numZMPErrorAdded  = 0;
			updateCounter = 1;
			sumStepModify = Vector3<double>(0,0,0);
			numStepModify = 0;
		}
		lastUpdateModelFlag = pRobotState->fallState.updateModel;
	}
	//============
	if(Open_Step_Modify || Open_Step_Learning)
	{
		if(lastSupMode != sup_mod && sup_mod == NaoConfig::SUPPORT_MODE_LEFT)
		{
			errorZMP.x  = 0; errorZMP.y = 0;nErrors = 0;onSeeSupMode = sup_mod;
			spController.resetZMPError();
		}else if(lastSupMode != sup_mod && sup_mod == NaoConfig::SUPPORT_MODE_RIGHT)
		{
			errorZMP.x  = 0; errorZMP.y = 0;nErrors = 0;onSeeSupMode = sup_mod;
			spController.resetZMPError();
		}
		if(state.walkStatus == WalkTypeBase::running && sup_mod == onSeeSupMode && nErrors < 10)
		{
			errorZMP += ezmp;
			nErrors++;
			spController.addZMPError(ezmp);
		}
		if(sup_mod == onSeeSupMode && nErrors == 10)
		{
			nErrors++;
			//============switch to local system========
			RotationMatrix R = RotationMatrix::getRotationZ(-state.rSupFootZMP.z) * Rfw;
			Vector3<double> p = R * naoStructure.getCoM(NaoStructure::Space_Supprot_Leg);
			Vector3<double> v = RotationMatrix::getRotationZ(-state.rSupFootZMP.z)  * inputState.v;
			Vector3<double> a = comWa;
			if(state.supmod == NaoConfig::SUPPORT_MODE_RIGHT || state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT)
			{
				p.y = - p.y; v.y = -v.y; a.y = -a.y;
			}
			Vector3<double> xState(p.x, v.x, a.x), yState(p.y, v.y, a.y);
			Vector3<double> dP = spController.getOutput(xState, yState);
			// Become effect in next frame....
			preBufferGenrator.modifyStepPosition(Vector3<double>(dP.x, dP.y, 0), Vector3<double>(0,0,0), 16);
			sumStepModify.x += dP.x * dP.x;
			sumStepModify.y += dP.y * dP.y;
			numStepModify ++;
			//MATLABHELP_PLOT(dPy, "dPy");
			//MATLABHELP_PLOT(spController.getSumZMPError(), "mErrrory");
		}
	}

	//=== Get Preview Controller Output============
	PrviewControllerOutput outputPC ;
// 	if(walkStatus != WalkTypeBase::starting && (sup_mod == 1 || sup_mod == 3))
// 		outputPC = previewController.getRefCOM(previewBuffer, walkParam, inputState);
// 	else
		outputPC = previewController.getRefCOM(previewBuffer, walkParam);
	Vector3<double> pCOMRefW(outputPC.p); 
	// CoM Ref world system to body system
	Vector3<double> pCOMRef =RotationMatrix::getRotationZ(-state.rSupFootZMP.z) * (pCOMRefW - state.pSupFootZMP);
	//Vector3<double> pCOMRef =Rfw.invert() * (pCOMRefW - state.pSupFootZMP);
	Vector3<double> realCOM = naoStructTmp.pCOM;
	Vector3<double> dCOM =  (pCOMRef - realCOM);
// 	MATLABHELP_PLOT(dCOM.y, "dCOMy");
// 	MATLABHELP_PLOT(pCOMRef.y, "pCOMRefy");
	//====Add ZMP mesaured feedback
// 	Vector3<double> zmpError = (lastState.pSupFootZMP - pRobotState->mZMP);
// 	dCOM -= Vector3<double>(zmpError.x * 0.01, zmpError.y * 0.1, 0);

	Vector3<double> refRBody(0,walkParam.torsoAngleY,0);
/*	refRBody +=  Vector3<double>(zmpError.y, zmpError.x * 0.1, 0);*/
	double gain = 1;//1.2, 1.8
	if(sup_mod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT || sup_mod == NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT)
		gain = 1;
	Vector3<double> pBodyNext = dCOM *gain+ naoStructTmp.getPosition(NaoStructure::bodyLink, NaoStructure::Space_Supprot_Leg);

	double fRr, fRl;//z rotation of right and left foot
	if(sup_mod == NaoConfig::SUPPORT_MODE_LEFT || sup_mod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT){
		fRr = rSwgFoot2Sup.z;
		fRl = 0;
	}else{
		fRr = 0;
		fRl = rSwgFoot2Sup.z;
	}
	Vector3<double> equalHipAngles;
	double rHipYawPitchAngle;
	//search for best rHipYawPitchAngle, Feed angle relative to body...
	binarySearchforYawRollPitch(fRr, fRl, equalHipAngles, rHipYawPitchAngle);
	RotationMatrix brz  = RotationMatrix::getRotationY(walkParam.torsoAngleY);
	double zmpErrorY = pRobotState->mZMP.y;
	//brz.rotateX(-0.05*mRBody.x).rotateY(-0.05*mRBody.y);
	if(state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT || state.supmod == NaoConfig::SUPPORT_MODE_LEFT)
	{
		brz.rotateZ(equalHipAngles.z);
		Kinematics::calculateSafeLegJoints(walkingEngineOutput, NaoConfig::LegIDLeft, Vector3<double>(0,0,NaoConfig::getInstance()->FootHeight), RotationMatrix(), pBodyNext, brz,
			rHipYawPitchAngle, equalHipAngles.x, equalHipAngles.y);
	}else// if(state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT)
	{
		brz.rotateZ(-equalHipAngles.z);
		Kinematics::calculateSafeLegJoints(walkingEngineOutput, NaoConfig::LegIDRight, Vector3<double>(0,0,NaoConfig::getInstance()->FootHeight), RotationMatrix(), pBodyNext, brz,
			rHipYawPitchAngle, equalHipAngles.x, equalHipAngles.y);
	}
	//======为消除身体的运动给游脚带来的影响, 先重新计算下Nao Structure
	naoStructTmp.setLinksJoints(walkingEngineOutput);
	naoStructTmp.uLink[NaoStructure::bodyLink].p = Vector3<double>(0,0,0);
	naoStructTmp.uLink[NaoStructure::bodyLink].R = RotationMatrix();
	Kinematics::execForwardKinematics(naoStructTmp, NaoStructure::bodyLink);
	Kinematics::recalcBodyPos2SupFoot(naoStructTmp, Vector3<double>(0,0,0), naoStructTmp.supportingMode);
	Kinematics::execForwardKinematics(naoStructTmp, NaoStructure::bodyLink);
	naoStructTmp.pCOM = Kinematics::calcCOM(naoStructTmp);
	//In case we get NAN or IND num.... print some thing and halt execution when run on robot!!
	ASSERT(naoStructTmp.pCOM == naoStructTmp.pCOM);
	//====== Swing Foot Joints================

	double supRotNowZ = 0;//TODO: error
	Vector3<double> psfoot = pSwgFoot2Sup;
	Vector3<double> psupfoot = state.pSupFootZMP;
	RotationMatrix frz = RotationMatrix::getRotationZ(rSwgFoot2Sup.z - supRotNowZ);
	Vector3<double> bodyAnglesNow = naoStructTmp.getRotAngles(NaoStructure::bodyLink, NaoStructure::Space_Supprot_Leg);
	if(state.supmod == NaoConfig::SUPPORT_MODE_LEFT || state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		Kinematics::calculateSafeLegJoints(walkingEngineOutput, NaoConfig::LegIDRight, psfoot, frz, naoStructTmp.uLink[NaoStructure::bodyLink].p, naoStructTmp.uLink[NaoStructure::bodyLink].R,
			rHipYawPitchAngle, equalHipAngles.x, equalHipAngles.y);
	}else{
		Kinematics::calculateSafeLegJoints(walkingEngineOutput, NaoConfig::LegIDLeft, psfoot, frz, naoStructTmp.uLink[NaoStructure::bodyLink].p, naoStructTmp.uLink[NaoStructure::bodyLink].R,
			rHipYawPitchAngle, equalHipAngles.x, equalHipAngles.y);
	}
	//===========
//	plotJointAngles(walkingEngineOutput, "O1_");
// 	if(state.supmod == NaoConfig::SUPPORT_MODE_LEFT && abs(zmpErrorY) > 0.01 && abs(zmpErrorY) < 0.025)
// 		walkingEngineOutput.angles[JointData::LAnkleRoll] += zmpErrorY * deg2rad(200);
// 	else if(state.supmod == NaoConfig::SUPPORT_MODE_RIGHT && abs(zmpErrorY) > 0.01 && abs(zmpErrorY) < 0.025)
// 		walkingEngineOutput.angles[JointData::RAnkleRoll] -= zmpErrorY * deg2rad(200);
//	plotJointAngles(walkingEngineOutput, "O2_");
	//=========LPF smoother=================
	if(Use_Smooth_Filter)
		smoothJoints(walkingEngineOutput, lastWalkOoutput);
	lastWalkOoutput = walkingEngineOutput;
	//===Compensate HipRoll When Single Supproting====//
	if(walkParam.useHipRollCompensation && (executingCmd.speed.translation.x != 0.0) && (walkStatus == WalkTypeBase::running ))
	{		
		compensateHipRoll(istep, sup_mod, walkParam,walkingEngineOutput);
	}
	if((walkParam.useHipRollCompensationRight&&(executingCmd.speed.translation.y < 0.0)||walkParam.useHipRollCompensationLeft&&(executingCmd.speed.translation.y > 0.0)) && (walkStatus == WalkTypeBase::running ))
	{	
		compensateHipRollindependent(istep, sup_mod, walkParam,walkingEngineOutput,executingCmd);
	}
	finishFrame();     
	//======Print something=========
	Vector3<double> pBodyRefW = RotationMatrix(state.rSupFootZMP.x, state.rSupFootZMP.y, state.rSupFootZMP.z) *pBodyNext + state.pSupFootZMP;

	// TODO: Check direction of com and acc
	//Vector3<double> comW = Rfw * naoStructure.getCoM(NaoStructure::Space_Supprot_Leg)+ lastState.pSupFootZMP;
	Vector3<double> comWSup = naoStructure.getCoM(NaoStructure::Space_Supprot_Leg) + lastState.pSupFootZMP;
	/*MATLABHELP_PLOT(pCOMRefW.x, "pCOMRefWx");
	MATLABHELP_PLOT(pCOMRefW.y, "pCOMRefWy");
	MATLABHELP_PLOT(pCOMRefW.z, "pCOMRefWz");*/
	//MATLABHELP_PLOT(outputPC.v.x, "CoMRefWvx");
	//MATLABHELP_PLOT(outputPC.v.y, "CoMRefWvy");
	//MATLABHELP_PLOT(outputPC.v.z, "CoMRefWvz");
	//MATLABHELP_PLOT(outputPC.a.x, "CoMRefWax");
	//MATLABHELP_PLOT(outputPC.a.y, "CoMRefWay");
	//MATLABHELP_PLOT(outputPC.a.z, "CoMRefWaz");

	//MATLABHELP_PLOT(pBodyRefW.x, "pBodyWx");
	//MATLABHELP_PLOT(pBodyRefW.y, "pBodyWy");
	//MATLABHELP_PLOT(pBodyRefW.z, "pBodyWz");
	/*MATLABHELP_PLOT(comW.x, "comWx");
	MATLABHELP_PLOT(comW.y, "comWy");
	MATLABHELP_PLOT(comW.z, "comWz");*/
	//MATLABHELP_PLOT(comWSup.x, "comWSupx");
	//MATLABHELP_PLOT(comWSup.y, "comWSupy");


	//MATLABHELP_PLOT(rSwgFoot2Sup.z, "swg2Foot");

	//MATLABHELP_PLOT(pSwgFootW.x, "pSwgFootx");
	//MATLABHELP_PLOT(pSwgFootW.y, "pSwgFooty");
	//MATLABHELP_PLOT(pSwgFootW.z, "pSwgFootz");
	//MATLABHELP_PLOT(rSwgFootW.x, "rSwgFootx");
	//MATLABHELP_PLOT(rSwgFootW.y, "rSwgFooty");
	//MATLABHELP_PLOT(rSwgFootW.z, "rSwgFootz");

	//Vector3<double> lfoot2B = naoStructTmp.getPosition(NaoStructure::lFootLink, NaoStructure::Space_Body);
	////Vector3<double> rfoot2B = naoStructTmp.getPosition(NaoStructure::rFootLink, NaoStructure::Space_Body);
	//MATLABHELP_PLOT(lfoot2B.x, "lfoot2Bx");
	//MATLABHELP_PLOT(lfoot2B.y, "lfoot2By");
	//MATLABHELP_PLOT(lfoot2B.z, "lfoot2Bz");

	//ZMP measured from LIPM
	/*MATLABHELP_PLOT(zmpW.x, "zmpWx");
	MATLABHELP_PLOT(zmpW.y, "zmpWy");
	MATLABHELP_PLOT(zmpW.z, "zmpWz");*/

	//// zmp Ideal
	//Vector3<double> zmpWIdeal = outputPC.p -  outputPC.a * pCOMRefW.z / 9.81;
	//MATLABHELP_PLOT(zmpWIdeal.x, "zmpWIdealx");
	//MATLABHELP_PLOT(zmpWIdeal.y, "zmpWIdealy");
	//cop
	MDEBUG_SAVE(MDebug::idPreviewState,state)
	MDEBUG_SAVE(MDebug::idRefCOM, pCOMRefW)
    MDEBUG_SAVE(MDebug::idRealCOM, comW)
    MDEBUG_SAVE(MDebug::idBodyPos, pBodyRefW)
    //MDEBUG_SAVE(MDebug::idBodyAngles, )

	Vector3<double> copW;
	const CoP& LCoP = pRobotState->mCoPL;
	const CoP& RCoP = pRobotState->mCoPR;

	if(naoStructure.supportingMode == NaoConfig::SUPPORT_MODE_LEFT || naoStructure.supportingMode == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT){
		copW =  LCoP.p* LCoP.pressure + (naoStructure.getRotationMatrix(NaoStructure::rFootLink) * RCoP.p 
			+naoStructTmp.getPosition(NaoStructure::rFootLink, NaoStructure::Space_Supprot_Leg)) * RCoP.pressure;
		copW /= (LCoP.pressure + RCoP.pressure);
	}else{
		copW =  RCoP.p* RCoP.pressure + (naoStructure.getRotationMatrix(NaoStructure::lFootLink) * LCoP.p 
			+naoStructTmp.getPosition(NaoStructure::lFootLink, NaoStructure::Space_Supprot_Leg)) * LCoP.pressure;
		copW /= (LCoP.pressure + RCoP.pressure);
	}
	copW = RotationMatrix(0, 0, lastState.rSupFootZMP.z) *copW + lastState.pSupFootZMP;
	/*MATLABHELP_PLOT(copW.x, "copWx");
	MATLABHELP_PLOT(copW.y, "copWy");
	MATLABHELP_PLOT(copW.z, "copWz");

	MATLABHELP_PLOT(pRobotState->groundContactLeft.contact?0.75:0.25, "isContactLeft");
	MATLABHELP_PLOT(pRobotState->groundContactRight.contact?0.75:0.25, "isContactRight");*/
	lastState = state;
	lastCoMW = comW;
}
void PreviewWalk::reset()
{

}
void PreviewWalk::updateWalkCmd(const WalkCmd& newCmd)
{
	pendingCmd = newCmd;
	isCmdPending = true;
}

bool PreviewWalk::isCmdChangePossible()
{
	bool possible = true;
	if(preBufferGenrator.isPreparetoStop())
	{
		return false;
	}
	//bool possible = false;
	//std::cout<<"Preview Walk Checking New Walk Cmd... ";
	//
	//if(previewBuffer.empty())	//preview buffer is empty(), we can change speed and reset the buffer
	//{
	//	possible =  true;
	//}else
	//{
	//	PreviewState& state =previewBuffer.back();
	//	if(state.walkStatus == WalkTypeBase::running && (state.istep == 0 || state.istep == walkParam.PG/2))
	//	{
	//		possible =  true;
	//	}else if(state.walkStatus == WalkTypeBase::standing)
	//	{
	//		possible = true;
	//	}
	//}
	//if(possible)
	//	std::cout<<" CHANGE POSSIBLE!"<<std::endl;
	//else
	//	std::cout<<" NO CHANGE!"<<std::endl;

	return possible;
}

void PreviewWalk::finishFrame()
{
    
}

static const double MAX_RHIPYAWPITCH = deg2rad(45);
static const double MIN_RHIPYAWPITCH = deg2rad(-45);
static const double STOP_DELTA = deg2rad(0.2);
static const double STOP_STEPS = 20;
void PreviewWalk::binarySearchforYawRollPitch(const double& fRr, const double& fRl, Vector3<double>& equalAngles, double& rYawPitchAngle)
{
	double Bup = MAX_RHIPYAWPITCH, Bdown = MIN_RHIPYAWPITCH;
	double rYawPitch = 0, delta = 10000;
	int steps = 0;
	RotationMatrix Rr, Rl;
	Vector3<double> ar(0, -1, 1), al(0,1,-1);
	double RAnglesZ, LAnglesZ;
	double t1, t2;
	while(true){
		steps++;
		Rr = Kinematics::rodrigues(ar, rYawPitch);
		Rl = Kinematics::rodrigues(al, rYawPitch);
		RAnglesZ = Rr.getZAngle();
		LAnglesZ = Rl.getZAngle();
		t1 = (RAnglesZ- LAnglesZ);
		t2 = (fRr - fRl);
		delta = fabs(t1-t2);
		//printf("Got target angle: %.4f, delta: %.6f, steps cost: %d\n", rad2deg(rYawPitch), rad2deg(delta), steps);
		if(delta < STOP_DELTA || steps > STOP_STEPS ||
			(rYawPitch - MIN_RHIPYAWPITCH) <= deg2rad(1) || (MAX_RHIPYAWPITCH - rYawPitch) <= deg2rad(1))
			break;
		else{
			if( t1 < t2){
				Bdown = rYawPitch;
				Bup = Bup;
				rYawPitch = (Bdown + Bup)/2;
			}else{
				Bdown = Bdown;
				Bup = rYawPitch;
				rYawPitch = (Bdown + Bup)/2;
			}
		}
	}
	equalAngles =  Rr.getXYZAngles();
	equalAngles.y = - equalAngles.y;
	equalAngles.x = -equalAngles.x;
	rYawPitchAngle = rYawPitch;
}

void PreviewWalk::smoothJoints(WalkingEngineOutput& walkOutput_, const WalkingEngineOutput& lastWalkOutput_)
{
	double a = 0.2;
	//for(int i = JointData::RHipYawPitch; i < JointData::numOfJoints; ++i)
	for(int i = JointData::HeadYaw; i < JointData::numOfJoints; ++i)
	{
		walkOutput_.angles[i] = a * walkOutput_.angles[i] + (1-a) * lastWalkOutput_.angles[i];
	}
}

void PreviewWalk::plotJointAngles(const JointData& joints, const std::string& prefix)
{
	std::string name;
	for(int i = 1; i < JointData::numOfJoints; ++i)
	{
		name = prefix + joints.getJointName(JointData::Joint(i));
		//MATLABHELP_PLOT(joints.angles[i], name);
	}
}

void PreviewWalk::compensateHipRoll(int istep_, int supmod_, const PreviewWalkParam& walkParam, WalkingEngineOutput& walkOutput_)
{
	int stageLen = 0;
	int stageiStep = 0;
	double ratio = 0;
	double compensator = 0;
	int index = 0;
	if(supmod_ == NaoConfig::SUPPORT_MODE_RIGHT)
	{
		stageiStep = istep_ - static_cast<int>((walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT] + walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT])* walkParam.PG+0.5);
		stageLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_RIGHT] * walkParam.PG+0.5);
		ratio = (double)stageiStep / stageLen;
		compensator = walkParam.pRHipRollCompensator;
		index = JointData::RHipRoll;
	}else if(supmod_ == NaoConfig::SUPPORT_MODE_LEFT)
	{
		stageiStep = istep_;
		stageLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5);
		ratio = (double)stageiStep / stageLen;
		compensator = walkParam.pLHipRollCompensator;
		index = JointData::LHipRoll;
	}else{
		return;//No Compensation at Double Support Stage
	}
	
	double angle = compensator * sin(ratio * pi);
	angle = CLIP(angle,fabs(compensator) * 0.7);
	walkOutput_.angles[index] += angle;
}
void PreviewWalk::compensateHipRollindependent(int istep_, int supmod_, const PreviewWalkParam& walkParam, WalkingEngineOutput& walkOutput_, WalkCmd& walkcmd_)
{
	int stageLen = 0;
	int stageiStep = 0;
	double ratio = 0;
	double compensator = 0;
	int index = 0;
	if(supmod_ == NaoConfig::SUPPORT_MODE_RIGHT)
	{
		stageiStep = istep_ - static_cast<int>((walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT] + walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT])* walkParam.PG+0.5);
		stageLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_RIGHT] * walkParam.PG+0.5);
		ratio = (double)stageiStep / stageLen;
		if(walkcmd_.speed.translation.y < 0.0 /*|| walkcmd_.speed.rotation < 0.0*/)
		    compensator = walkParam.pRHipRollCompensatorRight;
		else if(walkcmd_.speed.translation.y > 0.0 /*|| walkcmd_.speed.rotation > 0*/)
		    compensator = walkParam.pRHipRollCompensatorLeft;
		index = JointData::RHipRoll;
	}else if(supmod_ == NaoConfig::SUPPORT_MODE_LEFT)
	{
		stageiStep = istep_;
		stageLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5);
		ratio = (double)stageiStep / stageLen;
		if(walkcmd_.speed.translation.y < 0.0 /*|| walkcmd_.speed.rotation < 0.0*/)
			compensator = walkParam.pLHipRollCompensatorRight;
		else if(walkcmd_.speed.translation.y > 0.0 /*|| walkcmd_.speed.rotation > 0.0*/)
			compensator = walkParam.pLHipRollCompensatorLeft;
		index = JointData::LHipRoll;
	}else{
		return;//No Compensation at Double Support Stage
	}

	double angle = compensator * sin(ratio * pi);
	angle = CLIP(angle,fabs(compensator) * 0.7);
	walkOutput_.angles[index] += angle;
}
