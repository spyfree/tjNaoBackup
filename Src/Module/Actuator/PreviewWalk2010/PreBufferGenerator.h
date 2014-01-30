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
#include <list>
/**
* @class PreviewState
* A class that represents the state vector at each frame in our preview control walk
*/
class PreviewState
{
public:
	PreviewState():istep(0),supmod(0), walkStatus(0), COMz(0)
	{
	}
	PreviewState(int supmod, int walkStatus, double comZ,
		const Vector3<double>& pZMP):istep(0),supmod(supmod), walkStatus(walkStatus),
			COMz(comZ), pZMP(pZMP)
	{
	}
public:
	int istep;			/**< Pending, not used*/
	int supmod;		/**< Supporting mode*/
	int walkStatus; /**< Indicating whether the robot is running , or starting , or ending...*/
	double COMz;	/**< z of COM*/
	Vector3<double> pZMP; /**< x, y,z: position of ZMP projected on x-y plane, world coordinate*/
	Vector3<double> pSwingFoot; /**< position of swing foot, ??? coordinate, Pending, not used*/
	Vector3<double> rSwingFoot; /**< Rotation angles of foot, Pending, not used*/
	Vector3<double> pSupFootZMP; /**< Position of supporting foot (NOT the ZMP) , world coordinate!*/
	Vector3<double> rSupFootZMP; /**< Rotation of supporting foot (NOT the ZMP) , world coordinate!*/
};

/**
* @class PreviewBuffer
* A warpper class for circular_buffer of boost lib
*/
class PreviewBuffer
{
public:
	PreviewBuffer(size_t _capacity):
	  buffer(_capacity){}
	PreviewBuffer():buffer(0){}
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
	class Knot : public PreviewState
	{
	public:
		Knot():fstamp(0), fnum(0)
		{
		}
		Knot(const PreviewState& state):PreviewState(state), fstamp(0), fnum(0)
		{
		}
		int fstamp;/**< frame stamp of this knot*/
		int fnum;/**< num of frames of the phase  next to this knot*/
	};
	/**
	* A class represent information about swing foot
	*/
	class SwgFInfo
	{
	public:
		SwgFInfo():fStart(0), fEnd(0){}
		Vector3<double> pStart, pEnd;
		Vector3<double> rStart, rEnd;
		double fStart, fEnd;
	};
public:
	PreBufferGenerator();
	void setWalkParam(const PreviewWalkParam& walkParam_)
	{
		walkParam = walkParam_;
	}
	/**
	* Reset when new walk comming.
	*/
	void reset(const WalkCmd& walkCmd, const NaoStructure& naoStructure);
	/**
	* Change the walk speed, Check whether you can change speed now outside!!
	*/
	void changeSpeed(const WalkCmd& cmd, const NaoStructure& naoStructure);
	/**
	* Generate state vector for next frame
	* @return Next Preview State
	*/
	void generateNext();
	/**
	* Return whether the Generator is prepared to work.
	*/
	bool hasInitialized(){return initialized;}
	/**
	* Set the Buffer Generator prepare to stop
	*/
	void setToStop(){
		preparetoStop = true;
		genStopKnot();
	}
	bool isPreparetoStop(){return preparetoStop;}
	void finishEnding(){preparetoStop=false;}
	//void modifyFootPrint(PreviewBuffer& preBuffer, Vector3<double>* pDeltaZMP);
	void doLandingEvent(const Vector3<double>& pStepError, const Vector3<double>& rStepError);
	void modifyStepPosition(const Vector3<double>& pStepError, const Vector3<double>& rStepError, int fDelay);
	PreviewBuffer& getPreviewBuffer(){ return previewBuffer;}
	const SwgFInfo& getSwingFootInfo(){ return swgFInfo; }
	void getCurrentSwgF(Vector3<double>& pnew, Vector3<double>& rnew, int ileg);
	/**
	* Find the knot next to current frame(cF)
	*/
	const Knot* findNextKnot();
	/**
	* Find the knot control current frame
	*/
	const Knot* findCurrentKnot();
private:
	bool checkSpeedStand(const WalkCmd& cmd);
	void genStartKnot();
	void genStopKnot();
	int addKnot(const PreviewState& state, int fnum);
	void fillPreviewBuffer();
 	void deleteOldKnot();
 	void genNextWalkKnot();
	void checkSwingFoot();
// 	void doFootLandError();
// 	void modify();
	/**
	* If Current Frame in this knot, i.e. cF in the phase controlled by this knot
	*/
	bool cFInKnot(const Knot* pthis)
	{
		return (pthis->fstamp <= cF) && ((pthis->fstamp+pthis->fnum) > cF);
	}
//	bool findNextKnot(const Knot** pknot, std::list< Knot >::const_iterator& itr, int supmod);
    bool findNextKnot(const Knot** pknot, std::list< Knot >::iterator& itr, int supmod);
	void interpolateSwingFootLocus(Vector3<double>& pnew, Vector3<double>& rnew,
		const Vector3<double>& pStart, const Vector3<double>& pEnd,
		const Vector3<double>& rStart, const Vector3<double>& rEnd, int iphase, int phaseLen, int ileg);
private:
	PreviewWalkParam walkParam;
	PreviewBuffer previewBuffer;
	std::list< Knot > knotList;
	int phaseLens[4];
	SwgFInfo swgFInfo;
	const int numPreviews;
	double standingSx;/**< Sx when standing still*/
	double standingSy;/**< Sy when standing still*/
	double standingCoMz;
	double walkingSx;
	double walkingSy;
	double walkingSz;
	double walkingdLeft;		/**< Left size of each step, when moving lateral*/
	double walkingTurnCCW;	/**< Turn CCW of each step, when walking*/
	double walkingCoMz;
	bool preparetoStop;
	bool leftStopLanding;
	bool finishFinalStep;
	int walkStatus; /**< Status of current walking, active, run, or stop...*/
	int istep;			/**<  istep in one circle*/
	int cF; /**< Frames Stamp of Current frame, currentFrame*/
	//int pF;/**< Frames Stamp of Preview,previewFrame*/
	//int bF;	/**< Frames of last point in the buffer now, bufferFrame*/
	int firstStepLeg;
private:
	bool initialized;
};
#endif
