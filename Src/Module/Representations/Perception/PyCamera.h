#ifndef PyCamera_H
#define PyCamera_H

#include "Module/Representations/Perception/CameraConfig.h"
#include <boost/python.hpp>
#include <boost/python/tuple.hpp>
using namespace std;
using namespace boost;
using namespace boost::python;

void c_init_camera();
void set_camera_reference (const CameraConfig *_cameraConfig);


#endif