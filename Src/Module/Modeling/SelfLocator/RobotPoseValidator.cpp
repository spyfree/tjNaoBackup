/**
* @file RobotPoseValidator.cpp
* The Implementation of the module that validates and improves robot poses from the self location.
* @author laura
*/

#include "RobotPoseValidator.h"
#include "math.h"


RobotPoseValidator::RobotPoseValidator() :
translationCov(10000. * 10000.), rotation(0), rotationCov(pi * pi), lastResetTime(0), validated(false), validGoalSightingSinceLastReset(0), validUnknownGoalSightingSinceLastReset(0),ownSide(true),lastOwnSide(true),sideValidity(1.),direction(0.),crossMiddleLine(false),middleTranslationX(10000)
{
	theCurrentGlobalInfo = new GlobalInfo();

	p.respawnMaxPoseDistance = Pose2D(0.6, 1000., 1000.);
	p.respawnPoseDeviation = Pose2D(0.6, 1000., 1000.);
	p.lineRelationCorridor = 300.;
	p.odometryDeviation = Pose2D(0.2, 0.2, 0.2);
	p.filterProcessDeviation = Pose2D(0.001, 0.1, 0.1);
	p.robotRotationDeviation = Vector2<double>(0.02, 0.06);
	p.validationMaxDeviation = Pose2D(0.1, 100., 100. );
	p.validationMaxGoalPerceptDistance = 0.1;
	p.validationMinGoalSightings = 6;
	p.validationMinUnknownGoalSightings = 10;
	Rotation=0.0;
	Translation=Vector2<double>(0.0,0.0);
	init();
	std::cout<<"end of pose "<<std::endl;
}

void RobotPoseValidator::init()
{
	// prepare relevant field line table
	countOfFieldLines = 0;
	for(unsigned int i = 0, count = theFieldDimensions->fieldLines.lines.size(); i < count; ++i)
	{
		const FieldDimensions::LinesTable::Line& fieldLine = theFieldDimensions->fieldLines.lines[i];
		if(!fieldLine.isPartOfCircle && fieldLine.length > 300.)
		{
			ASSERT(countOfFieldLines < int(sizeof(fieldLines) / sizeof(*fieldLines)));
			FieldLine& relevantFieldLine = fieldLines[countOfFieldLines++];
			relevantFieldLine.start =fieldLine.corner.translation;
			relevantFieldLine.end = fieldLine.corner * Vector2<double>(fieldLine.length, 0);
			relevantFieldLine.dir = relevantFieldLine.end - relevantFieldLine.start;
			relevantFieldLine.dir.normalize();
			relevantFieldLine.length = fieldLine.length;
			relevantFieldLine.vertical = fabs(fieldLine.corner.rotation) < 0.001f || fabs(normalize(fieldLine.corner.rotation - pi)) < 0.001f;
		}
	}

	// and goal posts
	ASSERT(GoalPercept::NUMBER_OF_GOAL_POSTS == 4);
	goalPosts[GoalPercept::LEFT_OPPONENT] = Vector2<double>(theFieldDimensions->xPosOpponentGoalpost, theFieldDimensions->yPosLeftGoal);
	goalPosts[GoalPercept::RIGHT_OPPONENT] = Vector2<double>(theFieldDimensions->xPosOpponentGoalpost,theFieldDimensions->yPosRightGoal);
	goalPosts[GoalPercept::LEFT_OWN] = Vector2<double>(theFieldDimensions->xPosOwnGoalpost, theFieldDimensions->yPosRightGoal);
	goalPosts[GoalPercept::RIGHT_OWN] = Vector2<double>(theFieldDimensions->xPosOwnGoalpost,theFieldDimensions->yPosLeftGoal);

	lastResetTime = theFrameInfo->time;
}

void RobotPoseValidator::inGlobalInfo()
{
	if (theGlobalInfo != NULL)
	{
		memcpy(theCurrentGlobalInfo,theGlobalInfo,sizeof(GlobalInfo));		
	}

}

void RobotPoseValidator::update(RobotPose*theRobotPose)
{

	inGlobalInfo();
	motionUpdate();
	updateDirection();
	theRobotPose->middleTranslationX = middleTranslationX;
	theRobotPose->direction = direction;


	// respawn?
	if(fabs(translation.x - theRobotPose->translation.x) > p.respawnMaxPoseDistance.translation.x ||
		fabs(translation.y - theRobotPose->translation.y) > p.respawnMaxPoseDistance.translation.y ||
		fabs(normalize(rotation - theRobotPose->rotation)) > p.respawnMaxPoseDistance.rotation ||
		/*!theGroundContactState.contact || */theFallDownState->state!= FallDownState::upright)
	{
		translation = Vector<2>(theRobotPose->translation.x, theRobotPose->translation.y);
		translationCov = MatrixMXN<2,2>();
		translationCov[0][0] = sqr(p.respawnPoseDeviation.translation.x);
		translationCov[1][1] = sqr(p.respawnPoseDeviation.translation.y);
		rotation = theRobotPose->rotation;
		rotationCov = sqr(p.respawnPoseDeviation.rotation);
		bool wasValidated = validated;
		validated = false;
		validGoalSightingSinceLastReset = 0;
		validUnknownGoalSightingSinceLastReset = 0;
		if(wasValidated)
			lastResetTime = theFrameInfo->time;
	}


	// try to relate the line percepts
	Vector2<double> intersection, ortogonalProjection;
 	double sqrLineRelationCorridor = sqr(p.lineRelationCorridor);
	LinePercept linePercept;
	for(std::list<LinesPercept::LineOnField>::const_iterator it =theLinesPercept->lineOnField.begin(), lastIt = theLinesPercept->lineOnField.end(); it != lastIt; ++it)
	{
		const LinesPercept::LineOnField& line = *it;
		linePercept.start = Vector2<double>(line.first.x, line.first.y);
		linePercept.end = Vector2<double>(line.last.x, line.last.y);
		Vector2<double> startOnField = *theRobotPose * linePercept.start;
		Vector2<double> endOnField = *theRobotPose * linePercept.end;
		Vector2<double> dirOnField = endOnField - startOnField;
		dirOnField.normalize();
		Vector2<double> ortogonalOnField(dirOnField.y, -dirOnField.x);

		int index = -1;
		for(int i = 0; i < countOfFieldLines; ++i)
		{
			const FieldLine& fieldLine = fieldLines[i];

			if(getSqrDistanceToLine(fieldLine.start, fieldLine.dir, fieldLine.length, startOnField) > sqrLineRelationCorridor ||
				getSqrDistanceToLine(fieldLine.start, fieldLine.dir, fieldLine.length, endOnField) > sqrLineRelationCorridor)
				continue;
			if(!intersectLineWithLine(startOnField, ortogonalOnField, fieldLine.start, fieldLine.dir, intersection))
				continue;
			if(getSqrDistanceToLine(startOnField, dirOnField, intersection) > sqrLineRelationCorridor)
				continue;
			if(!intersectLineWithLine(endOnField, ortogonalOnField, fieldLine.start, fieldLine.dir, intersection))
				continue;
			if(getSqrDistanceToLine(startOnField, dirOnField, intersection) > sqrLineRelationCorridor)
				continue;
			if(index != -1)
			{ // ambiguous?//找到两条匹配的线，则这条线不计算
				index = -1;
				break;
			}
			index = i;
		}

		if(index != -1) // success
			useLinePercept(linePercept, fieldLines[index]);
	}

	// try to relate the goal posts
	for(int i = 0; i < GoalPercept::NUMBER_OF_GOAL_POSTS; ++i)
	{
		const GoalPost& post = theGoalPercept->posts[i];
		if(post.timeWhenLastSeen == theFrameInfo->time)//if(post.perceptionType==GoalPost::SEEN_IN_IMAGE)//if(post.timeWhenLastSeen == theFrameInfo->time) 离线
			                                            //仿真时，当开始没看到门柱时，timeWhenLastSeen,
		{
			Vector2<double> position(post.positionOnField.x,post.positionOnField.y);
			Vector2<double> positionOnField = *theRobotPose * position;
			if((positionOnField - goalPosts[i]).squareAbs() < sqrLineRelationCorridor)
				useGoalPost(position, goalPosts[i]);
		}
	}
	for(int i = 0; i < GoalPercept::NUMBER_OF_UNKNOWN_GOAL_POSTS; ++i)
	{
		const GoalPost& post = theGoalPercept->unknownPosts[i];
		   if(post.timeWhenLastSeen == theFrameInfo->time)
		{
			Vector2<double> position(post.positionOnField.x, post.positionOnField.y);
			Vector2<double> positionOnField = *theRobotPose * position;
			ASSERT(GoalPercept::LEFT_OPPONENT == GoalPercept::RIGHT_OPPONENT - 1);
			ASSERT(GoalPercept::LEFT_OWN == GoalPercept::RIGHT_OWN - 1);
			int index = -1;
			for(int j = i == GoalPercept::UNKNOWN_OWN ? GoalPercept::LEFT_OWN : GoalPercept::LEFT_OPPONENT, end = j + 2; j < end; ++j)
			{
				if((positionOnField - goalPosts[j]).squareAbs() > sqrLineRelationCorridor)
					continue;
				if(index != -1)
				{  // ambiguous?
					index = -1;
					break;
				}
				index = j;
			}
			if(index != -1)
				useGoalPost(position, goalPosts[index]);
		}
	}

	// try to use the center circle
	if(theLinesPercept->circleCenter.found)
	{
		Vector2<double> circlePos(theLinesPercept->circleCenter.pos.x, theLinesPercept->circleCenter.pos.y);
		Vector2<double> circlePosOnField = *theRobotPose * circlePos;
		if(circlePosOnField.squareAbs() < sqrLineRelationCorridor)
			useCenterCircle(circlePos);
	}


	// try to validate the pose via goal posts
	Pose2D ownPose(rotation, translation.x, translation.y);
	if(!validated)
	{
		for(int i = 0; i < GoalPercept::NUMBER_OF_GOAL_POSTS; ++i)
		{
			const GoalPost& post = theGoalPercept->posts[i];
			if(post.timeWhenLastSeen == theFrameInfo->time)
			{
				Vector2<double> seenPositionOnField = ownPose * Vector2<double>(post.positionOnField.x, post.positionOnField.y);
				const Vector2<double>& realPositionOnField = goalPosts[i];

				if(fabs(realPositionOnField.x - translation.x) > (theFieldDimensions->xPosOpponentGroundline - theFieldDimensions->xPosOpponentPenaltyArea) &&
					(seenPositionOnField - realPositionOnField).squareAbs() / (realPositionOnField - Vector2<double>(translation.x, translation.y)).squareAbs() < sqr(p.validationMaxGoalPerceptDistance))
					validGoalSightingSinceLastReset++;
			}
		}
		for(int i = 0; i < GoalPercept::NUMBER_OF_UNKNOWN_GOAL_POSTS; ++i)
		{
			const GoalPost& post = theGoalPercept->unknownPosts[i];
			if(post.timeWhenLastSeen == theFrameInfo->time)
			{
				Vector2<double> seenPositionOnField = ownPose * Vector2<double>(post.positionOnField.x, post.positionOnField.y);
				const Vector2<double>& realPositionOnField1 = goalPosts[i == GoalPercept::UNKNOWN_OPPONENT ? GoalPercept::LEFT_OPPONENT : GoalPercept::LEFT_OWN];
				const Vector2<double>& realPositionOnField2 = goalPosts[i == GoalPercept::UNKNOWN_OPPONENT ? GoalPercept::RIGHT_OPPONENT : GoalPercept::RIGHT_OWN];
				if(fabs(realPositionOnField1.x - translation.x) > (theFieldDimensions->xPosOpponentGroundline - theFieldDimensions->xPosOpponentPenaltyArea) &&
					(seenPositionOnField - realPositionOnField1).squareAbs() / (realPositionOnField1 - Vector2<double>(translation.x, translation.y)).squareAbs() < sqr(p.validationMaxGoalPerceptDistance))
					validUnknownGoalSightingSinceLastReset++;
				if(fabs(realPositionOnField2.x - translation.x) > (theFieldDimensions->xPosOpponentGroundline - theFieldDimensions->xPosOpponentPenaltyArea) &&
					(seenPositionOnField - realPositionOnField2).squareAbs() / (realPositionOnField2 - Vector2<double>(translation.x, translation.y)).squareAbs() < sqr(p.validationMaxGoalPerceptDistance))
					validUnknownGoalSightingSinceLastReset++;
			}
		}
		/*std::cout<<"--validGoalSightingSinceLastReset--"<<"\t"<<validGoalSightingSinceLastReset<<"--validUnknownGoalSightingSinceLastReset--"<<"\t"
			<<validUnknownGoalSightingSinceLastReset<<"--translationCov[0][0]--"<<"\t"<<sqrt(fabs(translationCov[0][0]))<<
			"--translationCov[1][1]--"<<"\t"<<sqrt(fabs(translationCov[1][1]))<<"\t"<<"--rotationCov--"<<fabs(rotationCov)<<std::endl;*/
		if((validGoalSightingSinceLastReset >= p.validationMinGoalSightings || validUnknownGoalSightingSinceLastReset >= p.validationMinUnknownGoalSightings) &&
			translationCov[0][0] <= sqr(p.validationMaxDeviation.translation.x) &&
			translationCov[1][1] <= sqr(p.validationMaxDeviation.translation.y) &&
			rotationCov <= sqr(p.validationMaxDeviation.rotation))
			validated = true;

	}
    if(!validated)
	{
		if(translationCov[0][0] <= sqr(p.validationMaxDeviation.translation.x) &&translationCov[1][1] <= sqr(p.validationMaxDeviation.translation.y) &&
			rotationCov <= sqr(p.validationMaxDeviation.rotation))
			validated = true;
	}
	// generate model


	if(validated)
	{
		theRobotPose->rotation = ownPose.rotation;
		theRobotPose->translation = ownPose.translation;
		//theRobotPose->validity = 1.;
		theRobotPose->deviation = sqrt(fabs((std::max)(translationCov[0].x, translationCov[1].y)));
		Rotation=theRobotPose->rotation;
	 Translation=theRobotPose->translation;//为了update robotsamplebuffer 将kalman滤波后的robotpose 传回pc机
	}
	else
	{
		theRobotPose->rotation = theRobotPose->rotation;
		theRobotPose->translation = theRobotPose->translation;
		theRobotPose->validity = theRobotPose->validity;
		theRobotPose->deviation = RobotPose::unknownDeviation;
		Rotation=theRobotPose->rotation;
		Translation=theRobotPose->translation;//为了update robotsamplebuffer 将kalman滤波后的robotpose 传回pc机
		//std::cout<<"####robotPosevalidator deviation"<<sqrt(fabs((std::max)(translationCov[0].x, translationCov[1].y)))<<std::endl;
	}

	if(abs(middleTranslationX) < 150 )	
	{
		theRobotPose->rotation = direction;
	}



	//if(theRobotPose->translation.x >=0)
	//	ownSide = false;
	//else
	//	ownSide = true;
	updateSide(theRobotPose);



#ifdef NAO_DEBUG

		ColorRGBA color(0,0,(1-theRobotPose->validity)*255);
		if (validated)
		{
			color.b = 1;
		}
		theRobotPose->draw(color);
#endif
	std::cout<<"robotpose x is "<<theRobotPose->translation.x<<", y is "<<theRobotPose->translation.y<<"validity is "<<theRobotPose->validity<<std::endl;
	std::cout<<"*****robotpose validity***** "<<theRobotPose->validity<<"\t"<<"*****robotpose deviation***** "<<theRobotPose->deviation<<std::endl;

	// send own position to the team mates
	//TEAM_OUTPUT(idTeamMateRobotPose, bin, robotPose);
}


bool RobotPoseValidator::intersectLineWithLine(const Vector2<double>& lineBase1, const Vector2<double>& lineDir1,
											   const Vector2<double>& lineBase2, const Vector2<double>& lineDir2, Vector2<double>& intersection) const
{
	double h = lineDir1.x * lineDir2.y - lineDir1.y * lineDir2.x;
	if(h == 0.)
		return false;
	double scale = ((lineBase2.x - lineBase1.x) * lineDir1.y - (lineBase2.y - lineBase1.y) * lineDir1.x) / h;
	intersection.x = lineBase2.x + lineDir2.x * scale;
	intersection.y = lineBase2.y + lineDir2.y * scale;
	return true;
}

double RobotPoseValidator::getSqrDistanceToLine(const Vector2<double>& base, const Vector2<double>& dir, double length, const Vector2<double>& point) const
{
	double l = (point.x - base.x) * dir.x + (point.y - base.y) * dir.y;
	if(l < 0)
		l = 0;
	if(l > length)
		l = length;
	return ((base + dir * l) - point).squareAbs();
}

double RobotPoseValidator::getSqrDistanceToLine(const Vector2<double>& base, const Vector2<double>& dir, const Vector2<double>& point) const
{
	double l = (point.x - base.x) * dir.x + (point.y - base.y) * dir.y;
	return ((base + dir * l) - point).squareAbs();
}

Vector2<double> RobotPoseValidator::getOrthogonalProjection(const Vector2<double>& base, const Vector2<double>& dir, const Vector2<double>& point) const
{
	double l = (point.x - base.x) * dir.x + (point.y - base.y) * dir.y;
	return base + dir * l;
}

void RobotPoseValidator::motionUpdate()
{
	// add process noise
	translationCov[0][0] += sqr(p.filterProcessDeviation.translation.x);
	translationCov[1][1] += sqr(p.filterProcessDeviation.translation.y);
	rotationCov += sqr(p.filterProcessDeviation.rotation);
   ASSERT(rotation<1000000.);
	// add odometry
	Pose2D odometryOffset = *theOdometryData - lastOdometryData;
	Vector2<double> odo(odometryOffset.translation.x, odometryOffset.translation.y);
	odo.rotate(rotation);
	odoOffset = Vector<2>(odo.x,odo.y);
	//float tempX = translation[0];
	translation += Vector<2>(odo.x, odo.y);

	//if(tempX<=0&&translation[0]>0&&theRobotPose->validity>0.7)
	//{
	//	std::cout<<"entring opp side ================"<<std::endl;
	//	ownSide = false;
	//	crossMiddleLine = true;//meaning cross the middle line
	//}
	//else if(tempX>=0&&translation[0]<0&&theRobotPose->validity>0.7)
	//{
	//	std::cout<<"entring own side ================="<<std::endl;
	//	ownSide = true;
	//	crossMiddleLine = true;
	//}
	//else
	//{
	//	crossMiddleLine = false;
	//}

	rotation = normalize(rotation + odometryOffset.rotation);



	// add odometry noise
	translationCov[0][0] += sqr(odo.x * p.odometryDeviation.translation.x);
	translationCov[1][1] += sqr(odo.y * p.odometryDeviation.translation.y);
	rotationCov += sqr(odometryOffset.rotation * p.odometryDeviation.rotation);//得到预测估计的协方差矩阵
}

void RobotPoseValidator:: sensorUpdate(double angle, double x, double y, double angleVariance, double xOrYVariance)
{
	if(angle < 100000.)
	{
		double k = rotationCov / (rotationCov + angleVariance);
		rotation = normalize(rotation + k * normalize(angle - rotation));
		rotationCov -= k * rotationCov;
	}

	ASSERT((x < 100000. && y >= 100000.) || (y < 100000. && x >= 100000.) || (x >= 100000. && y >= 100000.));
	if(x < 100000.)
	{
		MatrixMXN<1,1> translationSensorCov;
		translationSensorCov[0][0] = xOrYVariance;
		Vector<1> z;
		z[0] = x;
		MatrixMXN<1,2> c;
		c[0][0] = 1.;
		MatrixMXN<2,1> cTrans = c.transpose();
		MatrixMXN<2,1> k = translationCov * cTrans *(c * translationCov * cTrans + translationSensorCov).invert();
		translation += k * (z - c * translation);
		translationCov -= k * c * translationCov;
	}
	if(y < 100000.)
	{
		MatrixMXN<1,1> translationSensorCov;
		translationSensorCov[0][0] = xOrYVariance;
		Vector<1> z;
		z[0] = y;
		MatrixMXN<1,2> c;
		c[1][0] = 1.;
		MatrixMXN<2,1> cTrans = c.transpose();
		MatrixMXN<2,1> k = translationCov * cTrans * (c * translationCov * cTrans + translationSensorCov).invert();
		translation += k * (z - c * translation);
		translationCov -= k * c * translationCov;
	}
}

void RobotPoseValidator::useLinePercept(const LinePercept& linePercept, const FieldLine& fieldLine)
{
	Vector2<double> center = (linePercept.start + linePercept.end) * 0.5f;
	Vector2<double> dir = linePercept.end - linePercept.start;
	dir.normalize();
	Vector2<double> ortogonalProjection = getOrthogonalProjection(linePercept.start, dir, Vector2<double>());
	double measuredAngle = -atan2(ortogonalProjection.y, ortogonalProjection.x);
	measuredAngle = normalize(measuredAngle + (fieldLine.vertical ? pi_2 : 0));
	double possibleAngle2 = normalize(measuredAngle - pi);
	if(fabs(normalize(possibleAngle2 - theRobotPose->rotation)) < fabs(normalize(measuredAngle - theRobotPose->rotation)))
		measuredAngle = possibleAngle2;
	double c = cos(measuredAngle), s = sin(measuredAngle);
	Matrix2x2<> angleRotationMatrix(Vector2<double>(c, s), Vector2<double>(-s, c));
	ortogonalProjection = angleRotationMatrix * ortogonalProjection;

	Matrix2x2<> cov = getCovOfPointInWorld(center, 0.);
	cov = angleRotationMatrix * cov * angleRotationMatrix.transpose();

	if(fieldLine.vertical)
	{
		double measuredY = fieldLine.start.y - ortogonalProjection.y;
		double yVariance = cov[1][1];
		double angleVariance = sqr(atan(sqrt(fabs(4. * yVariance / (linePercept.start - linePercept.end).squareAbs()))));
		sensorUpdate(measuredAngle, 100000., measuredY, angleVariance, yVariance);
	}
	else
	{
		double measuredX = fieldLine.start.x - ortogonalProjection.x;
		double xVariance = cov[0][0];
		double angleVariance = sqr(atan(sqrt(fabs((4. * xVariance / (linePercept.start - linePercept.end).squareAbs())))));
		sensorUpdate(measuredAngle, measuredX, 100000., angleVariance, xVariance);
	}
}

void RobotPoseValidator::useGoalPost(const Vector2<double>& postPos, const Vector2<double>& postOnField)
{
	double seenAngleToGoal = atan2(postPos.y, postPos.x);
	double measuredAngle = normalize((postOnField - Vector2<double>(translation.x, translation.y)).angle() - seenAngleToGoal);

	Matrix2x2<> cov = getCovOfPointInWorld(postPos, 0.);
	double c = cos(-seenAngleToGoal), s = sin(-seenAngleToGoal);
	Matrix2x2<> angleRotationMatrix(Vector2<double>(c, s), Vector2<double>(-s, c));
	cov = angleRotationMatrix * cov * angleRotationMatrix.transpose();
	double angleVariance = sqr(atan(sqrt(fabs((cov[1][1]) / (postPos.squareAbs())))));

	sensorUpdate(measuredAngle, 100000., 100000., angleVariance, 100000.);
}

void RobotPoseValidator::useCenterCircle(const Vector2<double>& circlePos)
{
	Vector2<double> measurement = -Vector2<double>(circlePos).rotate(rotation);
	double circleDistance = circlePos.abs();
	Vector2<double> increasedCirclePos = circlePos * ((circleDistance + theFieldDimensions->centerCircleRadius) / circleDistance);
	Matrix2x2<> cov = getCovOfPointInWorld(increasedCirclePos, 0.);
	sensorUpdate(100000., 100000., measurement.y, 100000., cov[1][1]);
	sensorUpdate(100000., measurement.x, 100000., 100000., cov[0][0]);
}

Matrix2x2<> RobotPoseValidator::getCovOfPointInWorld(const Vector2<double>& pointInWorld2, double pointZInWorld) const
{
	Vector3<double>unscaledVectorToPoint = theCameraMatrix->invert() * Vector3<double>(pointInWorld2.x, pointInWorld2.y, pointZInWorld);
	const Vector3<double>unscaledWorld = theCameraMatrix->rotation * unscaledVectorToPoint;
	const double h = theCameraMatrix->translation.z - pointZInWorld;
	const double scale = h / (-unscaledWorld.z);
	Vector2<double> pointInWorld(unscaledWorld.x * scale, unscaledWorld.y * scale);
	const double distance = pointInWorld.abs();
	const double angle = pointInWorld.angle();
	const double c = cos(angle), s = sin(angle);
	Matrix2x2<> rot(Vector2<double>(c, s), Vector2<double>(-s, c));
	Matrix2x2<> cov(Vector2<double>(sqr(h / tan((distance == 0. ? pi_2 : atan(h / distance)) - p.robotRotationDeviation.x) - distance), 0.),
		Vector2<double>(0., sqr(tan(p.robotRotationDeviation.y) * distance)));
	return rot * cov * rot.transpose();
}
void RobotPoseValidator::update(Robotsamplebuffer*theRobotsamplebuffer)
{
	if(validated)
	{
		theRobotsamplebuffer->Robotsample[1].Robotrotation=theRobotPose->rotation;
		theRobotsamplebuffer->Robotsample[1].RobotTranslationX=theRobotPose->translation.x;
		theRobotsamplebuffer->Robotsample[1].RobotTranslationY=theRobotPose->translation.y;
		theRobotsamplebuffer->Robotsample[1].color.r=uint8(0);
		theRobotsamplebuffer->Robotsample[1].color.g=uint8(0);
		theRobotsamplebuffer->Robotsample[1].color.b=uint8(1);
	}
	else
	{
		theRobotsamplebuffer->Robotsample[1].Robotrotation=theRobotPose->rotation;
		theRobotsamplebuffer->Robotsample[1].RobotTranslationX=theRobotPose->translation.x;
		theRobotsamplebuffer->Robotsample[1].RobotTranslationY=theRobotPose->translation.y;
		theRobotsamplebuffer->Robotsample[1].color.r=uint8(0);
		theRobotsamplebuffer->Robotsample[1].color.g=uint8(0);
		theRobotsamplebuffer->Robotsample[1].color.b=uint8((1-theRobotPose->validity)*255);
	}

}

double RobotPoseValidator::computerLocWeighting(const Pose2D& robotPose,RobotLocation& shadowLoc,double standardDeviation, double bestPossibleWeighting)
{
	double Weighting1 = gaussianProbability(fabs(shadowLoc.x - robotPose.translation.x), standardDeviation) / bestPossibleWeighting;
	double Weighting2 = gaussianProbability(fabs(shadowLoc.y - robotPose.translation.y), standardDeviation) / bestPossibleWeighting;
	//return Weighting1;
	return (Weighting1 + Weighting2)/2*shadowLoc.validity;
}
double RobotPoseValidator::computeBallWeighting(const BallLocation& ballseen,BallLocation& globalBall,double standardDeviation, double bestPossibleWeighting)
{
	double Weighting1 = gaussianProbability(fabs(ballseen.x - globalBall.x),standardDeviation) / bestPossibleWeighting;
	double Weighting2 = gaussianProbability(fabs(ballseen.y - globalBall.y),standardDeviation) / bestPossibleWeighting;
	return (Weighting1+Weighting2)/2;
}
double RobotPoseValidator::applyShaValidity(const RobotPose &robotPose)
{
	double maxValidity(0);
	double currentValidity(0);
	double standardDeviation = 200;
	const double bestPossibleWeighting = gaussianProbability(0.0, standardDeviation);
	for(int i =0 ;i<theCurrentGlobalInfo->shadowLoc.size();++i)
	{
		currentValidity =  computerLocWeighting(robotPose,theCurrentGlobalInfo->shadowLoc[i],standardDeviation,bestPossibleWeighting);
		maxValidity = maxValidity > currentValidity? maxValidity:currentValidity;
	}
	return maxValidity;
}
double RobotPoseValidator::applyGlobalBall(const RobotPose& robotpose)
{
	if (theBallPercept->invion && theRobotPose->validity > 0.7 && theCurrentGlobalInfo->ballLoc.x != 0)
	{
		double standardDeviation = 200;
		const double bestPossibleWeighting = gaussianProbability(0.0, standardDeviation);

		double dis = theBallPercept->relativePositionOnField.abs();
		double ballx = theBallPercept->relativePositionOnField.x;
		double bally = theBallPercept->relativePositionOnField.y;
		double angle = robotpose.rotation + atan2(bally,ballx);
		BallLocation seenBall;
		seenBall.x = robotpose.translation.x + dis*cos(angle);
		seenBall.y = robotpose.translation.y + dis*sin(angle);
		return computeBallWeighting(seenBall,theCurrentGlobalInfo->ballLoc,standardDeviation,bestPossibleWeighting);
	}
	
	return 0;
}
void RobotPoseValidator::preUpdateSide()
{
	lastOwnSide = ownSide;
}
void RobotPoseValidator::updateSide(RobotPose* robotPose)
{
	/*  test side according the teammate position*/
	RobotPose oppPose;
	oppPose.translation.x = - robotPose->translation.x;
	oppPose.translation.y = - robotPose->translation.y;
	if (robotPose->rotation >= 0)
	{
		oppPose.rotation = -pi + robotPose->rotation;
	}
	else
	{
		oppPose.rotation = pi + robotPose->rotation;
	}

	
	double weight1 = applyGlobalBall(*robotPose);
	double weight2 = applyGlobalBall(oppPose);
	std::cout<<"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"<<weight1<<std::endl;
	std::cout<<"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"<<weight2<<std::endl;
	if (weight1 != 0 || weight2 != 0)
	{
		if ((weight2 - weight1 > 0.3)&&weight2>0.4)
		{
			std::cout<<"CHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"<<std::endl;
			ownSide = !ownSide;
		}
	}
	//double weight1 = applyShaValidity(*robotPose);
	//double weight2 = applyShaValidity(oppPose);
	//if(weight2 > weight1 && weight2 > 0.7)
	//{
	//	;
	//	//ownSide=!ownSide;
	//}
	///////////////////////////////////////////////


	//std::cout<<"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"<<int(theRoboCupGameControlData->state)<<std::endl;
	if(int(theRoboCupGameControlData->secondaryState) == STATE2_NORMAL)
	{

		if(theCurrentGlobalInfo->ballLoc.x!=0&&theCurrentGlobalInfo->ballLoc.y!=0)
		{
			if(theBallPercept->invion&&theBallPercept->relativePositionOnField.abs()< 300)
			{
				if(theCurrentGlobalInfo->ballLoc.x< 0)
				{
					ownSide = true;
					//std::cout<<"CCCCCCCCCCCCCCCCCCC"<<std::endl;
					//ownSide = false;//for debug
				}
			}

		}

		if(int(theRoboCupGameControlData->state) == STATE_SET || int(theRoboCupGameControlData->state) == STATE_INITIAL )
		{
			ownSide = true;
			//std::cout<<"OAAAAAAAAAAAAAAAAAAAAA"<<std::endl;
			//return;
		}
		if(lastPenalty != PENALTY_NONE && theRobotInfo->penalty == PENALTY_NONE)
		{
			ownSide =true;
			//return;
		}

		lastPenalty = theRobotInfo->penalty;

	}
	if(int(theRoboCupGameControlData->secondaryState) == STATE2_PENALTYSHOOT) // when penalty shoot ,the attacker should be in opp side.
	{
		ownSide = false;
	}
#ifndef NAO_DEBUG
	if(theTeamCfgInfo->playerNum == 1) // goal keeper will always in own side
	{
		ownSide = true;
	}
#endif





	afterUpdateSide(robotPose);

	if(ownSide)
		std::cout<<"OWNNNNNNNNNNNNNNNNNNNN SIDE"<<std::endl;
	else
		std::cout<<"OPPPPPPPPPPPPPPPPPPPPP SIDE"<<std::endl;



}
void RobotPoseValidator::afterUpdateSide(RobotPose* robotPose)
{
	if(crossMiddleLine == true)
		robotPose->sideValidity = 2;
	else
		robotPose->sideValidity = 1;
	if ((ownSide == true && !isOwnSide(robotPose)) || (ownSide == false&&isOwnSide(robotPose)))
	{
		robotPose->translation.x = -robotPose->translation.x;
		robotPose->translation.y = -robotPose->translation.y;
		if (robotPose->rotation >= 0)
		{
			robotPose->rotation = -pi + robotPose->rotation;
		}
		else
		{
			robotPose->rotation = pi + robotPose->rotation;
		}
	}
	if((ownSide == true && translation[0]>0) || (ownSide == false && translation[0] <0) )
	{
		translation[0] = -translation[0];
		translation[1] = -translation[1];
	}

	robotPose->ownSide = ownSide;
}
bool RobotPoseValidator::isOwnSide(RobotPose* robotPose)
{
	if (theRobotPose->translation.x <=0 )
	{
		return true;
	}
	else
		return false;
}
void RobotPoseValidator::updateDirection()
{

	double lastmiddleTranslationX = middleTranslationX;

	Pose2D odometryOffset = *theOdometryData - lastOdometryData;
	Vector2<double> odo(odometryOffset.translation.x, odometryOffset.translation.y);
	odo.rotate(direction);
	odoOffset = Vector<2>(odo.x,odo.y);

	direction +=odometryOffset.rotation;
	middleTranslationX +=odoOffset[0];

	if (int(theRoboCupGameControlData->state) == STATE_SET)
	{
		direction = 0;
		middleTranslationX = theRobotPose->translation.x;
	}
	if(abs(theRobotPose->translation.x) > 200)
	{
		if (abs(theRobotPose->translation.x) > 400)
		{
			//if(theRobotPose->validity > 0.8)
			//{
			direction = theRobotPose->rotation;
			middleTranslationX = theRobotPose->translation.x;
			//}
		}
		else
		{
			middleTranslationX = theRobotPose->translation.x;
		}

	}

	direction = fmod(direction,pi*2);
	if(direction > pi)
	{
		direction = -(pi - fmod(direction,pi));
	}
	else if(direction < -pi)
	{
		direction = pi + fmod(direction,pi);
	}

	if(lastmiddleTranslationX <= 0&&middleTranslationX > 0)
	{
		std::cout<<"entring opp side ================"<<std::endl;
		ownSide = false;
		crossMiddleLine = true;//meaning cross the middle line
	}
	else if(lastmiddleTranslationX > 0&&middleTranslationX <= 0)
	{
		std::cout<<"entring own side ================="<<std::endl;
		ownSide = true;
		crossMiddleLine = true;
	}
	else
	{
		crossMiddleLine = false;
	}


	lastOdometryData = *theOdometryData;


}
MAKE_KS(RobotPoseValidator)
