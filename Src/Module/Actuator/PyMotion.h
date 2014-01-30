#ifndef PyMotion_H
#define PyMotion_H
#include "Module/Common/MotionCmd.h"
#include <boost/python.hpp>
#include <boost/python/tuple.hpp>
using namespace std;
using namespace boost;
using namespace boost::python;

void c_init_Motion();
void set_motion_reference (MotionCmd *_motionCmd);

#endif