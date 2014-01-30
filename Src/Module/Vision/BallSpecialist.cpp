/**
* @file BallSpecialist.cpp
* This file contains a class for Image Processing.
*/
#include "BallSpecialist.h"
BallSpecialist::BallSpecialist()
{
	std::string path=std::string(GetSetting::GetConfigDir())+"/VisionConfig/BallSpecialist.cfg";
	BallParaments.readCfgFile(path);
	BallParaments.massAssign();
	//BallParaments.paramReset();
	std::cout<<"end of ball spe"<<std::endl;
}
BallSpecialist::~BallSpecialist()
{

}
void BallSpecialist::update(BallPercept* theBallPercept)
{
	//std::cout<<"**********ball*********"<<std::endl;
	unsigned  timeBall=SystemCall::getCurrentSystemTime();
	theBallPercept->reset();
	for(std::vector<BallSpot>::const_iterator it=theBallSpots->ballSpots.begin(),end=theBallSpots->ballSpots.end();it!=end;++it)
	{
		const BallSpot&ballspot=*it;
		if(!checkBallSpot(ballspot))
			continue;

		if(!searchBallPoints(ballspot))
			continue;

		if(!checkBallPoints())
			continue;

		if(!calculateBallInImage())
			continue;

		if(!evaluateBallInImage())
			continue;

		if(!calculateBallOnField())
			continue;

    theBallPercept->timewhenlastseen = theFrameInfo->time;
    theBallPercept->BallCenter=center;
	theBallPercept->radius=radius;
	theBallPercept->relativePositionOnField=Vector2<double>(usedCenterOnField.x,usedCenterOnField.y);
	theBallPercept->coordinateRobot=Vector3<double>(usedCenterOnField.x,usedCenterOnField.y,BALL_RADIUS);
    theBallPercept->invion=true;
#ifndef NAO_DEBUG
	keeperDive();
#endif

	lastPercept = *theBallPercept;


	DRAW_CIRCLE(center.x,center.y,radius,2,Drawings::ps_SolidLine,Drawings::red);
    outBallPercept();
	unsigned timeBallUsed=SystemCall::getTimeSince(timeBall);
	std::cout<<"**********ball time used************"<<timeBallUsed<<std::endl;
	return;

	}
	theBallPercept->invion=false;
	unsigned timeBallUsed=SystemCall::getTimeSince(timeBall);
	std::cout<<"**********ball time used************"<<timeBallUsed<<std::endl;




}
void BallSpecialist::keeperDive()
{
	if(theTeamCfgInfo->playerNum == 1&&int(theRoboCupGameControlData->secondaryState) == STATE2_PENALTYSHOOT&&int(theRoboCupGameControlData->state) == STATE_PLAYING)
	{
		if (theBallPercept->relativePositionOnField.x < 1300 && lastPercept.relativePositionOnField.x > 1300 )
		{
			if (theBallPercept->relativePositionOnField.y > 50)
			{
				//myProxyContainer.ttsProxy->say("Left");
				AL::ALValue names      = "Body";
				AL::ALValue angleLists = 1.0f;
				angleLists.clear();
				angleLists.arraySetSize(22);
				angleLists[0] = -0.0015759468078613281;
				angleLists[1] = 0.035239934921264648;
				angleLists[2] = -1.6121920347213745;
				angleLists[3] = -0.1318819522857666;
				angleLists[4] = 0.68420600891113281;
				angleLists[5] = -0.075207948684692383;
				angleLists[6] = -0.02143406867980957; 
				angleLists[7] = -0.37126994132995605;
				angleLists[8] = -0.40655207633972168;
				angleLists[9] = 0.72562408447265625;
				angleLists[10] = -0.45709013938903809;
				angleLists[11] = 0.51998400688171387;
				angleLists[12] = -0.02143406867980957;
				angleLists[13] = -0.31911396980285645;
				angleLists[14] = -0.39573001861572266;
				angleLists[15] = 0.55680012702941895;
				angleLists[16] = -0.28229808807373047;
				angleLists[17] = 0.34050607681274414;
				angleLists[18] = 2.0233039855957031;
				angleLists[19] = -0.19170808792114258;
				angleLists[20] = 1.3959817886352539;
				angleLists[21] = 1.3943641185760498;

				AL::ALValue timeLists  = 1.0f;
				timeLists.clear();
				timeLists.arraySetSize(22);
				for (int i =0;i<22;++i)
				{
					timeLists[i] = 0.45;
				}

				bool isAbsolute        = true;
				myProxyContainer.motionProxy->angleInterpolation(names, angleLists, timeLists, isAbsolute);
			}
			else if(theBallPercept->relativePositionOnField.y < - 50)
			{
				//myProxyContainer.ttsProxy->say("Right");
				AL::ALValue names      = "Body";
				AL::ALValue angleLists = 1.0f;
				angleLists.clear();
				angleLists.arraySetSize(22);
				angleLists[0] = -0.0015759468078613281;
				angleLists[1] = 0.035239934921264648;
				angleLists[2] = 2.0233039855957031;
				angleLists[3] = 0.19170808792114258;
				angleLists[4] = -1.3959817886352539;
				angleLists[5] = -1.3943641185760498;
				angleLists[6] = -0.02143406867980957;
				angleLists[7] = 0.31911396980285645;
				angleLists[8] = -0.39573001861572266;
				angleLists[9] = 0.55680012702941895;
				angleLists[10] = -0.28229808807373047;
				angleLists[11] = -0.34050607681274414;
				angleLists[12] = -0.02143406867980957;
				angleLists[13] = 0.37126994132995605;
				angleLists[14] = -0.40655207633972168;
				angleLists[15] = 0.72562408447265625;
				angleLists[16] = -0.45709013938903809;
				angleLists[17] = -0.51998400688171387;
				angleLists[18] = -1.6121920347213745;
				angleLists[19] = 0.1318819522857666;
				angleLists[20] = -0.68420600891113281;
				angleLists[21] = 0.075207948684692383;

				AL::ALValue timeLists  = 1.0f;
				timeLists.clear();
				timeLists.arraySetSize(22);
				for (int i =0;i<22;++i)
				{
					timeLists[i] = 0.45;
				}

				bool isAbsolute        = true;
				myProxyContainer.motionProxy->angleInterpolation(names, angleLists, timeLists, isAbsolute);
			}
		}
	}
}
bool BallSpecialist::createBallPerceptLevenbergMarquardt
(
 Vector2<int>& center,
 double& radius
 )
{

	if (validBallPoints < 3)
		return false;

	double Mx = 0.0;
	double My = 0.0;
	double Mxx = 0.0;
	double Myy = 0.0;
	double Mxy = 0.0;
	double Mz = 0.0;
	double Mxz = 0.0;
	double Myz = 0.0;

	for (const BallPoint* ballPoint = ballPoints, * end = ballPoints + sizeof(ballPoints) / sizeof(*ballPoints); ballPoint < end; ++ballPoint)
	{
		if (ballPoint->isValid)
		{
		double x = ballPoint->point.x;
		double y = ballPoint->point.y;
		double xx = x*x;
		double yy = y*y;
		double z = xx + yy;

		Mx += x;
		My += y;
		Mxx += xx;
		Myy += yy;
		Mxy += x*y;
		Mz += z;
		Mxz += x*z;
		Myz += y*z;
		}
	}

	// try
	// {
	Matrix3x3<double> M(Vector3<double>(Mxx, Mxy, Mx),
		Vector3<double>(Mxy, Myy, My),
		Vector3<double>(Mx, My, validBallPoints)
		);

	Vector3<double> v;

	v.x = -Mxz;
	v.y = -Myz;
	v.z = -Mz;

	Vector3<double> BCD;
	Matrix3x3<double> temp;
	if(M.det())
	{temp=M.invert();
	BCD = temp * v; }     //solve M*x=v
	else
		return false;

	center.x = (int)(-BCD.x / 2.0);
	center.y = (int)(-BCD.y / 2.0);

	double tmpWurzel = BCD.x*BCD.x/4.0 + BCD.y*BCD.y/4.0 - BCD.z;

	if (tmpWurzel < 0.0)
		return false;

	radius = sqrt(tmpWurzel);

	return true;
}

bool BallSpecialist::checkBallSpot(const BallSpot& ballSpot)
{

	//Todo: we don't have done motion compensation
	// calculate an approximation of the radius based on bearing distance of the ball spot
	//But the radius of the ball may be not accurate
	const Vector2<int>& spot = ballSpot.position;
	Vector3<double> cameraToStart(FOCAL_LENGTH, IMAGE_WIDTH/2.0- spot.x,IMAGE_HEIGHT/2.0-spot.y);
	Vector3<double> unscaledField = theCameraMatrix->rotation* cameraToStart;
	if(unscaledField.z >= 0.0)
		return false; // above horizon
	const double scaleFactor = (theCameraMatrix->translation.z- BALL_DIAMETER/2.0) / unscaledField.z;
	cameraToStart *= scaleFactor;
	unscaledField *= scaleFactor;
	if(sqrt(Vector2<double>(unscaledField.x, unscaledField.y).squareAbs()) > BallParaments.sqrMaxBallDistance)
		return false; // too far away
	cameraToStart.y += cameraToStart.y > 0 ? -BALL_RADIUS: BALL_RADIUS;
	cameraToStart /= scaleFactor;
	approxRadius1 = fabs(IMAGE_WIDTH/2.0- cameraToStart.y - spot.x);

	return true;
}
bool BallSpecialist::searchBallPoints(const BallSpot& ballSpot)
{
	//
	Vector2<int> start = ballSpot.position;
    double approxDiameter = approxRadius1 * BallParaments.clippingApproxRadiusScale + BallParaments.clippingApproxRadiusPixelBonus;
	approxDiameter=approxDiameter >BallParaments.minApproDiameterforScan?approxDiameter:BallParaments.minApproDiameterforScan;
	int halfApproxRadius = int(approxRadius1 * 0.5);
	DRAW_CROSS(start.x,start.y,2,1,Drawings::ps_SolidLine,Drawings::white);
	DRAW_CIRCLE(start.x,start.y,approxDiameter,1,Drawings::ps_SolidLine,Drawings::darkGreen);//画近似的圆红色，没有修正圆心

	// try to improve the start point，修正圆心
	int maxColorDistance = BallParaments.scanMaxColorDistance;
	int resolutionWidth = IMAGE_WIDTH;
	int resolutionHeight = IMAGE_HEIGHT;
	startPixel.y = theTJImage->unCorrectedRaw[start.y][start.x][0];
	startPixel.u = theTJImage->unCorrectedRaw[start.y][start.x][1];
	startPixel.v = theTJImage->unCorrectedRaw[start.y][start.x][2];
	Vector2<int> preScanPoints[4] = {
		Vector2<int>(start.x + halfApproxRadius, start.y + halfApproxRadius),
		Vector2<int>(start.x - halfApproxRadius, start.y - halfApproxRadius),
		Vector2<int>(start.x + halfApproxRadius, start.y - halfApproxRadius),
		Vector2<int>(start.x - halfApproxRadius, start.y + halfApproxRadius)
	};
	bool preScanResults[4];
	for(int i = 0; i < 4; ++i)
	{
		if(preScanPoints[i].x < 0 || preScanPoints[i].x >= resolutionWidth ||
			preScanPoints[i].y < 0 || preScanPoints[i].y >= resolutionHeight)
		{
			i -= i % 2;
			preScanResults[i++] = false;
			preScanResults[i] = false;
		}
		else
		{
			 PixelYUV pixel;
			pixel.y=theTJImage->unCorrectedRaw[preScanPoints[i].y][preScanPoints[i].x][0];
			pixel.u=theTJImage->unCorrectedRaw[preScanPoints[i].y][preScanPoints[i].x][1];
		    pixel.v=theTJImage->unCorrectedRaw[preScanPoints[i].y][preScanPoints[i].x][2];//u==cb,v==cr
			preScanResults[i] = abs(startPixel.u- pixel.u) + abs(startPixel.v - pixel.v) < maxColorDistance;
		}
	}
	if(preScanResults[0] != preScanResults[1] && preScanResults[2] != preScanResults[3])
	{
		start = Vector2<int>();
		if(preScanResults[0])
			start += preScanPoints[0];
		else
			start += preScanPoints[1];
		if(preScanResults[2])
			start += preScanPoints[2];
		else
			start += preScanPoints[3];
		start /= 2;
	}
	else if(preScanResults[0] != preScanResults[1])
		start = preScanResults[0] ? preScanPoints[0] : preScanPoints[1];
	else if(preScanResults[2] != preScanResults[3])
		start = preScanResults[2] ? preScanPoints[2] : preScanPoints[3];


	DRAW_CROSS(start.x,start.y,2,1,Drawings::ps_SolidLine,Drawings::green);
	DRAW_CIRCLE(start.x,start.y,approxDiameter,1,Drawings::ps_SolidLine,Drawings::green);//修正圆心后的圆绿色

	// prepare scans
	totalPixelCount = 0;
	totalCbSum = 0;
	totalCrSum = 0;

	// vertical scan
	if(!findEndOfBall(start, startPixel, Vector2<int>(0, 1), approxDiameter, ballPoints[0]) ||
		!findEndOfBall(start, startPixel, Vector2<int>(0, -1), approxDiameter, ballPoints[4]))
	//if(!findEndOfBall(start, ORANGE, Vector2<int>(0, 1), approxDiameter, ballPoints[0]) ||
	//	!findEndOfBall(start, ORANGE, Vector2<int>(0, -1), approxDiameter, ballPoints[4]))
		return false;
	if(ballPoints[0].atBorder && ballPoints[4].atBorder)
		return false; // too large
	else if(ballPoints[0].atBorder)
	{
		start.y = ballPoints[4].point.y + int(approxRadius1);
		if(start.y > ballPoints[0].point.y - 1)
			start.y = ballPoints[0].point.y - 1;
	}
	else if(ballPoints[4].atBorder)
	{
		start.y = ballPoints[0].point.y - int(approxRadius1);
		if(start.y < ballPoints[4].point.y + 1)
			start.y = ballPoints[4].point.y + 1;
	}
	else
		start.y = (ballPoints[0].point.y + ballPoints[4].point.y) / 2;

	// horizontal scan
	if(!findEndOfBall(start, startPixel, Vector2<int>(1, 0), approxDiameter, ballPoints[2]) ||
		!findEndOfBall(start, startPixel, Vector2<int>(-1, 0), approxDiameter, ballPoints[6]))
		return false;
	if(ballPoints[2].atBorder && ballPoints[6].atBorder)
		return false; // too large
	else if(ballPoints[2].atBorder)
	{
		start.x = ballPoints[6].point.x + int(approxRadius1);
		if(start.x > ballPoints[2].point.x - 1)
			start.x = ballPoints[2].point.x - 1;
	}
	else if(ballPoints[6].atBorder)
	{
		start.x = ballPoints[2].point.x - int(approxRadius1);
		if(start.x < ballPoints[6].point.x + 1)
			start.x = ballPoints[6].point.x + 1;
	}
	else
		start.x = (ballPoints[2].point.x + ballPoints[6].point.x) / 2;
	approxCenter2 = start; //竖直扫描水平后近似的圆心

	// maybe repeat vertical and horizontal scans
	int skipArea = min(ballPoints[0].point.y - ballPoints[4].point.y, ballPoints[2].point.x - ballPoints[6].point.x) / 4;
	double maxLength = approxDiameter - skipArea;
	if(abs(start.x - ballPoints[0].start.x) > halfApproxRadius)
	{
		if(!findEndOfBall(start + Vector2<int>(0, skipArea), startPixel, Vector2<int>(0, 1), maxLength, ballPoints[0]) ||
			!findEndOfBall(start + Vector2<int>(0, -skipArea),startPixel, Vector2<int>(0, -1), maxLength, ballPoints[4]))
			return false;
	}
	if(abs(start.y - ballPoints[2].start.y) > halfApproxRadius)
	{
		if(!findEndOfBall(start + Vector2<int>(skipArea, 0), startPixel, Vector2<int>(1, 0), maxLength, ballPoints[2]) ||
			!findEndOfBall(start + Vector2<int>(-skipArea, 0), startPixel, Vector2<int>(-1, 0), maxLength, ballPoints[6]))
			return false;
	}

	// diagonal scans
	skipArea =min(ballPoints[0].point.y - ballPoints[4].point.y, ballPoints[2].point.x - ballPoints[6].point.x) / 4;
	maxLength = approxDiameter - 1.41421356f * skipArea;
	if(!findEndOfBall(start + Vector2<int>(skipArea, skipArea),startPixel, Vector2<int>(1, 1), maxLength, ballPoints[1]) ||
		!findEndOfBall(start + Vector2<int>(-skipArea, -skipArea),startPixel, Vector2<int>(-1, -1), maxLength, ballPoints[5]) ||
		!findEndOfBall(start + Vector2<int>(skipArea, -skipArea), startPixel, Vector2<int>(1, -1), maxLength, ballPoints[3]) ||
		!findEndOfBall(start + Vector2<int>(-skipArea, skipArea), startPixel, Vector2<int>(-1, 1), maxLength, ballPoints[7]))
		return false;

	// improve ball points
	if(totalPixelCount == 0)
		return false;
	int cbAvg = totalCbSum / totalPixelCount;
	int crAvg = totalCrSum / totalPixelCount;
	int refineMaxPixelCount = BallParaments.refineMaxPixelCount;
	int refineMaxColorDistance = BallParaments.refineMaxColorDistance;
	for(unsigned j = 0; j < sizeof(ballPoints) / sizeof(*ballPoints); ++j)
	{
		BallPoint& ballPoint = ballPoints[j];
		const Vector2<int>& step = ballPoint.step;
		Vector2<int> pos = ballPoint.point;
		int i = 0;
		for(; i < refineMaxPixelCount; ++i)
		{
			pos += step;
			if(pos.x < 0 || pos.x >= resolutionWidth ||
				pos.y < 0 || pos.y >= resolutionHeight)
				break;
			if(abs(theTJImage->unCorrectedRaw[pos.y][pos.x][1] - cbAvg) + abs(theTJImage->unCorrectedRaw[pos.y][pos.x][2] - crAvg) > refineMaxColorDistance)
				break;
		}
		if(i)
			ballPoint.point = pos - step;
		ballPoint.pointf = Vector2<double>(double(ballPoint.point.x), double(ballPoint.point.y));
	}


	return true;
}
bool BallSpecialist::findEndOfBall(const Vector2<int>&start,PixelYUV pixel,const Vector2<int>&step,int maxLength,BallPoint&ballPoint)
{
	ASSERT(step.x == 0 || step.x == 1 || step.x == -1);
	ASSERT(step.y == 0 || step.y == 1 || step.y == -1);

	Vector2<int> pos = start;
	Vector2<int> lastValidPos = pos;
	unsigned int overtime = 0;//该扫描线上
	int cbSum = startPixel.u;
	int crSum = startPixel.v;
	int pixelCount = 1;
	int cb, cr;

	ballPoint.step = step;
	ballPoint.atBorder = false;
	ballPoint.start = start;

	int maxColorDistance = BallParaments.scanMaxColorDistance;
	unsigned int maxOvertime = BallParaments.scanPixelTolerance;
	int resolutionWidth = IMAGE_WIDTH;
	int resolutionHeight = IMAGE_HEIGHT;
	int stepAbs1024 = (step.x == 0 || step.y == 0) ? 1024 : 1448; // 1448 = sqrt(2) * 1024
	int maxAbs1024 = int(maxLength * 1024.); // + 1024;
	int length1024 = 0;
	for(;;)
	{
		pos += step;
		length1024 += stepAbs1024;

		if(length1024 > maxAbs1024)
		{
			if(overtime > 0)
				break;
			DRAW_LINE(start.x, start.y, pos.x, pos.y, 1, Drawings::ps_SolidLine,Drawings::green);//错误的线
			return false;
		}

		if(pos.x < 0 || pos.x >= resolutionWidth ||
			pos.y < 0 || pos.y >= resolutionHeight)
		{
			ballPoint.atBorder = true;
			break;
		}

		cb = theTJImage->unCorrectedRaw[pos.y][pos.x][1];
		cr =  theTJImage->unCorrectedRaw[pos.y][pos.x][2];
		if(abs(cb - cbSum / pixelCount) + abs(cr - crSum / pixelCount) > maxColorDistance)
		{
			if(overtime == 0)
				lastValidPos = pos - step;
			++overtime;
			if(overtime <= maxOvertime)
				continue;
			break;
		}
		cbSum += cb;
		crSum += cr;
		++pixelCount;
		overtime = 0;
	}

	 ballPoint.point = lastValidPos;

	if(pixelCount > 0)
	{
		--pixelCount;
		cbSum -= startPixel.u;
		crSum -= startPixel.v;
		totalPixelCount += pixelCount;
		totalCbSum += cbSum;
		totalCrSum += crSum;
	}

	DRAW_LINE(ballPoint.start.x, ballPoint.start.y, ballPoint.point.x, ballPoint.point.y, 1, Drawings::ps_SolidLine,Drawings::gray);//找边缘点的扫描线,灰色扫描线

	return true;
}
bool BallSpecialist::findEndOfBall(const Vector2<int>&start,unsigned char ballColor,const Vector2<int>&step,int maxLength,BallPoint&ballPoint)
{
	ASSERT(step.x == 0 || step.x == 1 || step.x == -1);
	ASSERT(step.y == 0 || step.y == 1 || step.y == -1);

	Vector2<int> pos = start;
	Vector2<int> lastValidPos = pos;
	unsigned int overtime = 0;//该扫描线上

	int pixelCount = 1;

	ballPoint.step = step;
	ballPoint.atBorder = false;
	ballPoint.start = start;

	int maxColorDistance = BallParaments.scanMaxColorDistance;
	unsigned int maxOvertime = BallParaments.scanPixelTolerance;
	int resolutionWidth = IMAGE_WIDTH;
	int resolutionHeight = IMAGE_HEIGHT;
	int stepAbs1024 = (step.x == 0 || step.y == 0) ? 1024 : 1448; // 1448 = sqrt(2) * 1024
	int maxAbs1024 = int(maxLength * 1024.); // + 1024;
	int length1024 = 0;
	for(;;)
	{
		pos += step;
		length1024 += stepAbs1024;

		if(length1024 > maxAbs1024)
		{
			if(overtime > 0)
				break;
			DRAW_LINE(start.x, start.y, pos.x, pos.y, 1, Drawings::ps_SolidLine,Drawings::green);//错误的线
			return false;
		}

		if(pos.x < 0 || pos.x >= resolutionWidth ||
			pos.y < 0 || pos.y >= resolutionHeight)
		{
			ballPoint.atBorder = true;
			break;
		}

		unsigned char currrentColor = theTJImage->getClassifyedColor(pos.x,pos.y);

		if(currrentColor != ballColor)
		{
			if(overtime == 0)
				lastValidPos = pos - step;
			++overtime;
			if(overtime <= maxOvertime)
				continue;
			break;
		}

		++pixelCount;
		overtime = 0;
	}

	ballPoint.point = lastValidPos;

	if(pixelCount > 0)
	{
		--pixelCount;

		totalPixelCount += pixelCount;

	}

	DRAW_LINE(ballPoint.start.x, ballPoint.start.y, ballPoint.point.x, ballPoint.point.y, 1, Drawings::ps_SolidLine,Drawings::gray);//找边缘点的扫描线,灰色扫描线

	return true;
}
bool BallSpecialist::checkBallPoints()
{
	// find "valid" ball points
	validBallPoints = 0;
	static const int countOfBallPoints = sizeof(ballPoints) / sizeof(*ballPoints);
	for(int i = 0; i < countOfBallPoints; ++i)
	{
		BallPoint& ballPoint(ballPoints[i]);
		ballPoint.isValid = !ballPoint.atBorder && ballPoint.point != ballPoint.start;
		if(ballPoint.isValid)
		{
			++validBallPoints;
			DRAW_BIG_DOT(ballPoint.point.x, ballPoint.point.y,Drawings::black,Drawings::red);
		}
	}
	if(validBallPoints < 3)
		return false;

	// find duplicated ball points (may occur in small balls)
	for(int i = 0; i < countOfBallPoints; ++i)
	{
		BallPoint& ballPoint(ballPoints[i]);
		if(ballPoint.isValid)
		{
			const BallPoint& nextBallPoint(ballPoints[(i + 1) % countOfBallPoints]);
			if(nextBallPoint.isValid && ballPoint.point == nextBallPoint.point)
			{
				ballPoint.isValid = false;
				--validBallPoints;
			}
		}
	}
	if(validBallPoints < 3)
		return false;

	// drop mismatching ball points
	while(validBallPoints > 3)
	{
		Vector2<int> preCenter;
		double preRadius;
		if(!createBallPerceptLevenbergMarquardt(preCenter, preRadius))
			return false;

		double minDist = 0;
		BallPoint* minDistBallPoint = 0;
		for(int i = 0; i < countOfBallPoints; ++i)
		{
			BallPoint& ballPoint(ballPoints[i]);
			if(ballPoint.isValid)
			{
				double dist = (ballPoint.pointf - Vector2<double>(preCenter.x,preCenter.y)).squareAbs();
				if(!minDistBallPoint || dist < minDist)
				{
					minDist = dist;
					minDistBallPoint = &ballPoint;
				}
			}
		}
		minDistBallPoint->isValid = false;
		--validBallPoints;

		if((preRadius - (sqrt(minDist) + 2.0f)) / preRadius < 0.1f)
			break;
	}


		for(BallPoint* ballPoint = ballPoints, * end = ballPoints + sizeof(ballPoints) / sizeof(*ballPoints); ballPoint < end; ++ballPoint)
			if(ballPoint->isValid)
			{
				DRAW_BIG_DOT( ballPoint->point.x, ballPoint->point.y, Drawings::black, Drawings::magenta);
			}



	return true;
}
bool BallSpecialist::evaluateBallInImage()
{   //std::cout<<" evaluateBallInImage 1"<<std::endl;
	// check if the ball covers approxCenter2
	if((Vector2<int>(int(center.x + 0.5), int(center.y + 0.5)) - approxCenter2).squareAbs() > sqr(radius))
		return false;

	// check ball radius
	if(radius>=BallParaments.minRadiusthreshold&&(radius > approxRadius1 * BallParaments.checkMaxRadiusDifference || radius + BallParaments.checkMinRadiusPixelBonus < approxRadius1 * BallParaments.checkMinRadiusDifference))//有待考虑
		return false;

	//std::cout<<" evaluateBallInImage 2"<<std::endl;
	double noBallRadius = radius * BallParaments.checkOutlineRadiusScale + (radius<=10.0?BallParaments.checkOutlineRadiusPixelBonus+1.0:BallParaments.checkOutlineRadiusPixelBonus);

	DRAW_CIRCLE(center.x, center.y, noBallRadius, 1, Drawings::ps_SolidLine,Drawings::white);
	Vector2<int> center32(center.x*32, center.y * 32);
	int noBallRadius32 = int(noBallRadius * 32.0);
	int noBallDRadius32 = int(noBallRadius * 32.0/ 1.41421356);
	Vector2<int> noBallPoints32[8 + 4 * 4] = {
		Vector2<int>(center32.x + noBallRadius32, center32.y),
		Vector2<int>(center32.x - noBallRadius32, center32.y),
		Vector2<int>(center32.x, center32.y + noBallRadius32),
		Vector2<int>(center32.x, center32.y - noBallRadius32),
		Vector2<int>(center32.x + noBallDRadius32, center32.y + noBallDRadius32),
		Vector2<int>(center32.x - noBallDRadius32, center32.y - noBallDRadius32),
		Vector2<int>(center32.x + noBallDRadius32, center32.y - noBallDRadius32),
		Vector2<int>(center32.x - noBallDRadius32, center32.y + noBallDRadius32),
	};
	int noBallPointCount = 8;
	int resolutionWidth = IMAGE_WIDTH;
	int resolutionHeight = IMAGE_HEIGHT;
	int borderDists[2] = {0, 3};
	if(center32.x + noBallRadius32 >= resolutionWidth * 32)
		for(int i = 0; i < 2; ++i)
		{
			int x = resolutionWidth - 1 - borderDists[i];
			double d = sqrt(double(sqr(noBallRadius) - sqr(x - center.x)));
			noBallPoints32[noBallPointCount++] = Vector2<int>(x * 32, int((center.y + d) * 32.0));
			noBallPoints32[noBallPointCount++] = Vector2<int>(x * 32, int((center.y - d) * 32.0));
		}
		if(center32.y + noBallRadius32 >= resolutionHeight * 32)
			for(int i = 0; i < 2; ++i)
			{
				int y = resolutionHeight - 1 - borderDists[i];
				double d = sqrt(double(sqr(noBallRadius) - sqr(y - center.y)));
				noBallPoints32[noBallPointCount++] = Vector2<int>(int((center.x + d) * 32.0), y * 32);
				noBallPoints32[noBallPointCount++] = Vector2<int>(int((center.x - d) * 32.0), y * 32);
			}
			if(center32.x - noBallRadius32 < 0)
				for(int i = 0; i < 2; ++i)
				{
					int x = borderDists[i];
					double d = sqrt(double(sqr(noBallRadius) - sqr(x - center.x)));
					noBallPoints32[noBallPointCount++] = Vector2<int>(x * 32, int((center.y + d) * 32.0));
					noBallPoints32[noBallPointCount++] = Vector2<int>(x * 32, int((center.y - d) * 32.0));
				}
				if(center32.y - noBallRadius32 < 0)
					for(int i = 0; i < 2; ++i)
					{
						int y = borderDists[i];
						double d = sqrt(double(sqr(noBallRadius) - sqr(y - center.y)));
						noBallPoints32[noBallPointCount++] = Vector2<int>(int((center.x + d) * 32.0), y * 32);
						noBallPoints32[noBallPointCount++] = Vector2<int>(int((center.x - d) * 32.0), y * 32);
					}
					int maxColorDistance = BallParaments.scanMaxColorDistance;
					//int cbStart = startPixel.cb, crStart = startPixel.cr;
					int cbStart = totalCbSum / totalPixelCount, crStart = totalCrSum / totalPixelCount;
					for(int i = 0; i < noBallPointCount; ++i)
					{
						Vector2<int> pos(noBallPoints32[i] / 32);
						if(pos.x < 0 || pos.x >= resolutionWidth ||
							pos.y < 0 || pos.y >= resolutionHeight)
							continue;


						if(abs(theTJImage->unCorrectedRaw[pos.y][pos.x][1] - cbStart) + abs(theTJImage->unCorrectedRaw[pos.y][pos.x][2] - crStart) <= maxColorDistance)
						{
							DRAW_BIG_DOT(pos.x, pos.y,Drawings::red,Drawings::black);
							return false;
						}
					}

					return true;
}
bool BallSpecialist::calculateBallInImage()
{
    if (!createBallPerceptLevenbergMarquardt(center,radius))
         return false;
	return true;
}
bool BallSpecialist::calculateBallOnField()
{


	Vector2<double>centerAsAngles;
	Geometry::calculateAnglesForPoint(Vector2<int>(int(center.x + 0.5), int(center.y + 0.5)),
		*theCameraMatrix, centerAsAngles);
	double radiusAsAngle = Geometry::pixelSizeToAngleSize(radius);
	double ballRadius = BALL_DIAMETER/2;

	// Bearing based ball offset:
	Vector3<double> bbo; // bearing based
	const double distanceBearingBased = centerAsAngles.y < 0 ? (theCameraMatrix->translation.z - ballRadius) / tan(-centerAsAngles.y) : 10000;
	bbo.x = theCameraMatrix->translation.x + distanceBearingBased * cos(centerAsAngles.x);
	bbo.y =theCameraMatrix->translation.y + distanceBearingBased * sin(centerAsAngles.x);
	bbo.z =theCameraMatrix->translation.z;
	//修正参数
	if(bbo.x < 800)
        ;
	else if(bbo.x >= 800&&bbo.x <1500)
        {bbo.x=bbo.x +20;}
	else if(bbo.x >= 1500&&bbo.x < 1430)
		{bbo.x=bbo.x +50;}	
	else
        {bbo.x=bbo.x +80;}



	// Size based ball offset:
	Vector3<double> sbo;
	const double distanceFromCameraToBallCenter = Geometry::getBallDistanceByAngleSize(int(2.0 * ballRadius), 2.0 * radiusAsAngle);
	double distanceFromCameraToBallCenterOnGround;
	if(distanceFromCameraToBallCenter > fabs(theCameraMatrix->translation.z - ballRadius))
		distanceFromCameraToBallCenterOnGround = sqrt(sqr(distanceFromCameraToBallCenter) - sqr(theCameraMatrix->translation.z  - ballRadius));
	else
		distanceFromCameraToBallCenterOnGround = 0;
	sbo.x = theCameraMatrix->translation.x + distanceFromCameraToBallCenterOnGround * cos(centerAsAngles.x);
	sbo.y = theCameraMatrix->translation.y + distanceFromCameraToBallCenterOnGround * sin(centerAsAngles.x);
	sbo.z =theCameraMatrix->translation.z;

	const double sbDist(sbo.abs());
	const double bbDist(bbo.abs());

	//ATTENTION: These two const numbers is relation to the distance determination.
	//If the result is not very accurate,you need recalculate.
	//const double SIZE_BASED_DIST(1000.0);
	const double SIZE_BASED_DIST(800.0);
	const double BEARING_BASED_ONLY_DIST(400.0);
	const double DIST_COMPENSATED(2000.0);
	//In some cases, use size based computation only:
	if(!theCameraMatrix->isValid || centerAsAngles.y >= 0||bbDist>SIZE_BASED_DIST)
	{
		usedCenterOnField = bbo;
	}
	//At medium distance, interpolate between both offsets:
	else if(bbDist >= BEARING_BASED_ONLY_DIST)
	{
		//double factor = (SIZE_BASED_DIST - sbDist) / (SIZE_BASED_DIST - BEARING_BASED_ONLY_DIST);
		//usedCenterOnField = bbo * factor + sbo * (1.0 - factor);
		usedCenterOnField = bbo;
	}
	//In all remainung cases (ball is near, no other problems), use bearing based distance only:
	else
	{
	   usedCenterOnField = bbo;
	}

	/*Vector3<double> cameraToBall(FOCAL_LENGTH, IMAGE_WIDTH/2.0-center.x,IMAGE_HEIGHT - center.y);
	cameraToBall.normalize(BALL_RADIUS *FOCAL_LENGTH / radius);
	Vector3<double> rotatedCameraToBall = theCameraMatrix->rotation * cameraToBall;
	sizeBasedCenterOnField = theCameraMatrix->translation + rotatedCameraToBall;
	bearingBasedCenterOnField =  theCameraMatrix->translation - rotatedCameraToBall * ((theCameraMatrix->translation.z - BALL_RADIUS) / rotatedCameraToBall.z);

     usedCenterOnField = rotatedCameraToBall.z < 0 ? bearingBasedCenterOnField : sizeBasedCenterOnField;*/

	return true;

}
void BallSpecialist::outBallPercept()
{
#ifdef TJPIC
	sendBallResult();
#endif
}
void BallSpecialist::sendBallResult()
{
	netBall * netballvision;
	netballvision = new netBall();
	netballvision->ballSeen=theBallPercept->invion;
	netballvision->balldis=(int32)(100*theBallPercept->getdistanceOfBall());
	netballvision->ballx=(int32)(100*theBallPercept->relativePositionOnField.x);
	netballvision->bally=(int32)(100*theBallPercept->relativePositionOnField.y);
	selfMessageQueue->SetMessageData(idBallresult,idRobotThread,idNetworkThread,
		sizeof(netBall),(char*)netballvision);
	delete netballvision;
}
MAKE_KS(BallSpecialist)


