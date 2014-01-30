#ifndef __LINEPOINT_H_
#define __LINEPOINT_H_
#include "Tools/Math/Vector2.h"
class LinePoint
{
public:
	Vector2<int> pointOnLine;
	Vector2<double> normalToLine;
	int belongsToLineNo;
	bool circlePoint;
protected:
private:
};
#endif
