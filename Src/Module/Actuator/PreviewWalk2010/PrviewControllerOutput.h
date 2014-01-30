#ifndef __PrviewControllerOutput_h_
#define __PrviewControllerOutput_h_
#include "Tools/Math/Vector3.h"
class PrviewControllerOutput
{
public:
	Vector3<double> p;
	Vector3<double> v;
	Vector3<double> a;
};
class PrviewControllerState : public PrviewControllerOutput
{

};
#endif