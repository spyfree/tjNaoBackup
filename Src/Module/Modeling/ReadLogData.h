#ifndef ReadLogData_H
#define ReadLogData_H

/**
*该文件用来读取log中的数据，一帧一帧传给定位
*/

#include "Tools/Math/Vector2.h"
#include "Tools/Math/Pose2D.h"
#include "Platform/SystemCall.h"
#include "Module/Representations/MotionControl/OdometryData.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Representations/Perception/GoalPercetp.h"
#include "Module/Representations/Perception/BallPercept.h"
#include "Module/Representations/Perception/LinesPercept.h"
#include "Module/Common/MotionCmd.h"
#include <iostream>
#include <fstream>

class SimGoalData
{
public:
	SimGoalData();
	SimGoalData& operator=(const SimGoalData& other)
	{
		this->objectID = other.objectID;
		this->distanceType = other.distanceType;
		this->perceptionType = other.perceptionType;
		this->positionOnField = other.positionOnField;
		this->timeWhenLastSeen = other.timeWhenLastSeen;
		return *this;
	}
	unsigned int objectID;
	int distanceType;
	int perceptionType;
	Vector2<int> positionOnField;
	unsigned timeWhenLastSeen;
private:
	friend std::istream& operator >>(std::istream &is,SimGoalData &simData);
	//friend std::ostream& operator <<(std::ostream &out,SimGoalData &simData);
};

std::istream& operator >>(std::istream &is,SimGoalData &simData);
//std::ostream& operator <<(std::ostream &out,SimGoalData &simData);

class SimLogData
{
public:
	SimLogData(){}
	SimLogData& operator=(const SimLogData& other)
	{
		this->odometryData = other.odometryData;
		this->time = other.time;
		this->cameraMatrix = other.cameraMatrix;
		this->simGoalData[0] = other.simGoalData[0];
		this->simGoalData[1] = other.simGoalData[1];
		this->simGoalData[2] = other.simGoalData[2];
		this->simGoalData[3] = other.simGoalData[3];
		this->simGoalData[4] = other.simGoalData[4];
		this->simGoalData[5] = other.simGoalData[5];
		this->ballPercept.invion = other.ballPercept.invion;
		this->ballPercept.relativePositionOnField.x = other.ballPercept.relativePositionOnField.x;
		this->ballPercept.relativePositionOnField.y = other.ballPercept.relativePositionOnField.y;
		this->circle.found = other.circle.found;
		this->circle.pos.x = other.circle.pos.x;
		this->circle.pos.y = other.circle.pos.y;
		this->walkCmd = other.walkCmd;
		return *this;
	}
	OdometryData odometryData;
	unsigned time; //FrameInfo
	CameraMatrix cameraMatrix; 
	SimGoalData simGoalData[GoalPercept::NUMBER_OF_GOAL_POSTS+GoalPercept::NUMBER_OF_UNKNOWN_GOAL_POSTS];
	BallPercept ballPercept;
	LinesPercept::CircleSpot circle;
	WalkCmd walkCmd;

private:
	friend std::istream& operator >> (std::istream &is,SimLogData &simData);
	friend std::istream& operator >> (std::istream &is,LinesPercept::CircleSpot &circ);
	friend std::istream& operator >> (std::istream &is,BallPercept &ballData);
	//friend std::ostream& operator <<(std::ostream &out,SimLogData &simData);
};

std::istream& operator >>(std::istream &is,SimLogData &simData);
//std::ostream& operator <<(std::ostream &out,SimLogData &simData);

class ReadLogData 
{
public:
	ReadLogData();
	bool isPerceptFlagStr(const char* str);
	bool isLogFlagStr(const char* str);
	bool isBallStr(const char* str);
	bool isCircStr(const char* str);
	void readLog();
	static ReadLogData *getInstance();
public:
	SimLogData logDataBuffer;
private:
	PROCESS_WIDE_STORAGE_STATIC ReadLogData *theReadLogData;
	char* logPath;
	char* logTempPath;
};


#endif