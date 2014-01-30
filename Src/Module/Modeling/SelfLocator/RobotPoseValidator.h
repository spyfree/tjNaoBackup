/**
* @file RobotPoseValidator.h
* Declares a class that validates poses from the self locator.
* @author Colin laura
*/

#ifndef RobotPoseValidator_H
#define RobotPoseValidator_H
#include "Module/Representations/Blackboard.h"
#include "Module/Representations/BaseProvider.h"
#include "Tools/SampleSet.h"
#include "Tools/Math/MatrixMXN.h"
#include "Tools/RingBuffer.h"
#include "Module/Common/FrameInfo.h"
#include "SampleTemplateGenerator.h"
#include "SensorModels/SensorModel.h"
#include "SensorModels/FieldModel.h"
#include "Module/Network/RoboCupGameControlData.h"
#include "Module/Network/networkComm/Wdatainfo.h"
#include "Module/Representations/KS.h"
#include "Module/Representations/MotionControl/OdometryData.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Representations/Perception/GoalPercetp.h"
#include "Module/Representations/Perception/LinesPercept.h"
#include "Module/Representations/Perception/BallPercept.h"
#include "Module/Representations/Modeling/RobotPose.h"
#include "Module/Representations/Modeling/RobotPoseHypotheses.h"
#include "Module/Representations/Modeling/SelfLocatorSampleSet.h"
#include "Module/Representations/Configuration/FieldDimensions.h"
#include "Module/Network/networkComm/NetworkMsg.h"
#include "Module/Modeling/SelfLocator/SampleTransferredToPc.h"
#include "Module/Common/FallDownState.h"
#include "Tools/Math/Pose2D.h"
#include "Tools/Math/Pose3D.h"
#include <fstream>
#include <iostream>
#include <vector>

DEFINE_KS(RobotPoseValidator)
REQUIRES(RobotPose)
REQUIRES(CameraMatrix)
REQUIRES(FieldDimensions)
REQUIRES(RoboCupGameControlData)
//REQUIRES(CameraInfo)
REQUIRES(LinesPercept)
//REQUIRES(OwnTeamInfo)
//REQUIRES(MotionInfo)
REQUIRES(OdometryData)
REQUIRES(Robotsamplebuffer)
REQUIRES(GoalPercept)
REQUIRES(FallDownState)
REQUIRES(FrameInfo)
REQUIRES(BallPercept)
REQUIRES(GlobalInfo)
REQUIRES(TeamCfgInfo)
REQUIRES(RobotInfo)
PROVIDES(RobotPose)
PROVIDES(Robotsamplebuffer)
END_KS

/**
* @class RobotPoseValidator
* A modules that determines the validity of a robot pose.
*/

class Parameters 
{
public:
	/** 
	* Default constructor.    
	*/
	Parameters() {}

	Pose2D respawnMaxPoseDistance; /**< The maximal admissible distance between the estimate from the self locator and the pose from the validator. */
	Pose2D respawnPoseDeviation; /**< The "deviation" of the pose from the self locator. (As long as it is near the correct pose.) */
	double lineRelationCorridor; /**< The corridor used for relating seen lines with field lines. */
	Pose2D odometryDeviation; /**< The percentage inaccuracy of the odometry. */
	Pose2D filterProcessDeviation; /**< The process noise for estimating the robot pose. */
	Vector2<double> robotRotationDeviation; /**< Deviation of the rotation of the robot's torso */
	Pose2D validationMaxDeviation; /**< The maximale admissible deviation of the robot pose. (Used for detecing valid poses.) */
	double validationMaxGoalPerceptDistance; /**< The maximale admissible distance of goal percepts to their expected position. (Used for detecing valid poses; In percentage of the distance to the goal post). */
	unsigned int validationMinGoalSightings; /**< The minimal required goal sightings used for detecting valid poses. */
	unsigned int validationMinUnknownGoalSightings; /**< The minimal required unknown goal sightings used for detecting valid poses. */
};

/**
* A field line relative to the robot.
*/
class FieldLine
{
public:
	Vector2<double> start; /**< The starting point of the line. */
	Vector2<double> end; /**< The ending point of the line. */
	Vector2<double> dir; /**< The normalized direction of the line (from starting point). */
	double length; /**< The length of the line. */
	bool vertical; /**< Whether this is a vertical or horizontal line. */
};

/**
* A line percept relative to the robot (in floats).
*/
class LinePercept
{
public:
	Vector2<double> start; /**< The starting point on field */
	Vector2<double> end; /**< The ending point on field */
};


class RobotPoseValidator : public RobotPoseValidatorBase,BaseProvider
{
public:
	/**
	* Default constructor.
	*/
	RobotPoseValidator();
	~RobotPoseValidator(){}
	void inGlobalInfo();
	void inRobotInfo(){}
	void inRobotPose(){}
	void inCameraMatrix(){}
	void inFieldDimensions(){}
	void inLinesPercept(){}
	void inBallPercept(){}
	void inOdometryData(){}
	void inGoalPercept(){}
	void inFallDownState(){}
	void inFrameInfo(){}
	void inTeamCfgInfo(){}
	void inRoboCupGameControlData(){}
	void inRobotsamplebuffer(){}
	void update(RobotPose*theRobotPose);
	void update(Robotsamplebuffer*theRobotsamplebuffer);
	void outRobotPose(){}
	void outRobotsamplebuffer(){}

private:
	/**
	* A collection of parameters for the robot pose validator.
	*/

	Parameters p; /**< A set of parameters for the module. */
	FieldLine fieldLines[24]; /**< Relevant field lines relative to the robot. */
	int countOfFieldLines; /**< Count of relevant field lines. */
	Vector2<double> goalPosts[4]; /**< The positions of the goal posts. */
	Vector<2> translation; /**< The estimate of the translation. */
	MatrixMXN<2,2> translationCov; /**< The covariance MatrixMXN of the estimate. */
	double rotation; /**< The estimate of the rotation. */
	double rotationCov; /**< The variance of the estimate of the rotation. */
	Pose2D lastOdometryData; /**< OdometryData of the previous iteration. */
	Vector<2> odoOffset;/** <odometryData offset */

	unsigned int lastResetTime; /**< The time when the robot pose was reset. */
	bool validated; /**< Whether the pose is validated or not. */
	bool ownSide;/**<Whether the pose is on own side */
	double sideValidity;/**the Validity of  side 0-1*/
	bool lastOwnSide;
	bool crossMiddleLine;
	GlobalInfo *theCurrentGlobalInfo;
	int lastPenalty;/** the penalty state in the last frame*/
	unsigned int validGoalSightingSinceLastReset; /**< Amount of matching goal post sightings since last loss of validity. */
	unsigned int validUnknownGoalSightingSinceLastReset; /**< Amout of "unknown" matching goal post sightings since last loss of validity. */
    
	double Rotation;//将kalman滤波后的结果传回pc
	double direction;//the direction based on the odometrydata
	Vector2<double> Translation;
	double middleTranslationX;
	/**
	* Initializes the module.
	*/
	void init();

	/**
	* Updates the validated robot pose provided from this module.
	* @param robotPose The robotPose updated from the module
	*/
	//void update(RobotPose& robotPose);

	/**
	* Updates further information about the robot pose.
	* @param robotPoseInfo The robotPoseInfo updated from the module
	*/
	//void update(RobotPoseInfo& robotPoseInfo);

	bool intersectLineWithLine(const Vector2<double>& lineBase1, const Vector2<double>& lineDir1, const Vector2<double>& lineBase2, const Vector2<double>& lineDir2, Vector2<double>& intersection) const;
	double getSqrDistanceToLine(const Vector2<double>& base, const Vector2<double>& dir, double length, const Vector2<double>& point) const;
	double getSqrDistanceToLine(const Vector2<double>& base, const Vector2<double>& dir, const Vector2<double>& point) const;
	Vector2<double> getOrthogonalProjection(const Vector2<double>& base, const Vector2<double>& dir, const Vector2<double>& point) const;

	void motionUpdate();
	void sensorUpdate(double angle, double x, double y, double angleVariance, double xOrYVariance);

	void useLinePercept(const LinePercept& linePercept, const FieldLine& fieldLine);
	void useCenterCircle(const Vector2<double>& circlePos);
	void useGoalPost(const Vector2<double>& postPos, const Vector2<double>& postOnField);

	Matrix2x2<> getCovOfPointInWorld(const Vector2<double>& pointInWorld, double pointZInWorld) const;

	double applyShaValidity(const RobotPose& RobotPose);
	double applyGlobalBall(const RobotPose& robotpose);
	void preUpdateSide();
	void updateSide(RobotPose* robotPose);
	void afterUpdateSide(RobotPose* robotPose);
	bool isOwnSide(RobotPose* robotPose);
	double computerLocWeighting(const Pose2D& robotPose,RobotLocation& shadowLoc,double standardDeviation, double bestPossibleWeighting);
	double computeBallWeighting(const BallLocation& ballseen,BallLocation& globalBall,double standardDeviation, double bestPossibleWeighting);

	void updateDirection();
};

#endif // RobotPoseValidator_H
