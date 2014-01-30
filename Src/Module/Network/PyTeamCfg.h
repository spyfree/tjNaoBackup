#ifndef PyTeamCfg_H
#define PyTeamCfg_H
#include "Module/Network/RoboCupGameControlData.h"
#include <boost/python.hpp>
#include <boost/python/tuple.hpp>
using namespace std;
using namespace boost;
using namespace boost::python;

void c_init_teamcfg();
void set_teamcfg_reference (TeamCfgInfo *_teamcfg);


#endif