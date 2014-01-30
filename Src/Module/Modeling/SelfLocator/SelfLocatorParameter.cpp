#include "SelfLocatorParameter.h"


SelfLocatorParameter::SelfLocatorParameter():startPose(0.0,0,0),
startPoseStandardDeviation(0.0,300,300),
clipTemplateGenerationRangeX(0,0),
clipTemplateGenerationRangeY(0,0)
{
	//load("selfloc.cfg");
	//TODO：在这个下面给其赋值 
	 numberOfSamples = 100;
	 standardDeviationFieldLines = 1024;//1024;//in 1/1024 rad
	 standardDeviationCorners = 512;//512;//in 1/1024 rad

	 standardDeviationGoalpostAngle = 0.2;//0.96;//0.2;//in rad//增大增加权值or减小？
	 standardDeviationGoalpostBearingDistance = 0.3;//0.32;//0.4;//Standard deviation for the vertical angle (in rad) for the bearing-based distance to a goal post (in sensor update)
	 standardDeviationGoalpostSizeDistance = 0.2;//0.436;//0.2;//Standard deviation for the vertical angle (in rad) for the size-based distance to a goal post (in sensor update)
	 standardDeviationGoalpostSampleBearingDistance = 150;//150;// Standard deviation (in mm) for the bearing-based distance to a goal post (when adding new samples)
	 standardDeviationGoalpostSampleSizeDistance = 150;//150;//Standard deviation (in mm) for the size-based distance to a goal post (when adding new samples)
	 
	 standardDeviationCenterCircleAngle = 0.2;//0.402;//0.2;//Standard deviation for angle to center circle (in rad)
	 standardDeviationCenterCircleDistance = 0.4;//0.2;//0.4;//Standard deviation for the vertical angle (in rad) to the center circle (in sensor update)
	 
	 alphaSlow=0.0059;//4.8;//0.005;// 
	 alphaFast = 0.006;//12.43;//0.0055;//
	 resamplingThreshold =1.5;//0; //4;
	 translationNoise = 10;//115.2;//25;//mm
	 rotationNoise = 0.072;//0.1;//radians
	 movedDistWeight = 0.002;//0.002;
	 movedAngleWeight = 1;//1;
	 majorDirTransWeight = 2;//2;
	 minorDirTransWeight = 1;//1;
	 maxCrossingLength = 180;//mm
	 numberOfObservations = 6;
	 disableSensorResetting = 0;//0=false (enable sensor resetting), 1=true
	 considerGameState = 1;//consider information about game state and penalties
	 knownStartPose = 0;//0=false, 1=true
	 //startPose(0.0,0,0);
	 //startPoseStandardDeviation(0.0,300,300);
	 clipTemplateGeneration = 0;//0=false, 1=true
	 //clipTemplateGenerationRangeX(0,0);
	 //clipTemplateGenerationRangeY(0,0);
}

//void SelfLocatorParameter::load(const std::string& fileName)
//{
//	InConfigFile file(Global::getSettings().expandLocationFilename(fileName), "default");
//	ASSERT(file.exists());
//	file >> *this;
//}
