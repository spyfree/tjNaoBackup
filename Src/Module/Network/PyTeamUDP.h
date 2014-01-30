#ifndef PyTeamUDP_H
#define PyTeamUDP_H
#include "Module/Network/networkComm/Wdatainfo.h"
#include "Module/Network/TeamMateInfo.h"
#include <boost/python.hpp>
#include <boost/python/list.hpp>
using namespace std;
using namespace boost;
using namespace boost::python;

class MateInfo
{
public:
	MateInfo();
	~MateInfo();

	const int getPlayerNumber();
	const float getBallLocationX();
	const float getBallLocationY();
	const float getBallCellX();
	const float getBallCellY();
	const int getState();
	boost::python::dict GetMateDict();

private:
	const TeamMateInfo* matesInfo;

};




void c_init_teamUDP();
void set_teamUDP_reference (TeamUDPData *_teamUDPdata,TeamMateInfo*_teammateData);


#endif
