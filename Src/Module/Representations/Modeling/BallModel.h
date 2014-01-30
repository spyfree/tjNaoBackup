#ifndef BallModel_H
#define BallModel_H

#include "Tools/DrawDebugging/PaintMethods.h"
#include "Module/Representations/Modeling/RobotPose.h"

/**
* @class BallState
*
* Base class for ball position and velocity.
*/
class BallState
{
public: 

	Vector2<double> position; /**< The position of the ball relative to the robot (in mm)*/  
	Vector2<double> velocity; /**< The velocity of the ball relative to the robot (in mm/s)*/

	inline double getAngle() const
	{
		return atan2(position.y, position.x);
	}

	inline double getDistance() const
	{
		return position.abs();
	}

	/*inline Vector2<double> getPositionInFieldCoordinates(const RobotPose& rp) const
	{
		return Geometry::relative2FieldCoord(rp, position.x, position.y);
	}

	Vector2<double> getVelocityInFieldCoordinates(const RobotPose& rp) const
	{
		double c(rp.getCos());
		double s(rp.getSin());
		return Vector2<double>(velocity.x*c - velocity.y*s, velocity.x*s + velocity.y*c);
	}

	void setPositionAndVelocityInFieldCoordinates(const Vector2<double>& positionOnField,
		const Vector2<double>& velocityOnField,
		const RobotPose& rp)
	{
		position = Geometry::fieldCoord2Relative(rp, positionOnField);
		double c(rp.getCos());
		double s(rp.getSin());
		velocity = Vector2<double>(velocityOnField.x*c + velocityOnField.y*s,
			-velocityOnField.x*s + velocityOnField.y*c);
	}*/

};

class BallSquare
{
public:
	Vector2<double> LeftUpPosition;
	double length;
	double width;
};
/**
* @class BallModel
*
* Contains all current knowledge about the ball.
*/
class BallModel 
{
public:
	/** Constructor */
	BallModel() : timeWhenLastSeen(0) {}

	BallState lastPerception; /**< The last seen position of the ball */  
	BallState estimate; /**< The state of the ball estimated from own observations;
						it is propagated even if the ball is not seen */
	BallState estimateGlobe;//it is in world coordinator
	BallState lastSeenEstimate; /**< The last seen estimate */  
	unsigned timeWhenLastSeen; /**< Time stamp, indicating what its name says*/

	Vector2<double> goalieStandSpot;/*It is in world coordinator*/

	BallSquare ballsquare;
	/** Draws something*/
	void draw(RobotPose robotPose) 
	{
		//Vector2<double>& position(estimate.position);
		//position+=robotPose;
		//Vector2<double>& velocity(estimate.velocity);
		Vector2<double> position(estimateGlobe.position);
		//position+=robotPose;
		Vector2<double> velocity(estimateGlobe.velocity);
		CIRCLE(
			position.x, 
			position.y, 
			40, 
			1, // pen width 
			LocDrawings::ps_solid, 
			ColorClasses::red,
			LocDrawings::bs_solid, 
			ColorClasses::red);
		ARROW(position, position + velocity.rotate(robotPose.rotation), 5, 1, ColorClasses::red);
		
		//position and velocity of ball when last frame was seen
		//Vector2<double>& lastPosition(lastSeenEstimate.position);
		//lastPosition+=robotPose;
		//Vector2<double>& lastVelocity(lastSeenEstimate.velocity);
		Vector2<double> lastPosition(lastSeenEstimate.position);
		lastPosition=lastPosition.rotate(robotPose.rotation)+robotPose.translation;
		Vector2<double> lastVelocity(lastSeenEstimate.velocity);
		CIRCLE(
			lastPosition.x, 
			lastPosition.y, 
			45, 
			1, // pen width 
			LocDrawings::ps_solid, 
			ColorRGBA(0, 0, 0, 220),
			LocDrawings::bs_solid, 
			ColorRGBA(255, 128, 128, 220));
		ARROW(lastPosition, lastPosition + lastVelocity.rotate(robotPose.rotation), 5, 1, ColorRGBA(255, 128, 0, 220));

	}
};

#endif