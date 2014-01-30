#ifndef __GoalPercept_h_
#define __GoalPercept_h_
#include "Tools/Math/Vector2.h"
#include "Tools/Math/Vector3.h"
#include"Tools/Math/Common.h"
//#include "Module/common/VisionHeader.h"
#include <vector>
class GoalPost
{
public:
	/** The position of the goal post in the current image */
	Vector2<int> positionInImage;      
	//	/** The position of the goal post relative to the robot*/
	Vector2<int> positionOnField;  
	//	/** Timestamp of the last perception of this pole */
	unsigned timeWhenLastSeen;
	//	/** The two different kinds of distance computation*/
	enum DistanceType {HEIGHT_BASED = 0, BEARING_BASED, IS_CLOSER} distanceType;
	//	/** PerceptionType **/
	enum PerceptionType {SEEN_IN_IMAGE = 0, CALCULATED, NEVER_SEEN} perceptionType;
	GoalPost() : positionInImage(Vector2<int>(0,0)), positionOnField(Vector2<int>(0,0)),
		timeWhenLastSeen(0), distanceType(GoalPost::BEARING_BASED),
		perceptionType(GoalPost::NEVER_SEEN) {}
};
class GoalPercept
{
 public:
	/** Constants*/
	enum {LEFT_OPPONENT=0,RIGHT_OPPONENT,LEFT_OWN,RIGHT_OWN,NUMBER_OF_GOAL_POSTS,
		UNKNOWN_OPPONENT=0, UNKNOWN_OWN, NUMBER_OF_UNKNOWN_GOAL_POSTS};
	//	/** The known posts*/
	GoalPost posts[NUMBER_OF_GOAL_POSTS];
	//	/** Unknown posts, only one per team is possible */
	GoalPost unknownPosts[NUMBER_OF_UNKNOWN_GOAL_POSTS];
	//	/** Keep color for nicer drawing of the representation*/
	//	//ColorClasses::Color ownTeamColorForDrawing;
	//	/** Information for behaviors (redundant)*/
	unsigned timeWhenOppGoalLastSeen;
	unsigned timeWhenOwnGoalLastSeen;
	GoalPercept() : 
	timeWhenOppGoalLastSeen(0),
	timeWhenOwnGoalLastSeen(0){}
	Vector2<double> LeftShotAngle;
	Vector2<double> RightShotAngle;
	Vector2<double> MiddleShotAngle;
	Vector2<double> LeftOwnAngle;
	Vector2<double> RightOwnAngle;
	Vector2<int> MiddleOppPointImage;
	bool getownleftgoalseen()const
	{
		if(posts[LEFT_OWN].perceptionType==GoalPost::SEEN_IN_IMAGE)
			return true;
		else return false;
	}
	bool getownrightgoalseen()const
	{
		if(posts[RIGHT_OWN].perceptionType==GoalPost::SEEN_IN_IMAGE)
			return true;
		else return false;
	}
	bool getunowngoalseen()const
	{
		if(unknownPosts[UNKNOWN_OWN].perceptionType==GoalPost::SEEN_IN_IMAGE)
			return true;
		else return false;
	}
	bool getoppleftgoalseen()const
	{
		if(posts[LEFT_OPPONENT].perceptionType==GoalPost::SEEN_IN_IMAGE)
			return true;
		else return false;
	}
	bool getopprightgoalseen()const
	{
		if(posts[RIGHT_OPPONENT].perceptionType==GoalPost::SEEN_IN_IMAGE)
			return true;
		else return false;
	}
	bool getunoppgoalseen()const
	{
		if(unknownPosts[UNKNOWN_OPPONENT].perceptionType==GoalPost::SEEN_IN_IMAGE)
			return true;
		else return false;
	}
};
struct netGoalpost
{
	uint8 ownleftgoalSeen;
	uint8 ownrightgoalSeen;
	uint8 unknowowngoalSeen;
	uint8 oppleftgoalSeen;
	uint8 opprightgoalSeen;
	uint8 unknowoppgoalSeen;



	int32 ownleftGoalX;
	int32 ownleftGoalY;
	int32 ownrightGoalX;
	int32 ownrightGoalY;
	int32 unknowownGoalX;
	int32 unknowownGoalY;


	int32 oppleftGoalX;
	int32 oppleftGoalY;
	int32 opprightGoalX;
	int32 opprightGoalY;
	int32 unknowoppGoalX;
	int32 unknowoppGoalY;

    int32 horizonStartX;
    int32 horizonStartY ;
    int32 horizonEndX ;
    int32 horizonEndY ;
    uint8 HorizenInImage;

};


#endif