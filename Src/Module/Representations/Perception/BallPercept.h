#ifndef __BallPercept_h_
#define __BallPercept_h_
#include "Tools/Math/Vector2.h"
#include "Tools/Math/Vector3.h"
#include"Tools/Math/Common.h"
//#include "Module/common/VisionHeader.h"
#include <vector>
class BallPercept
{
public:
	BallPercept()
	{
		timewhenlastseen=0;}
	unsigned timewhenlastseen;
	Vector2<int> BallCenter;
	Vector3<double> coordinateRobot;
	Vector2<double> relativePositionOnField;
	double radius;
	double percentOfOrange;
	bool invion;
	double validity;

	void add(const Vector2<int>& Bcenter, const Vector3<double>& Bcoordinate, double Bradius, double BpercentOfOrange)
	{
		BallCenter = Bcenter;
		coordinateRobot = Bcoordinate;
		radius = Bradius;
		percentOfOrange = BpercentOfOrange;
		invion = true;
		relativePositionOnField.x=Bcoordinate.x;
		relativePositionOnField.y=Bcoordinate.y;
	}
	void add(const BallPercept& ballPercept)
	{
		BallCenter = ballPercept.BallCenter;
		coordinateRobot = ballPercept.coordinateRobot;
		radius = ballPercept.radius;
		percentOfOrange = ballPercept.percentOfOrange;
		invion = true;
		relativePositionOnField.x=ballPercept.coordinateRobot.x;
		relativePositionOnField.y=ballPercept.coordinateRobot.y;

	}



	/*const unsigned long getframeNumber()const{return timewhenlastseen;}
	const Vector2<int> getBallCenter()const {return BallCenter;}
	const int getBallCenterX()const{return BallCenter.x;}
	const int getBallCenterY()const{return BallCenter.y;}*/
	/*const Vector2<int> getBallPositionOnField()const
	{
	Vector2<int> BallPositionOnField;
	BallPositionOnField.x=coordinateRobot.x;
	BallPositionOnField.y=coordinateRobot.y;
	return BallPositionOnField;
	}*/
	/*const double getcoordinateRobotX()const{return coordinateRobot.x;}
	const double getcoordinateRobotY()const{return coordinateRobot.y;}
	const double getcoordinateRobotZ()const{return coordinateRobot.z;}*/
	const double getdistanceOfBall()const
	{
		Vector2<double> balldist(relativePositionOnField.x,relativePositionOnField.y);
		return balldist.abs();
	}
	/*const double getradius()const{return radius;}
	const double getpercentOfOrange()const{return percentOfOrange;}
	const bool getinvion()const{return invion;}*/
	void reset()
	{
		BallCenter.x = 0;
		BallCenter.y = 0;
		radius = 0.0;
		coordinateRobot.x = 0.0;
		coordinateRobot.y = 0.0;
		coordinateRobot.z = 0.0;
		relativePositionOnField.x=0.0;
		relativePositionOnField.y=0.0;
		percentOfOrange = 0.0;
		invion = false;
		validity=0.0;
		//KeeperBallInfo =0;
	}


};
struct netBall
{
	uint8 ballSeen;
	int32 ballx;
	int32 bally;
	int32 balldis;

};
#endif
