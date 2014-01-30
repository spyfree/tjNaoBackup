#ifndef SelfLocator_H
#define SelfLocator_H
#include "Module/Representations/Blackboard.h"
#include "Module/Representations/BaseProvider.h"
#include "Tools/SampleSet.h"
#include "Tools/RingBuffer.h"
#include "Module/Common/FrameInfo.h"

#include "SampleTemplateGenerator.h"
#include "SensorModels/SensorModel.h"
#include "SensorModels/FieldModel.h"

#include "Module/Network/RoboCupGameControlData.h"
#include "Module/Representations/KS.h"
#include "Module/Representations/MotionControl/OdometryData.h"
#include "Module/Network/networkComm/Wdatainfo.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Representations/Perception/GoalPercetp.h"
#include "Module/Representations/Perception/LinesPercept.h"
#include "Module/Representations/Perception/BallPercept.h"
#include "Module/Representations/Perception/RobotPercept.h"
#include "Module/Representations/Modeling/RobotPose.h"
#include "Module/Representations/Modeling/RobotPoseHypotheses.h"
#include "Module/Representations/Modeling/SelfLocatorSampleSet.h"
#include "Module/Representations/Configuration/FieldDimensions.h"
#include "Module/Network/networkComm/NetworkMsg.h"
#include "Module/Modeling/SelfLocator/SampleTransferredToPc.h"
#include "Tools/Math/Pose2D.h"
#include "Tools/Math/Pose3D.h"
#include "Tools/Math/GaussianTable.h"
#include <fstream>
#include <iostream>
#include <vector>

#ifdef NAO_DEBUG
#include "Simulation/LocatorGUI/FieldDrawing.h"
#endif

template <typename Sample, typename SampleContainer>
class PoseCalculator;
class SelfLocatorParameter;

#define MAX_BUFFER 100

DEFINE_KS(SelfLocator)
REQUIRES(OdometryData)
REQUIRES(GoalPercept)
REQUIRES(BallPercept)
REQUIRES(LinesPercept)
REQUIRES(RoboCupGameControlData)
REQUIRES(RobotInfo)
//REQUIRES(TeamCfgInfo)
REQUIRES(FrameInfo)
REQUIRES(FieldDimensions)
REQUIRES(CameraMatrix)
PROVIDES(RobotPose)
PROVIDES(Robotsamplebuffer)
USES(RobotPose)
//PROVIDES(RobotPoseHypotheses)
//PROVIDES(SelfLocatorSampleSet)
END_KS

class LogGoalData
{
public:
	LogGoalData();
	LogGoalData& operator=(const LogGoalData& other)
	{
		this->objectID = other.objectID;
		this->distanceType = other.distanceType;
		this->perceptionType = other.perceptionType;
		this->positionOnField = other.positionOnField;
		this->timeWhenLastSeen = other.timeWhenLastSeen;
		return *this;
	}
	unsigned int objectID;
	int distanceType;
	int perceptionType;
	Vector2<int> positionOnField;
	unsigned timeWhenLastSeen;
};
class LogData
{
public:
	LogData(){}
	LogData& operator=(const LogData& other)
	{
		this->odometryData = other.odometryData;
		this->robotInfo = other.robotInfo;
		//this->teamCfgInfo = other.teamCfgInfo;
		this->time = other.time;
		this->cameraMatrix = other.cameraMatrix;
		this->roboCupGameControlData = other.roboCupGameControlData;
		this->circleSpot = other.circleSpot;
		this->circleSpotFound = other.circleSpotFound;
		this->ballSpot = other.ballSpot;
		this->ballFound = other.ballFound;
		/*for(int i =0;i<GoalPercept::NUMBER_OF_GOAL_POSTS+GoalPercept::NUMBER_OF_UNKNOWN_GOAL_POSTS;i++)
			this->logGoalData[i] = other.logGoalData[i]*/
		return *this;
	}
	const OdometryData* odometryData;
	RobotInfo* robotInfo;
	//TeamCfgInfo* teamCfgInfo;
	unsigned time; //FrameInfo
	CameraMatrix* cameraMatrix; 
	RoboCupGameControlData* roboCupGameControlData;
	LogGoalData logGoalData[GoalPercept::NUMBER_OF_GOAL_POSTS+GoalPercept::NUMBER_OF_UNKNOWN_GOAL_POSTS];
    Vector2<int> circleSpot;
	bool circleSpotFound;
	Vector2<int> ballSpot;
	bool ballFound;
//private:
//	friend std::ostream& operator <<(std::ostream &out,LogGoalData &simData);
//	friend std::ostream& operator <<(std::ostream &out,LogData &otherData);
	
};
//std::ostream& operator <<(std::ostream &out,LogGoalData &simData);
//std::ostream& operator <<(std::ostream &out,LogData &otherData);

class SelfLocator : public SelfLocatorBase,BaseProvider
{
public:
	SelfLocator();
	~SelfLocator();
	void inOdometryData();
	void inGoalPercept(){}
	void inBallPercept(){}
	void inLinesPercept(){}
	void inFieldDimensions(){}
	void inFrameInfo();
	void inRoboCupGameControlData(){}
	void inRobotInfo(){}
	void inTeamCfgInfo(){}
	void inCameraMatrix(){}
	void update(RobotPose*theRobotPose);
	void update(SelfLocatorSampleSet* theSelfLocatorSampleSet);
	void update(RobotPoseHypotheses* theRobotPoseHypotheses);
	void update(Robotsamplebuffer*theRobotsamplebuffer);
	void outRobotPose(){};
	void outRobotsamplebuffer(){}
	//void outRobotPoseHypotheses(){}
	//void outSelfLocatorSampleSet(){}

	void publish(const GoalPercept& goalPercept ,const LinesPercept& linePercept,const BallPercept& ballPercept,const OdometryData* odometry,RobotInfo* theRobotInfo,
		/*TeamCfgInfo* theTeamCfgInfo,*/unsigned _time,CameraMatrix* cameraMatrix,
		RoboCupGameControlData* theRoboCupGameControlData);
	void writeToLog();
	void SimulationLog();
	void ClusterSamples(Robotsamplebuffer&robotsamplebuffe);
	LogData logDataBuffer;

	int logDataCounter;
   // CommandData cmd;
private:
	/**
	* A sample/particle (defined in Tools/SampleSet.h)
	*/
	typedef SelfLocatorSample Sample;

	enum PoseCalculatorType
	{
		POSE_CALCULATOR_2D_BINNING = 0,
		POSE_CALCULATOR_PARTICLE_HISTORY,
		POSE_CALCULATOR_BEST_PARTICLE,
		POSE_CALCULATOR_OVERALL_AVERAGE,
		POSE_CALCULATOR_K_MEANS_CLUSTERING,
		NUMBER_OF_POSE_CALCULATORS
	} poseCalculatorType;

	/** 
	* The function returns names for different pose calculator types
	* @param type The type
	* @return The corresponding name.
	*/
	static const char* getPoseCalculatorTypeName(PoseCalculatorType type)
	{
		switch (type)
		{
		case POSE_CALCULATOR_2D_BINNING: return "PoseCalculator2DBinning";
		case POSE_CALCULATOR_BEST_PARTICLE: return "PoseCalculatorBestParticle";
		case POSE_CALCULATOR_OVERALL_AVERAGE: return "PoseCalculatorOverallAverage";
		case POSE_CALCULATOR_PARTICLE_HISTORY: return "PoseCalculatorParticleHistory";
		case POSE_CALCULATOR_K_MEANS_CLUSTERING: return "PoseCalculatorKMeansClustering";
		default: return "none";
		}
	}

	SelfLocatorParameter* parameter; /**< All tweaking values. */
	FieldModel fieldModel; /**< The model of proximity to features on the field. */
	SampleSet<Sample>* samples; /**< Container for all samples. */
	double totalWeighting; /**< The current weighting sum of all samples. */
	double slowWeighting; /**< This value follows the average weighting slowly. */
	double fastWeighting; /**< This value follows the average weighting more quickly. */
	Pose2D lastOdometry; /**< The last odometry state for calculating the offset since the last call. */
	bool updatedBySensors; /**< Was there an actual update during the previous observation update? */
	PoseCalculator< Sample, SampleSet<Sample> >* poseCalculator; /**< External class for computing a pose from a set of samples */
	//PerceptValidityChecker* perceptValidityChecker; /** A module for checking perceptions before their use by the self locator */
	unsigned lastPoseComputationTimeStamp; /**< Point of time of last pose computation*/
    RobotPose lastComputedPose;            /**< The pose computed at lastPoseComputationTimeStamp*/
	SampleTemplateGenerator sampleTemplateGenerator; /**< Submodule for generating new samples */
	std::vector<SensorModel*> sensorModels; /**< List of all sensor models applied to the sample set*/
	std::vector<double> sensorModelWeightings; /**< Weightings for sample set after execution of sensor model*/
	std::vector<double> locWeightings; /**<Weightings for sample set after apply the shadow loc*/
	int gameInfoPenaltyLastFrame; /**< Was the robot penalised in the last frame? */
	int gameInfoGameStateLastFrame; /**< The game state in the last frame */
	std::vector<const LinesPercept::LineOnField*> lines; /**< Pointers to the lines of the line percept. */
	std::vector<SensorModel::Observation> observations, /**< The indices of the observations that might be selected for sensor update. */
		selectedObservations; /**< The indices of the observations that are selected for sensor update. */
	std::vector<int> selectedIndices; /**< The indices of the observations that are selected to be updated by a single sensor model. */
	bool lastOwnSide;/**the last own side calculated from samples */
	bool ownSide;/** the  own side get from robotPoseValidator */
	int teamNum;/** team number 1 goalier 2 ,4 attacker,3 defender */

	
	//加罚球点坐标
	Vector2<double> oppcross;
	Vector2<double> owncross;
	//为了滤掉波动大的值
    RingBuffer<RobotPose,10> robotPoseRingBuffer;
	Pose2D odometryoffset;
	bool whetherbadvalue;
	Matrix2x2<double> positionCov;
    bool flagPenatly;
  /*samplebuffer sampleToPc;
	RobotPose robotposeTopc*/;
	/** 
	* The method prepares execution and initializes some values.
	* @param robotPose The robot pose representation that is updated by this module.
	*/
	void preExecution(RobotPose& robotPose);

	/** 
	* The method performs the motion update step of particle filter localization.
	* @param noise Add random noise to the particles?
	*/
	void motionUpdate(bool noise);

	/** 
	* Applies all sensor models to adjust the weightings of the samples
	* @return true, if any new weightings have been computed. false, otherwise.
	*/
	bool applySensorModels();

	/*
	* adjust the weightings of the samples according shadow locations
    * @return true
	*/
	bool applyShadowLocs();

	/** 
	* The method performs the resampling step of particle filter localization.
	* It might add new samples generated from templates.
	*/
	void resampling();

	/**
	* Adapts the weightings for resampling percentage
	*/
	void adaptWeightings();

	/** 
	* Computes a template pose.
	* @param sample The resulting template sample.
	*/
	void generateTemplate(Sample& sample);

	/** 
	* Draws the particles.
	*/
	void drawSamples();

	/** 
	* A method that contains all intialisations that require 
	* representations from the blackboard.
	*/
	virtual void init();

	/** 
	* Initializes the sample set.
	* If the vectors are empty (i.e. no positions given) the samples will
	* represent a uniform probability distribution which covers the whole state space.
	* If one or more positions are given, the samples will be randomly (again uniform) assigned
	* to these positions. Each position is represented by a gaussian distribution. Within these
	* distributions, the samples are randomly placed.
	* @param poses A vector of preferred sample poses
	* @param standardDeviations The standard deviations of the distributions of the points
	*/
	void readTeamConfig();
	void initSamplesAtGivenPositions(const std::vector<Pose2D>& poses,
		const std::vector<Pose2D>& standardDeviations);

	void setNumberOfSamples(const int num);


	/************************************************************************/
	/* A method reverse all  the samples to the other side of field                                                                    */
	/************************************************************************/
	void reverseSamples();
	void checkSamplesSide();
    bool CheckBigChangeOfselfLocalization();
	void CalculatepositionCovariance();
	bool calculateHeadAngle(double&minYaw,double&maxYaw,double&minPitch,double&maxPitch);
	void writeLocatorData();

	//void applyLocWeighting(const SampleSet<SelfLocatorSample>& samples,std::vector<double>& weightings);
	//double computerLocWeighting(const Pose2D& robotPose,RobotLocation& shadowLoc,double standardDeviation, double bestPossibleWeighting);
};




#endif