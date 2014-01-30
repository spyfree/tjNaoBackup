#include "MotionDebugging.h"
#include <fstream>
#include <iostream>
MotionDebugging* MotionDebugging::theInstance = 0;

MotionDebugging::MotionDebugging()
:record(false)
{
	MotionDebugging::theInstance = this;
	saveBuffer.set_capacity(2000);   //newly modified
	saveBuffer.clear();
}
MotionDebugging::~MotionDebugging()
{
	MotionDebugging::theInstance = 0;
}
void MotionDebugging::saveMDebugData(const std::string &filename)
{
	if(record){
	    //std::cout<<"MotionDebugging::saveMDebugData Output Joints Log to "<<filename.c_str();
		std::ofstream fout(filename.c_str());
		if(fout.is_open())
		{
			int j;
			for(int i = 0; i < saveBuffer.size(); ++i)
			{
				MDebugSaveData& data = saveBuffer[i];

				unsigned time = data.timeStamp;
				const PreviewState& state = data.pState;
				const Vector3<double>& refCOMW = data.refCOM;
				const Vector3<double>& rlCOM = data.realCOM;
				const Vector3<double>& bodyPos = data.bodyPos;
				const JointCmd& jCmd = data.jCmd;
				const SensoryJointData& sJoints = data.sJoints;
				const SafeSensorData& sensorData = data.safeSensorData;
				const Vector3<double>& bodyAngles = data.bodyAngles;
				const Vector3<double>& psupfootori = data.psupfootori;  //newly added
				const Vector3<double>& rsupfootori = data.rsupfootori;
				const Vector3<double>& pbody2supfoot = data.pbody2supfoot;
				const Vector3<double>& rbody2supfoot = data.rbody2supfoot;
				fout<<time<<" ";
				fout<<state.istep<<"   "<<state.supmod<<" "<<state.pZMP.x<<" "<<state.pZMP.y<<" "<<state.COMz<<" ";
				fout<<state.pSwingFoot.x<<" "<<state.pSwingFoot.y<<" "<<state.pSwingFoot.z<<"     ";
				fout<<state.pSupFootZMP.x<<" "<<state.pSupFootZMP.y<<" "<<state.pSupFootZMP.z<<"      ";
				fout<<refCOMW.x<< " "<< refCOMW.y<<" " <<refCOMW.z<<"      ";
				fout<<rlCOM.x<< " "<< rlCOM.y<<" " <<rlCOM.z<<"    ";
				fout<<bodyPos.x<< " "<< bodyPos.y<<" " <<bodyPos.z<<" ";
				for(int j= 1; j < JointData::numOfJoints; ++j)
				{
					fout<< jCmd.angles[j]<<"  ";
				}
				for(int j= 1; j < JointData::numOfJoints; ++j)
				{
					fout<< sJoints.angles[j]<<" ";
				}
				for(int j = 0; j < SensorData::numOfSensors; ++j)
				  	fout<< sensorData.data[j] <<" ";
				fout<<bodyAngles.x <<" "<<bodyAngles.y<<" "<<bodyAngles.z<<" ";
				fout<<psupfootori.x<<" "<<psupfootori.y<<" "<<psupfootori.z<<"    ";   //newly added
                fout<<rsupfootori.x<<" "<<rsupfootori.y<<" "<<rsupfootori.z<<"     ";
                fout<<pbody2supfoot.x<<" "<<pbody2supfoot.y<<" "<<pbody2supfoot.z<<"     ";
                fout<<rbody2supfoot.x<<" "<<rbody2supfoot.y<<" "<<rbody2supfoot.z<<"     ";
				fout<<std::endl;
			}
		}
		//std::cout<<" DONE!"<<std::endl;
	}else{
        std::cout<<"MotionDebugging::saveMDebugData No Save Request to"<<filename.c_str()<<std::endl;
    }
}
//=========Static Functions===========//
void MotionDebugging::setRecording(bool record_)
{
	if(theInstance)
		theInstance->record = record_;
}
void MotionDebugging::addSaveData(int id, const char* data)
{
	if(theInstance && theInstance->record)
	{
		switch(id)
		{
		case MDebug::idPreviewState:
			theInstance->saveData.pState = *(PreviewState*)(data);
			break;
		case MDebug::idRefCOM:
			theInstance->saveData.refCOM = *((Vector3<double>*)(data));
			break;
		case MDebug::idRealCOM:
			theInstance->saveData.realCOM = *((Vector3<double>*)(data));
			break;
		case MDebug::idBodyPos:
			theInstance->saveData.bodyPos = *((Vector3<double>*)(data));
			break;
		case MDebug::idJointCmd:
			theInstance->saveData.jCmd = *(JointCmd*)(data);
			break;
		case MDebug::idSensoryJointData:
			theInstance->saveData.sJoints = *(SensoryJointData*)(data);
			break;
		case MDebug::idSafeSensorData:
			theInstance->saveData.safeSensorData = *(SafeSensorData*)(data);
			break;
		case MDebug::idBodyAngles:
			theInstance->saveData.bodyAngles = *(Vector3<double>*)(data);
			break;
		case MDebug::idpSupFootOri:     //newly added
			theInstance->saveData.psupfootori = *(Vector3<double>*)(data);
			break;
		case MDebug::idrSupFootOri:
			theInstance->saveData.rsupfootori = *(Vector3<double>*)(data);
			break;
		case MDebug::idpBody2SupFoot:
			theInstance->saveData.pbody2supfoot = *(Vector3<double>*)(data);
			break;
		case MDebug::idrBody2SupFoot:
			theInstance->saveData.rbody2supfoot = *(Vector3<double>*)(data);
			break;
		default:
			break;
		}
	}
}
void MotionDebugging::finishFrame(unsigned time)
{
	if(theInstance && theInstance->record)
	{
		theInstance->saveData.timeStamp = time;
		theInstance->saveBuffer.push_back(theInstance->saveData);
	}
}

void MotionDebugging::finishSave(const std::string& filename)
{
	if(theInstance)
		theInstance->saveMDebugData(filename);
}
void MotionDebugging::newSaving()
{
	if(theInstance)
		theInstance->saveBuffer.clear();
}

// void MotionDebugging::initInstance()
// {
// 	if(!theInstance)
// 	{
// 		new MotionDebugging();
// 	}
// }
