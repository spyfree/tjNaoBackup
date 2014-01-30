/*@ the class is used to detect Robot in field,
    the class is a KS
*/

#ifndef RobotSpecialist_H
#define RobotSpecialist_H

#include "Module/Vision/DebugDrawings.h"
#include "Tools/Math/Vector2.h"
#include "Module/Vision/TJImage.h"
#include "Module/Common/VisionHeader.h"
#include "Module/Common/Geometry.h"
#include "Module/Representations/KS.h"
#include "Module/Representations/Blackboard.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Representations/Perception/RobotPercept.h"
#include "Module/Representations/Perception/BlobPercept.h"
#include "Module/Vision/BresenhamLineScan.h"
#include "Tools/ColorClasses.h"
#include "Tools/Boundary.h"
#include <vector>
#include "Tools/Math/Common.h"
#include <iostream>
#include "Platform/GetSetting.h"

DEFINE_KS(RobotSpecialist)
REQUIRES(BlobPercept)
REQUIRES(TJImage)
REQUIRES(CameraMatrix)
PROVIDES(RobotPercept)
USES(RobotPercept)
END_KS

class RobotSpecialist:public RobotSpecialistBase,BaseProvider
{
public:  RobotSpecialist();
	 ~RobotSpecialist(){}

class Parameters{
public:
	int teamMarkerRegionMinSize;
	Vector2<double> eccentricityBoundaries;
	double aboveWhiteRatio;
	double belowWhiteRatio;
	Vector2<int> axisND;
	double greenGroundRatio;
	/** The distance in pixels between exploring scanlines. */
	int exploreStepSize;
	/** The distance in pixels between neighboring scan lines. */
	int gridStepSize;
	/** The maximum number of pixels to skip when grouping pixels to segments. */
	int skipOffset;
	/** The minimal size/length for a segment in pixels for each color. */
	//int minSegSize[ColorClasses::numOfColors];
	;

};

typedef std::vector<Vector2<int> >::const_iterator V2CIt;
Parameters params;

/** Updates the RobotPercept. */
void update(RobotPercept*theRobotPercept);
void inBlobPercept(){}
void inTJImage(){}
void inCameraMatrix(){}
void outRobotPercept(){}
/**
* Prepares the robotPercept for this cycle.
*/
private:
void prepareRobotPercept(RobotPercept& robotPercept);
bool calLocPosition(RobotPercept::Robot& robot ,const CameraMatrix& cameraMatrix);
double areaMembership(int area);
double eccentricityMembership(double eccentricity);
double colorRatio(const TJImage&theImage,const unsigned char & col, const Boundary<int>& area, int stepSize);
void principleAxisLength(double& len_min, double& len_max, const BlobPercept::Blob* region, const Vector2<int>& cog, const Vector2<double>& achse1, const Vector2<double>& achse2);
int scanline(
			 const TJImage& theImage,
			 const unsigned char teamColor,
			 const Vector2<int>& start,
			 Vector2<int>& end,
			 int& whitePixels)
{
	BresenhamLineScan scan = BresenhamLineScan(start, end);
	scan.init();
	end = start;
	for(int i = 0; i < scan.numberOfPixels; i++)
	{
		scan.getNext(end);
		if(end.x >= 0 && end.x < IMAGE_WIDTH
			&& end.y >= 0 && end.y < IMAGE_HEIGHT)
		{
			if(theImage.getClassifyedColor(end.x, end.y) == WHITE)
			{
				whitePixels++;
				continue;
			}
			else
			{   //更独立于色表，当机器人身上的白色标的不好时
				unsigned char y = theImage.unCorrectedRaw[end.y][end.x][0];
				unsigned char cb = theImage.unCorrectedRaw[end.y][end.x][1];
				unsigned char cr = theImage.unCorrectedRaw[end.y][end.x][2];
				if(y > 100 && y>cb-10 && y>cr-10)//if(y > 100 && y > cb && y > cr)
					whitePixels++;
			}
		}
		else
		{
			return i;
		}
	}
	return scan.numberOfPixels;
}
inline bool exactLocalisation(const Vector2<int>& p)
{
	return IMAGE_HEIGHT != p.y;
}




};

#endif