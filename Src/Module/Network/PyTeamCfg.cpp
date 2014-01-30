#include "PyTeamCfg.h"

static TeamCfgInfo* teamcfg_reference = 0;

class PyTeamConfig
{
public:
	PyTeamConfig(int playNumber)
	{
		_playNumber = playNumber;
	}
	int getPlayNumber(){ return _playNumber; }
private:
	int _playNumber;
};

class PyTeamInterface
{
public:
	PyTeamInterface()
	{
		teamcfg = teamcfg_reference;
	}
	void setTeamConfig(PyTeamConfig* command)
	{
		teamcfg->playerNum = command->getPlayNumber();
	}

private:
	TeamCfgInfo* teamcfg;
};

BOOST_PYTHON_MODULE(PyTeamCfg)
{

	class_<PyTeamConfig>("TeamData",init<int>());
	class_<PyTeamInterface>("TeamCfg")
		.def("setTeamCfg", &PyTeamInterface::setTeamConfig)
		;

}

void c_init_teamcfg()
{
	if (!Py_IsInitialized())
		Py_Initialize();

	try 
	{
		initPyTeamCfg();
	} 
	catch (error_already_set) 
	{
		PyErr_Print();
	}
}

void set_teamcfg_reference (TeamCfgInfo* _teamConfig)
{
	teamcfg_reference = _teamConfig;
}
