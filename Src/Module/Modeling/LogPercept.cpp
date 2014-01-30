#include "LogPercept.h"

LogPercept::LogPercept()
{

}

LogPercept::~LogPercept()
{

}
void LogPercept::update(GoalPercept *theGoalPercept)
{
	ReadLogData::getInstance()->readLog();
	SimLogData simLogData = ReadLogData::getInstance()->logDataBuffer;

	//Analysis data	
	theGoalPercept->posts[0].distanceType = static_cast<GoalPost::DistanceType>(simLogData.simGoalData[0].distanceType);
	theGoalPercept->posts[0].perceptionType = static_cast<GoalPost::PerceptionType>(simLogData.simGoalData[0].perceptionType);
	theGoalPercept->posts[0].positionOnField = simLogData.simGoalData[0].positionOnField;
	theGoalPercept->posts[0].timeWhenLastSeen = simLogData.simGoalData[0].timeWhenLastSeen;

	theGoalPercept->posts[1].distanceType = static_cast<GoalPost::DistanceType>(simLogData.simGoalData[1].distanceType);
	theGoalPercept->posts[1].perceptionType = static_cast<GoalPost::PerceptionType>(simLogData.simGoalData[1].perceptionType);
	theGoalPercept->posts[1].positionOnField = simLogData.simGoalData[1].positionOnField;
	theGoalPercept->posts[1].timeWhenLastSeen = simLogData.simGoalData[1].timeWhenLastSeen;

	theGoalPercept->posts[2].distanceType = static_cast<GoalPost::DistanceType>(simLogData.simGoalData[2].distanceType);
	theGoalPercept->posts[2].perceptionType = static_cast<GoalPost::PerceptionType>(simLogData.simGoalData[2].perceptionType);
	theGoalPercept->posts[2].positionOnField = simLogData.simGoalData[2].positionOnField;
	theGoalPercept->posts[2].timeWhenLastSeen = simLogData.simGoalData[2].timeWhenLastSeen;

	theGoalPercept->posts[3].distanceType = static_cast<GoalPost::DistanceType>(simLogData.simGoalData[3].distanceType);
	theGoalPercept->posts[3].perceptionType = static_cast<GoalPost::PerceptionType>(simLogData.simGoalData[3].perceptionType);
	theGoalPercept->posts[3].positionOnField = simLogData.simGoalData[3].positionOnField;
	theGoalPercept->posts[3].timeWhenLastSeen = simLogData.simGoalData[3].timeWhenLastSeen;

	theGoalPercept->unknownPosts[0].distanceType = static_cast<GoalPost::DistanceType>(simLogData.simGoalData[4].distanceType);
	theGoalPercept->unknownPosts[0].perceptionType = static_cast<GoalPost::PerceptionType>(simLogData.simGoalData[4].perceptionType);
	theGoalPercept->unknownPosts[0].positionOnField = simLogData.simGoalData[4].positionOnField;
	theGoalPercept->unknownPosts[0].timeWhenLastSeen = simLogData.simGoalData[4].timeWhenLastSeen;

	theGoalPercept->unknownPosts[1].distanceType = static_cast<GoalPost::DistanceType>(simLogData.simGoalData[5].distanceType);
	theGoalPercept->unknownPosts[1].perceptionType = static_cast<GoalPost::PerceptionType>(simLogData.simGoalData[5].perceptionType);
	theGoalPercept->unknownPosts[1].positionOnField = simLogData.simGoalData[5].positionOnField;
	theGoalPercept->unknownPosts[1].timeWhenLastSeen = simLogData.simGoalData[5].timeWhenLastSeen;

	theFrameInfo->time = simLogData.time;
	theOdometryData->rotation = simLogData.odometryData.rotation;
	theOdometryData->translation = simLogData.odometryData.translation;
	theCameraMatrix = &(simLogData.cameraMatrix);
	theBallPercept->invion = simLogData.ballPercept.invion;
	//theBallPercept->BallCenter = simLogData.ballPercept.BallCenter;
	//theBallPercept->coordinateRobot = simLogData.ballPercept.coordinateRobot;
	theBallPercept->relativePositionOnField.x = simLogData.ballPercept.relativePositionOnField.x;
	theBallPercept->relativePositionOnField.y = simLogData.ballPercept.relativePositionOnField.y;
	//theMotionCmd->walkCmd = simLogData.walkCmd;
	theLinesPercept->circleCenter.pos.x=simLogData.circle.pos.x;
	theLinesPercept->circleCenter.pos.y=simLogData.circle.pos.y;
	theLinesPercept->circleCenter.found=simLogData.circle.found;
}

void LogPercept::update(RoboCupGameControlData *theRoboCupGameControlData)
{
	theRoboCupGameControlData->state = STATE_PLAYING;
	theRoboCupGameControlData->secondaryState = STATE2_NORMAL;
	theRoboCupGameControlData->teams[0].teamColour = TEAM_BLUE;
	theRoboCupGameControlData->teams[1].teamColour = TEAM_RED;

}

void LogPercept::update(RobotInfo *theRobotInfo)
{
	theRobotInfo->penalty = PENALTY_NONE;
	theRobotInfo->secsTillUnpenalised = 0;

}

//void LogPercept::update(TeamCfgInfo *theTeamCfgInfo)
//{
//	theTeamCfgInfo->playerNum = 2;
//}

MAKE_KS(LogPercept)