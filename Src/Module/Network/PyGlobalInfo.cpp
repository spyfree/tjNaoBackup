#include "PyGlobalInfo.h"
const static GlobalInfo* globalInfo_reference=0;
GlobalInfoModule::GlobalInfoModule()
{
	globalInfo=globalInfo_reference;
}
GlobalInfoModule::~GlobalInfoModule()
{

}
void set_GlobalInfo_reference(GlobalInfo* _globalInfo)
{
	globalInfo_reference = _globalInfo;
}
//======================Ball Information==============================
const double GlobalInfoModule::getGlobalBallX()
{
	return globalInfo->ballLoc.x;
	//return 1.0;
}

const double GlobalInfoModule::getGlobalBallY()
{
	return globalInfo->ballLoc.y;
}

const double GlobalInfoModule::getGlobalBallcellX()
{
	return globalInfo->ballLoc.cellX;
}

const double GlobalInfoModule::getGlobalBallcellY()
{
	return globalInfo->ballLoc.cellY;
}

const double GlobalInfoModule::getBallDistanceInfo()
{
	return globalInfo->ballLoc.distance;
}

//======================keeperLoc Information==============================

const double GlobalInfoModule::getkeeperLocx()
{
	return globalInfo->keeperLoc.x;
}

const double GlobalInfoModule::getkeeperLocy()
{
	return globalInfo->keeperLoc.y;
}

const double GlobalInfoModule::getkeeperLocvalidity()
{
	return globalInfo->keeperLoc.validity;
}

//======================attack1Loc Information==============================

const double GlobalInfoModule::getattack1Locx()
{
	return globalInfo->attack1Loc.x;
}

const double GlobalInfoModule::getattack1Locy()
{
	return globalInfo->attack1Loc.y;
}

const double GlobalInfoModule::getattack1Locvalidity()
{
	return globalInfo->attack1Loc.validity;
}

//======================attack2Loc Information==============================

const double GlobalInfoModule::getattack2Locx()
{
	return globalInfo->attack2Loc.x;
}

const double GlobalInfoModule::getattack2Locy()
{
	return globalInfo->attack2Loc.y;
}

const double GlobalInfoModule::getattack2Locvalidity()
{
	return globalInfo->attack2Loc.validity;
}

//======================defenderLoc Information==============================

const double GlobalInfoModule::getdefenderLocx()
{
	return globalInfo->defenderLoc.x;
}

const double GlobalInfoModule::getdefenderLocy()
{
	return globalInfo->defenderLoc.y;
}

const double GlobalInfoModule::getdefenderLocvalidity()
{
	return globalInfo->defenderLoc.validity;
}

//=========================ball distance info======================================
const double GlobalInfoModule::getBallDis1()
{
	return globalInfo->ballDis1;
}

const double GlobalInfoModule::getBallDis2()
{
	return globalInfo->ballDis2;
}
const double GlobalInfoModule::getBallDis3()
{
	return globalInfo->ballDis3;
}
const double GlobalInfoModule::getBallDis4()
{
	return globalInfo->ballDis4;
}

//========================robot states info============================================
const int GlobalInfoModule::getState1()
{
	return globalInfo->state1;
}
const int GlobalInfoModule::getState2()
{
	return globalInfo->state2;
}
const int GlobalInfoModule::getState3()
{
	return globalInfo->state3;
}
const int GlobalInfoModule::getState4()
{
	return globalInfo->state4;
}

//========================kick num=================================================
const int GlobalInfoModule::getKickNum()
{
	return globalInfo->kickerNum;
}

//========================frames info=====================================
const int GlobalInfoModule::getFrame1()
{
	return globalInfo->frame1;
}
const int GlobalInfoModule::getFrame2()
{
	return globalInfo->frame2;
}
const int GlobalInfoModule::getFrame3()
{
	return globalInfo->frame3;
}
const int GlobalInfoModule::getFrame4()
{
	return globalInfo->frame4;
}
BOOST_PYTHON_MODULE(PyGlobalInfo)
{

	class_<GlobalInfoModule>("globalInfo")
		.def("GlobalBallX",&GlobalInfoModule::getGlobalBallX)
		.def("GlobalBallY",&GlobalInfoModule::getGlobalBallY)
		.def("GlobalBallcellX",&GlobalInfoModule::getGlobalBallcellX)
		.def("GlobalBallcellY",&GlobalInfoModule::getGlobalBallcellY)
		.def("BallDistanceInfo",&GlobalInfoModule::getBallDistanceInfo)

		.def ("keeperLocx",&GlobalInfoModule::getkeeperLocx)
		.def ("keeperLocy",&GlobalInfoModule::getkeeperLocy)
		.def("keeperLocvalidity",&GlobalInfoModule::getkeeperLocvalidity)

		.def ("attack1Locx",&GlobalInfoModule::getattack1Locx)
		.def ("attack1Locy",&GlobalInfoModule::getattack1Locy)
		.def("attack1Locvalidity",&GlobalInfoModule::getattack1Locvalidity)

		.def ("attack2Locx",&GlobalInfoModule::getattack2Locx)
		.def ("attack2Locy",&GlobalInfoModule::getattack2Locx)
		.def("attack2Locvalidity",&GlobalInfoModule::getattack2Locvalidity)

		.def ("defenderLocx",&GlobalInfoModule::getdefenderLocx)
		.def ("defenderLocy",&GlobalInfoModule::getdefenderLocy)
		.def("defenderLocvalidity",&GlobalInfoModule::getdefenderLocvalidity)
		.def("Dis1",&GlobalInfoModule::getBallDis1)
		.def("Dis2",&GlobalInfoModule::getBallDis2)
		.def("Dis3",&GlobalInfoModule::getBallDis3)
		.def("Dis4",&GlobalInfoModule::getBallDis4)
		.def("State1",&GlobalInfoModule::getState1)
		.def("State2",&GlobalInfoModule::getState2)
		.def("State3",&GlobalInfoModule::getState3)
		.def("State4",&GlobalInfoModule::getState4)

		.def("Frame1",&GlobalInfoModule::getFrame1)
		.def("Frame2",&GlobalInfoModule::getFrame2)
		.def("Frame3",&GlobalInfoModule::getFrame3)
		.def("Frame4",&GlobalInfoModule::getFrame4)

		.def("kickNum",&GlobalInfoModule::getKickNum)
		;
}
void c_init_GlobalInfo()
{
	if (!Py_IsInitialized())
	{
		Py_Initialize();
	}
	try {
		Py_Initialize();
		initPyGlobalInfo();
	} catch (error_already_set) {
		PyErr_Print();
	}
}
