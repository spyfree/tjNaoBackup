#include "PreBufferGenerator.h"
#include "Module/Common/NaoConfig.h"
#include "Platform/GTAssert.h"
//#include "../MatlabHelp/MatlabHelp.h"
#include <iostream>
const double MAX_FORWARD_X = 0.2; //m/step
const double MAX_BACKWARD_X = -0.05;
const double MAX_LEFT_Y = 0.04;
const double MAX_TURNCCW = deg2rad(30);//rad/step
const double MIN_SPEED_X = 0.01;
const double MIN_SPEED_Y = 0.01;
const double MIN_TURNCCW = deg2rad(2);
PreBufferGenerator::PreBufferGenerator()
:initialized(false), cF(0), numPreviews(80),preparetoStop(false)
{
	previewBuffer.set_capacity(numPreviews);
}

void PreBufferGenerator::reset(const WalkCmd& walkCmd, const NaoStructure& naoStructure)
{
	preparetoStop = false;
	standingSx =walkParam.sx;
	//standingSy = fabs(naoStructure.uLink[NaoStructure::lFootLink].p.y - naoStructure.uLink[NaoStructure::rFootLink].p.y) - (walkParam.ZmpOffsetRight.y - walkParam.ZmpOffsetLeft.y);
	standingSy = walkParam.sy;
	standingCoMz = naoStructure.pCOM.z;

	walkingSy = walkParam.sy;
	walkingSz = 0;
	walkingCoMz = walkParam.Zc;
//	changeSpeed(walkCmd);
	leftStopLanding = false;
	if(walkCmd.speed.translation.y > 0 || walkCmd.speed.rotation > 0)
		firstStepLeg = NaoConfig::LegIDLeft;
	else
		firstStepLeg = NaoConfig::LegIDRight;
	int sum = 0;
	for(int i = 0; i < 4; ++i)
	{
		phaseLens[i] = static_cast<int>(walkParam.sLen[i] * walkParam.PG +0.5);
		sum += phaseLens[i];
	}
	ASSERT(sum == walkParam.PG);
	preparetoStop = false;
	finishFinalStep = false;
	walkStatus = WalkTypeBase::deactive;
	istep = 0;
// 	pF = 0;
// 	bF = 0;
	if(firstStepLeg == NaoConfig::LegIDLeft){
		swgFInfo.pStart = Vector3<double>(-walkParam.ZmpOffsetLeft.x, -standingSy/2 - walkParam.ZmpOffsetLeft.y, 0);
		swgFInfo.pEnd = Vector3<double>(-walkParam.ZmpOffsetLeft.x, -standingSy/2 - walkParam.ZmpOffsetLeft.y, 0);
	}else{
		swgFInfo.pStart = Vector3<double>(-walkParam.ZmpOffsetLeft.x, standingSy/2 - walkParam.ZmpOffsetRight.y, 0);
		swgFInfo.pEnd = Vector3<double>(-walkParam.ZmpOffsetLeft.x, standingSy/2 - walkParam.ZmpOffsetRight.y, 0);
	}
	swgFInfo.rStart = Vector3<double>(0.0,0.0,0.0); swgFInfo.rEnd = Vector3<double>(0.0,0.0,0.0);
	swgFInfo.fStart = 0; swgFInfo.fEnd = 0;
	//======Generate Initial Knots========//
	knotList.clear();
	genStartKnot();
	initialized = true;
}
void PreBufferGenerator::changeSpeed(const WalkCmd& cmd, const NaoStructure& naoStructure)
{
	//walking Sx
	walkingSx = CLIP(cmd.speed.translation.x * (walkParam.PG * walkParam.dt) , MAX_BACKWARD_X, MAX_FORWARD_X);
	if(fabs(walkingSx) < MIN_SPEED_X)
		walkingSx = 0;
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
		walkStatus = WalkTypeBase::standing;
	}
	if(walkStatus == WalkTypeBase::standing && !cmdStand)
	{
		reset(cmd, naoStructure);
		walkStatus = WalkTypeBase::running;

	}
	if(cmdStand){
 		setToStop();
	}
}
bool PreBufferGenerator::checkSpeedStand(const WalkCmd& cmd)
{
	return (fabs(cmd.speed.translation.x)< MIN_SPEED_X && fabs(cmd.speed.translation.y) <MIN_SPEED_Y
		&& fabs(cmd.speed.rotation) < MIN_TURNCCW);
}
void PreBufferGenerator::genStartKnot()
{
	previewBuffer.clear();
	//knotList.clear();
	if(firstStepLeg == NaoConfig::LegIDRight)
	{
		PreviewState state(NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT, WalkTypeBase::starting, standingCoMz, Vector3<double>(0,0,0));
		//PreviewState state(NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT, WalkTypeBase::starting, standingCoMz, Vector3<double>(0,standingSy/2,0));
		state.pSupFootZMP = Vector3<double>(-walkParam.ZmpOffsetLeft.x,  -standingSy/2 - walkParam.ZmpOffsetRight.y, 0);
		addKnot(state, 0);//starting knot
		state.COMz = walkingCoMz;
		addKnot(state, 20);//squatting
		addKnot(state, 10);//wait a moment
		state.pZMP.y = standingSy/2;
		state.pSupFootZMP = Vector3<double>(-walkParam.ZmpOffsetRight.x, standingSy/2 - walkParam.ZmpOffsetLeft.y, 0);
		state.supmod = NaoConfig::SUPPORT_MODE_LEFT;
		state.walkStatus = WalkTypeBase::running;
		addKnot(state, 20);//moving CoM to Left Leg
		//addKnot(state, 5);
	}else
	{
		PreviewState state(NaoConfig::SUPPORT_MODE_DOUBLE_LEFT, WalkTypeBase::starting, standingCoMz, Vector3<double>(0,0,0));
		//PreviewState state(NaoConfig::SUPPORT_MODE_DOUBLE_LEFT, WalkTypeBase::starting, standingCoMz, Vector3<double>(0,-standingSy/2,0));
		state.pSupFootZMP = Vector3<double>(-walkParam.ZmpOffsetRight.x, standingSy/2 - walkParam.ZmpOffsetLeft.y, 0);
		addKnot(state, 0);//starting knot
		state.COMz = walkingCoMz;
		addKnot(state, 20);//squatting
		addKnot(state, 10);//wait a moment
		state.pZMP.y = -standingSy/2;
		state.pSupFootZMP = Vector3<double>(-walkParam.ZmpOffsetLeft.x,  -standingSy/2 - walkParam.ZmpOffsetRight.y, 0);
		state.supmod = NaoConfig::SUPPORT_MODE_RIGHT;
		state.walkStatus = WalkTypeBase::running;
		addKnot(state, 20);//moving CoM to Right Leg
		//addKnot(state, 5);
	}
}
int PreBufferGenerator::addKnot(const PreviewState& state, int fnum)
{
	Knot knot = state;
	if(knotList.size() > 0)
	{
		Knot& lastKnot = knotList.back();
		lastKnot.fnum = fnum;
		knot.fstamp = lastKnot.fstamp + lastKnot.fnum;
	}else
		knot.fstamp = cF;
	knot.fnum = -1;//unknown, this is only known when next knot comes
	//=======
	knotList.push_back(knot);
	return knot.fstamp;
}

void PreBufferGenerator::generateNext()
{
	ASSERT(initialized);
	const Knot* cKnot = 0;
	bool moveBufferWindow = false;
	cKnot = findCurrentKnot();
	if(!cKnot || cKnot->walkStatus != WalkTypeBase::standing){
		moveBufferWindow = true;
	}
	cF++;
	if(!moveBufferWindow){
		walkStatus = WalkTypeBase::standing;
		return;
	}
	if(knotList.empty())
		genStartKnot();
	// Before Delete the old Knot, we calculate the Start and End of swing foot
	checkSwingFoot();
	// Delete old knot
	deleteOldKnot();
	// Generate new knot
	const Knot* lastKnot = &(knotList.back());
	while(!preparetoStop && cF + numPreviews >= lastKnot->fstamp)
	{
		genNextWalkKnot();
		lastKnot = &(knotList.back());
	}
	if( !previewBuffer.empty())
		previewBuffer.pop_front();
	fillPreviewBuffer();
	if(!previewBuffer.empty())
	{
		walkStatus = previewBuffer[0].walkStatus;
	}
	//===print knot=====
	PROCESS_WIDE_STORAGE_STATIC const Knot* pknotlast = 0;
	const Knot* pknot = 0;
	if((pknot = findCurrentKnot()) && pknot != pknotlast)
	{
		/*MATLABHELP_PLOT(pknot->fstamp, "Knotfstamp");
		MATLABHELP_PLOT(pknot->fnum, "Knotfnum");
		MATLABHELP_PLOT(pknot->COMz, "KnotfCOMz");
		MATLABHELP_PLOT(pknot->pZMP.x,"KnotZMPx");
		MATLABHELP_PLOT(pknot->pZMP.y,"KnotZMPy");
		MATLABHELP_PLOT(pknot->pZMP.z,"KnotZMPz");
		MATLABHELP_PLOT(pknot->pSupFootZMP.x,"KnotSupFootx");
		MATLABHELP_PLOT(pknot->pSupFootZMP.y,"KnotSupFooty");
		MATLABHELP_PLOT(pknot->pSupFootZMP.z,"KnotSupFootz");*/
		pknotlast = pknot;
	}
	//=======Print PreviewBuffer======
	const PreviewState& state = previewBuffer.front();
	/*MATLABHELP_PLOT(state.pZMP.x, "StateZMPx");
	MATLABHELP_PLOT(state.pZMP.y, "StateZMPy");
	MATLABHELP_PLOT(state.pZMP.z, "StateZMPz");
	MATLABHELP_PLOT(state.supmod,"Statesupmod");
	MATLABHELP_PLOT(state.walkStatus,"StatewalkStatus");
	MATLABHELP_PLOT(state.pSupFootZMP.x,"StateSupFx");
	MATLABHELP_PLOT(state.pSupFootZMP.y,"StateSupFy");
	MATLABHELP_PLOT(state.pSupFootZMP.z,"StateSupFz");*/
}

void PreBufferGenerator::checkSwingFoot()
{
	std::list< Knot >::const_iterator itr = knotList.begin();
	std::list< Knot >::const_iterator itrNext = itr;
	itrNext++;
	while(itr!= knotList.end())
	{
		const Knot& knot = *itr;
		const Knot& knotNow = *itrNext;
		if(knotNow.fstamp == cF)
		{
			//knot before cF found
			if(knotNow.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT){
				swgFInfo.pStart = swgFInfo.pEnd;
				swgFInfo.rStart = swgFInfo.rEnd;
				swgFInfo.fStart = swgFInfo.fEnd;
			}else if(knotNow.supmod == NaoConfig::SUPPORT_MODE_RIGHT){
				swgFInfo.pStart = knot.pSupFootZMP;
				swgFInfo.rStart = knot.rSupFootZMP;
				swgFInfo.fStart = knotNow.fstamp;//Single_Right Start time
				//find swg end, ther first single left supporting
				std::list< Knot >::const_iterator itrEnd = itr;
				while(itrEnd != knotList.end())
				{
					const Knot& knotEnd = *itrEnd;
					if(knotEnd.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT)
					{
						swgFInfo.fEnd = knotEnd.fstamp;
						if(preparetoStop)
						{
							swgFInfo.pEnd = knotEnd.pSupFootZMP + RotationMatrix().rotateZ(knotEnd.rSupFootZMP.z)*(Vector3<double>(0, walkingSy, 0));
							swgFInfo.rEnd = knotEnd.rSupFootZMP;
							break;
						}
					}
					else if(!preparetoStop && knotEnd.supmod == NaoConfig::SUPPORT_MODE_LEFT)
					{
						swgFInfo.pEnd = knotEnd.pSupFootZMP;
						swgFInfo.rEnd = knotEnd.rSupFootZMP;
						break;
					}
					itrEnd++;
				}
				//End swg not previewed yet, so we calculate it
				if(itrEnd == knotList.end())
				{
					swgFInfo.pEnd = swgFInfo.pStart
						+ RotationMatrix::getRotationZ(swgFInfo.rStart.z) * Vector3<double>(walkingSx, -walkingSy + walkingdLeft, walkingSz)
						+ RotationMatrix::getRotationZ(swgFInfo.rStart.z + walkingTurnCCW/2) * Vector3<double>(walkingSx, walkingSy + walkingdLeft, walkingSz);
					swgFInfo.rEnd = swgFInfo.rStart + Vector3<double>(0,0,walkingTurnCCW);
					swgFInfo.fEnd = swgFInfo.fStart + (phaseLens[NaoConfig::SUPPORT_MODE_RIGHT]);
				}
			}else if(knotNow.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT){
				swgFInfo.pStart = swgFInfo.pEnd;
				swgFInfo.rStart = swgFInfo.rEnd;
				swgFInfo.fStart = swgFInfo.fEnd;
			}else if(knotNow.supmod == NaoConfig::SUPPORT_MODE_LEFT){
				swgFInfo.pStart = knot.pSupFootZMP;
				swgFInfo.rStart = knot.rSupFootZMP;
				swgFInfo.fStart = knot.fstamp + knot.fnum;//Single_Left Start time
				//find swg end
				std::list< Knot >::const_iterator itrEnd = itr;
				while(itrEnd != knotList.end())
				{
					const Knot& knotEnd = *itrEnd;
					if(knotEnd.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
					{
						swgFInfo.fEnd = knotEnd.fstamp;
						if(preparetoStop)
						{
							swgFInfo.pEnd = knotEnd.pSupFootZMP + RotationMatrix().rotateZ(knotEnd.rSupFootZMP.z)*(Vector3<double>(0, -walkingSy, 0));
							swgFInfo.rEnd = knotEnd.rSupFootZMP;
							break;
						}
					}
					else if(!preparetoStop && knotEnd.supmod == NaoConfig::SUPPORT_MODE_RIGHT)
					{
						swgFInfo.pEnd = knotEnd.pSupFootZMP;
						swgFInfo.rEnd = knotEnd.rSupFootZMP;
						break;
					}
					itrEnd++;
				}
				//End swg not previewed yet, so we calculate it
				if(itrEnd == knotList.end())
				{
					swgFInfo.pEnd = swgFInfo.pStart
						+ RotationMatrix::getRotationZ(swgFInfo.rStart.z) * Vector3<double>(walkingSx, walkingSy + walkingdLeft, walkingSz)
						+ RotationMatrix::getRotationZ(swgFInfo.rStart.z + walkingTurnCCW/2) * Vector3<double>(walkingSx, -walkingSy + walkingdLeft, walkingSz);
					swgFInfo.rEnd = swgFInfo.rStart  + Vector3<double>(0,0,walkingTurnCCW);
					swgFInfo.fEnd = swgFInfo.fStart + (phaseLens[NaoConfig::SUPPORT_MODE_LEFT]);
				}
			}
		}else if(knot.fstamp >= cF)
			break;
		itr++;
		itrNext++;
	}
}

void PreBufferGenerator::deleteOldKnot()
{
	ASSERT(knotList.size() > 1);
	std::list< Knot >::const_iterator itr = knotList.begin();
	itr++;
	while(!knotList.empty() && itr != knotList.end() && (*itr).fstamp < cF)
	{
		knotList.pop_front();
		itr++;
	}
}

void PreBufferGenerator::genNextWalkKnot()
{
	const Knot& lastKnot = knotList.back();
	Knot nextKnot;
	// new supmod
	nextKnot.supmod = lastKnot.supmod+1;
	if(nextKnot.supmod > NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT)
		nextKnot.supmod = NaoConfig::SUPPORT_MODE_LEFT;
	// fnum
	int fnum = static_cast<int>(walkParam.sLen[lastKnot.supmod] * walkParam.PG+0.5) ;
	// walkStatus
	nextKnot.walkStatus = WalkTypeBase::running;
	// CoMz
	// TODO: adding walkingdCoMz so robot can walk up stairs
	nextKnot.COMz = lastKnot.COMz; // + dCoMz

	// rSupFoot// pZMP // pSupFootZMP
	// TODO: add Vector3<double>(walkingSx, walkingSy + walkingdLeft, walkingSz);
	if(nextKnot.supmod == NaoConfig::SUPPORT_MODE_LEFT)
	{
		nextKnot.rSupFootZMP.z = lastKnot.rSupFootZMP.z + walkingTurnCCW/2;
		nextKnot.pZMP = lastKnot.pZMP
			+ RotationMatrix().rotateZ(nextKnot.rSupFootZMP.z) * Vector3<double>(walkingSx, walkingSy + walkingdLeft, walkingSz);
		nextKnot.pSupFootZMP = nextKnot.pZMP
			+ RotationMatrix().rotateZ(nextKnot.rSupFootZMP.z) * Vector3<double>(-walkParam.ZmpOffsetLeft.x, -walkParam.ZmpOffsetLeft.y, 0);
	}else if(nextKnot.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		nextKnot.rSupFootZMP.z = lastKnot.rSupFootZMP.z;
		nextKnot.pZMP =  lastKnot.pZMP;
		nextKnot.pSupFootZMP = lastKnot.pSupFootZMP;
	}else if(nextKnot.supmod == NaoConfig::SUPPORT_MODE_RIGHT)
	{
		nextKnot.rSupFootZMP.z = lastKnot.rSupFootZMP.z + walkingTurnCCW/2;
		nextKnot.pZMP = lastKnot.pZMP
			+ RotationMatrix().rotateZ(nextKnot.rSupFootZMP.z) * Vector3<double>(walkingSx, -walkingSy + walkingdLeft, walkingSz);
		nextKnot.pSupFootZMP = nextKnot.pZMP
			+ RotationMatrix().rotateZ(nextKnot.rSupFootZMP.z) * Vector3<double>(-walkParam.ZmpOffsetRight.x, -walkParam.ZmpOffsetRight.y, 0);
	}else// if(nextKnot.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT)
	{
		nextKnot.rSupFootZMP.z = lastKnot.rSupFootZMP.z;
		nextKnot.pZMP = lastKnot.pZMP;
		nextKnot.pSupFootZMP = lastKnot.pSupFootZMP;
	}
	addKnot(nextKnot, fnum);
}
void PreBufferGenerator::genStopKnot()
{
	std::list< Knot >::iterator itr = knotList.begin();
	const Knot *pthis = 0;
	Knot *pThat = 0;
	Knot newKnot;
	bool startDelete = false;
	while(itr != knotList.end())
	{
		pthis = &(*itr);
		if(!startDelete){
			if(cFInKnot(pthis))
			{
				if(pthis->supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT || pthis->supmod == NaoConfig::SUPPORT_MODE_LEFT)
				{
					while(itr != knotList.end())
					{
						pThat = &(*itr);
						if(pThat->supmod == NaoConfig::SUPPORT_MODE_RIGHT)
							break;
						itr++;
					}
					startDelete = true;
					leftStopLanding = true;
				}else if(pthis->supmod == NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT || pthis->supmod == NaoConfig::SUPPORT_MODE_RIGHT)
				{
					while(itr != knotList.end())
					{
						pThat = &(*itr);
						if(pThat->supmod == NaoConfig::SUPPORT_MODE_LEFT)
							break;
						itr++;
					}
					startDelete = true;
					leftStopLanding = false;
				}
			}
			if(itr != knotList.end())
				itr++;
		}
		else
		{
			itr =knotList.erase(itr);
		}
	}
	//====Adding Stopping Knot============
	if(leftStopLanding)
	{
		int fnum;
		pThat->fnum *= 0.75;
		newKnot = *pThat;
		newKnot.walkStatus = WalkTypeBase::ending;
		newKnot.supmod = NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT;
		newKnot.fstamp = pThat->fstamp + pThat->fnum;
		addKnot(newKnot, pThat->fnum);
		fnum = static_cast<int>(walkParam.sLen[newKnot.supmod] * walkParam.PG+0.5)/2;
		newKnot.pZMP = pThat->pZMP + RotationMatrix().rotateZ(pThat->rSupFootZMP.z)*(Vector3<double>(0, standingSy/2, 0));
		addKnot(newKnot, fnum);
		fnum = 20;
		newKnot.walkStatus = WalkTypeBase::standing;
		addKnot(newKnot, fnum);
		fnum = 100;
		addKnot(newKnot, fnum);
	}else
	{
		int fnum;
		pThat->fnum *= 0.75;
		newKnot = *pThat;
		newKnot.walkStatus = WalkTypeBase::ending;
		newKnot.supmod = NaoConfig::SUPPORT_MODE_DOUBLE_LEFT;
		newKnot.fstamp = pThat->fstamp + pThat->fnum;
		addKnot(newKnot, pThat->fnum);
		fnum = static_cast<int>(walkParam.sLen[newKnot.supmod] * walkParam.PG+0.5)/2;
		newKnot.pZMP = pThat->pZMP + RotationMatrix().rotateZ(pThat->rSupFootZMP.z)*(Vector3<double>(0, -standingSy/2, 0));
		addKnot(newKnot, fnum);
		fnum = 20;
		newKnot.walkStatus = WalkTypeBase::standing;
		addKnot(newKnot, fnum);
		fnum = 100;
		addKnot(newKnot, fnum);
	}
	previewBuffer.clear();
	fillPreviewBuffer();
}
void PreBufferGenerator::fillPreviewBuffer()
{
	ASSERT(knotList.size() > 1);
	int iframe = cF + previewBuffer.size();
	const Knot *pthis = 0, *plast = &(knotList.back());
	std::list< Knot >::const_iterator itr = knotList.begin();
	while(itr != knotList.end())
	{
		pthis = &(*itr);
		while(!previewBuffer.full() && (iframe >= plast->fstamp && iframe < pthis->fstamp))
		{
			int i = iframe - plast->fstamp;
			double ratio = static_cast<double>(i) / plast->fnum;
			ASSERT(ratio >= 0  & ratio < 1);
			PreviewState state;
			state.supmod = plast->supmod;
			state.walkStatus = plast->walkStatus;
			state.COMz = interpolate(plast->COMz, pthis->COMz, ratio, Interpolate_Linear);
			state.pZMP = interpolate(plast->pZMP, pthis->pZMP, ratio, Interpolate_Linear);
			state.pSupFootZMP =plast->pSupFootZMP;
			state.rSupFootZMP =plast->rSupFootZMP;
			previewBuffer.push_back(state);
			iframe ++;
		}
		plast = pthis;
		itr ++;
	}
}

void PreBufferGenerator::doLandingEvent(const Vector3<double>& pStepError, const Vector3<double>& rStepError)
{
	modifyStepPosition(pStepError, rStepError, 0);
}
void PreBufferGenerator::modifyStepPosition(const Vector3<double>& pStepError, const Vector3<double>& rStepError, int fDelay)
{
	if(preparetoStop)
		return;
	std::list< Knot >::iterator itr = knotList.begin();
	const Knot *pthis = 0;
	const Knot* pknot2 = 0;
	Knot nextKnot[3];
	int fnum[3];
	int nAdd = 0;
	bool startDelete = false;
	while(itr != knotList.end())
	{
		pthis = &(*itr);
		if(!startDelete)
		{
			if(cFInKnot(pthis))
			{
				if(pthis->supmod == NaoConfig::SUPPORT_MODE_LEFT)
				{
					ASSERT(findNextKnot(&pknot2, itr, NaoConfig::SUPPORT_MODE_DOUBLE_LEFT));
					nextKnot[0] = *pknot2;
					fnum[0] = cF - pthis->fstamp + fDelay;
					nextKnot[0].fstamp = cF + fDelay;

					fnum[1] = pknot2->fnum;// TODO: optimize this
					ASSERT(findNextKnot(&pknot2, itr, NaoConfig::SUPPORT_MODE_RIGHT));
					nextKnot[1] = *pknot2;
					nextKnot[1].fstamp = nextKnot[0].fstamp + fnum[1];
					nextKnot[1].pSupFootZMP = pknot2->pSupFootZMP + pStepError;
					nextKnot[1].pZMP = pknot2->pZMP + pStepError;
					nextKnot[1].rSupFootZMP = pknot2->rSupFootZMP + rStepError;
					nAdd = 2;
					startDelete = true;
					swgFInfo.pEnd += pStepError;
					swgFInfo.rEnd += rStepError;
					swgFInfo.fEnd = nextKnot[0].fstamp;
				}else if(pthis->supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
				{
					fnum[0] = pthis->fnum;
					ASSERT(findNextKnot(&pknot2, itr, NaoConfig::SUPPORT_MODE_RIGHT));
					nextKnot[0] = *pknot2;
					nextKnot[0].pSupFootZMP = pknot2->pSupFootZMP + pStepError;
					nextKnot[0].pZMP = pknot2->pZMP + pStepError;
					nextKnot[0].rSupFootZMP = pknot2->rSupFootZMP + rStepError;
					nAdd = 1;
					startDelete = true;
					swgFInfo.pEnd += pStepError;
					swgFInfo.rEnd += rStepError;
					swgFInfo.fEnd = nextKnot[0].fstamp;
				}else if(pthis->supmod == NaoConfig::SUPPORT_MODE_RIGHT)
				{
					ASSERT(findNextKnot(&pknot2, itr, NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT));
					nextKnot[0] = *pknot2;
					fnum[0] = cF - pthis->fstamp + fDelay;
					nextKnot[0].fstamp = cF + fDelay;

					fnum[1] = pknot2->fnum;// TODO: optimize this
					ASSERT(findNextKnot(&pknot2, itr, NaoConfig::SUPPORT_MODE_LEFT));
					nextKnot[1] = *pknot2;
					nextKnot[1].fstamp = nextKnot[0].fstamp + fnum[1];
					nextKnot[1].pSupFootZMP = pknot2->pSupFootZMP + pStepError;
					nextKnot[1].pZMP = pknot2->pZMP + pStepError;
					nextKnot[1].rSupFootZMP = pknot2->rSupFootZMP + rStepError;
					nAdd = 2;
					startDelete = true;
					swgFInfo.pEnd += pStepError;
					swgFInfo.rEnd += rStepError;
					swgFInfo.fEnd = nextKnot[0].fstamp;
				}else
				{
					fnum[0] = pthis->fnum;
					ASSERT(findNextKnot(&pknot2, itr, NaoConfig::SUPPORT_MODE_LEFT));
					nextKnot[0] = *pknot2;
					nextKnot[0].pSupFootZMP = pknot2->pSupFootZMP + pStepError;
					nextKnot[0].pZMP = pknot2->pZMP + pStepError;
					nextKnot[0].rSupFootZMP = pknot2->rSupFootZMP + rStepError;
					nAdd = 1;
					startDelete = true;
					swgFInfo.pEnd += pStepError;
					swgFInfo.rEnd += rStepError;
					swgFInfo.fEnd = nextKnot[0].fstamp;
				}
			}
			itr++;
		}else
		{
			itr =knotList.erase(itr);
		}
	}
	// preAdd some knot
	ASSERT(nAdd <= 3);
	for(int i = 0; i < nAdd; ++i)
	{
		addKnot(nextKnot[i], fnum[i]);
	}
	// Generate new knot
	const Knot* lastKnot = &(knotList.back());
	while(cF + numPreviews >= lastKnot->fstamp)
	{
		genNextWalkKnot();
		lastKnot = &(knotList.back());
	}
	previewBuffer.clear();
	fillPreviewBuffer();
}
bool PreBufferGenerator::findNextKnot(const PreBufferGenerator::Knot** pknot, std::list< PreBufferGenerator::Knot >::iterator& itr, int supmod)
{
	std::list< Knot >::const_iterator itr2 = itr;
	while(itr2 != knotList.end())
	{
		if(itr2->supmod == supmod){
			*pknot = &(*itr2);
			return true;
		}
		itr2++;
	}
	return false;
}

const PreBufferGenerator::Knot* PreBufferGenerator::findNextKnot()
{
	std::list< Knot >::const_iterator itr = knotList.begin();
	while(itr != knotList.end())
	{
		if(itr->fstamp > cF)
		{
			return &(*itr);
		}
		itr++;
	}
	return 0;
}

const PreBufferGenerator::Knot* PreBufferGenerator::findCurrentKnot()
{
	if(!(knotList.empty()))
	{
		std::list< Knot >::const_iterator itr = knotList.begin();
		while(itr != knotList.end())
		{
			if(cFInKnot(&(*itr)))
			{
				return &(*itr);
			}
			itr++;
		}
	}
	return 0;
}
void PreBufferGenerator::getCurrentSwgF(Vector3<double>& pnew, Vector3<double>& rnew, int ileg)
{
	if((swgFInfo.fEnd-swgFInfo.fStart) >0 )
		interpolateSwingFootLocus(pnew, rnew, swgFInfo.pStart, swgFInfo.pEnd,
			swgFInfo.rStart, swgFInfo.rEnd, (cF - swgFInfo.fStart), (swgFInfo.fEnd-swgFInfo.fStart), ileg);
	else{
		pnew = swgFInfo.pStart;
		rnew = swgFInfo.rStart;
		return;
	}
	Vector3<double> pVec = swgFInfo.pEnd - swgFInfo.pStart;
	if(pVec.abs()<0.01)
	{
		pnew = swgFInfo.pStart;
	}
	if((swgFInfo.rEnd - swgFInfo.rStart).abs() < 0.0001)
	{
		rnew = swgFInfo.rStart;
	}
}
//TODO: Add more types of locus and interpolate pnew.z
void PreBufferGenerator::interpolateSwingFootLocus(Vector3<double>& pnew, Vector3<double>& rnew,
							   const Vector3<double>& pStart, const Vector3<double>& pEnd,
							   const Vector3<double>& rStart, const Vector3<double>& rEnd, int iphase, int phaseLen, int ileg)
{
		Vector3<double> pVec = pEnd - pStart;
		double r = atan2(pVec.y, pVec.x);
		double a = pVec.abs() / (2 * pi);
		double theta = 2 * pi * iphase / phaseLen;
		double stepHeight = CLIP(walkParam.stepHeight, 0.05);
		Vector3<double> tmp;
		tmp.x = a * (theta - sin(theta));
// 		if(fabs(r) < pi_4 || fabs(r) > pi_4 * 3)
// 		{
// 			if(ileg == NaoConfig::LegIDLeft)
// 				tmp.y = 0.015 * (theta - sin(theta));
// 			else
// 				tmp.y = -0.015 * (theta - sin(theta));
// 		}
// 		else
			tmp.y  = 0;
		tmp.z = stepHeight/2 * ( 1- cos(theta));// +0.005;
		tmp = RotationMatrix::getRotationZ(r) * tmp;
		pnew = pStart + tmp;
		rnew =(rEnd - rStart ) * iphase / phaseLen + rStart;
}

