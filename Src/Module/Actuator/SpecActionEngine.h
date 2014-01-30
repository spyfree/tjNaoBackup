#ifndef __SpecActionEngine_h_
#define __SpecActionEngine_h_

#include "Module/Representations//KS.h"
#include "Module/Common/SpecActionOutput.h"
#include "Module/Common/MotionCmd.h"
#include "Module/Common/JointData.h"
//#include "Module/Common/SmartPtr.h"
DEFINE_KS(SpecActionEngine)
REQUIRES(MotionPlan)
REQUIRES(SensoryJointData)
REQUIRES(SafeJointData)
PROVIDES(SpecActionOutput)
END_KS

class SpecActionEngine : public SpecActionEngineBase
{
public:
	SpecActionEngine();
	~SpecActionEngine();
	bool load();
	/************************************************************************/
	/*		Implement functions derived from Base                                        */
	/************************************************************************/
private:
	void inSafeJointData(){}
	void inSensoryJointData(){}
	void inMotionPlan();
	void update(SpecActionOutput* theSpecActionOutput);
	void outSpecActionOutput();
private:
	PROCESS_WIDE_STORAGE_STATIC SpecActionEngine* theInstance;
public:
	class ActionNode
	{
	public:
		JointCmd jointCmd;
		unsigned duration;/**< duration in ms of this node*/ 
		int mode; /**< interpolation mode or pending for later use. 0:no interpolation;1:linear_interpo;*/
		ActionNode():duration(0), mode(0){}
	};
	class Action
	{
	public:
		bool available;	/**< Whether this action is available*/
		int numNodes;	/**< Number of nodes for this action*/
		//unsigned frameTime; /**< TIme in ms between each motion frame when the action designed*/
		unsigned totalDuration; /**< The total duration in ms of this action*/
		std::vector<ActionNode> actionNodes;	
		/**
		* Load Special action script from file
		*/
		bool load(const std::string& file);

		Action():available(false), numNodes(0), totalDuration(0){}
	};
private:
	bool getNextNode();
	void calcJointCmd(SpecActionOutput& specActionOutput);
private:
	Action actions[SpecActionCmd::numSpecActions];
	bool wasActive;
	bool actionCompleted;
	int currentAction;
	int currentNode;
	long durationCounter;
	unsigned dur;
	int istep;
	int interpolate_mode;
	JointCmd currentJointCmd,
		lastJointCmd;
};
#endif //__SpecActionEngine_h_