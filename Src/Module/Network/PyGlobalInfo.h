#ifndef PYGLOBALINFO
#define PYGLOBALINFO
#include "Module/Network/networkComm/Wdatainfo.h"
#include <boost/python.hpp>
using namespace std;
using namespace boost::python;
using namespace boost;
class GlobalInfoModule
{
public:
	GlobalInfoModule();
	~GlobalInfoModule();

	const double getGlobalBallX();
	const double getGlobalBallY();
	const double getGlobalBallcellX();
	const double getGlobalBallcellY();
	const double getBallDistanceInfo();

	const double getkeeperLocx();
	const double getkeeperLocy();
	const double getkeeperLocvalidity();

	const double getattack1Locx();
	const double getattack1Locy();
	const double getattack1Locvalidity();

	const double getattack2Locx();
	const double getattack2Locy();
	const double getattack2Locvalidity();

	const double getdefenderLocx();
	const double getdefenderLocy();
	const double getdefenderLocvalidity();

	const double getBallDis1();
	const double getBallDis2();
	const double getBallDis3();
	const double getBallDis4();

	const int getState1();
	const int getState2();
	const int getState3();
	const int getState4();

	const int getFrame1();
	const int getFrame2();
	const int getFrame3();
	const int getFrame4();

	const int getKickNum();

private:
	const GlobalInfo* globalInfo;


};

void set_GlobalInfo_reference(GlobalInfo* _globalInfo);
void c_init_GlobalInfo();

#endif