/*class that detect goals
author<laura>
correction time:2011.5.2  19:40
*/

#ifndef __GOALSPECIALIST__
#define __GOALSPECIALIST__
#include "Module/Vision/DebugDrawings.h"
#include "Module/Vision/Run.h"
#include "Module/Vision/TJImage.h"
#include "Module/Common/VisionHeader.h"
#include "Module/Representations/Perception/VisionInterface.h"
#include "Module/Common/Geometry.h"
#include "Module/Representations/Perception/CameraConfig.h"
#include "Module/Representations/Blackboard.h"
#include "Module/Representations/KS.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Vision/Regionpercept.h"
#include "Module/Representations/Perception/GoalPercetp.h"
#include <vector>
#include"Tools/Math/Common.h"
#include "Module/Common/FrameInfo.h"
#include "Module/Representations/MotionControl/OdometryData.h"
#include "Module/Representations/Modeling/RobotPose.h"
#include "Module/Network/RoboCupGameControlData.h"
#include <iostream>
#include <fstream>
DEFINE_KS(GoalSpecialist)
REQUIRES(TJImage)
REQUIRES(CameraMatrix)
REQUIRES(Regionpercept)
REQUIRES(RoboCupGameControlData)//比赛时加上获得对号信息
REQUIRES(FrameInfo)
REQUIRES(OdometryData)
REQUIRES(RobotPose)
PROVIDES(GoalPercept)
USES(GoalPercept)
END_KS


#define MIN_GOAL_HEIGHT  25
#define MIN_GOAL_WIDTH   4   //5
#define MAX_GOAL_WIDTH  40

const int PoleAccThreshold=5;
const int pixelsToTreatPoleAsNear=5;
const int PoleAccMinDistance=75;
const int GridStepSize=8;
const double allowedDistanceNoise=0.35;
const int SkipOffset=3;//向下找绿色时用得参数
const int SkipOffset2=2;
const int minHeadBandWidth=4;
const int headBand_gridStepSize=4;
const int minHeadBandHeight=3;
const int maxHeadBandHeight=30;
const int ScanforKeeperAbove=70;//gap上的扫描
const int ScanforKeeperBelow=10;
const unsigned motionUpdatecounter=20000000;//motionupdate时间帧


class blob
{
public:
	// bounding coordinates of the blob
	unsigned char  color;
	int xImage;
	int xImageTop;
	int xImageBottom;
	int yImageTop;
	int yImageBottom;
	int width;
	int widthLeft;
	int widthRight;
	bool footVisible;
	bool headVisible;
	bool discarded;
	bool discarded2;
	Vector2<double> onField;
	unsigned timeWhenLastSeen;
	bool seenInThisFrame;
	int accumulatorMatchSize;
	enum PoleSide 
	{
		LEFT_=0 , RIGHT_ , UNDEFINED
	} poleSide;
	GoalPost:: DistanceType  distanceType;
	blob():
	poleSide(UNDEFINED),
		distanceType(GoalPost::BEARING_BASED),
		xImage(0),
		xImageTop(0),
		xImageBottom(0),
		yImageTop(0),
		yImageBottom(0),
		widthLeft(0),
		widthRight(0),
		width(0),
		footVisible(true),
		headVisible(true),
		discarded(false),
		discarded2(false),
		timeWhenLastSeen(0), 
		seenInThisFrame(false),
		accumulatorMatchSize(0){};
	~blob(){}

	//blob& operator=(const blob& other)
	//{

	//	color = other.color;
	//	xImage = other.xImage;
	//	xImageTop = other.xImageTop;
	//	xImageBottom = other.xImageBottom;
	//	yImageTop = other.yImageTop;
	//	yImageBottom = other.yImageBottom;
	//	widthLeft = other.widthLeft;
	//	widthRight = other.widthRight;
	//	width = other.width;
	//	footVisible = other.footVisible;
	//	headVisible = other.headVisible;
	//	discarded = other.discarded;
	//	discarded2 = other.discarded2;
	//	timeWhenLastSeen = other.timeWhenLastSeen;
	//	seenInThisFrame = other.seenInThisFrame;
	//	accumulatorMatchSize = other.accumulatorMatchSize;
	//	poleSide = other.poleSide;
	//	distanceType = other.distanceType;
	//	onField = other.onField;
	//	return *this;
	//}
	// GoalPost::DistanceType distanceType;//whether the blob can give the post information--
};

class stop {
public:
	int x;
	int y;
	int bad;
	int good;
	int span;
};


class GoalSpecialist:public GoalSpecialistBase,BaseProvider
{
public:
	GoalSpecialist();
	~GoalSpecialist();
	//Matrix4x4 cameraMatrix;
	//CameraMatrix4X3 cameraMatrixScanvision;
	unsigned char oppcolour;
	unsigned char owncolour;
	

	void update(GoalPercept* theGoalPercept);
	void inTJImage();
	void inCameraMatrix();
	void inRegionpercept(){}
	void inRoboCupGameControlData(){}
	void inFrameInfo(){}
	void inOdometryData(){}
	void inRobotPose(){}
	void outGoalPercept();
	void sendGoalResult();//information transimitted through network
	int getOurTeamNumber();


	//找门的主函数
	void detectGoal(unsigned char goalColor, blob polePercepts[]);
    int findDown(int x, int y, unsigned char col, int yEnd);
	int findDown2(int x, int y, unsigned char col, int yEnd);
	int runDown(int x, int y, unsigned char col, int yEnd);


	//将门信息送出
	void createKnownGoalPercept(GoalPercept*goalPercept, int index, blob percept);
	void createUnknownGoalPercept(GoalPercept*goalPercept, int index, blob percept);
	void updateRepresentation(GoalPercept*goalPercept,blob oppPercepts[], blob ownPercepts[]);
    
	//识别前的信息更新
	void preExecution(GoalPercept*goalpercept);
	void MotionUpdate(GoalPercept*goalpercept);
	bool isCameraFacedOpp();
	void updateSideByDirection();
	void updateSideByDis();
	void updateSideByLoc();
	void updateSide();
	void PostExecution();

	//识别gap用的函数
	void calculateShot(GoalPercept &goalpercept);
 //   int findDown2Goal(int x, int y, unsigned char col1, int yEnd);
 //   std::pair<int,int> scanGoalkeeper(int xStart,int xEnd,int yTop,int yBottom,unsigned char curColor);
	//int runDownGoal(int x, int y, unsigned char col, int yEnd);
	//int runRightGoal(int x, int y, unsigned char col, int xMax);
	//int runLeftGoal(int x,int y,unsigned char col,int xMin);
	//int vectorAver(std::vector<int>& vec);
	//int vectorMax(std::vector<int>& vec);

   //计算门柱宽
   int calculateExpectedPixelWidth(Vector2<int> posImg,const CameraMatrix&cameraMatrix);
   double getGoalPostDistFromWidth(double postwidthInPixel,double GoalPostWidthInMM);
   double getGoalPostDistFromHeight(double heightInPixel,double GoalHeightInMM);

   	void  writeGoalLog();

	inline int ROUND2(double x) {
		if ((x-(int)x) >= 0.5) return ((int)x+1);
		if ((x-(int)x) <= -0.5) return ((int)x-1);
		return (int)x;
	};

	inline unsigned char convertColorAt(int x, int y)
	{
		unsigned char pixy,pixu,pixv;
		pixy = theTJImage->unCorrectedRaw[y][x][0];
		pixu = theTJImage->unCorrectedRaw[y][x][1];
		pixv = theTJImage->unCorrectedRaw[y][x][2];
		//std::cout<<"in GoalSpecialist::vertScan,we are in for circle "<<std::endl;
		unsigned char color =theTJImage->getClassifyedColor(pixy, pixu, pixv);
		return color;
	}
	// a blob structure that holds information about its own location, and
	// information involving its larger blob structure


private:

	//int horizonstartY;
	stop scan;//verscan中用来记录信息

	//这是一个跟水平线有关的量，是竖直的扫描线的倾斜度
	//double slope;//初始化为0

	//std::vector<blob> blobRegion;
	//int OurTeamNumber;
	enum { MAX_NUMBER_OF_POST=2};
	blob oppgoalpercepts[MAX_NUMBER_OF_POST];
	blob owngoalpercepts[MAX_NUMBER_OF_POST];
	blob goalpercepts[MAX_NUMBER_OF_POST];
	Pose2D lastOdometry;
	Pose2D odometryOffset;

	int goaldebugcounter;
   // int motionUpdateCounter;


};//end class GoalSpecialist


#endif




























