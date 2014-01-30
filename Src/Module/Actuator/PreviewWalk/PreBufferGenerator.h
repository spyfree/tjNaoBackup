#ifndef __PreBufferGenerator_h_
#define __PreBufferGenerator_h_
#include "PreviewWalkParam.h"
#include "Tools/Math/Vector2.h"
#include "Tools/Math/Vector3.h"
#include "Module/Common/NaoStructure.h"
#include "Module/Common/WalkingEngineOutput.h"
#include "Module/Common/WalkCmd.h"
#include "boost/circular_buffer.hpp"
#include "Module/Actuator/WalkTypeBase.h"
/**
* @class PreviewState
* A class that represents the state vector at each frame in our preview control walk
*/
class PreviewState
{
public:
	PreviewState(){}
	~PreviewState(){}
public:
	int istep;
	int supmod;		/**< Supporting mode*/
	int walkStatus; /**< Indicating whether the robot is running , or starting , or ending...*/
	int miniStatus;
	Vector2<double> pZMP; /**< x, y: position of ZMP projected on x-y plane, world coordinate*/
	double COMz;	/**< z of COM*/
	Vector3<double> pSwingFoot; /**< position of swing foot, ??? coordinate*/
	Vector3<double> rSwingFoot; /**< Rotation angles of foot*/
	Vector3<double> pSupFootZMP; /**< ZMP Position of supporting foot, world coordinate!*/
	Vector3<double> rSupFootZMP; /**< ZMP Rotation of supporting foot, world coordinate!*/
/*
	PreviewState& operator=(const PreviewState& other)
	{
		supmod = other.supmod;
		pZMP = other.pZMP;
		COMz = other.COMz;
		pSwingFoot =other.pSwingFoot;
		rSwingFoot = other.rSwingFoot;
		pSupFoot = other.pSupFoot;
		rSupFoot = other.pSupFoot;
		return *this;
	}*/
};

/**
* @class PreviewBuffer
* A warpper class for circular_buffer of boost lib
*/
class PreviewBuffer
{
public:
	PreviewBuffer(size_t _capacity):
	  buffer(_capacity)
	{
	}
	PreviewBuffer():buffer(0)
	{}
	void push_back(const PreviewState& state) {buffer.push_back(state);}
	void pop_front() { buffer.pop_front(); }
	void set_capacity(size_t size) { buffer.set_capacity(size); }
	size_t size() { return buffer.size(); }
	size_t capacity() const {return buffer.capacity(); }
	bool empty() const {return buffer.empty(); }
	bool full() const { return buffer.full(); }
	void clear() { buffer.clear(); }
	PreviewState& front() { return buffer.front(); }
	PreviewState& back() { return buffer.back(); }
	PreviewState& operator[] (int index) {return buffer[index]; }
private:
	boost::circular_buffer<PreviewState> buffer;
};
/**
* @class  PreBufferGenerator
* A generator for new preview state vector
*/
class PreBufferGenerator
{
public:
	/*PreBufferGenerator(const WalkCmd& walkCmd, const PreviewWalkParam& _walkParam,
		const NaoStructure& naoStructure);*/
	PreBufferGenerator();
	~PreBufferGenerator(){}
	/**
	* Reset when new walk comming.
	*/
	void reset(const WalkCmd& walkCmd, const PreviewWalkParam& _walkParam, const NaoStructure& naoStructure);
	/**
	* Change the walk speed, Check whether you can change speed now outside!!
	*/
	void changeSpeed(const WalkCmd& cmd);
	/**
	* Generate state vector for next frame
	* @return Next Preview State
	*/
	PreviewState generateNext();
	/**
	* Return whether the Generator is prepared to work.
	*/
	bool hasInitialized(){return initialized;}
	/**
	* Set the Buffer Generator prepare to stop
	*/
	void setToStop(){preparetoStop = true;}
// 	/**
// 	* Get Mini State 
// 	*/
// 	int getStartingMiniState(){
// 		if(walkStatus == WalkTypeBase::starting)
// 		{
// 			if(startCounter > framesSquatting + framesSmallRest)
// 				return 1;
// 			else
// 				return 0;
// 		}else{
// 			return -1;
// 		}
// 	}
	enum {
		Squatting = 0,
		LittleWaitting,
		PreSwinging
	};
public:
	static const double MAX_FORWARD_X; //m/step
	static const double MAX_BACKWARD_X;
	static const double MAX_LEFT_Y;
	static const double MAX_TURNCCW;/**<Max counter clock wise turning, rad/step*/
	static const double MIN_SPEED_X;
	static const double MIN_SPEED_Y;
	static const double MIN_TURNCCW;
private:
	void doFootSwitch();
	void generateNewZMP(Vector2<double>& newZMP, double& newCOMz, int& supmod_, int& iphase, int& phaseLen);
	void generateNewSwingFootPos(Vector3<double>& pfootnew, Vector3<double>& rfootnew,int supmod_,  int iphase, int phaseLen);
	void interpolateSwingFootLocus(Vector3<double>& pnew, Vector3<double>& rnew,
					const Vector3<double>& pStart, const Vector3<double>& pEnd,
					const Vector3<double>& rStart, const Vector3<double>& rEnd, int iphase, int phaseLen);
	bool checkSpeedStand(const WalkCmd& cmd);
private:
	PreviewWalkParam walkParam;
	Vector2<double> lastSupZMPp;
	Vector2<double> currentSupZMPp;
	Vector2<double> nextSupZMPp;
	double lastSupCOMz;
	double currentSupCOMz;
	double nextSupCOMz;
	Vector3<double> lastSupFootrot; /**< */
	Vector3<double> currentSupFootrot;
	Vector3<double> nextSupFootrot;/**<x, y of support foot, rot of support foot*/
	Vector2<double> lastKeyZMP; /**< x-y ZMP of last key point(switch point of two stages*/
	Vector2<double> nextKeyZMP;
	double lastKeyCOMz;
	double nextKeyCOMz;

	double standingSx;/**< Sx when standing still*/
	double standingSy;/**< Sy when standing still*/
	double walkingSx;
	double walkingSy;
	double walkingdLeft;		/**< Left size of each step, when moving lateral*/
	double lastWalkingLeft;
	double walkingTurnCCW;	/**< Turn CCW of each step, when walking*/
	double lastTurnCCW;
	bool walkJustChanged;

	bool isJustStarting;
	bool preparetoStop;
	bool finishFinalStep;
	bool stopOnLeft;	/**< If the final step performed by left leg?*/

	int walkStatus; /**< Status of current walking, active, run, or stop...*/
	int miniStatus;
	int istep;			/**<  istep in one circle*/
	int startCounter; /**< Frames since starting to walk*/
	int framesStarting; /**< Frames for Starting of walk*/
	int startIstep;
	int framesStartMovingLeft; /**< Frames of moving zmp to left foot, at starting stage, */
	int framesSmallRest;	/**< A Small rest between squatiting and moving*/
	int framesSquatting; /**< Frames used for robot to squat. */
	int standingCounter; /**< Counter after robot stopped, used to decide when to enter standing state*/
	int startSwingLeg; /**< The leg that first swing, e.g. when turning right, it should be right leg turn first, to avoid two legs coffilct with each other*/

private:
	bool initialized;
};
#endif
