#include "ReadLogData.h"
#include "Platform/Win32/GetSetting.h"

PROCESS_WIDE_STORAGE ReadLogData* ReadLogData::theReadLogData = NULL; 
ReadLogData::ReadLogData()
{
	std::string path = std::string(GetSetting::GetConfigDir()) +"/Simulation/Locator/"+  "locator.txt";
	std::string tempPath = std::string(GetSetting::GetConfigDir()) +"/Simulation/Locator/"+  "temp.txt";
	logPath = new char[strlen(path.c_str())+1];
	logTempPath=new char[strlen(tempPath.c_str()+1)];
	strcpy(logPath,path.c_str());
	strcpy(logTempPath,tempPath.c_str());
}

ReadLogData* ReadLogData::getInstance()
{
	if(theReadLogData == NULL)
	{

		theReadLogData = new ReadLogData();
	}

	return theReadLogData;
} 

void ReadLogData::readLog()
{
	
	char flagStr[10];
	bool justStart = true;
	bool termination = false;
	SimLogData tmpData, reladData;
	//char* printSymble = "#G#";
	std::vector<SimLogData> reloadBuffer;
	if(logPath == NULL)
	{
		std::cout<<"can't open the log path!!!"<<std::endl;
		return;
	}
	std::ifstream stream(logPath,std::ios_base::binary);
	if(!stream.is_open())
	{
		std::cout<<logPath<<" Open failed! "<<std::endl;
	}
	
	for(int i=0;i<6;i++)
	{
		stream>>flagStr;
		if(isPerceptFlagStr(flagStr))
		{
			stream>>logDataBuffer.simGoalData[i];
		}
	}
	stream>>flagStr;
	if (isCircStr(flagStr))
	{
		stream>>logDataBuffer.circle;
	}
	stream>>flagStr;
	if (isBallStr(flagStr))
	{
		stream>>logDataBuffer.ballPercept;
	}
	stream>>flagStr;
	if(isLogFlagStr(flagStr))
	{
		stream>>logDataBuffer;
		stream.ignore(1);
	}
	std::ofstream output(logTempPath,std::ios_base::binary);
	output<<stream.rdbuf();
	stream.close();
	output.close();
	std::ifstream infile(logTempPath,std::ios_base::binary);
	std::ofstream outfile(logPath,std::ios_base::binary);
	outfile<<infile.rdbuf();
	infile.close();
	outfile.close();
}

bool ReadLogData::isPerceptFlagStr(const char* str)
{
	return strcmp(str,"#G#") == 0;
}

bool ReadLogData::isLogFlagStr(const char* str)
{
	return strcmp(str,"#ocf#")==0;
}

bool ReadLogData::isBallStr(const char* str)
{
	return strcmp(str,"#Ball#")==0;
}
bool ReadLogData::isCircStr(const char* str)
{
	return strcmp(str,"#Circ#")==0;
}

SimGoalData::SimGoalData()
{
	objectID = 0;
}

std::istream& operator >> (std::istream &is, SimGoalData &simData)
{
	is >>simData.objectID >>simData.distanceType >>simData.perceptionType
		>>simData.positionOnField.x>>simData.positionOnField.y>>simData.timeWhenLastSeen;
	return is;
}

std::istream& operator >> (std::istream &is, BallPercept &ballData)
{
	is>>ballData.invion>>ballData.relativePositionOnField.x>>ballData.relativePositionOnField.y;
	return is;
}
std::istream& operator >> (std::istream &is, LinesPercept::CircleSpot &circ)
{
	is>>circ.found>>circ.pos.x>>circ.pos.y;
	return is;
}
//std::ostream& operator << (std::ostream &out, SimGoalData &simData)
//{
//	out <<"#G#  "<<simData.objectID <<"  "<<simData.distanceType <<"  "<<simData.perceptionType
//		<<"  "<<simData.positionOnField.x<<"  "<<simData.positionOnField.y<<"  "<<simData.timeWhenLastSeen
//		<<std::endl;
//	return out;
//}

std::istream& operator >> (std::istream &is, SimLogData &simData)
{
	is  >>simData.odometryData.rotation>>simData.odometryData.translation.x
		>>simData.odometryData.translation.y
		>>simData.cameraMatrix.rotation.c[0].x
		>>simData.cameraMatrix.rotation.c[0].y>>simData.cameraMatrix.rotation.c[0].z
		>>simData.cameraMatrix.rotation.c[1].x>>simData.cameraMatrix.rotation.c[1].y
		>>simData.cameraMatrix.rotation.c[1].z>>simData.cameraMatrix.rotation.c[2].x
		>>simData.cameraMatrix.rotation.c[2].y>>simData.cameraMatrix.rotation.c[2].z
		>>simData.cameraMatrix.translation.x
		>>simData.cameraMatrix.translation.y>>simData.cameraMatrix.translation.z
		>>simData.cameraMatrix.isValid>>simData.time>>simData.ballPercept.invion
		>>simData.ballPercept.BallCenter.x>>simData.ballPercept.BallCenter.y
		>>simData.ballPercept.coordinateRobot.x>>simData.ballPercept.coordinateRobot.y
		>>simData.walkCmd.speed.translation.x>>simData.walkCmd.speed.translation.y;
	return is;
}

//std::ostream& operator << (std::ostream &out, SimLogData &simData)
//{
//	out <<"#ocf#  "<<simData.odometryData.rotation<<"  "<<simData.odometryData.translation.x
//		<<"  "<<simData.odometryData.translation.y<<"  "<<simData.cameraBottomMatrix.rotation.c[0].x
//		<<"  "<<simData.cameraBottomMatrix.rotation.c[0].y
//		<<"  "<<simData.cameraBottomMatrix.rotation.c[0].z<<"  "<<simData.cameraBottomMatrix.rotation.c[1].x
//		<<"  "<<simData.cameraBottomMatrix.rotation.c[1].y<<"  "<<simData.cameraBottomMatrix.rotation.c[1].z
//		<<"  "<<simData.cameraBottomMatrix.rotation.c[2].x<<"  "<<simData.cameraBottomMatrix.rotation.c[2].y
//		<<"  "<<simData.cameraBottomMatrix.rotation.c[2].z<<"  "<<simData.cameraBottomMatrix.translation.x
//		<<"  "<<simData.cameraBottomMatrix.translation.y<<"  "<<simData.cameraBottomMatrix.translation.z
//		<<"  "<<simData.cameraBottomMatrix.isValid<<"  "<<simData.time<<"\n";
//	
//	return out;
//}