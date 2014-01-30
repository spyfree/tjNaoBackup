#include"PyGameControl.h"
#include <iostream>
const static RoboCupGameControlData* GameControlData_reference=0;
const static TeamCfgInfo* TeamCfgInfo_refernce=0;
GameControlModule::GameControlModule()
{
	GameControlInterface = GameControlData_reference;
	TeamCfgInfoInterface = TeamCfgInfo_refernce;
}
GameControlModule::~GameControlModule()
{


}
const int GameControlModule::getGameState()
{
	//std::cout<<"!!!!!!!!!!!!!!!!!!!!!state in C = "<<int(GameControlInterface->state)<<std::endl;
	return GameControlInterface->state;
}
const int GameControlModule::getPenalty()
{
	int tjarkNum=(uint32)GameControlInterface->teams[0].teamNumber==TJArkTeamNum?0:1;
	//std::cout<<"!!!!!!!!!!!!!!!!!!!!myplayerNum in C ="<<TeamCfgInfo_refernce->playerNum-1<<std::endl;
	//std::cout<<"!!!!!!!!!!!!!!!!!!!!!penalty in C = "<<GameControlInterface->teams[myteamNum].players[TeamCfgInfo_refernce->playerNum-1].penalty<<std::endl;
	return GameControlInterface->teams[tjarkNum].players[TeamCfgInfo_refernce->playerNum-1].penalty;


}
const uint16 GameControlModule::getScore()
{
	int tjarkNum=(uint32)GameControlInterface->teams[0].teamNumber==TJArkTeamNum?0:1;
	return GameControlInterface->teams[tjarkNum].score;
}
const int GameControlModule::getColor()
{
	int tjarkNum=(uint32)GameControlInterface->teams[0].teamNumber==TJArkTeamNum?0:1;
	return tjarkNum;
}
const uint16 GameControlModule::getPenalSecs()
{
	int tjarkNum=(uint32)GameControlInterface->teams[0].teamNumber==TJArkTeamNum?0:1;
	return GameControlInterface->teams[tjarkNum].players[TeamCfgInfo_refernce->playerNum-1].secsTillUnpenalised;
}

const uint8 GameControlModule::getKickOff()
{
	return GameControlInterface->kickOffTeam;
}

const uint8 GameControlModule::getSecondState()
{
	return GameControlInterface->secondaryState;
}
const int GameControlModule::getPenalty1()
{
	int tjarkNum=(uint32)GameControlInterface->teams[0].teamNumber==TJArkTeamNum?0:1;
	return GameControlInterface->teams[tjarkNum].players[0].penalty;
}
const int GameControlModule::getPenalty2()
{
	int tjarkNum=(uint32)GameControlInterface->teams[0].teamNumber==TJArkTeamNum?0:1;
	return GameControlInterface->teams[tjarkNum].players[1].penalty;
}
const int GameControlModule::getPenalty3()
{
	int tjarkNum=(uint32)GameControlInterface->teams[0].teamNumber==TJArkTeamNum?0:1;
	return GameControlInterface->teams[tjarkNum].players[2].penalty;
}
const int GameControlModule::getPenalty4()
{
	int tjarkNum=(uint32)GameControlInterface->teams[0].teamNumber==TJArkTeamNum?0:1;
	return GameControlInterface->teams[tjarkNum].players[3].penalty;
}

void set_GameControl_reference(const RoboCupGameControlData * _GameControlData,const TeamCfgInfo *_TeamCfgInfo)
{
	GameControlData_reference = _GameControlData;
	TeamCfgInfo_refernce=_TeamCfgInfo;
}


BOOST_PYTHON_MODULE(PyGameControl)
{

	class_<GameControlModule>("gamecontrol")
		.def("state",&GameControlModule::getGameState)
		.def("penalty",&GameControlModule::getPenalty)
		.def("Score",&GameControlModule::getScore)
		.def("Tcolor",&GameControlModule::getColor)
		.def("SecstillUnpenal",&GameControlModule::getPenalSecs)
		.def("kickOff",&GameControlModule::getKickOff)
		.def("secondaryState",&GameControlModule::getSecondState)
		.def("penalty1",&GameControlModule::getPenalty1)
		.def("penalty2",&GameControlModule::getPenalty2)
		.def("penalty3",&GameControlModule::getPenalty3)
		.def("penalty4",&GameControlModule::getPenalty4)
		;


}
void c_init_GameControl()
{
	if (!Py_IsInitialized())
		Py_Initialize();
	try {
		initPyGameControl();
	} catch (error_already_set) {
		PyErr_Print();
	}
}



















