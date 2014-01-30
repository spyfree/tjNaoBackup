#ifndef __PreviewWalkParam_h_
#define __PreviewWalkParam_h_

#include "Tools/Math/Vector2.h"
#include "Module/Common/JointData.h"
//#include "Tools/Math/Vector3.h"

class PreviewWalkParam
{
public:
	/**
	* Default constructor.
	*/
	PreviewWalkParam();
	~PreviewWalkParam();
	void setDefaultParams();
	bool load(const std::string& filename);
	void finalize();
public:
	enum {
		maxStages = 10
	};
	enum{
		DISABLE =0,
		ENABLE
	};
	int PG;		/**< Total frames that one walk cycle takes. */
	double g;	/**< gravity acceleration, m/s^2. */
	double dt;	/**< time between frames, in s. */
	double Zc;	/**< Z of COM in LIPM theory, m. */
	double sx;	/**< Refer to <<Humanoid Robots>> or <<·ÂÈË»úÆ÷ÈË>>, in m.*/
	double sy;
	double srot;	/**< Rotation of each step, in radians. */
	double stepX;/**< Step size in X direction, m. */
	double stepY;/**< Step size in Y direction, m. */
	double stepHeight;/**< The max height of the swing leg, m.*/
	double torsoAngleY;/**< The rotation angle of torso around Y axis when walking, in radians. */
	Vector2<double> ZmpOffsetRight;/**< ZMP original point relative to the foot original point, right leg. */
	Vector2<double> ZmpOffsetLeft; /**< ZMP original point relative to the foot original point, left leg. */
// 	double standingSx;/**< Sx when standing, reserved for later usage!*/
// 	double standingSy;	/**< Sy when standing, different from sy when walking.*/
	int numStages;	/**< Num of stages of PreviewControl walk, normally 4 for prview control walk*/
	double sLen[maxStages]; /**< time ratio of each stage */;
	//=========Arm Swing Related==============//
	class SwingJoint
	{
	public:
		SwingJoint():offset(0),amp(0), swingSign(1), swing(false){}
		SwingJoint(bool swing, double offset, double amp, int swingSign):
		offset(offset),amp(amp), swingSign(swingSign), swing(swing)
		{}
		void setParam(bool swing_, double offset_, double amp_, int swingSign_)
		{
			swing = swing_;
			swingSign = swingSign_;
			offset = offset_;
			amp = amp_;
		}
		double offset;
		double amp;
		int swingSign;
		bool swing;
	};
	int useArmSwing;
	SwingJoint armSwing[8];
	//=========Below is Calibration and Compensation Related=======//
	int useHipRollCompensation; /**< Whether to use HipRoll Compensation*/
	double pRHipRollCompensator, pLHipRollCompensator;/**< Compensator Param of HipRoll Angles when Single Supporting*/
	int useHipAnkleRollScale;
	double scaleRHipRoll, scaleLHipRoll, scaleRAnkleRoll, scaleLAnkleRoll;
	int useHipRollClip;
	double clipRHipRollLow, clipRHipRollUp, clipLHipRollLow, clipLHipRollUp;

	class JCalibration
	{
	public:
		JCalibration():
			offset(0){}
		double offset;	/**< Offset in radians*/
	};
	int useJointsCalibration;
	JCalibration jointsCalibration[12];/**< Joints Calibration for 12 Leg Joints*/

};
#endif