#ifndef BallLocator_H
#define BallLocator_H

#include "Tools/Math/GaussianDistribution2D.h"
#include "Tools/Math/GaussianTable.h"
#include "Tools/SampleSet.h"
#include "Tools/Math/Common.h"
#include "Module/Representations/KS.h"
#include "Module/Representations/KSManager.h"
#include "Module/Representations/MotionControl/OdometryData.h"
#include "Module/Representations/Modeling/BallModel.h"
#include "Module/Representations/Perception/BallPercept.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Representations/Configuration/FieldDimensions.h"
#include "Module/Representations/Modeling/RobotPose.h"
#include "Module/Actuator/ProxyContainer.h"
//#include "Module/Common/MotionCmd.h"
#include "Module/Common/FrameInfo.h"
#include "Module/Representations/BaseProvider.h"
#include <deque>
#include "Module/Modeling/SelfLocator/SampleTransferredToPc.h"
#include <iostream>
#include <fstream>
#ifdef NAO_DEBUG
#include "Simulation/LocatorGUI/FieldDrawing.h"
#endif
DEFINE_KS(BallLocator)
REQUIRES(BallPercept)
REQUIRES(CameraMatrix)
REQUIRES(OdometryData)
REQUIRES(FieldDimensions)
REQUIRES(RobotPose)
REQUIRES(FrameInfo)
//REQUIRES(MotionCmd)
PROVIDES(BallModel)
PROVIDES(Ballsamplebuffer)
USES(BallModel)
END_KS


/**
* @class BallLocatorParameters
* A collection for all parameters
*/
class BallLocatorParameters
{
public:
  /** Standard constructor*/
  BallLocatorParameters(): angleStandardDeviation(0.5*1000),
                                         closeBallDistanceStandardDeviation(100.0),
                                         farBallDistanceStandardDeviation(400.0),
                                         closeThreshold(200.0), farThreshold(2500.0),
                                         alphaSlow(0.2), alphaFast(0.3), scaleFactorVelocityVariance(0.4),
                                         scaleFactorOdometryVariance(3.0), templateSpeedDecreaseDistance(420.0),
                                         templateSpeedDecreaseFactor(5.0), ballMotionProbability(0.2),
                                         speedPercentageAfterOneSecond(70),
                                         maxVelocity(2000), minOdoTranslation(10), minOdoRotation(deg2rad(5)),
                                         robotBoxSizeX(80), robotBoxSizeY(100)
  {
  }

  double angleStandardDeviation;
  double closeBallDistanceStandardDeviation;
  double farBallDistanceStandardDeviation;
  double closeThreshold;
  double farThreshold;
  double alphaSlow;
  double alphaFast;
  double scaleFactorVelocityVariance;
  double scaleFactorOdometryVariance;
  double templateSpeedDecreaseDistance;
  double templateSpeedDecreaseFactor;
  double ballMotionProbability;
  int speedPercentageAfterOneSecond;
  int maxVelocity;
  double minOdoTranslation;
  double minOdoRotation;
  int robotBoxSizeX;
  int robotBoxSizeY;
  int ballCollisionCheckSquareDistance; //computed at runtime
};


/**
* @class BallLocator
* A BallLocator using a particle filter
*/
class BallLocator : public BallLocatorBase,public BaseProvider
{
public:
  /** Constructor */
  BallLocator();

  void inBallPercept(){}
  void inCameraMatrix(){}
  void inOdometryData(){}
  void inFieldDimensions(){}
  void inRobotPose(){}
  void inFrameInfo(){}
//  void inMotionCmd()
//  {
//#ifndef NAO_DEBUG
//	  //selfMessageQueue->SearchMyMessage(idMotionCmd,idMotionEngineWindow,idActuatorThread,
//		 // (char*)theMotionCmd,sizeof(MotionCmd));
//#endif
//  }

  /** Provides our own ball representation
  * @param ballModel The ball representation which is updated by this module
  */
  void update(BallModel* theBallModel);
  void update(Ballsamplebuffer* theBallsamplebuffer);
  void outBallModel();
  void outBallsamplebuffer(){}

  /** Provides the SampleSet representation
  * @param sampleSet The SampleSet representation which is updated by this module
  */
  //void update(BallLocatorSampleSet* sampleSet);

  /** Initialization of things. Called after all modules have been constructed
   *  and before the first call to update.
   */
  void init();

private:
  /** prepares execution, initializes some values*/
  void preExecution();

  /** motion update step of particle filter localization*/
  void motionUpdate();

  /** Check, if ball sample collides with robot
  * @param s The sample
  */
  void clipSampleWithFeet(BallSample& s);

  /** motion update step of particle filter localization*/
  void sensorUpdate();

  /** resampling step of particle filter localization*/
  void resampling();

  /** Computes a new sample
  * @return The sample
  */
  BallSample generateNewSample();

  /** Sets the member values of the BallModel
  * @param ballModel A reference to the BallModel*/
  void modelGeneration(BallModel& ballModel);

   /** Sets some values after everything else of this module was executed
  * @param ballModel A reference to the BallModel*/
  void postExecution(BallModel& ballModel);

  /** Draws the particles*/
  void drawSamples();

  /** Collects data and finally prints covariance values*/
  void uncertaintyCalibration();

  void calStandSpot(Vector2<double> ball,Vector2<double>& globleStandPoint);

  void ClusterSamples(Ballsamplebuffer&ballsamplebuffer);
  void CalculatBallCovariance();

  typedef SampleSet<BallSample> BallSampleSet;  /** Definition of type for sample set*/
  BallSampleSet* ballSamples;                                 /** Container for all samples*/
  double averageWeighting;                                   /** The current average weighting of a sample*/
  double slowWeighting;
  double fastWeighting;
  int numberOfGeneratedSamples;
  unsigned lastExecution;                                    /** time stamp*/
  unsigned lastSeenBallTime;                                 /** time stamp*/
  unsigned lastCheatingTime;                                 /** time stamp*/
  Pose2D lastOdometry;                                       /** save last odometry state*/
  Pose2D appliedOdometryOffset;                              /** odometry offset used for the current frame*/
  Pose2D accumulatedOdometry;                                /** accumulated odometry */
  GaussianDistribution2D ballPosUncertainty;                 /** distribution describing the uncertainty of ball positions in images*/
  enum {NUM_OF_CALIBRATION_MEASUREMENTS = 500};
  double ballXMeasurements[NUM_OF_CALIBRATION_MEASUREMENTS];
  double ballYMeasurements[NUM_OF_CALIBRATION_MEASUREMENTS];
  bool calibrateUncertainty;
  int currentCalibrationMeasurement;
  BallLocatorParameters parameters;            /** some parameters */
  GaussianTable gaussian;                                    /** precomputed gaussian values for distances*/  
  GaussianTable gaussianAngles;                              /** precomputed gaussian values for angles*/  
  int newSamplesNumberForDrawing;
  Matrix2x2<double> ballParticleCov;
  Vector2<double> BallVelocity;//¼ÆËãµÄÇòËÙ
  //ProxyContainer myProxyContainer;
  

  class BufferedPercept
  {
  public:
    BufferedPercept() {}
    BufferedPercept(const BallPercept& bp, unsigned timeStamp, const OdometryData& odometryData):
      pos(bp.relativePositionOnField), timeStamp(timeStamp), odometryData(odometryData)
    {}

    Vector2<double> getOdometryCorrectedPosition(const OdometryData& odoCurrent)
    {
      Pose2D offset = odoCurrent - odometryData;
      return offset*pos;
    }

    Vector2<double> pos;
    unsigned timeStamp;
    OdometryData odometryData;
  };

  class BufferedBallModel
  {
  public:
	  BufferedBallModel():timeStamp(0){}
	  BufferedBallModel(const Vector2<double>&s,const unsigned &time):pose(s),timeStamp(time)
	  {}
	  ~BufferedBallModel(){}
	  Vector2<double> pose;
	  unsigned timeStamp;
  };
  std::deque<BufferedPercept> ballPerceptBuffer;             /** a buffer for the last ball percepts*/
  std::deque<BufferedBallModel> ballModelBuffer;          /** a buffer for last ballmodel for velocity calculation**/
};

#endif
