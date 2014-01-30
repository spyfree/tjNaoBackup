#ifndef __RobotState_h_
#define __RobotState_h_
#include "Tools/Math/Vector2.h"
#include "Tools/Math/Vector3.h"
#include "CoP.h"
#include "GroundContactState.h"
class RobotState
{
public:
	/**
	* This Class Represent an external detected fall state and fall critia
	*/
	class FallState
	{
	public:
		FallState():fall(false), fallTime(0), recoverd(true), updateModel(false){}
		Vector2<double> fallMetric; /**< A metric measure of fall*/
		double fallTime;	/**< time of fall down*/
		bool fall;
		bool recoverd;
		bool updateModel;
	};
public:
	RobotState(){}
	FallState fallState;
	Vector3<double> mZMP;	/**< Measured ZMP from cart-table model, relative to supporting foot coordinate*/
	CoP mCoPL, mCoPR;		/**< Measured Center of Pressure(Right leg and Left leg)*/
	GroundContactState groundContactLeft, groundContactRight;

};
#endif
