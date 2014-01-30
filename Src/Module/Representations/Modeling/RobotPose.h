#ifndef __RobotPose_h_
#define __RobotPose_h_

#include "Tools/Math/Pose2D.h"
#include "Tools/DrawDebugging/PaintMethods.h"
#include "Tools//DrawDebugging/LocDrawings.h"
#include "Tools/Math/Matrix2x2.h"

/**
* @class RobotPose
* A Pose2D with validity.
*/
class RobotPose : public Pose2D
{
//protected:
//	virtual void serialize(In* in, Out* out)
//	{
//		STREAM_REGISTER_BEGIN();
//		STREAM_BASE(Pose2D);
//		STREAM(validity);
//		STREAM_REGISTER_FINISH();
//	}

public:
	double validity;                   /**< The validity of the robot pose.0=invalid,1=perfect */
	float deviation;               /**< The deviation of the robot pose. */
	bool ownSide;
	double sideValidity;
	ColorRGBA ownTeamColorForDrawing;  /**< As the name says... */
    Matrix2x2<double> positionCovariance;  //当前粒子分布的协方差矩阵
	Vector2<double> Cross;
	double direction; // ** the robot face diretion rad **/
	double middleTranslationX; //* the translation x of the middle side */
	enum
	{
		unknownDeviation = 100000,
	};

	/** Constructor */
	RobotPose():validity(0.0),deviation(unknownDeviation),ownSide(true),sideValidity(1.),direction(0.)  {}

	/** Assignment operator
	* @param other Another RobotPose
	* @return A reference to the object after the assignment
	*/
	const RobotPose& operator=(const RobotPose& other)
	{
		(Pose2D&) *this = (const Pose2D&) other;
		validity = other.validity;
		deviation = other.deviation;
		ownTeamColorForDrawing = other.ownTeamColorForDrawing;
		return *this;
	}

	/** Cast Contructor.
	* @param otherPose A Pose2D object
	*/
	RobotPose (const Pose2D& otherPose) 
	{
		(Pose2D&) *this = otherPose;
		validity = 0;
		deviation = 100000.0f;

	}

	/** Assignment operator for Pose2D objects
	* @param other A Pose2D object
	* @return A reference to the object after the assignment
	*/
	const RobotPose& operator=(const Pose2D& other)
	{
		(Pose2D&) *this = other;
		//validity is not set
		return *this;
	}

	/** Draws the robot pose to the field view*/
	void draw();
	void draw(ColorRGBA colour);
};


class PotentialRobotPose : public RobotPose
{
public:
	/** Draws the robot pose to the field view*/
};

/**
* @class RobotPoseInfo
* Encapsulates further information about the robot pose
*/
class RobotPoseInfo 
{
public:
	unsigned int timeLastPoseReset; /**< The time when the robot pose was reset. */
};

#endif //__RobotPose_h_
