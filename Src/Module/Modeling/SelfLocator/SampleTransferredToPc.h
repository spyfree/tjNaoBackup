#ifndef SampleTransferredToPc_H
#define SampleTransferredToPc_H
#include "Module/Representations/Modeling/RobotPose.h"
#include "Module/Representations/Modeling/BallModel.h"
#include "Tools/SampleSet.h"
const int SelfSampleSize=100;
const int BallSampleSize=40;
const int RobotSampleSize=2;
const int Num_Cell_Size = 14;



//该类用于将机器人粒子打包传回pc机
class Robotsamplebuffer
{
public:
   //该类用于定位在线调试时将粒子信息传回
	class singleSample
	{
	public:
		int32 translationX,   /**< The position in mm. */
			translationY,
			rotationX,
			rotationY;      /**< Cosinus and sinus of the rotation multiplied by 1024. */
		double angle,               /**< The rotation in radians. */
			weighting;           /**< The weighting of a sample*/
		int32 cluster;                /**< The number of the cluster this sample belongs to*/
		uint8 InbestCluster;
		singleSample() :translationX(0),translationY(0), rotationX(1024),rotationY(0), angle(0.0), weighting(1.0), cluster(0),InbestCluster(uint8(false)){}
	};
	class singleRobotsample
	{
		public:	
		  class drawColorForRobot
		{
		public:
			uint8 r;
			uint8 g;
			uint8 b;
			//用于补齐内存位置
			uint8 a1;
			uint8 a2;
			uint8 a3;
			uint8 a4;
			uint8 a5;
			drawColorForRobot():r(0),g(0),b(0),a1(0),a2(0),a3(0),a4(0),a5(0){}
		};
		double Robotrotation;
		double RobotTranslationX;
		double RobotTranslationY;
		//所有粒子的权值和
		double totalWeighting;
		//用于画robotPose，给定画图颜色
		drawColorForRobot color;
		singleRobotsample():Robotrotation(0.0),RobotTranslationX(0.0),RobotTranslationY(0.0),totalWeighting(1.0){}
	};
	Robotsamplebuffer()
	{
		memset(samples,0,sizeof(singleSample)*SelfSampleSize);
		memset(Robotsample,0,sizeof(singleRobotsample)*RobotSampleSize);
	}
	~Robotsamplebuffer(){}
	singleSample samples[SelfSampleSize];
	//robotpose的数据
    singleRobotsample Robotsample[RobotSampleSize];//selflocator,//robotPosevalidator
};


class Ballsamplebuffer
{
public:
	struct BallStateLocOnline
	{
		double positionX; /**< The position of the ball relative to the robot (in mm)*/
		double positionY;
		double velocityX;
		double velocityY;
	};
	class BallModelLocOnline
	{
	public:
		/** Constructor */
		BallModelLocOnline():goalieStandSpotX(0),goalieStandSpotY(0){}

		BallStateLocOnline lastPerception; /**< The last seen position of the ball */
		BallStateLocOnline estimate; /**< The state of the ball estimated from own observations;
									 it is propagated even if the ball is not seen */
		BallStateLocOnline estimateGlobe;//it is in world coordinator
		BallStateLocOnline lastSeenEstimate; /**< The last seen estimate */
		//unsigned timeWhenLastSeen; /**< Time stamp, indicating what its name says*/

		double goalieStandSpotX;/*It is in world coordinator*/
		double goalieStandSpotY;
		
	};
	//球的粒子在网络传回的形式
	class singleBallSample
	{
	public:
		/** A ball position */
		int32 posX;
		int32 posY;
		/** A ball velocity */
		int32 velocityX;
		int32 velocityY;
		/** The weighting of a sample*/
		double weighting;

		enum State {LYING, ROLLING} state;
		int useless;//用于内存对齐
		/** Default constructor*/
		singleBallSample():posX(0),posY(0),velocityX(0),velocityY(0)
		{
			weighting = -1.0;
			state = ROLLING;
			useless=0;
		}
	};
	Ballsamplebuffer():RobotTranslationX(0.0),RobotTranslationY(0.0)
	{
		memset(BallParticleSamples,0,sizeof(singleBallSample)*BallSampleSize);
	}
	~Ballsamplebuffer(){}
	singleBallSample BallParticleSamples[BallSampleSize];
	//BallModel
	BallModelLocOnline BallModelToPc;
	double RobotTranslationX;//用于将粒子转换成全局坐标系；
	double RobotTranslationY;
	double RobotRotation;
};

class FreePartData{
public:

    
	/** this array holds values (likeliness a robot is standing there) for each cell on the opponent goal */
	double cellsOnOppGoalline[Num_Cell_Size];

	/** highest (left side when facing the opponent goal) cell of the largest free part of opponent goal */
	int largestFreePartHigh;

	/** lowest (right side when facing the opponent goal) cell of the largest free part of opponent goal */
	int largestFreePartLow;
	
	FreePartData():largestFreePartHigh(0),largestFreePartLow(0)
	{
		memset(cellsOnOppGoalline,0,sizeof(double)*Num_Cell_Size);
	}
	~FreePartData(){}
};
/* this class is just for update sample in order to transfer them to network thread*/
class ForSampleToPc
{
public:
	int a;
	ForSampleToPc():a(0){}
};

#endif
