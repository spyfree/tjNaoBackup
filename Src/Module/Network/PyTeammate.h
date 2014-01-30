#ifndef PyTeammate_H
#define PyTeammate_H
#include "Module/Network/TeamMateInfo.h"
#include <boost/python.hpp>
#include <boost/python/tuple.hpp>
using namespace std;
using namespace boost;
using namespace boost::python;

void c_init_teammate();
void set_teammate_reference (const perceiveInfo *_locData,const TeamMateInfo*_teammateData);


#endif