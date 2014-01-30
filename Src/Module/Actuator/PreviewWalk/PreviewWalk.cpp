#include "PreviewWalk.h"
#include "Tools/Streams/OutStreams.h"
#include "Module/Common/Kinematics.h"
#include "Module/Actuator/MotionDebugging.h"
#include "Platform/GTAssert.h"
#include "Platform/GetSetting.h"
static const double jStandingDegs[] = {0, 0,
														100, -30, 90, 90,
														100, 30, -90, -90,
														0, 0, -50, 77, -32, 0,
														0, 0, -50, 77, -32, 0};
PreviewWalk::PreviewWalk()
:isCmdPending(false), initialSlowCounter(50), fout("out.txt"), foutJoints("outJoints.txt")
{
	//Load Walking Params from Config File
	std::string path = std::string(GetSetting::GetConfigDir()) +"/WalkCfg/PreviewWalkParam.cfg";
	if(!walkParam.load(path))
		walkParam.setDefaultParams();
	previewController.resetCoefficient(walkParam.PG, walkParam.Zc, walkParam.dt, walkParam.g);
	previewBuffer.set_capacity(previewController.getNumPreviewFrames());
	// TODO: Pre fill the buffer with points of standing state
}
PreviewWalk::~PreviewWalk()
{
	previewBuffer.clear();
	if(fout.is_open())
		fout.close();
	if(foutJoints.is_open())
		foutJoints.close();
}
#define NEW_HIPYAWPITCH_COMENSATION
static const bool Use_Smooth_Filter = true;
void PreviewWalk::goNextFrame(WalkingEngineOutput& lastWalkOoutput, WalkingEngineOutput& walkingEngineOutput, const NaoStructure& naoStructure)
{
	lastWalkStatus = walkStatus;
	// Copy from global naoStructure, as we will temporary modify this buffer for some calculation
	naoStructTmp = naoStructure;
	//first check for cmd change
	if(isCmdPending && isCmdChangePossible())
	{
		executingCmd = pendingCmd;
		isCmdPending = false;
		// TODO: when we switch from other walk type , we need to reset the generator
		if(!preBufferGenrator.hasInitialized() || previewBuffer.empty()
			/*|| previewBuffer.front().walkStatus == WalkTypeBase::standing*/)
		{
			preBufferGenrator.reset(executingCmd, walkParam, naoStructTmp);
			initialSlowCounter = walkParam.PG;
			// TODO: use measured COM Vector(p, vel, acc)
			double cposx = naoStructure.pCOM.x;
			double cposy = naoStructure.pCOM.y - (naoStructure.uLink[NaoStructure::lFootLink].p.y + naoStructure.uLink[NaoStructure::rFootLink].p.y)/2 ;
			previewController.resetController(Vector3<double>(cposx,0,0), Vector3<double>(cposy,0,0));
			// TODO: Consider: maybe the ending state(standing) buffer of last walk can be used as the starting buffer of the new walk!!!
			previewBuffer.clear();
			pRefCoMLast = naoStructure.pCOM;
		}else
		{
			preBufferGenrator.changeSpeed(executingCmd);
		}
		std::cout<<"New Walk Cmd Comes: speed: "<<executingCmd.speed.translation.x<<", "<<executingCmd.speed.translation.y<<std::endl;
	}
	//if PreBufferGenerator not initialized, do nothing!! This seems silly!!
	if(!preBufferGenrator.hasInitialized())
		return;

	//====Starting Preview Control Walk=======//
	fillPreviewBuffer();
	PreviewState state;
	if(!previewBuffer.empty())
		state= previewBuffer.front();
	//======
	int lastSupMode = sup_mod;
	int miniStatus = state.miniStatus;
	// Assign important data !!
	istep = state.istep;
	walkStatus = state.walkStatus;
	sup_mod = state.supmod;
	naoStructTmp.supportingMode = sup_mod;
	//=======Do Standing....============//
	if(walkStatus == WalkTypeBase::standing){
		/*if(lastWalkStatus != walkStatus){
			std::cout<<"Change from walk to stand, reset BufferGenerator"<<std::endl;
			preBufferGenrator.reset(executingCmd, walkParam, naoStructTmp);
			initialSlowCounter = walkParam.PG;
			// TODO: use measured COM Vector(p, vel, acc)
			double cposx = naoStructure.pCOM.x;
			double cposy = naoStructure.pCOM.y - (naoStructure.uLink[NaoStructure::lFootLink].p.y + naoStructure.uLink[NaoStructure::rFootLink].p.y)/2 ;
			previewController.resetController(Vector3<double>(cposx,0,0), Vector3<double>(cposy,0,0));
			// TODO: Consider: maybe the ending state(standing) buffer of last walk can be used as the starting buffer of the new walk!!!
			previewBuffer.clear();
			fillPreviewBuffer();
		}*/
		/*for(int i = JointData::RHipYawPitch; i  < JointData::numOfJoints; ++i){
			walkingEngineOutput.angles[i] = deg2rad(jStandingDegs[i-1]);
		}*/
		smoothJoints(walkingEngineOutput, lastWalkOoutput);
		lastWalkOoutput = walkingEngineOutput;
		walkingEngineOutput.isLeavingPossible = true;
		//===Debug print something
		MDEBUG_SAVE(MDebug::idPreviewState, state)
		//MDEBUG_SAVE(MDebug::idRefCOM, refCOMW)
		//MDEBUG_SAVE(MDebug::idRealCOM, (RotationMatrix::getRotationZ(supRotNowZ) * recCOM+psupfoot))
		//MDEBUG_SAVE(MDebug::idBodyPos,(RotationMatrix::getRotationZ(supRotNowZ) * pBodyNext+psupfoot));
		MDEBUG_SAVE(MDebug::idBodyAngles, naoStructure.uLink[NaoStructure::bodyLink].R.getXYZAngles());

		finishFrame();
		return;
	}
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
	Vector3<double> realCOM = naoStructTmp.pCOM;
	Vector3<double> recCOM = realCOM;
	// TODO: do Preview Controller
	Vector3<double> refCOM = previewController.getRefCOM(previewBuffer, walkParam);

	//===Debug print something
	Vector3<double> refCOMW = refCOM;
	if(state.supmod == NaoConfig::SUPPORT_MODE_LEFT || state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
		refCOMW = refCOMW - Vector3<double>(walkParam.ZmpOffsetLeft.x, walkParam.ZmpOffsetLeft.y, 0);
	else
		refCOMW = refCOMW - Vector3<double>(walkParam.ZmpOffsetRight.x, walkParam.ZmpOffsetRight.y, 0);
	refCOMW = RotationMatrix::getRotationZ(state.rSupFootZMP.z) * refCOMW;
	refCOMW += Vector3<double>(state.pSupFootZMP.x, state.pSupFootZMP.y, 0);

// 		if(fout.is_open()){
// 			fout<<state.istep<<" "<<state.supmod<<" "<<state.pZMP.x<<" "<<state.pZMP.y<<" "<<state.COMz<<" ";
// 			fout<<state.pSwingFoot.x<<" "<<state.pSwingFoot.y<<" "<<state.pSwingFoot.z<<" ";
// 			fout<<state.pSupFootZMP.x<<" "<<state.pSupFootZMP.y<<" "<<state.pSupFootZMP.z<<" ";
// 			fout<<refCOMW.x<< " "<< refCOMW.y<<" " <<refCOMW.z<<" ";
// 			fout<< naoStructure.supportFootPosWorld.x<<" "<< naoStructure.supportFootPosWorld.y<<" "<< naoStructure.supportFootPosWorld.z<<" ";
// 			fout<<naoStructure.supportFootRotWorld.z <<" ";
// 			fout<<std::endl;
// 		}
	// TODO: do ZMP and Swing Foot control
#ifndef NEW_HIPYAWPITCH_COMENSATION
	//First, COM control
	Vector3<double> dCOM =  refCOM - naoStructTmp.pCOM;
	Vector3<double> pBodyNext = dCOM + naoStructTmp.uLink[NaoStructure::bodyLink].p;
	RotationMatrix brz  = RotationMatrix::getRotationY(walkParam.torsoAngleY);
	if(state.supmod == NaoConfig::SUPPORT_MODE_LEFT || state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		Kinematics::calculateSafeLegJoints(walkingEngineOutput, NaoConfig::LegIDLeft, Vector3<double>(0,0,NaoConfig::getInstance()->FootHeight), RotationMatrix(), pBodyNext, brz);
	}else{
		Kinematics::calculateSafeLegJoints(walkingEngineOutput, NaoConfig::LegIDRight, Vector3<double>(0,0,NaoConfig::getInstance()->FootHeight), RotationMatrix(), pBodyNext, brz);
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
	//Then, Swing Leg Joints
	double supRotNowZ = state.rSupFootZMP.z;
	Vector3<double> psfoot = state.pSwingFoot;
	Vector3<double> psupfoot = state.pSupFootZMP;
	//convert foot pos from world coordinate to robot support coordinate
	if(state.supmod == NaoConfig::SUPPORT_MODE_LEFT || state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		psupfoot += RotationMatrix::getRotationZ(supRotNowZ) * Vector3<double>(-walkParam.ZmpOffsetLeft.x, -walkParam.ZmpOffsetLeft.y, 0);
	}else{
		psupfoot += RotationMatrix::getRotationZ(supRotNowZ) * Vector3<double>(-walkParam.ZmpOffsetRight.x, -walkParam.ZmpOffsetRight.y, 0);
	}
	psfoot  = RotationMatrix::getRotationZ(-supRotNowZ) *(psfoot - psupfoot);

	RotationMatrix frz = RotationMatrix::getRotationZ(state.rSwingFoot.z - supRotNowZ);

	if(state.supmod == NaoConfig::SUPPORT_MODE_LEFT || state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		double lastAnkleRoll = walkingEngineOutput.angles[JointData::RAnkleRoll];
		Kinematics::calculateSafeLegJoints(walkingEngineOutput, NaoConfig::LegIDRight, psfoot, frz, naoStructTmp.uLink[NaoStructure::bodyLink].p, naoStructTmp.uLink[NaoStructure::bodyLink].R);
// 		if(NoAnkleRollWhenSwing && state.supmod == NaoConfig::SUPPORT_MODE_LEFT)
// 		{
// 			if(fabs(lastAnkleRoll) > deg2rad(3))
// 				lastAnkleRoll -= sign(lastAnkleRoll) * deg2rad(3);
// 			else
// 				lastAnkleRoll = 0;
// 			walkingEngineOutput.angles[JointData::RAnkleRoll] = lastAnkleRoll;
// 		}
	}else{
		double lastAnkleRoll = walkingEngineOutput.angles[JointData::LAnkleRoll];
		Kinematics::calculateSafeLegJoints(walkingEngineOutput, NaoConfig::LegIDLeft, psfoot, frz, naoStructTmp.uLink[NaoStructure::bodyLink].p, naoStructTmp.uLink[NaoStructure::bodyLink].R);
// 		if(NoAnkleRollWhenSwing && state.supmod == NaoConfig::SUPPORT_MODE_RIGHT)
// 		{
// 			if(fabs(lastAnkleRoll) > deg2rad(3))
// 				lastAnkleRoll -= sign(lastAnkleRoll) * deg2rad(3);
// 			else
// 				lastAnkleRoll = 0;
// 			walkingEngineOutput.angles[JointData::LAnkleRoll] = lastAnkleRoll;
// 		}
	}
// 	if(NoAnkleRollWhenSwing)
// 	{
// 		walkingEngineOutput.angles[JointData::RAnkleRoll] = 0;
// 		walkingEngineOutput.angles[JointData::LAnkleRoll] = 0;
// 	}
#else//NEW_HIPYAWPITCH_COMENSATION
	Vector3<double> vRefCoM = (refCOM - pRefCoMLast) / walkParam.dt;
	Vector3<double> aRefCoM = (vRefCoM - vRefCoMLast) / walkParam.dt;
	Vector3<double> dCoMLast = pRefCoMLast - naoStructTmp.pCOM;
	double kpx = 0.6, kdx = 1;
	double kpy = 0.8, kdy = 1;
	//First, COM control
	ASSERT(pSensorData != 0);
// 	Vector3<double> dCOM = refCOM - naoStructTmp.pCOM;//refCOM - pRefCoMLast;
// 	dCOM.x -= kdx * (aRefCoM.x - pSensorData->data[SensorData::accX]) * walkParam.dt * walkParam.dt/2;
// 	dCOM.y -= kdy * (aRefCoM.y - pSensorData->data[SensorData::accY]) * walkParam.dt * walkParam.dt/2;
// 	//dCOM.x += kpx * dCoMLast.x + kdx * (aRefCoM.x - pSensorData->data[SensorData::accX]);
// 	//dCOM.y += kpy * dCoMLast.y + kdy * (aRefCoM.y - pSensorData->data[SensorData::accY]);
// 	dCOM.z = refCOM.z - naoStructTmp.pCOM.z;
	vRefCoMLast = vRefCoM;
	pRefCoMLast = refCOM;
	Vector3<double> dCOM =  refCOM - naoStructTmp.pCOM;
	Vector3<double> pBodyNext = dCOM + naoStructTmp.uLink[NaoStructure::bodyLink].p;
	double fRr, fRl;
	if(state.supmod == NaoConfig::SUPPORT_MODE_LEFT || state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT){
		fRr = state.rSwingFoot.z;
		fRl = state.rSupFootZMP.z;
	}else{
		fRr = state.rSupFootZMP.z;
		fRl = state.rSwingFoot.z;
	}
	Vector3<double> equalHipAngles;
	double rHipYawPitchAngle;
	//search for best rHipYawPitchAngle
	binarySearchforYawRollPitch(fRr, fRl, equalHipAngles, rHipYawPitchAngle);
	double dTilt = (walkParam.torsoAngleY - pSensorData->data[SensorData::angleY]) * 0.2;
	double dRoll = (0 - pSensorData->data[SensorData::angleX]) * 0.2;
	RotationMatrix brz  = RotationMatrix::getRotationY(walkParam.torsoAngleY + dTilt);
	//brz.rotateX(dRoll);
	if(state.supmod == NaoConfig::SUPPORT_MODE_LEFT || state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		brz.rotateZ(equalHipAngles.z);
		Kinematics::calculateSafeLegJoints(walkingEngineOutput, NaoConfig::LegIDLeft, Vector3<double>(0,0,NaoConfig::getInstance()->FootHeight), RotationMatrix(), pBodyNext, brz,
															rHipYawPitchAngle, equalHipAngles.x, equalHipAngles.y);
	}else{
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
	//-----newly added for save body position and rotation relative to sup_foot
	Vector3<double> pbody2supfoot;
	Vector3<double> rbody2supfoot;
	pbody2supfoot = naoStructTmp.uLink[NaoStructure::bodyLink].p;
	rbody2supfoot = naoStructTmp.uLink[NaoStructure::bodyLink].R.getXYZAngles();
	//-----------------------------
	//Then, Swing Leg Joints
	double supRotNowZ = state.rSupFootZMP.z;
	Vector3<double> psfoot = state.pSwingFoot;
	Vector3<double> psupfoot = state.pSupFootZMP;
	Vector3<double> rsupfoot = state.rSupFootZMP;
	//convert foot pos from world coordinate to robot support coordinate
	if(state.supmod == NaoConfig::SUPPORT_MODE_LEFT || state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		psupfoot += RotationMatrix::getRotationZ(supRotNowZ) * Vector3<double>(-walkParam.ZmpOffsetLeft.x, -walkParam.ZmpOffsetLeft.y, 0);
	}else{
		psupfoot += RotationMatrix::getRotationZ(supRotNowZ) * Vector3<double>(-walkParam.ZmpOffsetRight.x, -walkParam.ZmpOffsetRight.y, 0);
	}
	psfoot  = RotationMatrix::getRotationZ(-supRotNowZ) *(psfoot - psupfoot);

	RotationMatrix frz = RotationMatrix::getRotationZ(state.rSwingFoot.z - supRotNowZ);
	Vector3<double> bodyAnglesNow = naoStructTmp.uLink[NaoStructure::bodyLink].R.getXYZAngles();
	if(state.supmod == NaoConfig::SUPPORT_MODE_LEFT || state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		Kinematics::calculateSafeLegJoints(walkingEngineOutput, NaoConfig::LegIDRight, psfoot, frz, naoStructTmp.uLink[NaoStructure::bodyLink].p, naoStructTmp.uLink[NaoStructure::bodyLink].R,
																rHipYawPitchAngle, equalHipAngles.x, equalHipAngles.y);
	}else{
		Kinematics::calculateSafeLegJoints(walkingEngineOutput, NaoConfig::LegIDLeft, psfoot, frz, naoStructTmp.uLink[NaoStructure::bodyLink].p, naoStructTmp.uLink[NaoStructure::bodyLink].R,
																rHipYawPitchAngle, equalHipAngles.x, equalHipAngles.y);
	}
#endif//NEW_HIPYAWPITCH_COMENSATION

	//===add arms swing here=====//
	if(walkParam.useArmSwing == PreviewWalkParam::ENABLE)
		swingArms(istep, walkParam, walkStatus, walkingEngineOutput);
	//Check whether leaving possible
	if(state.walkStatus ==  WalkTypeBase::standing)
	{
		walkingEngineOutput.isLeavingPossible = true;
	}else{
		walkingEngineOutput.isLeavingPossible = false;
	}
	if(!Use_Smooth_Filter){
		//Slow joint increase when changing from initial state to walk standing....
		if(state.walkStatus ==  WalkTypeBase::starting && initialSlowCounter-- > 0)
		{
			int startID;
			if(walkParam.useArmSwing == PreviewWalkParam::ENABLE)
				startID = JointData::HeadYaw;
			else
				startID = JointData::RHipYawPitch;
			for(int i = startID; i < JointData::numOfJoints; ++i)
			{
				walkingEngineOutput.angles[i] =lastWalkOoutput.angles[i] + CLIP(walkingEngineOutput.angles[i] - lastWalkOoutput.angles[i], deg2rad(0.3));
			}
		}
	}

	//Further calibration....
	if(walkParam.useHipAnkleRollScale == PreviewWalkParam::ENABLE){
		walkingEngineOutput.angles[JointData::RHipRoll] *= walkParam.scaleRHipRoll;
		walkingEngineOutput.angles[JointData::LHipRoll] *= walkParam.scaleLHipRoll;
		walkingEngineOutput.angles[JointData::RAnkleRoll] *= walkParam.scaleRAnkleRoll;
		walkingEngineOutput.angles[JointData::LAnkleRoll] *= walkParam.scaleLAnkleRoll;
	}
	//walkingEngineOutput.angles[JointData::RHipYawPitch] = 0;
	//walkingEngineOutput.angles[JointData::LHipYawPitch] = 0;
	//walkingEngineOutput.angles[JointData::RHipYawPitch] = walkingEngineOutput.angles[JointData::LHipYawPitch];
	//Smoothing Joints with a low-pass smoothing filter.
	if(Use_Smooth_Filter)
		smoothJoints(walkingEngineOutput, lastWalkOoutput);
	//====
	/*if(walkStatus == WalkTypeBase::running){
		if(state.supmod == NaoConfig::SUPPORT_MODE_LEFT || state.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
		{
			if(walkingEngineOutput.angles[JointData::RAnkleRoll] > deg2rad(3.4))
				walkingEngineOutput.angles[JointData::RAnkleRoll] = lastWalkOoutput.angles[JointData::RAnkleRoll];
		}else// if(state.supmod == NaoConfig::SUPPORT_MODE_RIGHT)
		{
			if(walkingEngineOutput.angles[JointData::LAnkleRoll] < deg2rad(3.4))
				walkingEngineOutput.angles[JointData::LAnkleRoll] = lastWalkOoutput.angles[JointData::LAnkleRoll];
		}
	}*/
	//walkingEngineOutput.angles[JointData::RAnklePitch] = lastWalkOoutput.angles[JointData::RAnklePitch];
	//walkingEngineOutput.angles[JointData::LAnklePitch] = lastWalkOoutput.angles[JointData::LAnklePitch];
	//Assign Last Walking Output
	lastWalkOoutput = walkingEngineOutput;

	//===Compensate HipRoll When Single Supproting====//
	if(walkParam.useHipRollCompensation && (walkStatus == WalkTypeBase::running || (walkStatus == WalkTypeBase::starting && miniStatus == PreBufferGenerator::PreSwinging)))
	{
		//double Rbefore = walkingEngineOutput.angles[JointData::RHipRoll];
		//double Lbefore = walkingEngineOutput.angles[JointData::LHipRoll];
		compensateHipRoll(istep, sup_mod, walkStatus, miniStatus, walkParam,walkingEngineOutput);
		//double Rafter = walkingEngineOutput.angles[JointData::RHipRoll];
		//double Lafter = walkingEngineOutput.angles[JointData::LHipRoll];
		//printf("### %.4f %.4f %.4f $.4f\n", Rbefore, Rafter, Lbefore, Lafter);
	}
	//===Joints Calibration========//
	if(walkParam.useJointsCalibration == PreviewWalkParam::ENABLE
		&& walkStatus == WalkTypeBase::running)
	{
		for(int i = 0; i < 12; ++i)
		{
			//if(JointData::RHipYawPitch+i == JointData::LHipRoll)
				walkingEngineOutput.angles[JointData::RHipYawPitch+i] += walkParam.jointsCalibration[i].offset;
		}
	}
	if(walkParam.useHipRollClip == PreviewWalkParam::ENABLE && walkStatus == WalkTypeBase::running)
	{
		//walkingEngineOutput.angles[JointData::RHipRoll] = CLIP(walkingEngineOutput.angles[JointData::RHipRoll], walkParam.clipRHipRollLow, walkParam.clipRHipRollUp);
		//walkingEngineOutput.angles[JointData::LHipRoll] = CLIP(walkingEngineOutput.angles[JointData::LHipRoll], walkParam.clipLHipRollLow,walkParam.clipLHipRollUp);
		walkingEngineOutput.angles[JointData::RAnkleRoll] = CLIP(walkingEngineOutput.angles[JointData::RAnkleRoll], walkParam.clipRHipRollLow, walkParam.clipRHipRollUp);
		walkingEngineOutput.angles[JointData::LAnkleRoll] = CLIP(walkingEngineOutput.angles[JointData::LAnkleRoll], walkParam.clipLHipRollLow,walkParam.clipLHipRollUp);
	}

	//===Debug print something
	MDEBUG_SAVE(MDebug::idPreviewState, state)
	MDEBUG_SAVE(MDebug::idRefCOM, refCOMW)
	MDEBUG_SAVE(MDebug::idRealCOM, (RotationMatrix::getRotationZ(supRotNowZ) * recCOM+psupfoot))
	MDEBUG_SAVE(MDebug::idBodyPos,(RotationMatrix::getRotationZ(supRotNowZ) * pBodyNext+psupfoot));
	MDEBUG_SAVE(MDebug::idBodyAngles, naoStructure.uLink[NaoStructure::bodyLink].R.getXYZAngles());
	MDEBUG_SAVE(MDebug::idpSupFootOri,psupfoot);
	MDEBUG_SAVE(MDebug::idrSupFootOri,rsupfoot);
	MDEBUG_SAVE(MDebug::idpBody2SupFoot,pbody2supfoot);
	MDEBUG_SAVE(MDebug::idrBody2SupFoot,rbody2supfoot);
// 	if(foutJoints.is_open())
// 	{
// 		for(int i = 1; i < JointData::numOfJoints; ++i)
// 		{
// 			foutJoints<< walkingEngineOutput.angles[i]<<" ";
// 		}
// 		foutJoints<< std::endl;
// 	}
	//====Finish one frame
	finishFrame();
}
void PreviewWalk::reset()
{

}
void PreviewWalk::updateWalkCmd(const WalkCmd& newCmd)
{
	std::cout<<"Preview Walk New Walk Cmd Comming...."<<std::endl;
	pendingCmd = newCmd;
	isCmdPending = true;
}

bool PreviewWalk::isCmdChangePossible()
{
	bool possible = false;
	std::cout<<"Preview Walk Checking New Walk Cmd... ";
/*	if(istep == 0 || istep == walkParam.PG / 2)
		return true;
	else if(!previewBuffer.empty() && previewBuffer.back().walkStatus == WalkTypeBase.ending)
		return true;
	else
		return false;*/
	if(previewBuffer.empty())	//preview buffer is empty(), we can change speed and reset the buffer
	{
		possible =  true;
	}else
	{
		PreviewState& state =previewBuffer.back();
		if(state.walkStatus == WalkTypeBase::running && (state.istep == 0 || state.istep == walkParam.PG/2))
		{
			possible =  true;
		}else if(state.walkStatus == WalkTypeBase::standing)
		{
			possible = true;
		}
	}
	if(possible)
		std::cout<<" CHANGE POSSIBLE!"<<std::endl;
	else
		std::cout<<" NO CHANGE!"<<std::endl;
	return possible;
}

void PreviewWalk::fillPreviewBuffer()
{
	PreviewState previewState;
	while(! previewBuffer.full())
	{
		previewBuffer.push_back(preBufferGenrator.generateNext());
	}
}

void PreviewWalk::finishFrame()
{
	//Remove first frame of buffer
	previewBuffer.pop_front();
	//Generate new to fill buffer
	fillPreviewBuffer();
}
static const double Max_Step_Move = deg2rad(5);
void PreviewWalk::swingArms(int istep_ , const PreviewWalkParam& walkParm_,  int walkStaus_, WalkingEngineOutput& walkOutput_)
{
	double ratio = static_cast<double>(istep_)/walkParm_.PG;
	double theta = ratio * pi2;
	int armStart = JointData::RShoulderPitch;
	int index = 0;
	double joint= 0, jmove = 0;
	if(walkStaus_ != WalkTypeBase::running)
		theta = 0;
	for(int i = armStart; i <= JointData::LElbowRoll; ++i)
	{
		index = i-armStart;
		if(walkParm_.armSwing[index].swing)
		{
			joint = walkParm_.armSwing[index].swingSign * walkParm_.armSwing[index].amp*sin(theta) + walkParm_.armSwing[index].offset;
		}else{
			joint = walkParm_.armSwing[index].offset;
		}
		jmove = joint - walkOutput_.angles[i];
		if(fabs(jmove) > Max_Step_Move)
			walkOutput_.angles[i] += sign(jmove) * Max_Step_Move;
		else
			walkOutput_.angles[i] = joint;
	}
}
//Add sin wave to hip roll angle when single supporting stage
void PreviewWalk::compensateHipRoll(int istep_, int supmod_, int walkStatus_, int miniStatus_, const PreviewWalkParam& walkParam, WalkingEngineOutput& walkOutput_)
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
	if(walkStatus_ == WalkTypeBase::starting && miniStatus_ == PreBufferGenerator::PreSwinging)
		compensator *= 1.2;
	//=====
	double angle = compensator * sin(ratio * pi);
	angle = CLIP(angle,fabs(compensator) * 0.7);
	walkOutput_.angles[index] += angle;
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

