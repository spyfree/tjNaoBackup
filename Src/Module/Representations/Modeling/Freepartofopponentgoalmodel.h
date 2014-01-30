/* the class that declare the free part of opponent goal that robot can shot at
   Author:laura
   Time:2011.5.2  20:15
*/
#ifndef FreePartOfOpponentGoalModel_H
#define FreePartOfOpponentGoalModel_H

#include"Tools/Math/Vector2.h"
class FreePartOfOpponentGoalModel
{
public:
	Vector2<double> LeftEnd; //the left point on abs field 
	Vector2<double> RightEnd; //the right point
	bool IsFree;//false :the whole goal is blocked
	//Vector2<double> LeftShotAngle;// shot angle for robot
	//Vector2<double> RightShotAngle;
};















#endif
