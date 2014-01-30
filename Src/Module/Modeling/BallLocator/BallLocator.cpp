#include "BallLocator.h"
#include "Tools/Math/AngleTables.h"
#include "Tools/Math/Matrix2x2.h"


BallLocator::BallLocator()
 : slowWeighting(0.0), fastWeighting(0.0), lastExecution(0), lastSeenBallTime(0), lastCheatingTime(0),
   calibrateUncertainty(false), currentCalibrationMeasurement(0), gaussian(5000, 100, 50, 400, 10),
   gaussianAngles(3200, 320, static_cast<int> (parameters.angleStandardDeviation),
   static_cast<int> (parameters.angleStandardDeviation), 1),newSamplesNumberForDrawing(0)
{
  ballSamples = new BallSampleSet(40); //40 samples
  for(int i = 0; i < ballSamples->size(); ++i)
  {
    Pose2D pose(theFieldDimensions->randomPoseOnField());
    ballSamples->at(i).pos.x = static_cast<int>(pose.translation.x);
    ballSamples->at(i).pos.y = static_cast<int>(pose.translation.y);
    if(randomDouble() <= parameters.ballMotionProbability)
    {
      ballSamples->at(i).setRandomVelocity(parameters.maxVelocity);
      ballSamples->at(i).state = BallSample::ROLLING;
    }
    else
    {
      ballSamples->at(i).state = BallSample::LYING;
    }
  }
  std::cout<<"end of ball locator"<<std::endl;
}


void BallLocator::init()
{
  lastOdometry = *theOdometryData;
  numberOfGeneratedSamples = 0;
  int dist = static_cast<int>(sqrt(static_cast<double>(parameters.robotBoxSizeX*parameters.robotBoxSizeX
    + parameters.robotBoxSizeY*parameters.robotBoxSizeY)) + 0.5);
  dist += theFieldDimensions->ballRadius;
  parameters.ballCollisionCheckSquareDistance = dist*dist;
}

//void BallLocator::update(BallLocatorSampleSet& sampleSet)
//{
//  ballSamples->link(sampleSet.sampleSetProxy);
//}

void BallLocator::update(BallModel* theBallModel)
{
  preExecution();
  motionUpdate();
  if(theBallPercept->invion)
  {
    sensorUpdate();
    resampling();
  }
  modelGeneration(*theBallModel);
  postExecution(*theBallModel);
  BallState estimate=theBallModel->estimate;
  theBallModel->estimate.velocity.x=BallVelocity.x;
  theBallModel->estimate.velocity.y=BallVelocity.y;
  if(abs(estimate.position.x)>0.001&&abs(estimate.position.y)>0.001)
  {
	  theBallModel->estimateGlobe.position=estimate.position.rotate(theRobotPose->rotation)+theRobotPose->translation;
	  //theBallModel->estimateGlobe.position=theBallModel->estimate.position+theRobotPose->translation;
	  theBallModel->estimateGlobe.velocity=theBallModel->estimate.velocity;
  }
  else
  {
	  theBallModel->estimateGlobe.position=theBallModel->estimate.position;
	  theBallModel->estimateGlobe.velocity=theBallModel->estimate.position;
  }
   CalculatBallCovariance();
   if(theBallPercept->invion)
   {
	   double BallSqurelength=sqrt(abs(ballParticleCov[0][0]));
       double BallSqureWidth=sqrt(abs(ballParticleCov[1][1]));
       theBallModel->ballsquare.LeftUpPosition=Vector2<double>(theBallModel->estimateGlobe.position.x-BallSqurelength/2.0,theBallModel->estimateGlobe.position.y-BallSqureWidth/2.0);
       theBallModel->ballsquare.length=BallSqurelength;
       theBallModel->ballsquare.width=BallSqureWidth;
   }
   else{
	   theBallModel->ballsquare.LeftUpPosition=Vector2<double>(0.0,0.0);
	   theBallModel->ballsquare.length=0.0;
	   theBallModel->ballsquare.width=0.0;

   }
  /*CalculatBallCovariance();
  Vector2<double> covInWorld=Vector2<double>(sqrt(fabs(ballParticleCov[0][0])),sqrt(fabs(ballParticleCov[1][1]))).rotate(theRobotPose->rotation);
  theBallModel->ballsquare.LeftUpPosition=Vector2<double>(theBallModel->estimateGlobe.position.x-fabs(covInWorld.x),theBallModel->estimateGlobe.position.y-fabs(covInWorld.y));
  theBallModel->ballsquare.length=fabs(covInWorld.x)+BALL_DIAMETER;
  theBallModel->ballsquare.width=fabs(covInWorld.y)+BALL_DIAMETER;*/
  //getBallSquare();
#ifdef NAO_DEBUG
  drawSamples();
  theBallModel->draw(*theRobotPose);//it won't changed the estimate value
#endif
  //Vector2<double> ballToWorldCentr(theBallModel->estimate.position);
  //ballToWorldCentr+=theRobotPose->translation;
  Vector2<double> ballToWorldCentr(theBallModel->estimateGlobe.position);
  Vector2<double> ballToGoalCentr(ballToWorldCentr.x+3000,ballToWorldCentr.y);
  calStandSpot(ballToGoalCentr,theBallModel->goalieStandSpot);
  //std::cout<<"ballLocator stand spot x is "<<theBallModel->goalieStandSpot.x<<" y is "<<theBallModel->goalieStandSpot.y<<std::endl;
  std::cout<<"estimate Ball location x is "<<theBallModel->estimate.position.x<<" y is "<<theBallModel->estimate.position.y<<" VX is "<<theBallModel->estimate.velocity.x<<" VY is "<<theBallModel->estimate.velocity.y<<std::endl;
  std::cout<<"estimate Ball Global x is "<<theBallModel->estimateGlobe.position.x<<" y is "<<theBallModel->estimateGlobe.position.y<<std::endl;

  //if(theBallModel->estimate.velocity.x > 0.1)
  //{
	 // if(theBallModel->estimate.velocity.y > 0.1 )
		//  myProxyContainer.ttsProxy->say("Left");
	 // else if(theBallModel->estimate.velocity.y < -0.1 )
		//  myProxyContainer.ttsProxy->say("Right");
  //}
  //outBallModel();
}
void BallLocator::outBallModel()
{
	selfMessageQueue->SetMessageData(idBallModel,idRobotThread,idActuatorThread,
		sizeof(BallModel),(char*)theBallModel);
}

void BallLocator::calStandSpot(Vector2<double> ball,Vector2<double>& globleStandPoint)
{
 double angle_radius=atan(ball.y/ball.x);//draw an line between ball and goal center
 double angle_sin2=sqr(sin(angle_radius)),angle_cos2=sqr(cos(angle_radius));
 double temp=angle_sin2/sqr(700)+angle_cos2/sqr(600);
 temp=1/temp;
 double r=sqrt(temp);
 Vector2<double> p(r*cos(angle_radius),r*sin(angle_radius));
 globleStandPoint.x=p.x-3000;
 globleStandPoint.y=p.y;

}

void BallLocator::preExecution()
{
	// Remove old elements from percept buffer and try to add a new one (if it exists):
	while(ballPerceptBuffer.size() && theFrameInfo->getTimeSince(ballPerceptBuffer.front().timeStamp) > 1000000)
	{
		ballPerceptBuffer.pop_front();
	}
	while(ballModelBuffer.size()&&theFrameInfo->getTimeSince(ballModelBuffer.front().timeStamp)>2000000)
	{
		ballModelBuffer.pop_front();
	}
	if(theBallPercept->invion)
	{
		ballPerceptBuffer.push_back(BufferedPercept(*theBallPercept, theFrameInfo->time, *theOdometryData));
	}
	//std::cout<<"  ^^^^^^^ ballperceptbuffer^^^^^^^"<<ballPerceptBuffer.size()<<std::endl;
	// Compute odometry offset applied in this frame:
	accumulatedOdometry += (*theOdometryData - lastOdometry);
	appliedOdometryOffset = Pose2D();
	if(fabs(accumulatedOdometry.rotation) > parameters.minOdoRotation)
	{
		appliedOdometryOffset.rotation = accumulatedOdometry.rotation;
		accumulatedOdometry.rotation = 0;
	}
	if(fabs(accumulatedOdometry.translation.x) > parameters.minOdoTranslation)
	{
		appliedOdometryOffset.translation.x = static_cast<int>(accumulatedOdometry.translation.x);
		accumulatedOdometry.translation.x -= appliedOdometryOffset.translation.x;
	}
	if(fabs(accumulatedOdometry.translation.y) > parameters.minOdoTranslation)
	{
		appliedOdometryOffset.translation.y = static_cast<int>(accumulatedOdometry.translation.y);
		accumulatedOdometry.translation.y -= appliedOdometryOffset.translation.y;
	}

	if(theBallPercept->invion && calibrateUncertainty &&
		currentCalibrationMeasurement < NUM_OF_CALIBRATION_MEASUREMENTS)
	{
		uncertaintyCalibration();
	}
}


void BallLocator::motionUpdate()
{
  //Rotation matrix for odometry:
  const int cosor = static_cast<int>(cos(appliedOdometryOffset.rotation) * 1024);
  const int sinor = static_cast<int>(sin(appliedOdometryOffset.rotation) * 1024);
  const Matrix2x2<int> R(Vector2<int>(cosor,-sinor), Vector2<int>(sinor,cosor));
  //Some values:
  const Vector2<int> odoTrans(static_cast<int>(appliedOdometryOffset.translation.x),
                              static_cast<int>(appliedOdometryOffset.translation.y));
  const int odometryLength = odoTrans.abs();
  const int t(theFrameInfo->getTimeSince(lastExecution));
  int minusSpeedPercentage(100-parameters.speedPercentageAfterOneSecond);
  minusSpeedPercentage *= 1024;
  minusSpeedPercentage *= t;
  minusSpeedPercentage /= 1000;
  int realSpeedPercentage = (100*1024) - minusSpeedPercentage;
  int numberOfRollingBalls(0);
  //Update samples:
  for(int i = 0; i < ballSamples->size(); ++i)
  {
    BallSample& s(ballSamples->at(i)); //pick each sample
    const int velocity = s.velocity.abs();
    //Ball motion update:
    if(s.state == BallSample::ROLLING)
    {
      s.pos.x += (s.velocity.x + sampleTriangularDistribution(static_cast<int>(velocity*parameters.scaleFactorVelocityVariance)))* t / 1000;
      s.pos.y += (s.velocity.y + sampleTriangularDistribution(static_cast<int>(velocity*parameters.scaleFactorVelocityVariance)))* t / 1000;
      ++numberOfRollingBalls;
    }
    //Robot odometry update:
    s.pos = (R * s.pos) / 1024;
    s.pos -= odoTrans;
    s.velocity = (R * s.velocity) / 1024;
    //Add uncertainty to position (depending on walked distance):
    s.pos.x += sampleTriangularDistribution(static_cast<int>(odometryLength*parameters.scaleFactorOdometryVariance));
    s.pos.y += sampleTriangularDistribution(static_cast<int>(odometryLength*parameters.scaleFactorOdometryVariance));
    if((s.pos * s.pos) < parameters.ballCollisionCheckSquareDistance)
      clipSampleWithFeet(s);
    //Add uncertainty to velocity:
    if(s.state == BallSample::ROLLING)
    {
      s.velocity.x += sampleTriangularDistribution(static_cast<int>(velocity*parameters.scaleFactorVelocityVariance))* t / 1000;
      s.velocity.y += sampleTriangularDistribution(static_cast<int>(velocity*parameters.scaleFactorVelocityVariance))* t / 1000;
      // Apply some friction:
      s.velocity *= realSpeedPercentage;
      s.velocity /= (100*1024);
    }
  }
  int rollingBallsPercentage = (numberOfRollingBalls*100)/ballSamples->size();
}


inline void BallLocator::clipSampleWithFeet(BallSample& s)
{
  // Check, if sample is really in relevant area (assume quadratic ball :-):
  Vector2<int>& b(s.pos);
  const int& maxX = parameters.robotBoxSizeX;
  const int& maxY = parameters.robotBoxSizeY;
  if((b.x > maxX + theFieldDimensions->ballRadius) ||
     (b.x < -maxX - theFieldDimensions->ballRadius) ||
     (b.y > maxY + theFieldDimensions->ballRadius) ||
     (b.y < -maxY - theFieldDimensions->ballRadius))
    return;

  const int xDiff = b.x >= 0 ? std::abs(maxX - b.x) : std::abs(-maxX - b.x);
  const int yDiff = b.y >= 0 ? std::abs(maxY - b.y) : std::abs(-maxY - b.y);
  // Move ball to valid position:
  if(xDiff <= yDiff)
  {
    if(b.x >= 0)
      b.x = maxX + theFieldDimensions->ballRadius;
    else
      b.x = -maxX - theFieldDimensions->ballRadius;
  }
  else
  {
    if(b.y >= 0)
      b.y = maxY + theFieldDimensions->ballRadius;
    else
      b.y = -maxY - theFieldDimensions->ballRadius;
  }
  // ROLLING ball bounces off: angle in == angle out, velocity becomes decreased by 50%
  // Robot motion is not relevant in this case
  if(s.state == BallSample::ROLLING)
  {
    if(xDiff < yDiff) // Bounce at front of box
    {
      s.velocity.x = -s.velocity.x;
      s.velocity /= 2;
    }
    else              // Bounce at side of box
    {
      s.velocity.y = -s.velocity.y;
      s.velocity /= 2;
    }
  }
  // In addition, the ball is moved in direction of robot movement. Ball speed is higher than robot speed:
  //s.velocity.x += static_cast<int>(theMotionCmd->walkCmd.speed.translation.x)*2;
  //s.velocity.y += static_cast<int>(theMotionCmd->walkCmd.speed.translation.y)*2;
  s.state = BallSample::ROLLING;
}


void BallLocator::sensorUpdate()
{
  averageWeighting = 0;
  const int observationDistance(static_cast<int>(theBallPercept->relativePositionOnField.abs()));
  const double observationAngle(atan2(theBallPercept->relativePositionOnField.y,theBallPercept->relativePositionOnField.x));
  double distStdDev;
  if(observationDistance <= parameters.closeThreshold)
    distStdDev = parameters.closeBallDistanceStandardDeviation;
  else if(observationDistance >= parameters.farThreshold)
    distStdDev = parameters.farBallDistanceStandardDeviation;
  else
    distStdDev = parameters.closeBallDistanceStandardDeviation +
    ((observationDistance - parameters.closeThreshold)/(parameters.farThreshold - parameters.closeThreshold))*(parameters.farBallDistanceStandardDeviation - parameters.closeBallDistanceStandardDeviation);
  for(int i = 0; i < ballSamples->size(); ++i)
  {
    BallSample& s(ballSamples->at(i));
    const int modelDistance(s.pos.abs());
    const double modelAngle(Atan2Approximation::atan2(s.pos.y, s.pos.x));
    const double weighting = gaussian.value(abs(observationDistance - modelDistance), (int)distStdDev) *
      gaussianAngles.value(int(1000*fabs(observationAngle - modelAngle)), (int)parameters.angleStandardDeviation);
    s.weighting = weighting;
    averageWeighting += weighting;
  }
  averageWeighting /= ballSamples->size();
  slowWeighting = slowWeighting + parameters.alphaSlow * (averageWeighting - slowWeighting);
  fastWeighting = fastWeighting + parameters.alphaFast * (averageWeighting - fastWeighting);
}


void BallLocator::resampling()
{
  // swap sample arrays
  BallSample* oldSet = ballSamples->swap();

  const double weightingsSum(averageWeighting*ballSamples->size());
  const double resamplingPercentage(std::max<double>(0.0, 1.0 - fastWeighting / slowWeighting));
  const double RESAMPLED_SAMPLES = ballSamples->size() * (1.0 - resamplingPercentage);
  const double weightingBetweenTwoDrawnSamples(weightingsSum  / double(RESAMPLED_SAMPLES));
  double nextPos(randomDouble()*weightingBetweenTwoDrawnSamples);
  double currentSum(0);

  // resample:
  int j(0);
  for(int i = 0; i < ballSamples->size(); ++i)
  {
    currentSum += oldSet[i].weighting;
    while(currentSum > nextPos)
    {
      ballSamples->at(j++) = oldSet[i];
      nextPos += weightingBetweenTwoDrawnSamples;
      if(j == ballSamples->size())  // This might be necessary because of floating point imprecision
        return;
    }
  }
  // Store number of generated samples
  numberOfGeneratedSamples = ballSamples->size() - j;
  // fill up remaining samples with new poses:
  for(; j < ballSamples->size(); ++j)
  {
    ballSamples->at(j) = generateNewSample();
  }
  newSamplesNumberForDrawing = numberOfGeneratedSamples;
}


BallSample BallLocator::generateNewSample()
{
  BallSample newSample;
  // If there have been some percepts, use two of them to create a new sample
  if(ballPerceptBuffer.size() > 1)
  {
    int a = random(ballPerceptBuffer.size());
    int b = random(ballPerceptBuffer.size());
    // a and b must be different, of course:
    if(a == b)
    {
      if(a != 0)
        a--;
      else
        a++;
    }
    // a must be smaller than b:
    if(a > b)
    {
      int temp = b;
      b = a;
      a = temp;
    }
    Vector2<double> p1 = ballPerceptBuffer[a].getOdometryCorrectedPosition(*theOdometryData);
    unsigned timeP1 = ballPerceptBuffer[a].timeStamp;
    Vector2<double> p2 = ballPerceptBuffer[b].getOdometryCorrectedPosition(*theOdometryData);
    unsigned timeP2 = ballPerceptBuffer[b].timeStamp;
    double timeInSeconds((double(timeP2) - double(timeP1))/1000.0);
    Vector2<double> speed(p2-p1);
    speed /= timeInSeconds;
    newSample.pos.x = static_cast<int>(p2.x);
    newSample.pos.y = static_cast<int>(p2.y);
    newSample.velocity.x = static_cast<int>(speed.x);
    newSample.velocity.y = static_cast<int>(speed.y);
    newSample.clipVelocity(parameters.maxVelocity);
    // Compute current position of sample
    double timeToNow = theFrameInfo->getTimeSince(timeP2);
    timeToNow /= 1000;
    Vector2<double> offset(newSample.velocity.x, newSample.velocity.y);
    offset *= timeToNow;
    newSample.pos.x += static_cast<int>(offset.x);
    newSample.pos.y += static_cast<int>(offset.y);
    //If sample is close, decrease velocity.
    if(newSample.pos.abs() < parameters.templateSpeedDecreaseDistance)
    {
      newSample.velocity /= static_cast<int>(parameters.templateSpeedDecreaseFactor);
    }
  }
  // Use the current percept and a random speed
  else
  {
    newSample.pos.x = static_cast<int>(theBallPercept->relativePositionOnField.x);
    newSample.pos.y = static_cast<int>(theBallPercept->relativePositionOnField.y);
    newSample.setRandomVelocity(parameters.maxVelocity);
    //If sample is close, decrease velocity.
    if(newSample.pos.abs() < parameters.templateSpeedDecreaseDistance)
    {
      newSample.velocity /= static_cast<int>(parameters.templateSpeedDecreaseFactor);
    }
  }
  //This must be integrated a bit better in the previous lines, but anyway:
  if(randomDouble() > parameters.ballMotionProbability)
    newSample.state = BallSample::LYING;
  else
    newSample.state = BallSample::ROLLING;
  return newSample;
}


void BallLocator::modelGeneration(BallModel& ballModel)
{
  //The estimated ball position:
  Vector2<double> averagePosition;
  Vector2<double> averageVelocity;
  double sumOfAllWeights(0.0);
  // This is a bad case that might happen:
  if(numberOfGeneratedSamples == ballSamples->size())
  {

    for(int i = 0; i < ballSamples->size(); ++i)
    {
      BallSample& s(ballSamples->at(i));
      averagePosition.x += s.pos.x;
      averagePosition.y += s.pos.y;
      if(s.state == BallSample::ROLLING)
      {
        averageVelocity.x += s.velocity.x;
        averageVelocity.y += s.velocity.y;
      }
	}
    averagePosition /= ballSamples->size();
    averageVelocity /= ballSamples->size();
  }
  // This is the standard case:
  else
  {
    for(int i = 0; i < ballSamples->size(); ++i)
    {
      BallSample& s(ballSamples->at(i));
      if(s.weighting != -1.0)
      {
        averagePosition.x += (s.pos.x * s.weighting);
        averagePosition.y += (s.pos.y * s.weighting);
        if(s.state == BallSample::ROLLING)
        {
          averageVelocity.x += (s.velocity.x * s.weighting);
          averageVelocity.y += (s.velocity.y * s.weighting);
        }
        sumOfAllWeights += s.weighting;
      }
    }
    averagePosition /= sumOfAllWeights;
    averageVelocity /= sumOfAllWeights;
  }
  if(averagePosition.x<7000&&averagePosition.x>-7000&&averagePosition.y<7000&&averagePosition.y>-7000)
	  ballModel.estimate.position = averagePosition;
  /*ballModel.estimate.position = averagePosition;*/
  ballModel.estimate.velocity = averageVelocity;

  //Information about what has actually been seen:
  if(theBallPercept->invion)
  {
    ballModel.timeWhenLastSeen = theFrameInfo->time;
    ballModel.lastPerception.position = theBallPercept->relativePositionOnField;
    ballModel.lastSeenEstimate = ballModel.estimate;
  }
  else
  {
    ballModel.lastPerception.position.rotate(-appliedOdometryOffset.rotation);
    ballModel.lastPerception.position -= appliedOdometryOffset.translation;
    ballModel.lastSeenEstimate.position.rotate(-appliedOdometryOffset.rotation);
    ballModel.lastSeenEstimate.position -= appliedOdometryOffset.translation;
  }
}


void BallLocator::postExecution(BallModel& ballModel)
{
	lastExecution = theFrameInfo->time;
	//if(theBallPercept->getinvion())
	if(theBallPercept->invion)
	{
		// There is currently one thing that cannot be handled properly:
		// Seeing a ball for the first time after a longer period. Then the model
		// does not adjust fast enough. Therefore we will deliver the percept for one
		// second ;-)

		//Currently cheating:
		if(theFrameInfo->getTimeSince(lastCheatingTime) < 1000000)
		{
			ballModel.estimate.position = theBallPercept->relativePositionOnField;
			ballModel.lastSeenEstimate.position = theBallPercept->relativePositionOnField;
		}
		//Detect cheating situation:
		else if(theFrameInfo->getTimeSince(lastSeenBallTime) > 2000000)
		{
			lastCheatingTime = theFrameInfo->time;
			ballModel.estimate.position = theBallPercept->relativePositionOnField;
			ballModel.lastSeenEstimate.position = theBallPercept->relativePositionOnField;
		}
		//Standard stuff:
		lastSeenBallTime = lastExecution;
	}
	lastOdometry = *theOdometryData;

	if(!ballModelBuffer.size())
		ballModelBuffer.push_back( BufferedBallModel(theBallModel->estimate.position,theFrameInfo->time));
	/*else if( theFrameInfo->time!=ballModelBuffer.back().timeStamp&&((sqrt(sqr(theBallModel->estimate.position.x-
		ballModelBuffer.back().pose.x)+sqr(theBallModel->estimate.position.y-ballModelBuffer.back().pose.y)))/(theFrameInfo->time-ballModelBuffer.back().timeStamp)<0.005))*/
	else if( theFrameInfo->time!=ballModelBuffer.back().timeStamp&&((sqrt(sqr(theBallModel->estimate.position.x-
		ballModelBuffer.back().pose.x)+sqr(theBallModel->estimate.position.y-ballModelBuffer.back().pose.y)))/*/(theFrameInfo->time-ballModelBuffer.back().timeStamp)*/<1000))	
	ballModelBuffer.push_back( BufferedBallModel(theBallModel->estimate.position,theFrameInfo->time));
	double averageVelocityX(0.0),averageVelocityY(0.0);
	if(ballModelBuffer.size()>=3)
	{

		for(std::deque<BufferedBallModel>::iterator iter=ballModelBuffer.begin();iter!=ballModelBuffer.end()-1;iter++)
		{
			std::deque<BufferedBallModel>::iterator Nextiter=iter+1;
			averageVelocityX+= (Nextiter->pose.x-iter->pose.x)/(Nextiter->timeStamp-iter->timeStamp);
			averageVelocityY+=(Nextiter->pose.y-iter->pose.y)/(Nextiter->timeStamp-iter->timeStamp);
		}
	}
	else
	{
		averageVelocityX=0.0;
		averageVelocityY=0.0;
	}

	if(ballModelBuffer.size()-1)
	{
		averageVelocityX/=ballModelBuffer.size()-1;
		averageVelocityY/=ballModelBuffer.size()-1;
	}
	averageVelocityX=averageVelocityX*1000;
	averageVelocityY=averageVelocityY*1000;
	BallVelocity=Vector2<double>(averageVelocityX,averageVelocityY);
	std::cout<<"++++++Ball Velocity+++++  "<<averageVelocityX<<"   "<<averageVelocityY<<"   "<<ballModelBuffer.size()<<"    "<<theFrameInfo->time <<std::endl;
	Vector2<double> position(theBallModel->estimateGlobe.position);
	//position+=robotPose;
	//Vector2<double> velocity(averageVelocityX,averageVelocityY);
	//CIRCLE(
	//	position.x, 
	//	position.y, 
	//	40, 
	//	1, // pen width 
	//	LocDrawings::ps_solid, 
	//	ColorClasses::red,
	//	LocDrawings::bs_solid, 
	//	ColorClasses::red);
	//ARROW(position, position + velocity.rotate(theRobotPose->rotation), 5, 1, ColorClasses::red);


	std::string path = "/home/nao/naoqi/Temp/ballmodel.txt";
	std::ofstream outFile(path.c_str(),std::ios::app);
	outFile<<"#OD#  "<<theBallModel->estimate.position.x<<"  "<<theBallModel->estimate.position.y<<"  "<<theFrameInfo->time<<"   "<<averageVelocityX<<"   "<<averageVelocityY<<"   "<<ballModelBuffer.size()<<"\n";
	outFile.close();
}
void BallLocator::drawSamples()
{                                
	//Default drawing color
	ColorRGBA sampleColor(255,255,255);
	//Determine alpha offset for nicer drawing ;-)
	double alphaOffset(1.0);
	for(int i = 0; i < ballSamples->size(); ++i)
	{
		const double offset(1.0 - ballSamples->at(i).weighting);
		if(offset < alphaOffset)
			alphaOffset = offset;
	}
	for(int i = 0; i < ballSamples->size(); ++i)
	{
		const BallSample& s(ballSamples->at(i));
		Vector2<double> p=Vector2<double>(s.pos.x,s.pos.y);
		Vector2<double> v=Vector2<double>(s.velocity.x,s.velocity.y);
		p=p.rotate(theRobotPose->rotation)+theRobotPose->translation;
		v=v.rotate(theRobotPose->rotation);
		if(s.weighting != -1.0)
			sampleColor.a = (unsigned char)((s.weighting + alphaOffset)*255.0);
		else //Draw new samples in a different color:
			sampleColor = ColorRGBA(255,255,0);

		CIRCLE(p.x,p.y,40,10,LocDrawings::ps_solid,sampleColor,LocDrawings::bs_solid,sampleColor);
		if(s.state == BallSample::ROLLING)
		{
			Vector2<double> start(p.x,p.y);
			Vector2<double> end(p.x+v.x,p.y+v.y);
			ARROW( start, end,5,LocDrawings::ps_solid,sampleColor);
		}
	}
}
void BallLocator::uncertaintyCalibration()
{
  if(currentCalibrationMeasurement < NUM_OF_CALIBRATION_MEASUREMENTS)
  {
    ballXMeasurements[currentCalibrationMeasurement] = theBallPercept->BallCenter.x;
    ballYMeasurements[currentCalibrationMeasurement] = theBallPercept->BallCenter.y;
    currentCalibrationMeasurement++;
  }
  if(currentCalibrationMeasurement == NUM_OF_CALIBRATION_MEASUREMENTS)
  {
    ballPosUncertainty.generateDistributionFromMeasurements(ballXMeasurements, NUM_OF_CALIBRATION_MEASUREMENTS,
      ballYMeasurements, NUM_OF_CALIBRATION_MEASUREMENTS);
  }
}
void BallLocator::update(Ballsamplebuffer *theBallsamplebuffer)
{
   ClusterSamples(*theBallsamplebuffer);
   //std::cout<<"Çò¶¨Î»Á£×Óupdate"<<theBallsamplebuffer->RobotRotation<<std::endl;
}
void BallLocator::ClusterSamples(Ballsamplebuffer &ballsamplebuffer)
{
	const int numberOfSamples(ballSamples->size());
	for(int i = 1; i < numberOfSamples; ++i)
	{
		const BallSample& s(ballSamples->at(i));
		Ballsamplebuffer::singleBallSample ss;
		ss.posX=int32(s.pos.x);
		ss.posY=int32(s.pos.y);
		ss.velocityX=int32(s.velocity.x);
		ss.velocityY=int32(s.velocity.y);
		ss.weighting=int32(s.weighting);
		int ballstate=static_cast<int>(s.state);
		ss.state=ballstate==0?Ballsamplebuffer::singleBallSample::LYING:Ballsamplebuffer::singleBallSample::ROLLING;
		ss.weighting=s.weighting;
		ballsamplebuffer.BallParticleSamples[i]=ss;
	}
	ballsamplebuffer.BallModelToPc.lastPerception.positionX=theBallModel->lastPerception.position.x;
	ballsamplebuffer.BallModelToPc.lastPerception.positionY=theBallModel->lastPerception.position.y;
	ballsamplebuffer.BallModelToPc.lastPerception.velocityX=theBallModel->lastPerception.velocity.x;
	ballsamplebuffer.BallModelToPc.lastPerception.velocityY=theBallModel->lastPerception.velocity.y;
	ballsamplebuffer.BallModelToPc.estimate.positionX=theBallModel->estimate.position.x;
	ballsamplebuffer.BallModelToPc.estimate.positionY=theBallModel->estimate.position.y;
	ballsamplebuffer.BallModelToPc.estimate.velocityX=theBallModel->estimate.velocity.x;
	ballsamplebuffer.BallModelToPc.estimate.velocityY=theBallModel->estimate.velocity.y;
	ballsamplebuffer.BallModelToPc.estimateGlobe.positionX=theBallModel->estimateGlobe.position.x;
	ballsamplebuffer.BallModelToPc.estimateGlobe.positionY=theBallModel->estimateGlobe.position.y;
	ballsamplebuffer.BallModelToPc.estimateGlobe.velocityX=theBallModel->estimateGlobe.velocity.x;
	ballsamplebuffer.BallModelToPc.estimateGlobe.velocityY=theBallModel->estimateGlobe.velocity.y;
	ballsamplebuffer.BallModelToPc.lastSeenEstimate.positionX=theBallModel->lastSeenEstimate.position.x;
	ballsamplebuffer.BallModelToPc.lastSeenEstimate.positionY=theBallModel->lastSeenEstimate.position.y;
	ballsamplebuffer.BallModelToPc.lastSeenEstimate.velocityX=theBallModel->lastSeenEstimate.velocity.x;
	ballsamplebuffer.BallModelToPc.lastSeenEstimate.velocityY=theBallModel->lastSeenEstimate.velocity.y;
	ballsamplebuffer.BallModelToPc.goalieStandSpotX=theBallModel->goalieStandSpot.x;
	ballsamplebuffer.BallModelToPc.goalieStandSpotY=theBallModel->goalieStandSpot.y;
	//ballsamplebuffer.BallModelToPc.timeWhenLastSeen=uint8(theBallModel->timeWhenLastSeen);
	ballsamplebuffer.RobotTranslationX=theRobotPose->translation.x;
	ballsamplebuffer.RobotTranslationY=theRobotPose->translation.y;
	ballsamplebuffer.RobotRotation=theRobotPose->rotation;

}
void BallLocator::CalculatBallCovariance()
{
	Vector2<int> newTrans(static_cast<int>(theBallModel->estimate.position.x), static_cast<int>(theBallModel->estimate.position.y));
	// Compute variance of position:
	int varianceX(0);
	int varianceY(0);
	for(int j = 0; j < ballSamples->size(); ++j)
	{
		BallSample& s(ballSamples->at(j));
		varianceX += (s.pos.x - newTrans.x) * (s.pos.x - newTrans.x);
		varianceY += (s.pos.y - newTrans.y) * (s.pos.y - newTrans.y);
	}
	varianceX /= (ballSamples->size() - 1);
	varianceY /= (ballSamples->size()- 1);
	ballParticleCov[0][0] = static_cast<double>(varianceX);
	ballParticleCov[1][1] = static_cast<double>(varianceY);
	// Compute covariance:
	int cov_xy(0);
	for(int j = 0; j < ballSamples->size(); ++j)
	{
		BallSample& s(ballSamples->at(j));
		cov_xy += (s.pos.x - newTrans.x) * (s.pos.y - newTrans.y);
	}
	cov_xy /= (ballSamples->size()- 1);
	ballParticleCov[0][1] = ballParticleCov[1][0] = static_cast<double>(cov_xy);
}
//void BallLocator::getBallSquare()
//{
//   /*Vector2<double> covInWorld=Vector2<double>(sqrt(ballParticleCov[0][0]),sqrt(ballParticleCov[1][1])).rotate(-theRobotPose->rotation)+theRobotPose->translation;
//   BallState ballstate=theBallModel->estimate;
//   Vector2<double> BallPositionInWorld=ballstate.position.rotate(-theRobotPose->rotation)+theRobotPose->translation;
//   theBallModel->ballsquare.LeftUpPosition=Vector2<double>(BallPositionInWorld.x-covInWorld.x,BallPositionInWorld.y-covInWorld.y);
//   theBallModel->ballsquare.length=ballParticleCov[0][0];
//   theBallModel->ballsquare.width=ballParticleCov[1][1];*/
//
//}
MAKE_KS(BallLocator)
