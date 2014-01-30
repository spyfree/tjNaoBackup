#include "SpecActionEngine.h"
#include <iostream>
#include <fstream>
#include "Platform/GetSetting.h"
static const std::string SpecActionPathName = std::string(GetSetting::GetConfigDir())+"/SpecActions/";//"../../../Config/SpecActions/";
static const std::string SpecActionPosfix = ".act";
PROCESS_WIDE_STORAGE SpecActionEngine* SpecActionEngine::theInstance = NULL;

bool SpecActionEngine::Action::load(const std::string& file)
{
	bool loadOK = false;
	numNodes = 0;
	/*frameTime = 40;*/
	actionNodes.clear();
	std::ifstream fin(file.c_str());
	if(fin)
	{
		fin >> numNodes/*>>frameTime*/;
		if(numNodes > 0)
		{
			totalDuration = 0;
			int i = 0;
			double tmp;
			for(i = 0; i < numNodes; ++i)
			{
				ActionNode node;
				// ignore space
				while(isspace(fin.peek()))
					fin.get();
				// ignore lines beginning with '#'
				if(fin.peek() == '#'){
					while(fin.peek() != '\n')
						fin.get();
					--i;
					continue;
				}
				if(! fin.good()){
					std::cout<<"Error in action file "<< file.c_str()<< " - unexpected EOF" << std::endl;
					loadOK = false;
					break;
				}
				for(int j = JointCmd::HeadYaw; j < JointCmd::numOfJoints; ++j)
				{
					// read to non-space char
					while(isspace(fin.peek()))
						fin.get();
					if(fin.peek() == '*'){
						fin.get();
						node.jointCmd.angles[j] = JointCmd::ignore;
					}
// 					else if(fin.peek() == '-'){
// 						fin.get();
// 						node.jointCmd.angles[j] = JointCmd::off;
// 					}
					else{
						fin >> tmp;
						node.jointCmd.angles[j] = deg2rad(tmp);
					}
				}
				fin >> node.mode >> node.duration;
				actionNodes.push_back(node);
				totalDuration += node.duration;
			}
			if(i == numNodes){
				available = true;
				loadOK = true;
			}
		}
	}
	//File load failed!
	if(!loadOK)
	{
		available = false;
		numNodes = 0;
		/*frameTime = 40;*/
		actionNodes.clear();
	}
	fin.close();
	return loadOK;
}

SpecActionEngine::SpecActionEngine()
: wasActive(false),
actionCompleted(true),
currentAction(SpecActionCmd::numSpecActions),
currentNode(0),
durationCounter(0),
istep(0)
{
	theInstance = this;
	load();
}
SpecActionEngine::~SpecActionEngine()
{

}
bool SpecActionEngine::load()
{
	for(int i = 0; i < SpecActionCmd::numSpecActions; ++i)
	{
		std::string filename = SpecActionPathName + SpecActionCmd::getSpecActionName(i) + SpecActionPosfix;
		if(!(actions[i].load(filename)))
		{
			std::cout<<"Load Special Action: "<<SpecActionCmd::getSpecActionName(i)<<" from "<<filename<<" Failed!"<<std::endl;
		}
	}
	return true;
}
bool SpecActionEngine::getNextNode()
{
	if(++currentNode < actions[currentAction].numNodes)
	{
		ASSERT(currentNode >= 0 && currentNode < actions[currentAction].numNodes);
		durationCounter = actions[currentAction].actionNodes[currentNode].duration;
		currentJointCmd = actions[currentAction].actionNodes[currentNode].jointCmd;
		interpolate_mode = actions[currentAction].actionNodes[currentNode].mode;
		dur = actions[currentAction].actionNodes[currentNode].duration;
	}else{
		//action finished
		return false;
	}
	return true;
}
void SpecActionEngine::calcJointCmd(SpecActionOutput& specActionOutput)
{
	if(interpolate_mode == 1){
		for(int i = 1; i < JointData::numOfJoints; ++i)
		{
			// if servo is switched off use JointData for further calculation
			if(lastJointCmd.angles[i] == JointData::off || lastJointCmd.angles[i] == JointData::ignore)
				lastJointCmd.angles[i] = theSafeJointData->angles[i];

			if(currentJointCmd.angles[i] == JointData::off || currentJointCmd.angles[i] == JointData::ignore)
				specActionOutput.angles[i] = currentJointCmd.angles[i];
			else
				specActionOutput.angles[i] = (double) (currentJointCmd.angles[i] +
				(lastJointCmd.angles[i] - currentJointCmd.angles[i]) * durationCounter / dur);
		}
	}else{
		for(int i  = 1; i < JointData::numOfJoints; ++i)
			specActionOutput.angles[i] = currentJointCmd.angles[i];
	}
}
void SpecActionEngine::inMotionPlan()
{
}
void SpecActionEngine::update(SpecActionOutput* theSpecActionOutput)
{
	double speedfactor = 1.0;
	unsigned time;
	if(theMotionPlan->motion == MotionPlan::SpecialAction &&
		actions[theMotionPlan->specActionCmd.specAction].available)
	{
		if(durationCounter <= 0){
			if(!wasActive)
			{
				istep = 0;
				currentNode = -1;
				durationCounter = 0;
				currentAction = theMotionPlan->specActionCmd.specAction;
				actionCompleted = false;
				for(int i = JointData::HeadYaw; i < JointData::numOfJoints; ++i)
					lastJointCmd.angles[i] = theSafeJointData->angles[i];
			}
			if(!getNextNode()){
				actionCompleted = true;
				theSpecActionOutput->odometryOffset = Pose2D();//currently no odometry data
				theSpecActionOutput->isLeavingPossible = true;
				wasActive = false;
				return;
			}
		}else{
			time = (int)(0.04*1000*speedfactor);//NaoConfig->frameTime = 0.04
			durationCounter -= time;
			istep += time;
			istep += durationCounter < 0?durationCounter : 0;
		}
		calcJointCmd(*theSpecActionOutput);
		theSpecActionOutput->isLeavingPossible = false;
		theSpecActionOutput->executedSpecAction = currentAction;
		//calculate odometry
		//update lastcmd
		if(durationCounter <= 0)
			lastJointCmd = currentJointCmd;

	}
	wasActive = (theMotionPlan->motion == MotionPlan::SpecialAction);
}
void SpecActionEngine::outSpecActionOutput()
{
}

MAKE_KS(SpecActionEngine)
