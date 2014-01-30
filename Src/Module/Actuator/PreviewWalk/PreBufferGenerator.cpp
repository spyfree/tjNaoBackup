#include "PreBufferGenerator.h"
#include "Module/Common/NaoConfig.h"
#include "Platform/GTAssert.h"
#include <iostream>

const double PreBufferGenerator::MAX_FORWARD_X = 0.1; //m/step
const double PreBufferGenerator::MAX_BACKWARD_X = -0.05;
const double PreBufferGenerator::MAX_LEFT_Y = 0.04;
const double PreBufferGenerator::MAX_TURNCCW = deg2rad(30);//rad/step
const double PreBufferGenerator::MIN_SPEED_X = 0.01;
const double PreBufferGenerator::MIN_SPEED_Y = 0.01;
//const double PreBufferGenerator::MIN_SPEED =Vector2<double>(PreBufferGenerator::MIN_SPEED_X, PreBufferGenerator::MIN_SPEED_Y).abs();
const double PreBufferGenerator::MIN_TURNCCW = deg2rad(2);

PreBufferGenerator::PreBufferGenerator()
:initialized(false)
{

}
/*PreBufferGenerator::PreBufferGenerator(const WalkCmd& walkCmd, const PreviewWalkParam& _walkParam, const NaoStructure& naoStructure)
{
	reset(walkCmd, _walkParam, naoStructure);
}*/

void PreBufferGenerator::reset(const WalkCmd& walkCmd, const PreviewWalkParam& _walkParam, const NaoStructure& naoStructure)
{
	// TODO: Consider: maybe the ending state(standing) buffer of last walk can be used as the starting buffer of the new walk!!!
	// TODO: calc sx, sy... according to the walkOutput..
	walkParam = _walkParam;
	standingSx =walkParam.sx;
	standingSy = naoStructure.uLink[NaoStructure::lFootLink].p.y - (walkParam.ZmpOffsetRight.y - walkParam.ZmpOffsetLeft.y);
	walkingSy = walkParam.sy;
	changeSpeed(walkCmd);
	if(walkCmd.speed.translation.y > 0 || walkCmd.speed.rotation > 0)
		startSwingLeg = NaoConfig::LegIDLeft;
	else
		startSwingLeg = NaoConfig::LegIDRight;
	// TODO: CLIPing!!
	//Right leg support first
	lastSupZMPp.x = walkParam.ZmpOffsetLeft.x;
	lastSupZMPp.y = standingSy/2;
	lastSupCOMz = naoStructure.pCOM.z;

	currentSupZMPp.x = walkParam.ZmpOffsetLeft.x;
	currentSupZMPp.y = standingSy/2;
	currentSupCOMz = naoStructure.pCOM.z;

	nextSupZMPp.x = walkParam.ZmpOffsetLeft.x;
	nextSupZMPp.y = -standingSy/2;
	nextSupCOMz =walkParam.Zc;

	lastKeyZMP = (currentSupZMPp + nextSupZMPp) /2;
	lastKeyCOMz = naoStructure.pCOM.z;
	nextKeyZMP = lastKeyZMP;
	nextKeyCOMz = walkParam.Zc;

	isJustStarting = true;
	preparetoStop = false;
	finishFinalStep = false;
	stopOnLeft = false;
	startCounter = 0;
	standingCounter = 0;

	walkStatus = WalkTypeBase::deactive;
	miniStatus = Squatting;
	istep = 0;

	framesSquatting = walkParam.PG / 2;
	framesSmallRest = walkParam.PG / 4;
	startIstep = static_cast<int>((walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT] + walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] / 2)* walkParam.PG + 0.5);
	framesStartMovingLeft = walkParam.PG -  startIstep;//static_cast<int>((walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] / 2 + walkParam.sLen[NaoConfig::SUPPORT_MODE_RIGHT] + walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT]) * walkParam.PG + 0.5);
	framesStarting = framesSquatting
		+  framesSmallRest
		+ framesStartMovingLeft;
	ASSERT((framesSquatting + framesStartMovingLeft) < framesStarting);
	initialized = true;
	walkJustChanged = false;
	lastWalkingLeft = walkingdLeft;
	lastTurnCCW = walkingTurnCCW;
}
void PreBufferGenerator::changeSpeed(const WalkCmd& cmd)
{
	lastWalkingLeft = walkingdLeft;
	lastTurnCCW = walkingTurnCCW;
    std::cout<<"Walking Speed Request: "<<cmd.speed.translation.x<<", "<<cmd.speed.translation.y<<", "<<cmd.speed.rotation<<std::endl;
	//walking Sx
	walkingSx = CLIP(cmd.speed.translation.x * (walkParam.PG * walkParam.dt) , MAX_BACKWARD_X, MAX_FORWARD_X);
	std::cout<<"Walking Speed Request: "<<walkingSx<<", MAX_BACKWARD_X: "<<MAX_BACKWARD_X<<", MAX_FORWARD_X: "<<MAX_FORWARD_X<<std::endl;
	if(fabs(walkingSx) < MIN_SPEED_X)
		walkingSx = 0;
    std::cout<<"Walking Speed Renoquest: "<<walkingSx<<", MIN_SPEED_X: "<<MIN_SPEED_X<<std::endl;
	//Walking left
	walkingdLeft = CLIP(cmd.speed.translation.y * (walkParam.PG * walkParam.dt), -MAX_LEFT_Y, MAX_LEFT_Y);
	if(fabs(walkingdLeft) < MIN_SPEED_Y)
		walkingdLeft = 0;
	//===Turn CCW
	walkingTurnCCW = CLIP(cmd.speed.rotation * (walkParam.PG * walkParam.dt), MAX_TURNCCW);
	if(fabs(walkingTurnCCW) < MIN_TURNCCW)
		walkingTurnCCW = 0;
	if(walkingTurnCCW > 0)
		walkingTurnCCW = walkingTurnCCW;
	bool cmdStand = false;
	cmdStand = checkSpeedStand(cmd);
	if(cmdStand == true)
	{
		walkingSx = 0;
		walkingdLeft = 0;
		walkingTurnCCW = 0;
		//setToStop();
		walkStatus = WalkTypeBase::standing;
	}
	if(walkStatus == WalkTypeBase::standing && !cmdStand)
	{
		//istep = 0;
		walkStatus = WalkTypeBase::running;
	}
	walkJustChanged = true;
	std::cout<<"Walking Speed Set: "<<walkingSx<<", "<<walkingdLeft<<", "<<walkingTurnCCW<<std::endl;
}
bool PreBufferGenerator::checkSpeedStand(const WalkCmd& cmd)
{
	return (fabs(cmd.speed.translation.x)< MIN_SPEED_X && fabs(cmd.speed.translation.y) <MIN_SPEED_Y
		&& fabs(cmd.speed.rotation) < MIN_TURNCCW);
}

PreviewState PreBufferGenerator::generateNext()
{
	int supmod = -1;
	PreviewState stateVect;
	if(walkStatus == WalkTypeBase::deactive)
	{
		walkStatus = WalkTypeBase::starting;
	}
	// Set prepare to stop now
	if(preparetoStop &&(walkStatus == WalkTypeBase::running || walkStatus == WalkTypeBase::starting))
	{
		walkStatus = WalkTypeBase::ending;
	}
	//=====Process Foot Switch Event====
	doFootSwitch();
	//=====Generate ZMP trajectory new vector====//
	Vector2<double> newZMP;
	double newCOMz = 0;
	int iphase, phaseLen;
	generateNewZMP(newZMP, newCOMz, supmod, iphase, phaseLen);
	//=====Generate Swing Foot Trajectory, actually the ankle trajectory!
	Vector3<double> pfootnew, rfootnew;
	generateNewSwingFootPos(pfootnew, rfootnew, supmod, iphase, phaseLen);
	//======set State Vector
	stateVect.pZMP = newZMP;
	stateVect.COMz = newCOMz;
	stateVect.pSupFootZMP = Vector3<double>(currentSupZMPp.x, currentSupZMPp.y, 0);
	stateVect.rSupFootZMP = currentSupFootrot;
	stateVect.pSwingFoot = pfootnew;
	stateVect.rSwingFoot = rfootnew;
	stateVect.supmod = supmod;
	stateVect.istep = istep;
	stateVect.walkStatus = walkStatus;
	stateVect.miniStatus = miniStatus;
	//======Increase istep=====
	if(walkStatus == WalkTypeBase::starting)
	{
		startCounter++;
		if(startCounter > framesSquatting + framesSmallRest){
			miniStatus = PreSwinging;
			istep ++;
		}
		else{
			istep = startIstep;
			if(startCounter > framesSquatting)
				miniStatus = LittleWaitting;
		}
		if(startCounter >= framesStarting)
			walkStatus = WalkTypeBase::running;
		if(istep >= walkParam.PG)
			istep = 0;
	}else if(walkStatus == WalkTypeBase::running)
	{
		istep ++;
		if(istep >= walkParam.PG)
			istep = 0;
	}else if(walkStatus == WalkTypeBase::ending)
	{
		istep ++;
		int tmp;
		if(stopOnLeft)
			tmp = static_cast<int>((walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT]  + walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] / 2 )* walkParam.PG);
		else
			tmp = static_cast<int>((walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT]  + walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] + walkParam.sLen[NaoConfig::SUPPORT_MODE_RIGHT] + walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT]/ 2 )* walkParam.PG);

		if(istep == tmp)
			standingCounter = 0;
		if(istep > tmp){
			istep = tmp + 1;
			if(standingCounter++ > 20)
				walkStatus = WalkTypeBase::standing;
		}
	}else if(walkStatus == WalkTypeBase::standing)
	{
		//istep = 1;
	}
	return stateVect;
}

void PreBufferGenerator::doFootSwitch()
{
	Vector2<double> vtmp;
	double dtmp;
	if(walkStatus == WalkTypeBase::starting)
	{
		if(istep == walkParam.PG / 2)
		{
			lastSupFootrot = currentSupFootrot;
			currentSupFootrot = nextSupFootrot;
			if(startSwingLeg == NaoConfig::LegIDRight){
				nextSupFootrot = nextSupFootrot;
				vtmp = lastSupZMPp;
				dtmp = 0;
				lastSupZMPp = currentSupZMPp;
				currentSupZMPp = nextSupZMPp;
				lastKeyCOMz = currentSupCOMz;
				currentSupCOMz = lastSupCOMz;
				nextSupZMPp = vtmp;
				nextSupCOMz += dtmp;
			}else{
				nextSupFootrot +=  Vector3<double>(0,0,walkingTurnCCW);
				vtmp = Vector2<double>(walkingSx, walkingSy);
				vtmp += Vector2<double>(0, sign(walkingdLeft) * 0.005);
				vtmp.rotate(nextSupFootrot.z);
				dtmp = 0;
				lastSupZMPp = currentSupZMPp;
				currentSupZMPp = nextSupZMPp;
				lastKeyCOMz = currentSupCOMz;
				currentSupCOMz = lastSupCOMz;
				nextSupZMPp += vtmp;
				nextSupCOMz += dtmp;
			}
		}
	}
	else if(walkStatus == WalkTypeBase::running)
	{
		if(istep == 0)
		{
			lastSupFootrot = currentSupFootrot;
			currentSupFootrot = nextSupFootrot;
			if(walkJustChanged){
				nextSupFootrot += Vector3<double>(0,0,(walkingTurnCCW + lastTurnCCW)/2);
			}else
				nextSupFootrot += Vector3<double>(0,0,walkingTurnCCW);

			lastSupZMPp = currentSupZMPp;
			currentSupZMPp = nextSupZMPp;
			lastKeyCOMz = currentSupCOMz;
			currentSupCOMz = lastSupCOMz;
			if(isJustStarting)
			{
				vtmp.x = walkingSx;
				vtmp.y = -(walkingSy /2 + standingSy/2);
				dtmp = 0;
				vtmp.rotate(nextSupFootrot.z);
				isJustStarting = false;
			}else{
				vtmp.x = walkingSx;
				vtmp.y = -walkingSy;
				dtmp = 0;
				if(walkJustChanged && (lastWalkingLeft <= 0 && walkingdLeft > 0)){
					vtmp += Vector2<double>(0, 0);
				}else
					vtmp = vtmp + Vector2<double>(0, walkingdLeft);
				vtmp.rotate(nextSupFootrot.z);
			}
			nextSupZMPp += vtmp;
			nextSupCOMz +=dtmp;
			if(walkJustChanged)
				walkJustChanged = false;
		}else if(istep == walkParam.PG /2)
		{
			lastSupFootrot = currentSupFootrot;
			currentSupFootrot = nextSupFootrot;
			if(walkJustChanged){
				nextSupFootrot += Vector3<double>(0,0,(walkingTurnCCW + lastTurnCCW)/2);
			}else
				nextSupFootrot +=  Vector3<double>(0,0,walkingTurnCCW);

			lastSupZMPp = currentSupZMPp;
			currentSupZMPp = nextSupZMPp;
			lastKeyCOMz = currentSupCOMz;
			currentSupCOMz = lastSupCOMz;

			dtmp = 0;
			vtmp = Vector2<double>(walkingSx, walkingSy);
			if(walkJustChanged && (lastWalkingLeft >= 0 && walkingdLeft< 0)){
					vtmp += Vector2<double>(0, 0);
			}else
				vtmp += Vector2<double>(0, walkingdLeft);
			vtmp.rotate(nextSupFootrot.z);
			nextSupZMPp += vtmp;
			nextSupCOMz += dtmp;
		}
		if(walkJustChanged)
			walkJustChanged = false;
	}else if(walkStatus == WalkTypeBase::ending)
	{
		if(istep == 0)
		{
			if(!finishFinalStep)
			{
				lastSupFootrot = currentSupFootrot;
				currentSupFootrot = nextSupFootrot;
				nextSupFootrot = nextSupFootrot;

				lastSupZMPp = currentSupZMPp;
				currentSupZMPp = nextSupZMPp;
				vtmp = Vector2<double>(walkingSx/2, -walkingSy);
				vtmp.rotate(nextSupFootrot.z);
				nextSupZMPp += vtmp;

				lastSupCOMz = currentSupCOMz;
				currentSupCOMz = nextSupCOMz;
				nextSupCOMz += 0;

				finishFinalStep = true;
				stopOnLeft = true;

			}
		}else if(istep == walkParam.PG / 2)
		{
			if(!finishFinalStep)
			{
				lastSupFootrot = currentSupFootrot;
				currentSupFootrot = nextSupFootrot;
				nextSupFootrot = nextSupFootrot;

				lastSupZMPp = currentSupZMPp;
				currentSupZMPp = nextSupZMPp;
				vtmp = Vector2<double>(walkingSx/2, walkingSy);
				vtmp.rotate(nextSupFootrot.z);
				nextSupZMPp += vtmp;

				lastSupCOMz = currentSupCOMz;
				currentSupCOMz = nextSupCOMz;
				nextSupCOMz += 0;

				finishFinalStep = true;
				stopOnLeft = false;
			}
		}
	}else if(walkStatus == WalkTypeBase::standing)
	{
	}
}

void PreBufferGenerator::generateNewZMP(Vector2<double>& newZMP, double& newCOMz, int& supmod_, int& iphase, int& phaseLen)
{
// 	Vector2<double> newZMP;
// 	double newCOMz= 0;
	double ratio = 0;
	//int iphase = 0, phaseLen = 1;
	//int supmod_ = 0;
	double ratioSum[PreviewWalkParam::maxStages];
	//Easy later use of sLen
	ratioSum[0] = walkParam.sLen[0];
	for(int i = 1; i < walkParam.numStages; ++i)
	{
		ratioSum[i] = ratioSum[i-1] + walkParam.sLen[i];
	}
	if(walkStatus == WalkTypeBase::starting)
	{
		int iFrameRemained = framesStarting - startCounter;
		iFrameRemained = CLIP(iFrameRemained, 0, framesStarting);
		//Squatting stage, first squat , then wait for a few frames,
		//then moving zmp to left foot
		if(startCounter == framesSquatting )
		{
			lastKeyZMP = nextKeyZMP;
			lastKeyCOMz = nextKeyCOMz;
			nextKeyZMP = nextKeyZMP;
			nextKeyCOMz = nextKeyCOMz;
		}
		if(iFrameRemained == framesStartMovingLeft)
		{
			lastKeyZMP = nextKeyZMP;
			lastKeyCOMz = nextKeyCOMz;
			nextKeyZMP = nextSupZMPp;
			nextKeyCOMz = nextSupCOMz;
		}else if(istep==static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT]* walkParam.PG+0.5))
		{
			lastKeyZMP = nextKeyZMP;
			lastKeyCOMz = nextKeyCOMz;
			nextKeyZMP = nextKeyZMP;
			nextKeyCOMz = nextKeyCOMz;
		}else if(istep ==static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_RIGHT]* walkParam.PG+0.5))
		{
			lastKeyZMP = nextKeyZMP;
			lastKeyCOMz = nextKeyCOMz;
			nextKeyZMP = nextSupZMPp;
			nextKeyCOMz = nextSupCOMz;
		}

		if(startCounter < framesSquatting)
		{
			iphase = startCounter;
			phaseLen = framesSquatting;
			supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_LEFT;
		}else if(iFrameRemained > framesStartMovingLeft)
		{
			iphase = startCounter - framesSquatting;
			phaseLen = framesSmallRest;
			supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_LEFT;
		}
		else if(istep < static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] * walkParam.PG+0.5))
		{
			iphase = istep - static_cast<int>((ratioSum[NaoConfig::SUPPORT_MODE_LEFT] + walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] /2 )* walkParam.PG + 0.5);
			phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] / 2 * walkParam.PG+0.5);
			supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_LEFT;
		}else if(istep < static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_RIGHT] * walkParam.PG+0.5))
		{
			iphase = istep - static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] * walkParam.PG+0.5);
			phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_RIGHT] * walkParam.PG+0.5);
			supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT;
		}else
		{
			iphase = istep - static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_RIGHT] * walkParam.PG+0.5);
			phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT] * walkParam.PG+0.5);
			supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT;
		}
		//interpolate between key points
		ASSERT(phaseLen > 0);
		ratio = static_cast<double>(iphase) / phaseLen;
		ASSERT((ratio >= 0 && ratio <= 1));
		newZMP = interpolate(lastKeyZMP, nextKeyZMP, ratio, Interpolate_Linear);
		newCOMz =  interpolate(lastKeyCOMz, nextKeyCOMz, ratio, Interpolate_Linear);
	}else if(walkStatus == WalkTypeBase::running)
	{
		//====switch and get new key points
		if(istep == 0)
		{
			lastKeyZMP = nextKeyZMP;
			lastKeyCOMz = nextKeyCOMz;
			nextKeyZMP = nextKeyZMP;//ZMP doesn't move at single support stage
			nextKeyCOMz = nextKeyCOMz;
		}else if(istep == static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5))
		{
			lastKeyZMP = nextKeyZMP;
			lastKeyCOMz = nextKeyCOMz;
			nextKeyZMP = nextSupZMPp;//ready for double_support_left
			nextKeyCOMz = nextSupCOMz;
		}else if(istep==static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT]* walkParam.PG+0.5))
		{
			lastKeyZMP = nextKeyZMP;
			lastKeyCOMz = nextKeyCOMz;
			nextKeyZMP = nextKeyZMP;
			nextKeyCOMz = nextKeyCOMz;
		}else if(istep ==static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_RIGHT]* walkParam.PG+0.5))
		{
			lastKeyZMP = nextKeyZMP;
			lastKeyCOMz = nextKeyCOMz;
			nextKeyZMP = nextSupZMPp;
			nextKeyCOMz = nextSupCOMz;
		}
		//=====
		if(istep >= 0 && istep <  static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5))
		{
			iphase = istep;
			phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5);
			supmod_ = NaoConfig::SUPPORT_MODE_LEFT;
		}else if(istep < static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] * walkParam.PG+0.5))
		{
			iphase = istep - static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG + 0.5);
			phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] * walkParam.PG+0.5);
			supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_LEFT;
		}else if(istep < static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_RIGHT] * walkParam.PG+0.5))
		{
			iphase = istep - static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] * walkParam.PG+0.5);
			phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_RIGHT] * walkParam.PG+0.5);
			supmod_ = NaoConfig::SUPPORT_MODE_RIGHT;
		}else
		{
			iphase = istep - static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_RIGHT] * walkParam.PG+0.5);
			phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT] * walkParam.PG+0.5);
			supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT;
		}
		//interpolate between key points
		ASSERT(phaseLen > 0);
		ratio = static_cast<double>(iphase) / phaseLen;
		ASSERT((ratio >= 0 && ratio <= 1));
		newZMP = interpolate(lastKeyZMP, nextKeyZMP, ratio, Interpolate_Linear);
		newCOMz =  interpolate(lastKeyCOMz, nextKeyCOMz, ratio, Interpolate_Linear);
	}else if(walkStatus == WalkTypeBase::ending)
	{
		if(stopOnLeft)
		{
			if(istep == 0)
			{
				lastKeyZMP = nextKeyZMP;
				lastKeyCOMz = nextKeyCOMz;
				nextKeyZMP = nextKeyZMP;
				nextKeyCOMz = nextKeyCOMz;
			}else if(istep == static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5))
			{
				lastKeyZMP = nextKeyZMP;
				lastKeyCOMz = nextKeyCOMz;
				nextKeyZMP = (nextSupZMPp + currentSupZMPp) / 2;
				nextKeyCOMz = nextKeyCOMz;
			}else if(istep == static_cast<int>((walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT]  + walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] / 2 )* walkParam.PG+0.5))
			{
				lastKeyZMP = nextKeyZMP;
				nextKeyZMP = nextKeyZMP;
				lastKeyCOMz = nextKeyCOMz;
				nextKeyCOMz = nextKeyCOMz;
			}
			//==
			if(istep >= 0 && istep <  static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5))
			{
				iphase = istep;
				phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5);
				supmod_ = NaoConfig::SUPPORT_MODE_LEFT;
			}else if(istep < static_cast<int>((walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT]  + walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] / 2 )* walkParam.PG+0.5))
			{
				iphase = istep - static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5);
				phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] / 2 * walkParam.PG+0.5);
				supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_LEFT;
			}else
			{
				//Now nao stand still
				iphase = 0;
				phaseLen = 1;
				supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_LEFT;
			}
			//interpolate between key points
			ASSERT(phaseLen > 0);
			ratio = static_cast<double>(iphase) / phaseLen;
			ASSERT((ratio >= 0 && ratio <= 1));
			newZMP = interpolate(lastKeyZMP, nextKeyZMP, ratio, Interpolate_Linear);
			newCOMz =  interpolate(lastKeyCOMz, nextKeyCOMz, ratio, Interpolate_Linear);
		}else
		{
			//====switch and get new key points
			if(istep == 0)
			{
				lastKeyZMP = nextKeyZMP;
				lastKeyCOMz = nextKeyCOMz;
				nextKeyZMP = nextKeyZMP;//ZMP doesn't move at single support stage
				nextKeyCOMz = nextKeyCOMz;
			}else if(istep == static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5))
			{
				lastKeyZMP = nextKeyZMP;
				lastKeyCOMz = nextKeyCOMz;
				nextKeyZMP = nextSupZMPp;//ready for double_support_left
				nextKeyCOMz = nextSupCOMz;
			}else if(istep==static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT]* walkParam.PG+0.5))
			{
				lastKeyZMP = nextKeyZMP;
				lastKeyCOMz = nextKeyCOMz;
				nextKeyZMP = nextKeyZMP;
				nextKeyCOMz = nextKeyCOMz;
			}else if(istep ==static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_RIGHT]* walkParam.PG+0.5))
			{
				lastKeyZMP = nextKeyZMP;
				lastKeyCOMz = nextKeyCOMz;
				nextKeyZMP = (nextSupZMPp + currentSupZMPp) / 2;
				nextKeyCOMz = nextSupCOMz;
			}else if(istep == static_cast<int>((ratioSum[NaoConfig::SUPPORT_MODE_RIGHT] + walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT] /2)* walkParam.PG+0.5))
			{
				lastKeyZMP = nextKeyZMP;
				lastKeyCOMz = nextKeyCOMz;
				nextKeyZMP = nextKeyZMP;
				nextKeyCOMz = nextKeyCOMz;
			}
			//=====
			if(istep >= 0 && istep <  static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5))
			{
				iphase = istep;
				phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5);
				supmod_ = NaoConfig::SUPPORT_MODE_LEFT;
			}else if(istep < static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] * walkParam.PG+0.5))
			{
				iphase = istep - static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_LEFT] * walkParam.PG+0.5);
				phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] * walkParam.PG+0.5);
				supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_LEFT;
			}else if(istep < static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_RIGHT] * walkParam.PG+0.5))
			{
				iphase = istep - static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_DOUBLE_LEFT] * walkParam.PG+0.5);
				phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_RIGHT] * walkParam.PG+0.5);
				supmod_ = NaoConfig::SUPPORT_MODE_RIGHT;
			}else if(istep < static_cast<int>((ratioSum[NaoConfig::SUPPORT_MODE_RIGHT] + walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT] /2 ) * walkParam.PG+0.5))
			{
				iphase = istep - static_cast<int>(ratioSum[NaoConfig::SUPPORT_MODE_RIGHT] * walkParam.PG+0.5);
				phaseLen = static_cast<int>(walkParam.sLen[NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT] / 2 * walkParam.PG+0.5);
				supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT;
			}
			else
			{
				//Now nao stand still
				iphase = 0;
				phaseLen = 1;
				supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT;
			}
			//interpolate between key points
			//ASSERT(phaseLen > 0)
			if(phaseLen <= 0)
				phaseLen = 1;
			ratio = static_cast<double>(iphase) / phaseLen;
			ASSERT((ratio >= 0 && ratio <= 1));
			newZMP = interpolate(lastKeyZMP, nextKeyZMP, ratio, Interpolate_Linear);
			newCOMz =  interpolate(lastKeyCOMz, nextKeyCOMz, ratio, Interpolate_Linear);
		}
	}else if(walkStatus == WalkTypeBase::standing)
	{
		iphase = 0;
		phaseLen = 1;
		if(stopOnLeft)
		{
			supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_LEFT;
		}else
		{
			supmod_ = NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT;
		}
		ASSERT(phaseLen > 0);
		ratio = static_cast<double>(iphase) / phaseLen;
		ASSERT((ratio >= 0 && ratio <= 1));
		newZMP = interpolate(lastKeyZMP, nextKeyZMP, ratio, Interpolate_Linear);
		newCOMz =  interpolate(lastKeyCOMz, nextKeyCOMz, ratio, Interpolate_Linear);

	}
}

void PreBufferGenerator::generateNewSwingFootPos(Vector3<double>& pfootnew, Vector3<double>& rfootnew,int supmod_,  int iphase, int phaseLen)
{
	//Vector3<double> pfootnew, rfootnew;
	// 	int iphase, phaseLen;
	// 	int supmod_ = 0;
	Vector3<double> pStart, pEnd;
	if(supmod_ == NaoConfig::SUPPORT_MODE_LEFT || supmod_ == NaoConfig::SUPPORT_MODE_RIGHT)
	{
		pStart = Vector3<double>(lastSupZMPp.x, lastSupZMPp.y, 0);
		pEnd = Vector3<double>(nextSupZMPp.x, nextSupZMPp.y, 0);
		//===add zmp offset
		// TODO: add foot rotation
		if(supmod_ == NaoConfig::SUPPORT_MODE_LEFT)
		{
			pStart = pStart - Vector3<double>(walkParam.ZmpOffsetRight.x, walkParam.ZmpOffsetRight.y, 0);
			pEnd = pEnd -  Vector3<double>(walkParam.ZmpOffsetRight.x, walkParam.ZmpOffsetRight.y, 0);
		}else
		{
			pStart = pStart - Vector3<double>(walkParam.ZmpOffsetLeft.x, walkParam.ZmpOffsetLeft.y, 0);
			pEnd = pEnd -  Vector3<double>(walkParam.ZmpOffsetLeft.x, walkParam.ZmpOffsetLeft.y, 0);
		}
		// Genrate and interpolate for new foot trajectory
		interpolateSwingFootLocus(pfootnew, rfootnew, pStart, pEnd, lastSupFootrot, nextSupFootrot, iphase, phaseLen);
	}else
	{
		//double supporting part
		pfootnew = Vector3<double>(nextSupZMPp.x, nextSupZMPp.y, 0);
		if(supmod_ == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
			pfootnew = pfootnew - Vector3<double>(walkParam.ZmpOffsetRight.x, walkParam.ZmpOffsetRight.y, 0);
		else
			pfootnew = pfootnew - Vector3<double>(walkParam.ZmpOffsetLeft.x, walkParam.ZmpOffsetLeft.y, 0);
		//3 Maybe move it to executing stage
		pfootnew.z = NaoConfig::getInstance()->FootHeight;
		rfootnew = nextSupFootrot;//Vector3<double>(0,0, nextSupFootrot.z);
	}
}

// TODO: Add more types of locus
void PreBufferGenerator::interpolateSwingFootLocus(Vector3<double>& pnew, Vector3<double>& rnew,
							   const Vector3<double>& pStart, const Vector3<double>& pEnd,
							   const Vector3<double>& rStart, const Vector3<double>& rEnd, int iphase, int phaseLen)
{
		Vector3<double> pVec = pEnd - pStart;
		pVec.z = 0;
		double r = atan2(pVec.y, pVec.x);
		double a = pVec.abs() / (2 * pi);
		double theta = 2 * pi * iphase / phaseLen;
		double stepHeight = CLIP(walkParam.stepHeight, 0.025);
		Vector3<double> tmp;
		tmp.x = a * (theta - sin(theta));
		tmp.y = 0;
		tmp.z = stepHeight * ( 1- cos(theta)) + NaoConfig::getInstance()->FootHeight+0.005;//3 move NaoConfig::FootHeight to executing stage
		tmp = RotationMatrix::getRotationZ(r) * tmp;
		pnew = pStart + tmp;
		rnew =(rEnd - rStart ) * iphase / phaseLen + rStart;
}
