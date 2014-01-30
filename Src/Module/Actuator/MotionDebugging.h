#ifndef __MotionDebugging_h_
#define __MotionDebugging_h_

namespace MDebug{
	enum{
		idPreviewState,
		idRefCOM,
		idRealCOM,
		idBodyPos,
		idJointCmd,
		idSensoryJointData,
		idSafeSensorData,
		idBodyAngles,
		idpSupFootOri,  //newly added 
		idrSupFootOri,
		idpBody2SupFoot,
		idrBody2SupFoot,
		numMDeugIDs
	};
}
#define OPEN_MOTION_DEBUG
#ifdef OPEN_MOTION_DEBUG
#include "Module/Actuator/PreviewWalk2010/PreBufferGenerator.h"
#include "Module/Common/JointData.h"
#include "Module/Common/SensorData.h"
#include "Platform/SystemCall.h"
#include <boost/circular_buffer.hpp>

/**
*
*/
#define MDEBUG_OPEN_RECORD(checked) \
	MotionDebugging::setRecording(checked);
/**
* Save some data, only data declared in MDebug can be saved.
*/
#define MDEBUG_SAVE(id, data) \
	MotionDebugging::addSaveData(id, (const char*)&data);
/**
* Push one data into the circular buffer when one frame finishes
*/
#define MDEBUG_FINISH_FRAME(time) \
	MotionDebugging::finishFrame(time);
/**
* Save all the data in saveBuffer to filename
*/
#define MDEBUG_FINISH_SAVE(filename) \
	MotionDebugging::finishSave(filename);
/**
* New debugging savings, clear the buffer
*/
#define MDEBUG_NEW() \
	MotionDebugging::newSaving();
/**
* Declare we will use motion debugging here, be care to use only once!
*/
#define MDEBUG_DECLARE \
	static MotionDebugging theMotionDebug;

class MotionDebugging {
public:
	MotionDebugging();
	~MotionDebugging();
	//MotionDebugging* getInstance() { return MotionDebugging::theInstance; }
private:
	static MotionDebugging* theInstance; /**< Not Thread Storage mode, one copy globally*/
private:
	void saveMDebugData(const std::string &filename);
private:
	class MDebugSaveData
	{
	public:
		unsigned  timeStamp;
		PreviewState pState;
		Vector3<double> refCOM;
		Vector3<double> realCOM;
		Vector3<double> bodyPos;
		JointCmd jCmd;
		SensoryJointData sJoints;
		SafeSensorData safeSensorData;
		Vector3<double> bodyAngles;
		Vector3<double> psupfootori;    //newly added 
        Vector3<double> rsupfootori;
		Vector3<double> pbody2supfoot;
		Vector3<double> rbody2supfoot;
	};
	boost::circular_buffer<MDebugSaveData> saveBuffer;
	MDebugSaveData saveData;
	bool record;
public:
	static void setRecording(bool record_);
	static void addSaveData(int id, const char* data);
	static void finishFrame(unsigned time);
	static void finishSave(const std::string& filename);
// 	static void initInstance();
// 	static void destroyInstance();
	static void newSaving();
};
#else
#define MDEBUG_SAVE(id, data)
#define MDEBUG_FINISH_FRAME(time)
#define MDEBUG_FINISH_SAVE(filename)
#define MDEBUG_NEW()
#define MDEBUG_DECLARE
#endif //OPEN_MOTION_DEBUG

#endif //__MotionDebugging_h_