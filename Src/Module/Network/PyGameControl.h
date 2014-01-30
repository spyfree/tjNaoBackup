#ifndef PyGameControl_H
#define PyGameControl_H
#include "Module/Network/RoboCupGameControlData.h"
#include <boost/python.hpp>
using namespace std;
using namespace boost::python;
using namespace boost;
class GameControlModule
{
public:
	GameControlModule();
	~GameControlModule();

	const int getGameState();
	const int getPenalty();
	const uint16 getScore();
	const int getColor();
	const uint16 getPenalSecs();
	const uint8 getKickOff();
	const uint8 getSecondState();
	const int getPenalty1();
	const int getPenalty2();
	const int getPenalty3();
	const int getPenalty4();

private:
	const RoboCupGameControlData * GameControlInterface;
	const  TeamCfgInfo *TeamCfgInfoInterface;
	
};

void set_GameControl_reference(const RoboCupGameControlData * _GameControlData,const TeamCfgInfo *_TeamCfgInfo);
void c_init_GameControl();

#endif