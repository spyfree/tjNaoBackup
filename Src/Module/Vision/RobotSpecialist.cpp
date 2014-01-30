#include "RobotSpecialist.h"
#include <algorithm>
//Constructor
RobotSpecialist::RobotSpecialist()
{
	params.teamMarkerRegionMinSize=48;
	params.eccentricityBoundaries=Vector2<double>(2.0,13.0);
	params.aboveWhiteRatio=0.6;
	params.belowWhiteRatio=0.6;
	params.axisND=Vector2<int>(2,5);
	params.greenGroundRatio=0.8;
	params.exploreStepSize=3;
	params.gridStepSize=2;
	params.skipOffset=2;
}

//update funtion
void RobotSpecialist::update(RobotPercept* theRobotPercept)
{
  prepareRobotPercept(*theRobotPercept);
  ASSERT(theRobotPercept->robots.empty());
  ASSERT(theRobotPercept->unlocalizableRobots.empty());
  for(const BlobPercept::Blob * start=theBlobPercept->regions,*end=theBlobPercept->regions+theBlobPercept->regionsCounter;start<end;++start)
  {
    switch(start->color)
	{
	case::RED:
	case::CYAN:
		{
		  double areaTruth=areaMembership(start->size);
	      if(areaTruth<0.0f)
			  break;
          //利用图像矩		 
		  const int m00 = start->calcMoment00(),
			  m10 = start->calcMoment10(),
			  m01 = start->calcMoment01();
		  if(m00 == 0.0f)
			  break;

		  const Vector2<int> cog(m10/m00, m01/m00);
		  DRAW_BIG_DOT(cog.x, cog.y,Drawings::black, Drawings::black);

		  const double m20 = (double) start->calcCMoment20(cog.x),//相对于x 轴
			  m02 = (double) start->calcCMoment02(cog.y),//相对于y轴
			  m11 = (double) start->calcCMoment11(cog.x, cog.y);
		  const double cm20 = m20/m00,
			  cm02 = m02/m00,
			  cm11 = m11/m00;
		  if(cm20 == cm02)
			  break;

		  const double cmsum = cm20 + cm02;
		  const double cmdiff = cm20 - cm02;
		  const double eccroot = sqrt(cmdiff*cmdiff + 4.0 * cm11*cm11);
		  const double eccentricity = (cmsum + eccroot) / (cmsum - eccroot);//可以刻画区域的伸长程度
		  double eccentricityTruth = eccentricityMembership(eccentricity);
		  if(eccentricityTruth < 0.0)
			  break;
		  const double alpha = 0.5 * atan2(-2.0 * m11, m02 - m20) + pi_2;
		  double len_max = 0.0, len_min = 0.0;
		  principleAxisLength(len_min, len_max, start, cog,
			  Vector2<double>(cos(alpha), sin(alpha)),
			  Vector2<double>(cos(alpha + pi_2), sin(alpha + pi_2)));
		  const double sinAlpha = sin(alpha);
		  const double cosAlpha = cos(alpha);
		  const double lenDiff = len_max - len_min;
 
		  //用来找一个主轴上加的基准点
		  Vector2<int> axis = Vector2<int>((int) (cosAlpha * lenDiff), (int) (sinAlpha * lenDiff)) * params.axisND.x / params.axisND.y;
		  if(axis.x < 0) axis = axis * (-1);
		  Vector2<int> left = cog - axis;
		  Vector2<int> right = cog + axis;
		  //垂直于主轴上加的基准点
		  Vector2<int> perpendicular = Matrix2x2<int>(0, -1, 1, 0) * axis;
         
		  //向上扫
		  Vector2<int> slLeftUp = left - perpendicular * 5;
		  Vector2<int> slMiddleUp = cog - perpendicular * 5;
		  Vector2<int> slRightUp = right - perpendicular * 5;
		  int whitePixelsAbove = 0;
		  int totalPixelsAbove = 0;
		  totalPixelsAbove += scanline(*theTJImage, start->color, left, slLeftUp, whitePixelsAbove);
		  totalPixelsAbove += scanline(*theTJImage,  start->color, cog, slMiddleUp, whitePixelsAbove);
		  totalPixelsAbove += scanline(*theTJImage,  start->color, right, slRightUp, whitePixelsAbove);
		  double aboveWhiteRatio = (double) whitePixelsAbove / (double) totalPixelsAbove;
		  double aboveTruth(0.0);
		  if(aboveWhiteRatio > 0.5 && aboveWhiteRatio < 0.6)
			  aboveTruth =(aboveWhiteRatio - 0.5) / 0.1;
		  else if(aboveWhiteRatio >= 0.6)
			  aboveTruth = 1.0;
		  ASSERT(aboveTruth >= 0.0);
		  ASSERT(aboveTruth <= 1.0);
          //向下扫
		  Vector2<int> slLeftDown = left + perpendicular * 5;
		  Vector2<int> slMiddleDown = cog + perpendicular * 1;
		  Vector2<int> slRightDown = right + perpendicular * 5;
		  int whitePixelsBelow = 0;
		  int totalPixelsBelow = 0;
		  totalPixelsBelow += scanline(*theTJImage,  start->color, left, slLeftDown, whitePixelsBelow);
		  totalPixelsBelow += scanline(*theTJImage,  start->color, cog, slMiddleDown, whitePixelsBelow);
		  totalPixelsBelow += scanline(*theTJImage,  start->color, right, slRightDown, whitePixelsBelow);
		  double belowWhiteRatio = (double) whitePixelsBelow / (double) totalPixelsBelow;
		  double belowTruth(0.0);
		  if(belowWhiteRatio > 0.4 && belowWhiteRatio < 0.5)
			  belowTruth =(belowWhiteRatio - 0.4) / 0.1;
		  else if(belowWhiteRatio >= 0.5)
			  belowTruth = 1.0;
		  ASSERT(belowTruth >= 0.0);
		  ASSERT(belowTruth <= 1.0);
		  DRAW_LINE(slLeftDown.x, slLeftDown.y, slLeftUp.x, slLeftUp.y, 1, Drawings::ps_SolidLine, Drawings::white);
		  DRAW_LINE( slMiddleDown.x, slMiddleDown.y, slMiddleUp.x, slMiddleUp.y,1, Drawings::ps_SolidLine, Drawings::white);
		  DRAW_LINE( slRightDown.x, slRightDown.y, slRightUp.x, slRightUp.y, 1, Drawings::ps_SolidLine, Drawings::white);
		  if(areaTruth * eccentricityTruth * aboveTruth * belowTruth < 0.5)
			  break;

		  Vector2<int> lowestPx(cog.x, cog.y);
		  while(lowestPx.y < IMAGE_HEIGHT
			  && (theTJImage->getClassifyedColor(lowestPx.x, lowestPx.y)!=FIELD
			  || colorRatio(*theTJImage,FIELD,Boundary<int>(Range<int>(int(lowestPx.x - len_max), int(lowestPx.x + len_max)),
			  Range<int>(lowestPx.y, lowestPx.y + 2)), 2) < params.greenGroundRatio))
		  {
			  lowestPx.y++;
		  }

		  if(((alpha >= 0.5 && alpha <= 1.2) || (alpha >= 2.0 && alpha <= 2.5)))
			  break;
		  bool standing = ((alpha >= 0.0 && alpha <= 0.5) || (alpha >= pi - 0.5 && alpha <= pi)) ? true : false;

		  int pixelHeight = lowestPx.y - cog.y;
		  // this is too high to be a horizontal robot (e. g. a goal post)
		  if(!standing && pixelHeight > 100) // TODO params
			  break;
		  // small high team markers are not relevant
		  if(start->size < 100 && pixelHeight > 100) // TODO params
			  break;
		 //draw debug

			 
		  const int axisLength = 30;
			  DRAW_CIRCLE(cog.x, cog.y, axisLength,1, Drawings::ps_SolidLine, Drawings::white);
			  DRAW_LINE(cog.x, cog.y, cog.x + axisLength, cog.y, 1, Drawings::ps_SolidLine,Drawings::white);
			  Vector2<double> normalizedAxis = Vector2<double>(cos(0.5), sin(0.5)) *  (double)(axisLength);
			  DRAW_LINE( cog.x, cog.y, cog.x + (int) normalizedAxis.x, cog.y + (int) normalizedAxis.y,
				  1, Drawings::ps_SolidLine, Drawings::white);
			  normalizedAxis = Vector2<double>(cos(pi-0.5), sin(pi-0.5)) * (double) axisLength;
			  DRAW_LINE( cog.x, cog.y,cog.x + (int) normalizedAxis.x, cog.y + (int) normalizedAxis.y,
				  1, Drawings::ps_SolidLine, Drawings::white);
			  normalizedAxis = Vector2<double>(cos(pi), sin(pi)) * (double) axisLength;
			  DRAW_LINE(cog.x, cog.y,  cog.x + (int) normalizedAxis.x, cog.y + (int) normalizedAxis.y,
				  1, Drawings::ps_SolidLine, Drawings::white);
			  normalizedAxis = Vector2<double>(cosAlpha, sinAlpha) * (double) (axisLength/2);
			  DRAW_LINE(cog.x, cog.y, cog.x + (int) normalizedAxis.x, cog.y + (int) normalizedAxis.y,
				  1, Drawings::ps_SolidLine, start->color==CYAN?Drawings::darkCyan:Drawings::red);
	

		       
			  Vector2<int> p1 = Vector2<int>((int)(cog.x + cosAlpha * len_max), (int)(cog.y + sinAlpha * len_max));
			  
			  DRAW_LINE( cog.x, cog.y, p1.x, p1.y,  2, Drawings::ps_SolidLine, start->color==CYAN?Drawings::darkCyan:Drawings::red);
              DRAW_CROSS(lowestPx.x, lowestPx.y, 2, 1, Drawings::ps_SolidLine, Drawings::green);

		  
		  if(exactLocalisation(lowestPx))
		  {
			  RobotPercept::Robot robot(lowestPx,cog, standing, start->color);
			  if (calLocPosition(robot,*theCameraMatrix))
				  theRobotPercept->robots.push_back(robot);
		  }
		  else
		  {
			  theRobotPercept->unlocalizableRobots.push_back(RobotPercept::Robot(lowestPx,cog, true, start->color));
		  }

		  break;

	}
	default:
		break;
		}//end switch
  }//end for
  //std::cout<<" ****after robotspecialist****    "<<std::endl;
  for(std::vector<RobotPercept::Robot>::iterator iter=theRobotPercept->robots.begin();iter!=theRobotPercept->robots.end();iter++)
  {
	  std::cout<<"robot position in field "<<iter->relativePositionOnField.x<<" "<<iter->relativePositionOnField.y<<std::endl;
  }
}//end update
void RobotSpecialist::prepareRobotPercept(RobotPercept& robotPercept)
{
	robotPercept.robots.clear();
	robotPercept.unlocalizableRobots.clear();
}
double RobotSpecialist::areaMembership(int area)
{
	double areaTruth(0.0);
	if(area <= params.teamMarkerRegionMinSize)
		areaTruth = -1.0;
	else if(area <= 500)
		areaTruth = 1.0;
	else if(area <= 1500)
		areaTruth = (1500.0 - (double) area) / 1000.0;
	ASSERT(areaTruth >= 0.0 || areaTruth == -1.0);
	ASSERT(areaTruth <= 1.0 || areaTruth == -1.0);
	return areaTruth;
}
double RobotSpecialist::eccentricityMembership(double eccentricity)
{
	double eccentricityTruth(0.0);
	// ensures that the eccentricity is within the boundaries and not inf or NaN
	if(!(eccentricity > 1.5 && eccentricity < params.eccentricityBoundaries.y))
		eccentricityTruth = -1.0;
	if(eccentricity >= 1.5 && eccentricity <= params.eccentricityBoundaries.x)
		eccentricityTruth = (2.0- eccentricity) / 0.5;
	if(eccentricity >= 2.0 && eccentricity < 12.5)
		eccentricityTruth = 1.0;
	else if(eccentricity >= 12.5 && eccentricity <= params.eccentricityBoundaries.y)
		eccentricityTruth = (eccentricity - 12.5) / 0.5;
	ASSERT(eccentricityTruth >= 0.0 || eccentricityTruth == -1.0);
	ASSERT(eccentricityTruth <= 1.0 || eccentricityTruth == -1.0);
	return eccentricityTruth;
}
//caculate the length of the main axis
void RobotSpecialist::principleAxisLength(double& len_min, double& len_max, const BlobPercept::Blob * region, const Vector2<int>& cog, const Vector2<double>& achse1, const Vector2<double>& achse2)
{
	double len_max2 = 0, len_min2 = 0;
	typedef std::vector<BlobPercept::Segment*>::const_iterator CSI;
	for(CSI child = region->childs.begin(); child != region->childs.end(); child++)
	{
		const BlobPercept::Segment* seg = *child;
		const Vector2<double> childRelToSwp(double(seg->x - cog.x), double(seg->y - cog.y)),
			child2RelToSwp(double(seg->x - cog.x), double(seg->y + seg->length - cog.y));

		const double dist = childRelToSwp * achse1;
		if(dist > len_max)
			len_max = dist;
		else if(dist < len_min)
			len_min = dist;

		const double dist2 = child2RelToSwp * achse1;
		if(dist2 > len_max)
			len_max = dist2;
		else if(dist2 < len_min)
			len_min = dist2;

		const double dist3 = childRelToSwp * achse2;
		if(dist3 > len_max2)
			len_max2 = dist3;
		else if(dist3 < len_min2)
			len_min2 = dist3;

		const double dist4 = child2RelToSwp * achse2;
		if(dist4 > len_max2)
			len_max2 = dist4;
		else if(dist4 < len_min2)
			len_min2 = dist4;
	}
}
double RobotSpecialist::colorRatio(const TJImage&theImage,const unsigned char& col, const Boundary<int>& area, int stepSize)
{
	const double numberOfPixels(double((area.x.max + 1 - area.x.min) * (area.y.max + 1 - area.y.min)));
	if (numberOfPixels == 0.0)
	{
		return 0.0;
	}
	const int stepSize2 = stepSize*stepSize;
	int sameColor(0);
	const int xStart=area.x.min > 0? area.x.min:0;
	const int xEnd=area.x.max <IMAGE_WIDTH-1?area.x.max:IMAGE_WIDTH-1;
	const int yStart=area.y.min >0?area.y.min:0;
	const int yEnd=area.y.max<IMAGE_HEIGHT-1?area.y.max:IMAGE_HEIGHT-1;

	const int lastColStepSize = (xEnd - xStart) % stepSize + 1;
	const int lastColStepSize2 = lastColStepSize * stepSize;
	const int lastRowStepSize = (yEnd - yStart) % stepSize + 1;
	const int lastRowStepSize2 = lastRowStepSize * stepSize;
	for(int x(xStart); x <= xEnd; x+=stepSize)
	{
		bool lastCol = x+stepSize > xEnd;
		for(int y(yStart); y <= yEnd; y+=stepSize)
		{
			bool lastRow = y+stepSize > yEnd;
			if(theImage.getClassifyedColor(x, y) == col)
			{
				sameColor += lastCol ? (lastRow ? lastColStepSize * lastRowStepSize : lastColStepSize2) : (lastRow ? lastRowStepSize2 : stepSize2);
				//DOT("module:PointExplorer:blob", x, y, col, col);
			}
		}
	}
	return static_cast<double>(sameColor) / numberOfPixels;
}
bool RobotSpecialist::calLocPosition(RobotPercept::Robot& robot ,const CameraMatrix& cameraMatrix)
{
	double x = robot.middlePx.x;
	double y = robot.middlePx.y;
	double z = robot.robotStandHeight;
	return Geometry::calculatePointOnH(x,y,z,*theCameraMatrix,robot.relativePositionOnField);

}

MAKE_KS(RobotSpecialist)