#include "SelfLocator.h"
#include "Platform/Win32/GetSetting.h"
#include "Platform/GetSetting.h"
#include "SelfLocatorParameter.h"
#include "Module/Modeling/SelfLocator/PoseCalculator/PoseCalculatorParticleHistory.h"
#include "Module/Modeling/SelfLocator/PoseCalculator/PoseCalculator2DBinning.h"
#include "Module/Modeling/SelfLocator/PoseCalculator/PoseCalculatorBestParticle.h"
#include "Module/Modeling/SelfLocator/PoseCalculator/PoseCalculatorOverallAverage.h"
#include "Module/Modeling/SelfLocator/PoseCalculator/PoseCalculatorKMeansClustering.h"
#include "Tools/Math/GaussianDistribution3D.h"
#include "Module/Modeling/SelfLocator/SensorModels/GoalPostsSensorModel.h"
#include "Module/Modeling/SelfLocator/SensorModels/LineSensorModel.h"
#include "Module/Modeling/SelfLocator/SensorModels/CenterCircleSensorModel.h"
#include "Module/Modeling/SelfLocator/SensorModels/CornersSensorModel.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
SelfLocator::SelfLocator() :
parameter(new SelfLocatorParameter),
samples(0), slowWeighting(0.0), fastWeighting(0.0), updatedBySensors(false),whetherbadvalue(false),
poseCalculator(0), /*perceptValidityChecker(0), */lastPoseComputationTimeStamp(0),odometryoffset(0.0),
sampleTemplateGenerator(*theGoalPercept, *theFrameInfo, *theFieldDimensions, *theOdometryData,
						parameter->standardDeviationGoalpostSampleBearingDistance,
						parameter->standardDeviationGoalpostSampleSizeDistance,
						parameter->clipTemplateGeneration,
						parameter->clipTemplateGenerationRangeX,
						parameter->clipTemplateGenerationRangeY),
						gameInfoPenaltyLastFrame(PENALTY_NONE),
						gameInfoGameStateLastFrame(STATE_INITIAL),
						logDataCounter(0),flagPenatly(true),oppcross(1800.0,0.0),owncross(-1800.0,0.0)
{
	observations.reserve(100);
	selectedObservations.reserve(100);
	selectedIndices.reserve(100);
	lines.reserve(100);
	readTeamConfig();
	std::cout<<"end of self locator"<<std::endl;
}

void SelfLocator::readTeamConfig()
{
	std::string buffer;
	std::string path = std::string(GetSetting::GetConfigDir()) + "/team.cfg";
	std::ifstream stream(path.c_str());
	std::cout<<path<<std::endl;
	if (!stream.is_open())
	{
		std::cout<<path<<" Open team configure file failed "<<std::endl;
	}
	stream>>teamNum;

}

void SelfLocator::publish(const GoalPercept& goalPercept ,const LinesPercept& linePercept,const BallPercept& ballPercept,const OdometryData* odometry,RobotInfo* theRobotInfo,
						  /*TeamCfgInfo* theTeamCfgInfo,*/unsigned _time,CameraMatrix* cameraMatrix,
						  RoboCupGameControlData* theRoboCupGameControlData)
{
	//LogData logData1;
	unsigned int IdOfObject = 0;
	int i = 0;
	for(i = 0;i <GoalPercept::NUMBER_OF_GOAL_POSTS;i++ )
	{
		//logDataBuffer.frameNum = visionInterface.goalpercept.posts[i].frameNum;
		logDataBuffer.logGoalData[i].objectID = i;
		logDataBuffer.logGoalData[i].distanceType = goalPercept.posts[i].distanceType;
		logDataBuffer.logGoalData[i].perceptionType = goalPercept.posts[i].perceptionType;
		logDataBuffer.logGoalData[i].positionOnField = goalPercept.posts[i].positionOnField;
		logDataBuffer.logGoalData[i].timeWhenLastSeen = goalPercept.posts[i].timeWhenLastSeen;
	}

	for(i = 0;i <GoalPercept::NUMBER_OF_UNKNOWN_GOAL_POSTS;i++ )
	{
		//logDataBuffer.frameNum = visionInterface.goalpercept.posts[i].frameNum;
		logDataBuffer.logGoalData[GoalPercept::NUMBER_OF_GOAL_POSTS+i].objectID = i + GoalPercept::NUMBER_OF_GOAL_POSTS;
		logDataBuffer.logGoalData[GoalPercept::NUMBER_OF_GOAL_POSTS+i].distanceType = goalPercept.unknownPosts[i].distanceType;
		logDataBuffer.logGoalData[GoalPercept::NUMBER_OF_GOAL_POSTS+i].perceptionType = goalPercept.unknownPosts[i].perceptionType;
		logDataBuffer.logGoalData[GoalPercept::NUMBER_OF_GOAL_POSTS+i].positionOnField = goalPercept.unknownPosts[i].positionOnField;
		logDataBuffer.logGoalData[GoalPercept::NUMBER_OF_GOAL_POSTS+i].timeWhenLastSeen = goalPercept.unknownPosts[i].timeWhenLastSeen;
	}
	logDataBuffer.circleSpot.x = linePercept.circleCenter.pos.x;
	logDataBuffer.circleSpot.y = linePercept.circleCenter.pos.y;
	logDataBuffer.circleSpotFound = linePercept.circleCenter.found;
	logDataBuffer.ballFound = ballPercept.invion;
    logDataBuffer.ballSpot.x = (int)ballPercept.relativePositionOnField.x;
    logDataBuffer.ballSpot.y = (int)ballPercept.relativePositionOnField.y;
	logDataBuffer.odometryData = odometry;
	logDataBuffer.robotInfo = theRobotInfo;
	//logDataBuffer.teamCfgInfo = theTeamCfgInfo;
	logDataBuffer.time = _time;
	logDataBuffer.cameraMatrix = cameraMatrix;
	logDataBuffer.roboCupGameControlData = theRoboCupGameControlData;

}

void SelfLocator::writeToLog()
{
	//std::string path1 = "/opt/naoqi/lib/naoqi/odometry.txt";;
	////std::string  path1="E://odometry.txt";
	//std::ofstream outFile1(path1.c_str(),ios::app);
	//outFile1<<"#OD#  "<<theOdometryData->rotation<<"  "<<theOdometryData->translation.x<<"  "<<theOdometryData->translation.y<<"\n";
	//outFile1.close();

	std::string path = "/opt/naoqi/lib/naoqi/locator.txt";
	std::ofstream outFile(path.c_str(),ios::app);
	outFile<<"#G#  "<<logDataBuffer.logGoalData[0].objectID <<"  "<<logDataBuffer.logGoalData[0].distanceType
	<<"  "<<logDataBuffer.logGoalData[0].perceptionType<<"  "<<logDataBuffer.logGoalData[0].positionOnField.x
	<<"  "<<logDataBuffer.logGoalData[0].positionOnField.y<<"  "<<logDataBuffer.logGoalData[0].timeWhenLastSeen<<"\n"
	<<"#G#  "<<logDataBuffer.logGoalData[1].objectID <<"  "<<logDataBuffer.logGoalData[1].distanceType
	<<"  "<<logDataBuffer.logGoalData[1].perceptionType<<"  "<<logDataBuffer.logGoalData[1].positionOnField.x
	<<"  "<<logDataBuffer.logGoalData[1].positionOnField.y<<"  "<<logDataBuffer.logGoalData[1].timeWhenLastSeen<<"\n"
	<<"#G#  "<<logDataBuffer.logGoalData[2].objectID <<"  "<<logDataBuffer.logGoalData[2].distanceType
	<<"  "<<logDataBuffer.logGoalData[2].perceptionType<<"  "<<logDataBuffer.logGoalData[2].positionOnField.x
	<<"  "<<logDataBuffer.logGoalData[2].positionOnField.y<<"  "<<logDataBuffer.logGoalData[2].timeWhenLastSeen<<"\n"
	<<"#G#  "<<logDataBuffer.logGoalData[3].objectID <<"  "<<logDataBuffer.logGoalData[3].distanceType
	<<"  "<<logDataBuffer.logGoalData[3].perceptionType<<"  "<<logDataBuffer.logGoalData[3].positionOnField.x
	<<"  "<<logDataBuffer.logGoalData[3].positionOnField.y<<"  "<<logDataBuffer.logGoalData[3].timeWhenLastSeen<<"\n"
	<<"#G#  "<<logDataBuffer.logGoalData[4].objectID <<"  "<<logDataBuffer.logGoalData[4].distanceType
	<<"  "<<logDataBuffer.logGoalData[4].perceptionType<<"  "<<logDataBuffer.logGoalData[4].positionOnField.x
	<<"  "<<logDataBuffer.logGoalData[4].positionOnField.y<<"  "<<logDataBuffer.logGoalData[4].timeWhenLastSeen<<"\n"
	<<"#G#  "<<logDataBuffer.logGoalData[5].objectID <<"  "<<logDataBuffer.logGoalData[5].distanceType
	<<"  "<<logDataBuffer.logGoalData[5].perceptionType<<"  "<<logDataBuffer.logGoalData[5].positionOnField.x
	<<"  "<<logDataBuffer.logGoalData[5].positionOnField.y<<"  "<<logDataBuffer.logGoalData[5].timeWhenLastSeen<<"\n"
	<<"#Circ# "<<logDataBuffer.circleSpotFound<<"  "<<logDataBuffer.circleSpot.x<<"  "<<logDataBuffer.circleSpot.y<<"\n"
	<<"#Ball# "<<logDataBuffer.ballFound<<"  "<<logDataBuffer.ballSpot.x<<"  "<<logDataBuffer.ballSpot.y<<"\n"
	<<"#ocf#  "<<logDataBuffer.odometryData->rotation<<"  "<<logDataBuffer.odometryData->translation.x
	<<"  "<<logDataBuffer.odometryData->translation.y
	<<"  "<<logDataBuffer.cameraMatrix->rotation.c[0].x<<"  "<<logDataBuffer.cameraMatrix->rotation.c[0].y
	<<"  "<<logDataBuffer.cameraMatrix->rotation.c[0].z<<"  "<<logDataBuffer.cameraMatrix->rotation.c[1].x
	<<"  "<<logDataBuffer.cameraMatrix->rotation.c[1].y<<"  "<<logDataBuffer.cameraMatrix->rotation.c[1].z
	<<"  "<<logDataBuffer.cameraMatrix->rotation.c[2].x<<"  "<<logDataBuffer.cameraMatrix->rotation.c[2].y
	<<"  "<<logDataBuffer.cameraMatrix->rotation.c[2].z<<"  "<<logDataBuffer.cameraMatrix->translation.x
	<<"  "<<logDataBuffer.cameraMatrix->translation.y<<"  "<<logDataBuffer.cameraMatrix->translation.z
	<<"  "<<logDataBuffer.cameraMatrix->isValid<<"  "<<logDataBuffer.time<<"\n";
	outFile.close();


}

LogGoalData::LogGoalData()
{
	objectID = 0;
}



//used by selfLocator::init()
void SelfLocator::setNumberOfSamples(const int num)
{
	if(samples)
		delete samples;
	samples = new SampleSet<Sample>(num);
	sensorModelWeightings.resize(num);


	//poseCalculatorType = POSE_CALCULATOR_PARTICLE_HISTORY;
	//poseCalculatorType = POSE_CALCULATOR_K_MEANS_CLUSTERING;
	poseCalculatorType = POSE_CALCULATOR_2D_BINNING;
	if(poseCalculator)
		delete poseCalculator;
	//poseCalculator = new PoseCalculatorParticleHistory< Sample, SampleSet<Sample> >(*samples);
	//poseCalculator = new PoseCalculator2DBinning< Sample, SampleSet<Sample>,100,100>(*samples,*theFieldDimensions);
	poseCalculator = new PoseCalculator2DBinning< Sample, SampleSet<Sample> ,20>(*samples,*theFieldDimensions);
}

SelfLocator::~SelfLocator()
{
	delete poseCalculator;
	delete samples;
	for(unsigned int i=0; i<sensorModels.size(); ++i)
		delete sensorModels[i];
	//if(perceptValidityChecker)
	//	delete perceptValidityChecker;
	delete parameter;
}
//used by update(robotpose)
void SelfLocator::init()
{
	fieldModel.init(*theFieldDimensions, parameter->maxCrossingLength);
	for(unsigned int i=0; i<sensorModels.size(); ++i)
		delete sensorModels[i];
	sensorModels.clear();
	//if(perceptValidityChecker)
	//	delete perceptValidityChecker;
	//perceptValidityChecker = new PerceptValidityChecker(theFieldDimensions);
	sensorModels.push_back(new GoalPostsSensorModel(*parameter,
		*theGoalPercept, *theFrameInfo, *theFieldDimensions, *theCameraMatrix/*, *perceptValidityChecker*/));
	sensorModels.push_back(new CenterCircleSensorModel(*parameter,
		*theLinesPercept, *theFrameInfo, *theFieldDimensions, *theCameraMatrix/*, *perceptValidityChecker*/));
	sensorModels.push_back(new LineSensorModel(*parameter,
		lines, *theFrameInfo, *theFieldDimensions, *theCameraMatrix, /**perceptValidityChecker,*/
		fieldModel));
	sensorModels.push_back(new CornersSensorModel(*parameter,
		*theLinesPercept, *theFrameInfo, *theFieldDimensions, *theCameraMatrix, /**perceptValidityChecker,*/
		fieldModel));

	setNumberOfSamples(parameter->numberOfSamples);
	sampleTemplateGenerator.init();
	std::vector<Pose2D> poses;
	std::vector<Pose2D> standardDeviations;
	if(parameter->knownStartPose)
	{
		poses.push_back(parameter->startPose);
		standardDeviations.push_back(parameter->startPoseStandardDeviation);
	}
	// If no start position is known, spread samples uniformly across state space
	initSamplesAtGivenPositions(poses, standardDeviations);
}
//used by init()
void SelfLocator::initSamplesAtGivenPositions(const std::vector<Pose2D>& poses,
											  const std::vector< Pose2D >& standardDeviations)
{
	for(int i = 0; i < samples->size(); ++i)
	{
		Sample& sample(samples->at(i));
		if(poses.size())
		{
			// Select one of the poses from the list:
			unsigned int index = random(poses.size());
			Pose2D pose = poses[index];
			Pose2D stdDev = standardDeviations[index];
			// Create sample:
			sample.translation.x = static_cast<int>(pose.translation.x);
			sample.translation.x += sampleTriangularDistribution(static_cast<int>(stdDev.translation.x));
			sample.translation.y = static_cast<int>(pose.translation.y);
			sample.translation.y += sampleTriangularDistribution(static_cast<int>(stdDev.translation.y));
			double rotation = pose.rotation;
			rotation += sampleTriangularDistribution(stdDev.rotation);
			normalize(rotation);
			sample.angle = rotation;
			sample.rotation = Vector2<int>(int(cos(rotation) * 1024), int(sin(rotation) * 1024));

			//set samples in own side
			//Vector2<double> pos = Vector2<double>(double(sample.translation.x),double(sample.translation.y));
			//if(!theFieldDimensions->isInsideOwnSide(pos))
			//{
			//	Pose2D pose(theFieldDimensions->randomPoseOnOwnSide());
			//	sample.translation = Vector2<int>(int(pose.translation.x), int(pose.translation.y));
			//	sample.rotation = Vector2<int>(int(cos(pose.rotation) * 1024), int(sin(pose.rotation) * 1024));
			//	sample.angle = sample.rotation.angle();
			//}

			
		}
		else //No given positions, spread uniformly:
		{
			//Pose2D pose(theFieldDimensions->randomPoseOnField());
			Pose2D pose(theFieldDimensions->randomPoseOnOwnSide());
			sample.translation = Vector2<int>(int(pose.translation.x), int(pose.translation.y));
			sample.rotation = Vector2<int>(int(cos(pose.rotation) * 1024), int(sin(pose.rotation) * 1024));
			sample.angle = sample.rotation.angle();
		}
	}
	lastOdometry = *theOdometryData;
	poseCalculator->init();
}
#ifdef NAO_DEBUG
void SelfLocator::inOdometryData()
{
	//selfMessageQueue->SearchMyMessage(idOdometryData,idActuatorThread,idVisionThread,
	//	(char*)theOdometryData,sizeof(OdometryData));

}
#else
void SelfLocator::inOdometryData()
{
	/*selfMessageQueue->SearchMyMessage(idOdometryData,idActuatorThread,idVisionThread,
		(char*)theOdometryData,sizeof(OdometryData));*/
}
#endif

void SelfLocator::inFrameInfo()
{
	//std::cout<<"*****************Frame time "<<theFrameInfo->time<<std::endl;
}


void SelfLocator::update(SelfLocatorSampleSet* sampleSet)
{
	//sample set needs to have been updated within this frame:
	if(lastPoseComputationTimeStamp != theFrameInfo->time)
	{
		RobotPose dummyPose;
		update(&dummyPose);
	}
	sampleSet->weightingsSum = totalWeighting;
	samples->link(sampleSet->sampleSetProxy);
}

void SelfLocator::update(RobotPose*theRobotPose)
{

	//fisrt get ownSide that been processed by RobotPostValitor
	ownSide = theRobotPose->ownSide;
	//SimulationLog();
	if (parameter->numberOfSamples != samples->size())
		init();
	// Maybe pose has already been computed by call to other update function (for clusters)
	if(lastPoseComputationTimeStamp == theFrameInfo->time)
	{
		theRobotPose = &lastComputedPose;
		return;
	}

	// Normal computation:
	preExecution(*theRobotPose);
	bool templatesOnly(false);
	bool odometryOnly(false);

	if(abs(theRobotPose->middleTranslationX) <= 200)
		odometryOnly= true;

	if(templatesOnly) //debug
	{
		if(sampleTemplateGenerator.templatesAvailable())
			for(int i = 0; i < this->samples->size(); ++i)
				generateTemplate(samples->at(i));
		poseCalculator->calcPose(*theRobotPose);

		//updatedBySensors = applySensorModels();
		//if(updatedBySensors)//视觉观测更新，计算每个粒子点的权值
		//{
		//	adaptWeightings();
		//	resampling();//重采样，删除权值低的粒子，复制权值高的粒子
		//}
		//poseCalculator->calcPose(*theRobotPose);//重新求机器人位姿
	}
	else if(odometryOnly) //debug
	{
		//Pose2D odometryOffset = *theOdometryData - lastOdometry;
		//*theRobotPose += odometryOffset;
		//lastOdometry = *theOdometryData;
		motionUpdate(0);
		poseCalculator->calcPose(*theRobotPose);
		theRobotPose->validity = 0.85;
	}
	else //normal case
	{

		motionUpdate(updatedBySensors);//动作模型更新，每个粒子点同步移动
		updatedBySensors = applySensorModels();
		if(updatedBySensors)//视觉观测更新，计算每个粒子点的权值
		{
			adaptWeightings();
			resampling();//重采样，删除权值低的粒子，复制权值高的粒子
			checkSamplesSide();
		}
		poseCalculator->calcPose(*theRobotPose);//重新求机器人位姿
	}
	theRobotPose->deviation = 100000.;
    lastComputedPose = *theRobotPose;
	lastPoseComputationTimeStamp = theFrameInfo->time;

    if(theRobotPose->translation.x<=0.0)
		theRobotPose->Cross=(owncross-theRobotPose->translation).rotate(-theRobotPose->rotation);
	else if(theRobotPose->translation.x>0.0)
		theRobotPose->Cross=(oppcross-theRobotPose->translation).rotate(-theRobotPose->rotation);

	//get ownSide been calculated from the samples
	if(theRobotPose->translation.x <= 0)
		lastOwnSide = true;
	else
		lastOwnSide = false;

	//if the ownSide from the poseValidator diff from the lastOwnSide from the samples,then reverse the samples
	if(ownSide != lastOwnSide&&theRobotPose->sideValidity!=2&&abs(theRobotPose->middleTranslationX) > 250)
	{
		reverseSamples();
		std::cout<<"RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR"<<std::endl;
	}

#ifdef NAO_DEBUG
	drawSamples();
	theRobotPose->draw();

#endif
	CalculatepositionCovariance();
	

	//writeLocatorData();

	//double minyaw,maxyaw,minpitch,maxpitch;
	//calculateHeadAngle(minyaw,maxyaw,minpitch,maxpitch);
	//std::cout<<"@@@@@@minyaw="<<-180*minyaw/pi<<" "<<"mixyaw="<<-180*maxyaw/pi<<" "<<"minpitch="<<-180*minpitch/pi<<" "<<"maxpitch="<<-180*maxpitch/pi<<std::endl;

}
void SelfLocator::checkSamplesSide()
{
	if (abs(theRobotPose->translation.x) < 500 )
	{
		return;
	}
	for(int i = 0 ;i < samples->size(); ++i)
	{
		if ((samples->at(i).translation.x > 0 && theRobotPose->ownSide == true)||(samples->at(i).translation.x < 0 && theRobotPose->ownSide == false))
		{
			samples->at(i).translation.x = -samples->at(i).translation.x;
			samples->at(i).translation.y = -samples->at(i).translation.y;
			if (samples->at(i).angle >= 0)
			{
				samples->at(i).angle = -pi + samples->at(i).angle;
			}
			else
			{
				samples->at(i).angle = pi + samples->at(i).angle;
			}
		}
	}
}

void SelfLocator::reverseSamples()
{
	for(int i = 0; i < samples->size(); ++i)
	{
		samples->at(i).translation.x = -samples->at(i).translation.x;
		samples->at(i).translation.y = -samples->at(i).translation.y;
		if (samples->at(i).angle >= 0)
		{
			samples->at(i).angle = -pi + samples->at(i).angle;
		}
		else
		{
			samples->at(i).angle = pi + samples->at(i).angle;
		}
	}
}
void SelfLocator::SimulationLog()
{
	logDataCounter++;
	publish(*theGoalPercept,*theLinesPercept,*theBallPercept,theOdometryData,theRobotInfo,/*theTeamCfgInfo,*/
		theFrameInfo->time,theCameraMatrix,theRoboCupGameControlData);
	if(logDataCounter>=50&&logDataCounter <=4000)
	{
		writeToLog();
	}
}

bool cmpClusterPairs(std::pair<int,int> a, std::pair<int,int> b)
{
	return a.second > b.second;
}

void SelfLocator::update(RobotPoseHypotheses* robotPoseHypotheses)
{
	robotPoseHypotheses->hypotheses.clear();
	//update only available for two types of pose calculation:
	if(poseCalculatorType != POSE_CALCULATOR_PARTICLE_HISTORY && poseCalculatorType != POSE_CALCULATOR_K_MEANS_CLUSTERING)
		return;
	//sample set needs to have been updated within this frame:
	if(lastPoseComputationTimeStamp != theFrameInfo->time)
	{
		RobotPose dummyPose;
		update(&dummyPose);
	}

	if(poseCalculatorType == POSE_CALCULATOR_PARTICLE_HISTORY)
	{
		//get hypotheses:
		PoseCalculatorParticleHistory< Sample, SampleSet<Sample> >* poseHistoryCalc
			= (PoseCalculatorParticleHistory< Sample, SampleSet<Sample> >*)poseCalculator;
		std::vector< std::pair<int,int> > clusters = poseHistoryCalc->getClusters();
		std::sort(clusters.begin(),clusters.end(),cmpClusterPairs);
		const unsigned int MAX_HYPOTHESES = std::min<unsigned int>(robotPoseHypotheses->MAX_HYPOTHESES, clusters.size());
		for(unsigned int i=0; i<MAX_HYPOTHESES; ++i)
		{
			// No mini clusters, please:  n
			if(clusters[i].second <= 3)
				break;
			// Compute average position:
			RobotPoseHypothesis newHypothesis;
			poseHistoryCalc->calcPoseOfCluster(newHypothesis, clusters[i].first);
			Vector2<int> newTrans(static_cast<int>(newHypothesis.translation.x), static_cast<int>(newHypothesis.translation.y));
			// Compute variance of position:
			int varianceX(0);
			int varianceY(0);
			for(int j = 0; j < samples->size(); ++j)
			{
				Sample& s(samples->at(j));
				if(s.cluster == clusters[i].first)
				{
					varianceX += (s.translation.x - newTrans.x) * (s.translation.x - newTrans.x);
					varianceY += (s.translation.y - newTrans.y) * (s.translation.y - newTrans.y);
				}
			}
			varianceX /= (clusters[i].second - 1);
			varianceY /= (clusters[i].second - 1);
			newHypothesis.positionCovariance[0][0] = static_cast<double>(varianceX);
			newHypothesis.positionCovariance[1][1] = static_cast<double>(varianceY);
			// Compute covariance:
			int cov_xy(0);
			for(int j = 0; j < samples->size(); ++j)
			{
				Sample& s(samples->at(j));
				if(s.cluster == clusters[i].first)
					cov_xy += (s.translation.x - newTrans.x) * (s.translation.y - newTrans.y);
			}
			cov_xy /= (clusters[i].second - 1);
			newHypothesis.positionCovariance[0][1] = newHypothesis.positionCovariance[1][0] = static_cast<double>(cov_xy);
			// Finally add to list:
			robotPoseHypotheses->hypotheses.push_back(newHypothesis);
		}
	}

	if(poseCalculatorType == POSE_CALCULATOR_K_MEANS_CLUSTERING)
	{
		//get hypotheses:
		PoseCalculatorKMeansClustering< Sample, SampleSet<Sample>, 5, 1000 >* poseKMeansCalc
			= (PoseCalculatorKMeansClustering< Sample, SampleSet<Sample>, 5, 1000 >*)poseCalculator;
		for(unsigned int i=0; i<5; ++i)
		{
			RobotPoseHypothesis newHypothesis;
			newHypothesis.validity = poseKMeansCalc->getClusterValidity(i);
			if(newHypothesis.validity > 0)
			{
				poseKMeansCalc->getClusterPose(newHypothesis, i);
				newHypothesis.positionCovariance[0][0] = 0.1;   //TODO: calculate covariances
				newHypothesis.positionCovariance[0][1] = 0.1;
				newHypothesis.positionCovariance[1][0] = 0.1;
				newHypothesis.positionCovariance[1][1] = 0.1;
				robotPoseHypotheses->hypotheses.push_back(newHypothesis);
			}
		}
	}
}

void SelfLocator::preExecution(RobotPose& robotPose)
{
	//添加新的，除去旧的门的perception
	sampleTemplateGenerator.bufferNewPerceptions();

	// Reset all weightings to 1.0
	for(int i = 0; i < samples->size(); ++i)
		samples->at(i).weighting = 1.0;


	PoseCalculatorType newPoseCalculatorType(poseCalculatorType);


	bool reloadPoseCalculator = (newPoseCalculatorType != poseCalculatorType);


	if(reloadPoseCalculator)
	{
		delete poseCalculator;
		switch(newPoseCalculatorType)
		{
		case POSE_CALCULATOR_2D_BINNING:
			poseCalculator = new PoseCalculator2DBinning< Sample, SampleSet<Sample>, 10 >
				(*samples, *theFieldDimensions);
			break;
		case POSE_CALCULATOR_PARTICLE_HISTORY:
			poseCalculator = new PoseCalculatorParticleHistory< Sample, SampleSet<Sample> >(*samples);
			poseCalculator->init();
			break;
		case POSE_CALCULATOR_BEST_PARTICLE:
			poseCalculator = new PoseCalculatorBestParticle< Sample, SampleSet<Sample> >(*samples);
			break;
		case POSE_CALCULATOR_OVERALL_AVERAGE:
			poseCalculator = new PoseCalculatorOverallAverage< Sample, SampleSet<Sample> >(*samples);
			break;
		case POSE_CALCULATOR_K_MEANS_CLUSTERING:
			poseCalculator = new PoseCalculatorKMeansClustering< Sample, SampleSet<Sample>, 5, 1000 >
				(*samples, *theFieldDimensions);
			break;
		default: ASSERT(false);
		}
		poseCalculatorType = newPoseCalculatorType;
	}

	// if the considerGameState flag is set, reset samples when necessary
	//回归原位
	if(parameter->considerGameState)
	{
		// penalty shoot: if game state switched to playing, reset samples to start pos
		if(int(theRoboCupGameControlData->secondaryState) == STATE2_PENALTYSHOOT /*||  theBehaviorConfiguration.agent.compare("penalty") == 0*/)
		{
			int tjarkNum=(uint32)theRoboCupGameControlData->teams[0].teamNumber==TJArkTeamNum?0:1;
			if(/*gameInfoGameStateLastFrame == STATE_SET && */gameInfoGameStateLastFrame!=STATE_PLAYING&&int(theRoboCupGameControlData->state) == STATE_PLAYING&&tjarkNum==0)
			{
				//init();
				std::vector<Pose2D> poses;
				std::vector<Pose2D> stdDevs;
				poses.push_back(Pose2D(0.0, -2500, 0.0));
				//poses.push_back(Pose2D(0.0, 0.0, 500));
				//stdDevs.push_back(Pose2D(0.2, 200, 200));
				stdDevs.push_back(Pose2D(0.2, 200, 200));
				initSamplesAtGivenPositions(poses, stdDevs);
				//aaaa++;
			}
			else if(gameInfoGameStateLastFrame!=STATE_PLAYING&&int(theRoboCupGameControlData->state) == STATE_PLAYING&&tjarkNum==1)
			{
				std::vector<Pose2D> poses;
				std::vector<Pose2D> stdDevs;
				poses.push_back(Pose2D(0.0, 0.0, 0.0));
				//poses.push_back(Pose2D(0.0, 0.0, 500));
				//stdDevs.push_back(Pose2D(0.2, 200, 200));
				stdDevs.push_back(Pose2D(0.2, 200, 200));
				initSamplesAtGivenPositions(poses, stdDevs);
				//aaaa++;
			}
		}
		// normal game: if penalty is over reset samples to reenter positions
		if(int(theRoboCupGameControlData->secondaryState) == STATE2_NORMAL /*|| theBehaviorConfiguration.agent.compare("soccer") == 0*/)
		{
			if(gameInfoPenaltyLastFrame != PENALTY_NONE && theRobotInfo->penalty == PENALTY_NONE)
			{
				std::vector<Pose2D> poses;
				std::vector<Pose2D> stdDevs;
				poses.push_back(Pose2D(-pi/2.0, -1200., theFieldDimensions->yPosLeftSideline));
				poses.push_back(Pose2D(pi/2.0, -1200., theFieldDimensions->yPosRightSideline));
				stdDevs.push_back(Pose2D(0.2, 200, 200));
				stdDevs.push_back(Pose2D(0.2, 200, 200));
				initSamplesAtGivenPositions(poses, stdDevs);

			}

			if(gameInfoGameStateLastFrame == STATE_INITIAL && int(theRoboCupGameControlData->state) == STATE_READY)
			{
				std::vector<Pose2D> poses;
				std::vector<Pose2D> stdDevs;
				switch(teamNum)
				{
				case 1:
					poses.push_back(Pose2D(pi/2.0,-1500.0,theFieldDimensions->yPosRightSideline));
					break;
				case 3:
					poses.push_back(Pose2D(-pi/2.0,-1500.0,theFieldDimensions->yPosLeftSideline));
					break;
				case 2:
					poses.push_back(Pose2D(-pi/2.0,-1000.0,theFieldDimensions->yPosLeftSideline));
					break;
				case 4:
					poses.push_back(Pose2D(pi/2.0,-1000.0,theFieldDimensions->yPosRightSideline));
					break;
				default:
					poses.push_back(Pose2D(pi/2.0,-1200.0,theFieldDimensions->yPosRightSideline));

				}
				
				stdDevs.push_back(Pose2D(0.2,200,200));
				initSamplesAtGivenPositions(poses,stdDevs);
			}
		}
		if(!flagPenatly){
			gameInfoPenaltyLastFrame = theRobotInfo->penalty;
			gameInfoGameStateLastFrame = theRoboCupGameControlData->state;
		}

	}

	// Initialize sample set again:
	//DEBUG_RESPONSE("module:SelfLocator:resetSamples", init(););
	// Set team color in robot pose

	robotPose.ownTeamColorForDrawing = theRoboCupGameControlData->teams[0].teamColour == TEAM_BLUE ? ColorRGBA(0,0,150) : ColorRGBA(255,0,0);
	 flagPenatly=false;
}

void SelfLocator::motionUpdate(bool noise)
{
	Pose2D odometryOffset = *theOdometryData - lastOdometry;
	odometryoffset=odometryOffset;
	const double transNoise = noise ? parameter->translationNoise : 0;
	const double rotNoise = noise ? parameter->rotationNoise : 0;
	const int transX = (int) odometryOffset.translation.x;
	const int transY = (int) odometryOffset.translation.y;
	const double dist = odometryOffset.translation.abs();
	const double angle = fabs(odometryOffset.rotation);
    lastOdometry += Pose2D(odometryOffset.rotation, transX, transY);

	// precalculate rotational error that has to be adapted to all samples
	const double rotError = std::max<double>(rotNoise,
		std::max<double>(dist * parameter->movedDistWeight,
		angle * parameter->movedAngleWeight));

	// precalculate translational error that has to be adapted to all samples
	const int transXError = (int) std::max<double>(transNoise,
		std::max<double>(fabs(transX * parameter->majorDirTransWeight),
		fabs(transY * parameter->minorDirTransWeight)));
	const int transYError = (int) std::max<double>(transNoise,
		std::max<double>(fabs(transY * parameter->majorDirTransWeight),
		fabs(transX * parameter->minorDirTransWeight)));
	for(int i = 0; i < samples->size(); i++)
	{
		Sample& s(samples->at(i));

		// the translational error vector


		const Vector2<int> transOffset((((transX - transXError) << 10) + 512 + ((transXError << 1) + 1) * (rand() & 0x3ff)) >> 10,
			(((transY - transYError) << 10) + 512 + ((transYError << 1) + 1) * (rand() & 0x3ff)) >> 10);

		// update the sample

		s.translation = Vector2<int>(((s.translation.x << 10) + transOffset.x * s.rotation.x - transOffset.y * s.rotation.y + 512) >> 10,
			((s.translation.y << 10) + transOffset.x * s.rotation.y + transOffset.y * s.rotation.x+ 512) >> 10);//s.rotation乘以了1024，此时应右移十位
		s.angle += odometryOffset.rotation + (randomDouble() * 2 - 1) * rotError;
		s.angle = normalize(s.angle);
		//s.rotation = Vector2<int>(int(cosf((float)s.angle) * 1024), int(sinf((float)s.angle) * 1024));
		s.rotation = Vector2<int>(int(cos(s.angle) * 1024), int(sin(s.angle) * 1024));
		// clip to field boundary
		Vector2<double> translationAsDouble(s.translation.x, s.translation.y);
		theFieldDimensions->clipToCarpet(translationAsDouble);
		s.translation.x = static_cast<int>(translationAsDouble.x);
		s.translation.y = static_cast<int>(translationAsDouble.y);
	}
}

//bool SelfLocator::applyShadowLocs()
//{
//	double standardDeviation = 100;
//	const double bestPossibleWeighting = gaussianProbability(0.0, standardDeviation);
//
//	for(int i =0;i < theCurrentGlobalInfo->shadowLoc.size();++i)
//	{
//		for(int j = 0; j < samples->size(); ++j)
//		{
//			samples->at(j).weighting *= computerLocWeighting(samples->at(j).toPose(),theCurrentGlobalInfo->shadowLoc[i],standardDeviation,bestPossibleWeighting);
//		}
//	}
//	return true;
//}
//double SelfLocator::computerLocWeighting(const Pose2D& robotPose,RobotLocation& shadowLoc,double standardDeviation, double bestPossibleWeighting)
//{
//	double Weighting1 = gaussianProbability(fabs(shadowLoc.x - robotPose.translation.x), standardDeviation) / bestPossibleWeighting;
//	double Weighting2 = gaussianProbability(fabs(shadowLoc.y - robotPose.translation.y), standardDeviation) / bestPossibleWeighting;
//	return (Weighting1 + Weighting2)/2;
//}
bool SelfLocator::applySensorModels()
{
	//每次门和中圈都优先被选
	selectedObservations.clear();
	for(int i = 0; i < GoalPercept::NUMBER_OF_GOAL_POSTS; ++i)
		if(theGoalPercept->posts[i].timeWhenLastSeen == theFrameInfo->time)
			selectedObservations.push_back(SensorModel::Observation(SensorModel::Observation::GOAL_POST, i));
	if(selectedObservations.empty()) // no known posts
		for(int i = 0; i < GoalPercept::NUMBER_OF_UNKNOWN_GOAL_POSTS; ++i)
			if(theGoalPercept->unknownPosts[i].timeWhenLastSeen == theFrameInfo->time)
				selectedObservations.push_back(SensorModel::Observation(SensorModel::Observation::GOAL_POST, i + GoalPercept::NUMBER_OF_GOAL_POSTS));
	//LogPercept doesn't provide line information
	if(theLinesPercept->circleCenter.found)
		selectedObservations.push_back(SensorModel::Observation(SensorModel::Observation::CENTER_CIRCLE, 0));

	// points and corners are optional
	observations.clear();
	lines.clear();
	int j = 0;
	for(std::list<LinesPercept::LineOnField>::const_iterator i = theLinesPercept->lineOnField.begin();
		i != theLinesPercept->lineOnField.end(); ++i)
	{
		lines.push_back(&*i);
		observations.push_back(SensorModel::Observation(SensorModel::Observation::POINT, j++));
		observations.push_back(SensorModel::Observation(SensorModel::Observation::POINT, j++));
	}
	for(int i = 0; i < (int) theLinesPercept->intersections.size(); ++i)
		observations.push_back(SensorModel::Observation(SensorModel::Observation::CORNER, i));

	if(selectedObservations.empty() && observations.empty())
		return false;

	// select observations
	while((int) selectedObservations.size() < parameter->numberOfObservations)
		if(observations.empty())
			selectedObservations.push_back(selectedObservations[rand() % selectedObservations.size()]);
		else
			selectedObservations.push_back(observations[rand() % observations.size()]);

	// apply sensor models
	bool sensorModelApplied(false);
	std::vector<SensorModel*>::iterator sensorModel = sensorModels.begin();
	for(; sensorModel != sensorModels.end(); ++sensorModel)
	{
		selectedIndices.clear();
		for(std::vector<SensorModel::Observation>::const_iterator i = selectedObservations.begin(); i != selectedObservations.end(); ++i)
			if(i->type == (*sensorModel)->type)
				selectedIndices.push_back(i->index);//这里的index是在selectedObservations里面的序号?

		SensorModel::SensorModelResult result;
		if(selectedIndices.empty())
			result = SensorModel::NO_SENSOR_UPDATE;
		else
		{
			//std::cout<<"before computeWeightings"<<std::endl;
			result = /*SensorModel::NO_SENSOR_UPDATE*/(*sensorModel)->computeWeightings(*samples, selectedIndices, sensorModelWeightings);
			//std::cout<<"after computeWeightings"<<std::endl;
		}
		if(result == SensorModel::FULL_SENSOR_UPDATE)//门柱返回的一定是这个result
		{
			for(int i = 0; i < samples->size(); ++i)
			{
				samples->at(i).weighting *= sensorModelWeightings[static_cast<unsigned int>(i)];
				sensorModelApplied = true;
			}
		}
		else if(result == SensorModel::PARTIAL_SENSOR_UPDATE)
		{
			// Compute average of all valid weightings, use this average for all invalid ones
			double sum(0.0);
			int numOfValidSamples(0);
			for(unsigned int i=0; i<sensorModelWeightings.size(); ++i)
			{
				if(sensorModelWeightings[i] != -1)
				{
					sum += sensorModelWeightings[i];
					++numOfValidSamples;
				}
			}
			if(numOfValidSamples == 0) //Should not happen, but you never know...
			{
				//OUTPUT(idText, text, "Meeeeek! SensorModel claimed to have made a partial update but did not provide any weighting!");
				continue;
			}
			const double averageWeighting(sum / numOfValidSamples);
			for(unsigned int i=0; i<sensorModelWeightings.size(); ++i)
			{
				if(sensorModelWeightings[i] != -1)
					samples->at(i).weighting *= sensorModelWeightings[i];
				else
					samples->at(i).weighting *= averageWeighting;
			}
			sensorModelApplied = true;
		}
	}
	//for(int i = 0; i < samples->size(); ++i)
	//{
	//	std::cout<<"*****sample weighting*  "<<"i"<<"  "<<samples->at(i).weighting<<std::endl;
	//	//sensorModelApplied = true;
	//}
	return sensorModelApplied;
}

void SelfLocator::resampling()
{
	// swap sample arrays
	Sample* oldSet = samples->swap();
	const int numberOfSamples(samples->size());
	const double weightingsSum(totalWeighting);
	const double resamplingPercentage(sampleTemplateGenerator.templatesAvailable() ? std::max<double>(0.0, 1.0 - fastWeighting / slowWeighting) : 0.0);
	const double numberOfResampledSamples = parameter->disableSensorResetting ? numberOfSamples : numberOfSamples * (1.0 - resamplingPercentage);
	const double threshold = parameter->resamplingThreshold * weightingsSum / numberOfSamples;
	const double weightingBetweenTwoDrawnSamples((weightingsSum + threshold * numberOfSamples) / numberOfResampledSamples);
	double nextPos(randomDouble() * weightingBetweenTwoDrawnSamples);
	double currentSum(0);

	// resample:
	int j(0);
	for(int i = 0; i < numberOfSamples; ++i)
	{
		currentSum += oldSet[i].weighting + threshold;
		while(currentSum > nextPos && j < numberOfSamples)
		{
			samples->at(j++) = oldSet[i];
			nextPos += weightingBetweenTwoDrawnSamples;
		}
	}
    int numberofresample =j;
	//std::cout<<"##########重采样的个数#######"<<j<<std::endl;

	// fill up remaining samples with new poses:
	if(sampleTemplateGenerator.templatesAvailable())
		for(; j < numberOfSamples; ++j)
			generateTemplate(samples->at(j));
	else if(j) // in rare cases, a sample is missing, so add one (or more...)
		for(; j < numberOfSamples; ++j)
			samples->at(j) = samples->at(rand() % j);
			//samples->at(j) = oldSet[rand() % j];
	else // resampling was not possible (for unknown reasons), so create a new sample set (fail safe)
#ifdef NDEBUG
	{
		for(int i = 0; i < numberOfSamples; ++i)
		{
			Sample& sample = samples->at(i);
			Pose2D pose(theFieldDimensions.randomPoseOnField());
			sample.translation = Vector2<int>(int(pose.translation.x), int(pose.translation.y));
			sample.rotation = Vector2<int>(int(cos(pose.rotation) * 1024), int(sin(pose.rotation) * 1024));
		}
		poseCalculator->init();
	}
#else
		ASSERT(false);
#endif

}

void SelfLocator::adaptWeightings()
{
	totalWeighting = 0;
	int numberOfSamples(samples->size());
	for(int i=0; i<numberOfSamples; ++i)
		totalWeighting += samples->at(i).weighting;
	if(totalWeighting == 0)
	{
		//ASSERT(totalWeighting); // For debugging
		return;                 // For "real" operation
	}
	const double averageWeighting = totalWeighting / numberOfSamples;
	if(slowWeighting)
	{
		slowWeighting = slowWeighting + parameter->alphaSlow * (averageWeighting - slowWeighting);
		fastWeighting = fastWeighting + parameter->alphaFast * (averageWeighting - fastWeighting);
	}
	else
	{
		slowWeighting = averageWeighting;
		if(parameter->knownStartPose)
			fastWeighting = averageWeighting;   // Must be done to avoid complete re-init in first cycle
	}
	//std::cout<<"##averageWeighting    "<<averageWeighting<<"   slowWeighting  "<<slowWeighting<<"  fastWeighting  "
	//	<<fastWeighting<<"  fast/slow "<<fastWeighting/slowWeighting <<std::endl;
	//PLOT("module:SelfLocator:averageWeighting", averageWeighting * 1e3);
	//PLOT("module:SelfLocator:slowWeighting", slowWeighting * 1e3);
	//PLOT("module:SelfLocator:fastWeighting", fastWeighting * 1e3);
}
//used by resampling and update(robotpose)
void SelfLocator::generateTemplate(Sample& sample)
{
	if(sampleTemplateGenerator.templatesAvailable())
	{
		const SampleTemplate st = sampleTemplateGenerator.getNewTemplate();
		sample.translation = Vector2<int>(int(st.translation.x), int(st.translation.y));
		sample.rotation = Vector2<int>(int(cos(st.rotation) * (1 << 10)),
			int(sin(st.rotation) * (1 << 10)));
		sample.angle = st.rotation;
		sample.weighting = 0;
		sample.cluster = poseCalculator->getNewClusterIndex();
	}
	return;
}

void SelfLocator::drawSamples()
{
	const int numberOfSamples(samples->size());
	const double maxWeighting = 2*totalWeighting / numberOfSamples;
	for(int i = 0; i < numberOfSamples; ++i)
	{
		const Sample& s(samples->at(i));
		const Pose2D pose(s.angle, s.translation.x, s.translation.y);
		unsigned char weighting = (unsigned char) (s.weighting / maxWeighting * 255);
		Vector2<double> bodyPoints[4] = {Vector2<double>(55,90),
			Vector2<double>(-55,90),
			Vector2<double>(-55,-90),
			Vector2<double>(55,-90)};
		for(int j = 0; j < 4; ++j)
		{
			bodyPoints[j] = Geometry::rotate(bodyPoints[j], pose.rotation);
			bodyPoints[j] += pose.translation;
		}
		Vector2<double> headPos(30,0);
		headPos = Geometry::rotate(headPos, pose.rotation);
		headPos += pose.translation;
		ColorRGBA color = s.weighting ? ColorRGBA(weighting,weighting,weighting) : ColorRGBA(255,0,0);
		if(s.cluster == poseCalculator->getIndexOfBestCluster())
			color = ColorRGBA(200,0,200);

		bool drawlikeGT=true;
		if(!drawlikeGT)
		{
			Vector2<double> start(pose.translation.x,pose.translation.y);
	     	Vector2<double> end(pose.translation.x+60*cos(pose.rotation),pose.translation.y+60*sin(pose.rotation));
		   CIRCLE(pose.translation.x,pose.translation.y,30,20,LocDrawings::ps_solid,color,LocDrawings::bs_null,color);
		   ARROW(start,end,10,LocDrawings::ps_solid,color);
		}
		else
		{
			POLYGON(4,bodyPoints,15,LocDrawings::ps_solid,color,LocDrawings::bs_solid,color);
			CIRCLE(headPos.x, headPos.y, 30, 1,
				LocDrawings::ps_solid, color, LocDrawings::bs_solid, color);
		}

		//Vector2<double> start(pose.translation.x,pose.translation.y);
		//Vector2<double> end(pose.translation.x+60*cos(pose.rotation),pose.translation.y+60*sin(pose.rotation));
		//CIRCLE(pose.translation.x,pose.translation.y,30,20,LocDrawings::ps_solid,color,LocDrawings::bs_null,color);
		//ARROW(start,end,10,LocDrawings::ps_solid,color);
	}
}
void SelfLocator::ClusterSamples(Robotsamplebuffer&robotsamplebuffer)
{
	const int numberOfSamples(samples->size());
	for(int i = 0; i < numberOfSamples; ++i)
	{
		const Sample& s(samples->at(i));
		Robotsamplebuffer::singleSample ss;
		ss.translationX=int32(s.translation.x);
		ss.translationY=int32(s.translation.y);
		ss.rotationX=int32(s.rotation.x);
		ss.rotationY=int32(s.rotation.y);
		ss.cluster=int32(s.cluster);
		ss.angle=s.angle;
		ss.weighting=s.weighting;
		ss.cluster=int32(s.cluster);
		if(s.cluster == poseCalculator->getIndexOfBestCluster()) ss.InbestCluster=true;
		else ss.InbestCluster=false;
		robotsamplebuffer.samples[i]=ss;
	}
	robotsamplebuffer.Robotsample[0].Robotrotation= theRobotPose->rotation;
	robotsamplebuffer.Robotsample[0].RobotTranslationX=theRobotPose->translation.x;
	robotsamplebuffer.Robotsample[0].RobotTranslationY=theRobotPose->translation.y;
	robotsamplebuffer.Robotsample[0].color.r=uint8(theRobotPose->ownTeamColorForDrawing.r);
	robotsamplebuffer.Robotsample[0].color.g=uint8(theRobotPose->ownTeamColorForDrawing.g);
	robotsamplebuffer.Robotsample[0].color.b=uint8(theRobotPose->ownTeamColorForDrawing.b);
	robotsamplebuffer.Robotsample[0].totalWeighting=totalWeighting;


}
void SelfLocator::update(Robotsamplebuffer*theRobotsamplebuffer)
{
   ClusterSamples(*theRobotsamplebuffer);
   //std::cout<<"人定位粒子update"<<std::endl;
}
void SelfLocator::writeLocatorData()
{
	std::string locatorDataPath = "/opt/naoqi/lib/naoqi/LocData.txt";
	std::ofstream locatorDataoutFile(locatorDataPath.c_str(),ios::app);
	locatorDataoutFile<<"#Loc# "<<theRobotPose->translation.x<<"  "<<theRobotPose->translation.y<<"  "<<theRobotPose->rotation<<
		"  "<<sqrt(positionCov[0][0])<<"  "<<sqrt(positionCov[1][1])<<"  "<<sqrt(positionCov[1][0])<<"\n";
	locatorDataoutFile.close();
}
bool SelfLocator::CheckBigChangeOfselfLocalization()
{
	int current=robotPoseRingBuffer.getCurrent();
	int beforecurrent=current-1;
   if(robotPoseRingBuffer.getNumberOfEntries()>=2)
     if(fabs(robotPoseRingBuffer[current].translation.x-robotPoseRingBuffer[beforecurrent].translation.x)+fabs(robotPoseRingBuffer[current].translation.y-
		 robotPoseRingBuffer[beforecurrent].translation.y)+fabs(robotPoseRingBuffer[current].rotation-robotPoseRingBuffer[beforecurrent].rotation)*320>3000)
	     return false;
	 else return true;

 return true;
}
void SelfLocator::CalculatepositionCovariance()
{
	Vector2<int> newTrans(static_cast<int>(theRobotPose->translation.x), static_cast<int>(theRobotPose->translation.y));
	// Compute variance of position:
	int varianceX(0);
	int varianceY(0);
	for(int j = 0; j < samples->size(); ++j)
	{
		Sample& s(samples->at(j));
			varianceX += (s.translation.x - newTrans.x) * (s.translation.x - newTrans.x);
			varianceY += (s.translation.y - newTrans.y) * (s.translation.y - newTrans.y);
	}
	varianceX /= (samples->size() - 1);
	varianceY /= (samples->size()- 1);
	positionCov[0][0] = static_cast<double>(varianceX);
	positionCov[1][1] = static_cast<double>(varianceY);
	// Compute covariance:
	int cov_xy(0);
	for(int j = 0; j < samples->size(); ++j)
	{
		Sample& s(samples->at(j));
			cov_xy += (s.translation.x - newTrans.x) * (s.translation.y - newTrans.y);
	}
	cov_xy /= (samples->size()- 1);
	positionCov[0][1] = positionCov[1][0] = static_cast<double>(cov_xy);
	// Finally add to list:
	//robotPoseHypotheses->hypotheses.push_back(newHypothesis);
}
bool SelfLocator::calculateHeadAngle(double&minYaw,double&maxYaw,double&minPitch,double&maxPitch)
{
	pair<double,double> oppgoalYawAngle,owngoalYawAngle,centerYawAngle,oppgoalPitchAngle,owngoalPitchAngle,centerPitchAngle;
	Vector2<double>LeftoppgoalpositionToRobot=Vector2<double>(double(theFieldDimensions->xPosOpponentGroundline)-theRobotPose->translation.x,double(theFieldDimensions->yPosLeftGoal)-theRobotPose->translation.y).rotate(-theRobotPose->rotation);//将门转到机器人坐标系下、
	Vector2<double>RightoppgoalpositionToRobot=Vector2<double>(double(theFieldDimensions->xPosOpponentGroundline)-theRobotPose->translation.x,double(-theFieldDimensions->yPosLeftGoal)-theRobotPose->translation.y).rotate(-theRobotPose->rotation);//将门转到机器人坐标系下、
	Vector2<double>LeftowngoalpositionToRobot=Vector2<double>(double(-theFieldDimensions->xPosOpponentGroundline)-theRobotPose->translation.x,double(-theFieldDimensions->yPosLeftGoal)-theRobotPose->translation.y).rotate(-theRobotPose->rotation);
	Vector2<double>RightowngoalpositionToRobot=Vector2<double>(double(-theFieldDimensions->xPosOpponentGroundline)-theRobotPose->translation.x,double(theFieldDimensions->yPosLeftGoal)-theRobotPose->translation.y).rotate(-theRobotPose->rotation);
	double angleforleftoppgoal=atan2(LeftoppgoalpositionToRobot.y,LeftoppgoalpositionToRobot.x);
	double angleforrightoppgoal=atan2(RightoppgoalpositionToRobot.y,RightoppgoalpositionToRobot.x);
	double angleforleftowngoal=atan2(LeftowngoalpositionToRobot.y,LeftowngoalpositionToRobot.x);
	double angleforrightowngoal=atan2(RightowngoalpositionToRobot.y,RightowngoalpositionToRobot.x);

	/*angleforleftoppgoal=angleforleftoppgoal<-pi/2.||angleforleftoppgoal>pi/2.?-angleforleftoppgoal:angleforleftoppgoal;
	angleforrightoppgoal=angleforrightoppgoal<-pi/2.||angleforrightoppgoal>pi/2.?-angleforrightoppgoal:angleforrightoppgoal;
	angleforleftowngoal=angleforleftowngoal<-pi/2.||angleforleftowngoal>pi/2.?-angleforleftowngoal:angleforleftowngoal;
	angleforrightowngoal=angleforrightowngoal<-pi/2.||angleforrightowngoal>pi/2.?-angleforrightowngoal:angleforrightowngoal;*/

	oppgoalYawAngle=make_pair(min(angleforleftoppgoal,angleforrightoppgoal),max(angleforleftoppgoal,angleforrightoppgoal));
	owngoalYawAngle=make_pair(min(angleforleftowngoal,angleforrightowngoal),max(angleforleftowngoal,angleforrightowngoal));
	Vector2<double>CenterPositionToRobot=Vector2<double>(0.0,0.0).rotate(theRobotPose->rotation)+theRobotPose->translation;
	double angleforcenter=atan2(CenterPositionToRobot.y,CenterPositionToRobot.x);
	centerYawAngle=make_pair(angleforcenter-pi/6.0,angleforcenter+pi/6.0);
	double oppleftpitch=LeftoppgoalpositionToRobot.abs()>=600.?0.:0.48;
	double opprightpitch=RightoppgoalpositionToRobot.abs()>=600.?0.:0.48;
	double ownleftpitch=LeftowngoalpositionToRobot.abs()>=600.?0.:0.48;
	double ownrightpitch=RightowngoalpositionToRobot.abs()>=600.?0.:0.48;
	double centerpitch=CenterPositionToRobot.abs()>=600.?0.:0.48;
	if(oppleftpitch!=opprightpitch)
		oppgoalPitchAngle=make_pair(-0.48,0.6);
	else if(oppleftpitch==0.)
		oppgoalPitchAngle=make_pair(0.,0.6);
	else
		oppgoalPitchAngle=make_pair(-0.48,0.);
	if(ownleftpitch!=ownrightpitch)
		owngoalPitchAngle=make_pair(-0.48,0.6);
	else if (ownleftpitch==0.)
		owngoalPitchAngle=make_pair(0.,0.6);
	else
		owngoalPitchAngle=make_pair(-0.48,0.);
	if(centerpitch==0.)
		centerPitchAngle=make_pair(0.,0.6);
	else
		centerPitchAngle=make_pair(-0.48,0.);
	minYaw=-1000.;maxYaw=1000.;minPitch=-1000.;maxPitch=1000.;
	if(oppgoalYawAngle.second<=2.0&&oppgoalYawAngle.first>=-2.0)
	{
		minYaw=oppgoalYawAngle.first;
		maxYaw=oppgoalYawAngle.second;
	}

	if (owngoalYawAngle.second<=2.0&&owngoalYawAngle.first>=-2.)
	{
		if(minYaw==-1000.) minYaw=owngoalYawAngle.first;
		else if(owngoalYawAngle.first<minYaw) minYaw=owngoalYawAngle.first;

		if(maxYaw==1000.) maxYaw=owngoalYawAngle.second;
		else if (owngoalYawAngle.second>maxYaw) maxYaw=owngoalYawAngle.second;

	}
	if (centerYawAngle.second<=2.0&&centerYawAngle.first>=-2.)
	{
		if(minYaw==-1000.) minYaw=centerYawAngle.first;
		else if(centerYawAngle.first<minYaw) minYaw=centerYawAngle.first;

		if(maxYaw==1000.) maxYaw=centerYawAngle.second;
		else if (centerYawAngle.second>maxYaw) maxYaw=centerYawAngle.second;

	}
	if(oppgoalPitchAngle.second<=0.6&&oppgoalPitchAngle.first>-0.48)
	{
		minPitch=oppgoalPitchAngle.first;
		maxPitch=oppgoalPitchAngle.second;
	}
	if (owngoalPitchAngle.second<=0.6&&owngoalPitchAngle.first>-0.48)
	{
		if(minPitch==-1000.) minPitch=owngoalPitchAngle.first;
		else if(owngoalPitchAngle.first<minPitch) minPitch=owngoalPitchAngle.first;

		if(maxPitch==1000.) maxPitch=owngoalPitchAngle.second;
		else if(owngoalPitchAngle.second>maxPitch) maxPitch=owngoalPitchAngle.second;

	}
	if (centerPitchAngle.second<=0.6&&centerPitchAngle.first>-0.48)
	{
		if(minPitch==-1000.) minPitch=centerPitchAngle.first;
		else if(centerPitchAngle.first<minPitch) minPitch=centerPitchAngle.first;

		if(maxPitch==1000.) maxPitch=centerPitchAngle.second;
		else if(centerPitchAngle.second>maxPitch) maxPitch=centerPitchAngle.second;

	}
	return true;
}
MAKE_KS(SelfLocator)
