/**
* @file FieldDimensions.cpp
*
* Some useful functions regarding field dimensions.
*
* @author Max Risler
*/

#include "FieldDimensions.h"
//#include <iostream>
#include <limits>

FieldDimensions::FieldDimensions()
: Boundary<double>(0,0)
{
	load();
}

void FieldDimensions::load()
{
  //int* const valuesInt[]={
  //  &xPosOpponentFieldBorder, &xPosOpponentGoal, &xPosOpponentGoalpost,
  //  &xPosOpponentGroundline, &xPosOpponentSideCorner, &xPosOpponentPenaltyArea, &xPosHalfWayLine,
  //  &xPosOwnPenaltyArea, &xPosOwnSideCorner, &xPosOwnGroundline, &xPosOwnGoalpost,
  //  &xPosOwnGoal, &xPosOwnFieldBorder,
  //  &yPosLeftFieldBorder, &yPosLeftSideline, &yPosLeftGroundline,
  //  &yPosLeftPenaltyArea, &yPosLeftGoal, &yPosCenterGoal, &yPosRightGoal,
  //  &yPosRightPenaltyArea, &yPosRightGroundline, &yPosRightSideline, &yPosRightFieldBorder,
  //  &centerCircleRadius, &goalHeight, &fieldLinesWidth, &goalPostRadius, 
  //  &xPosThrowInPointOpponentHalf, &xPosThrowInPointCenter, 
  //  &xPosThrowInPointOwnHalf, &ballRadius, &ballFriction
  //};
  //const char* const namesInt[]={
  //  "xPosOpponentFieldBorder", "xPosOpponentGoal", "xPosOpponentGoalpost",
  //  "xPosOpponentGroundline", "xPosOpponentSideCorner", "xPosOpponentPenaltyArea", "xPosHalfWayLine",
  //  "xPosOwnPenaltyArea", "xPosOwnSideCorner", "xPosOwnGroundline", "xPosOwnGoalpost",
  //  "xPosOwnGoal", "xPosOwnFieldBorder",
  //  "yPosLeftFieldBorder", "yPosLeftSideline", "yPosLeftGroundline",
  //  "yPosLeftPenaltyArea", "yPosLeftGoal", "yPosCenterGoal", "yPosRightGoal",
  //  "yPosRightPenaltyArea", "yPosRightGroundline", "yPosRightSideline", "yPosRightFieldBorder",
  //  "centerCircleRadius",
  //  "goalHeight", "fieldLinesWidth", "goalPostRadius",
  //  "xPosThrowInPointOpponentHalf", "xPosThrowInPointCenter", 
  //  "xPosThrowInPointOwnHalf", "ballRadius", "ballFriction"
  //};

  //const int numOfValuesInt = sizeof(valuesInt)/sizeof(int*);
  //ASSERT(sizeof(namesInt)/sizeof(char*) == numOfValuesInt);

  //bool initializedInt[numOfValuesInt];

  //for (int i = 0; i < numOfValuesInt; initializedInt[i++] = false);

  readDimensions();
  readLines();
  readCorners();

  add(Vector2<double>(xPosOpponentFieldBorder,yPosLeftFieldBorder));
  add(Vector2<double>(xPosOwnFieldBorder,yPosRightFieldBorder));
}

void FieldDimensions::readDimensions( )
{
	    xPosOpponentFieldBorder = 3700;
		xPosOpponentGoal = 3500;
		xPosOpponentGoalpost = 3050;
		xPosOpponentGroundline = 3000;
		xPosOpponentSideCorner = 3000;
		xPosOpponentPenaltyArea = 2400;
		xPosHalfWayLine = 0;
		xPosOwnPenaltyArea = -xPosOpponentPenaltyArea;
		xPosOwnSideCorner = -xPosOpponentSideCorner;
		xPosOwnGroundline = -xPosOpponentGroundline;
		xPosOwnGoalpost = -xPosOpponentGoalpost;
		xPosOwnGoal = -xPosOpponentGoal;
		xPosOwnFieldBorder = -xPosOpponentFieldBorder;

		yPosLeftFieldBorder = 2700;
		yPosLeftSideline = 2000;
		yPosLeftGroundline = 2000;
		yPosLeftPenaltyArea = 1100;
		yPosLeftGoal = 750;
		yPosCenterGoal = 0;
		yPosRightGoal = -yPosLeftGoal;
		yPosRightPenaltyArea = -yPosLeftPenaltyArea;
		yPosRightGroundline = -yPosLeftGroundline;
		yPosRightSideline = -yPosLeftSideline;
		yPosRightFieldBorder = -yPosLeftFieldBorder;

		//throw-in points
		xPosThrowInPointOpponentHalf = 1200;
		xPosThrowInPointCenter = 0;
		xPosThrowInPointOwnHalf = -xPosThrowInPointOpponentHalf;

		//other dimensions
		fieldLinesWidth = 50;
		goalPostRadius = 50;
		centerCircleRadius = 600;
		goalHeight = 800;
		ballRadius = 33;//new hockey ball
		ballFriction = 200;
}

void FieldDimensions::readLines()
{
  LinesTable* linesTables[] = {&carpetBorder, &fieldBorder, &fieldLines,&ownFieldBorder};

        //hu: carpetBorder
        Vector2<double> p1,p2;
		p1.x = xPosOpponentFieldBorder;
		p1.y = yPosRightFieldBorder;
		p2.x = xPosOpponentFieldBorder;
		p2.y = yPosLeftFieldBorder;
		linesTables[0]->push(p1,p2);
		p1.x = xPosOpponentFieldBorder;
		p1.y = yPosLeftFieldBorder;
		p2.x = xPosOwnFieldBorder;
		p2.y = yPosLeftFieldBorder;
		linesTables[0]->push(p1,p2);
		p1.x = xPosOwnFieldBorder;
		p1.y = yPosLeftFieldBorder;
		p2.x = xPosOwnFieldBorder;
		p2.y = yPosRightFieldBorder;
		linesTables[0]->push(p1,p2);
		p1.x = xPosOwnFieldBorder;
		p1.y = yPosRightFieldBorder;
		p2.x = xPosOpponentFieldBorder;
		p2.y = yPosRightFieldBorder;
		linesTables[0]->push(p1,p2);

		//hu: fieldBorder
		p1.x = xPosOpponentGroundline;
		p1.y = yPosRightSideline;
		p2.x = xPosOpponentGroundline; 
		p2.y = yPosLeftSideline; 
		linesTables[1]->push(p1,p2);
		p1.x = xPosOpponentGroundline; 
		p1.y = yPosLeftSideline; 
		p2.x = xPosOwnGroundline; 
		p2.y = yPosLeftSideline; 
		linesTables[1]->push(p1,p2);
		p1.x = xPosOwnGroundline; 
		p1.y = yPosLeftSideline;
		p2.x = xPosOwnGroundline; 
		p2.y = yPosRightSideline; 
		linesTables[1]->push(p1,p2);
        p1.x = xPosOwnGroundline; 
		p1.y = yPosRightSideline;
		p2.x = xPosOpponentGroundline; 
		p2.y = yPosRightSideline;
		linesTables[1]->push(p1,p2);

		//hu: fieldLines
		// field border lines
		p1.x = xPosOpponentGroundline; 
		p1.y = yPosRightSideline;
		p2.x = xPosOpponentGroundline; 
		p2.y = yPosLeftSideline;
		linesTables[2]->push(p1,p2);
		p1.x = xPosOpponentGroundline; 
		p1.y = yPosLeftSideline;
		p2.x = xPosOwnGroundline; 
		p2.y = yPosLeftSideline;
		linesTables[2]->push(p1,p2);
		p1.x = xPosOwnGroundline; 
		p1.y = yPosLeftSideline;
		p2.x = xPosOwnGroundline; 
		p2.y = yPosRightSideline;
		linesTables[2]->push(p1,p2);
		p1.x = xPosOwnGroundline; 
		p1.y = yPosRightSideline;
		p2.x = xPosOpponentGroundline; 
		p2.y = yPosRightSideline;
		linesTables[2]->push(p1,p2);
		// center line
		p1.x = xPosHalfWayLine; 
		p1.y = yPosLeftSideline;
		p2.x = xPosHalfWayLine; 
		p2.y = yPosRightSideline;
		linesTables[2]->push(p1,p2);//4
		//penalty areas
		p1.x = xPosOwnGroundline; 
		p1.y = yPosLeftPenaltyArea; 
		p2.x = xPosOwnPenaltyArea; 
		p2.y = yPosLeftPenaltyArea;
		linesTables[2]->push(p1,p2);//5
		p1.x = xPosOwnPenaltyArea; 
		p1.y = yPosLeftPenaltyArea;
		p2.x = xPosOwnPenaltyArea; 
		p2.y = yPosRightPenaltyArea;
		linesTables[2]->push(p1,p2);
		p1.x = xPosOwnPenaltyArea; 
		p1.y = yPosRightPenaltyArea;
		p2.x = xPosOwnGroundline; 
		p2.y = yPosRightPenaltyArea;
		linesTables[2]->push(p1,p2);

		p1.x = xPosOpponentGroundline; 
		p1.y = yPosLeftPenaltyArea;
		p2.x = xPosOpponentPenaltyArea; 
		p2.y = yPosLeftPenaltyArea;
		linesTables[2]->push(p1,p2);//8
		p1.x = xPosOpponentPenaltyArea; 
		p1.y = yPosLeftPenaltyArea;
		p2.x = xPosOpponentPenaltyArea; 
		p2.y = yPosRightPenaltyArea;
		linesTables[2]->push(p1,p2);
		p1.x = xPosOpponentPenaltyArea; 
		p1.y = yPosRightPenaltyArea;
		p2.x = xPosOpponentGroundline; 
		p2.y = yPosRightPenaltyArea;
		linesTables[2]->push(p1,p2);//10
		//throw-in lines
		p1.x = 1150;
		p1.y = 0; 
		p2.x = 1250; 
		p2.y = 0;
		linesTables[2]->push(p1,p2);
		p1.x = xPosThrowInPointOpponentHalf; //11
		p1.y = -50;
		p2.x = xPosThrowInPointOpponentHalf; 
		p2.y = 50;
		linesTables[2]->push(p1,p2);

		p1.x = -1150; 
		p1.y = 0;
		p2.x = -1250; 
		p2.y = 0;
		linesTables[2]->push(p1,p2);//13
		p1.x = xPosThrowInPointOwnHalf; 
		p1.y = -50;
		p2.x = xPosThrowInPointOwnHalf; 
		p2.y = 50;
		linesTables[2]->push(p1,p2);

		p1.x = -50; 
		p1.y = 0;
		p2.x = 50; 
		p2.y = 0;
		linesTables[2]->push(p1,p2);//15

	    //hu: circle
        Vector2<double> center;
        double radius;
        int numOfSegments;
        center.x = 0;
        center.y = 0;
        radius = centerCircleRadius;
        numOfSegments = 16;//31
        linesTables[2]->pushCircle(center, radius, numOfSegments);


		p1.x = 0; 
		p1.y = yPosLeftSideline; 
		p2.x = xPosOwnGroundline; 
		p2.y = yPosLeftSideline; 
		linesTables[3]->push(p1,p2);
		p1.x = xPosOwnGroundline; 
		p1.y = yPosLeftSideline;
		p2.x = xPosOwnGroundline; 
		p2.y = yPosRightSideline; 
		linesTables[3]->push(p1,p2);
		p1.x = 0; 
		p1.y = yPosLeftSideline;
		p2.x = 0; 
		p2.y = yPosRightSideline; 
		linesTables[3]->push(p1,p2);
		p1.x = xPosOwnGroundline; 
		p1.y = yPosRightSideline;
		p2.x = 0; 
		p2.y = yPosRightSideline;
		linesTables[3]->push(p1,p2);
 
}

void FieldDimensions::readCorners()
{

      Vector2<int> corner;
	  //xCorner
      corner.x = xPosHalfWayLine;
      corner.y = centerCircleRadius;
      corners[0].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = -centerCircleRadius;
	  corners[0].push_back(corner);
	  //tCorner0
	  corner.x = xPosHalfWayLine; 
	  corner.y = centerCircleRadius;
	  corners[1].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = -centerCircleRadius;
	  corners[1].push_back(corner);
	  corner.x = xPosOwnGroundline; 
	  corner.y = yPosLeftPenaltyArea;
	  corners[1].push_back(corner);
	  corner.x = xPosOwnGroundline; 
	  corner.y = yPosRightPenaltyArea;
	  corners[1].push_back(corner);
	  //tCorner90
	  corner.x = xPosHalfWayLine; 
	  corner.y = centerCircleRadius;
	  corners[2].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = -centerCircleRadius;
	  corners[2].push_back(corner);
	  corner.x = xPosHalfWayLine;
	  corner.y = yPosRightSideline;
      corners[2].push_back(corner);
	  //tCorner180
	  corner.x = xPosHalfWayLine; 
	  corner.y = centerCircleRadius;
	  corners[3].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = -centerCircleRadius;
	  corners[3].push_back(corner);
	  corner.x = xPosOpponentGroundline; 
	  corner.y = yPosLeftPenaltyArea;
	  corners[3].push_back(corner);
	  corner.x = xPosOpponentGroundline; 
	  corner.y = yPosRightPenaltyArea;
	  corners[3].push_back(corner);
	  //tCorner270
	  corner.x = xPosHalfWayLine; 
	  corner.y = centerCircleRadius;
	  corners[4].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = -centerCircleRadius;
	  corners[4].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = yPosLeftSideline;
	  corners[4].push_back(corner);
	  //lCorner0
	  corner.x = xPosHalfWayLine; 
	  corner.y = centerCircleRadius;
	  corners[5].push_back(corner);
	  corner.x = xPosHalfWayLine;
	  corner.y = -centerCircleRadius;
	  corners[5].push_back(corner);
	  corner.x = xPosOwnGroundline; 
	  corner.y = yPosLeftPenaltyArea;
	  corners[5].push_back(corner);
	  corner.x = xPosOwnGroundline; 
	  corner.y = yPosRightPenaltyArea;
	  corners[5].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = yPosRightSideline;
	  corners[5].push_back(corner);
	  corner.x = xPosOwnGroundline; 
	  corner.y = yPosRightSideline;
	  corners[5].push_back(corner);
	  corner.x = xPosOpponentPenaltyArea; 
	  corner.y = yPosRightPenaltyArea;
	  corners[5].push_back(corner);
	  //lCorner90
	  corner.x = xPosHalfWayLine; 
	  corner.y = centerCircleRadius;
	  corners[6].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = -centerCircleRadius;
	  corners[6].push_back(corner);
	  corner.x = xPosOpponentGroundline; 
	  corner.y = yPosLeftPenaltyArea;
	  corners[6].push_back(corner);
	  corner.x = xPosOpponentGroundline; 
	  corner.y = yPosRightPenaltyArea;
	  corners[6].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = yPosRightSideline;
	  corners[6].push_back(corner);
	  corner.x = xPosOpponentGroundline; 
	  corner.y = yPosRightSideline;
	  corners[6].push_back(corner);
	  corner.x = xPosOwnPenaltyArea; 
	  corner.y = yPosRightPenaltyArea;
	  corners[6].push_back(corner);
	  //lCorner180
	  corner.x = xPosHalfWayLine; 
	  corner.y = centerCircleRadius;
	  corners[7].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = -centerCircleRadius;
	  corners[7].push_back(corner);
	  corner.x = xPosOpponentGroundline; 
	  corner.y = yPosLeftPenaltyArea;
	  corners[7].push_back(corner);
	  corner.x = xPosOpponentGroundline; 
	  corner.y = yPosRightPenaltyArea;
	  corners[7].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = yPosLeftSideline;
	  corners[7].push_back(corner);
	  corner.x = xPosOpponentGroundline; 
	  corner.y = yPosLeftSideline;
	  corners[7].push_back(corner);
	  corner.x = xPosOwnPenaltyArea; 
	  corner.y = yPosLeftPenaltyArea;
	  corners[7].push_back(corner);
	  //lCorner270
	  corner.x = xPosHalfWayLine; 
	  corner.y = centerCircleRadius;
	  corners[8].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = -centerCircleRadius;
	  corners[8].push_back(corner);
	  corner.x = xPosOwnGroundline; 
	  corner.y = yPosLeftPenaltyArea;
	  corners[8].push_back(corner);
	  corner.x = xPosOwnGroundline; 
	  corner.y = yPosRightPenaltyArea;
	  corners[8].push_back(corner);
	  corner.x = xPosHalfWayLine; 
	  corner.y = yPosLeftSideline;
	  corners[8].push_back(corner);
	  corner.x = xPosOwnGroundline; 
	  corner.y = yPosLeftSideline;
	  corners[8].push_back(corner);
	  corner.x = xPosOpponentPenaltyArea; 
	  corner.y = yPosLeftPenaltyArea;
	  corners[8].push_back(corner);
}

Pose2D FieldDimensions::randomPoseOnField() const
{ 
  Pose2D pose;
  do
    pose = Pose2D::random(x,y,Range<double>(-pi,pi));
  while(!isInsideField(pose.translation));
  return pose;
}

Pose2D FieldDimensions::randomPoseOnCarpet() const
{ 
  Pose2D pose;
  do
    pose = Pose2D::random(x,y,Range<double>(-pi,pi));
  while(!isInsideCarpet(pose.translation));
  return pose;
}
Pose2D FieldDimensions::randomPoseOnOwnSide() const
{
	Pose2D pose;
	do 
	{
		pose = Pose2D::random(x,y,Range<double>(-pi,pi));
	} while (!isInsideOwnSide(pose.translation));
	return pose;
}

void FieldDimensions::LinesTable::push(const Pose2D& p, double l,bool isPartOfCircle)
{
  LinesTable::Line line;
  line.corner = p;
  line.length = l;
  line.isPartOfCircle=isPartOfCircle;
  lines.push_back(line);
}

void FieldDimensions::LinesTable::push(const Vector2<double>& s, const Vector2<double>& e,bool isPartOfCircle)
{
  Vector2<double> d = e - s;
  push(Pose2D(d.angle(), s), d.abs(),isPartOfCircle);
}

void FieldDimensions::LinesTable::pushCircle(const Vector2<double>& center, double radius, int numOfSegments)
{
  Vector2<double> p1, p2;
  for (double a = 0; a <= pi_4; a += pi2/numOfSegments)
  {
    p1 = Vector2<double>(sin(a), cos(a)) * radius;
    if (a > 0)
    {
      push(center + p1, center + p2,true);
      push(center + Vector2<double>(p1.x,-p1.y), center + Vector2<double>(p2.x,-p2.y),true);
      push(center + Vector2<double>(-p1.x,p1.y), center + Vector2<double>(-p2.x,p2.y),true);
      push(center - p1, center - p2,true);
      push(center + Vector2<double>(p1.y,p1.x), center + Vector2<double>(p2.y,p2.x),true);
      push(center + Vector2<double>(p1.y,-p1.x), center + Vector2<double>(p2.y,-p2.x),true);
      push(center + Vector2<double>(-p1.y,p1.x), center + Vector2<double>(-p2.y,p2.x),true);
      push(center + Vector2<double>(-p1.y,-p1.x), center + Vector2<double>(-p2.y,-p2.x),true);
    }
    p2 = p1;
  }
}

void FieldDimensions::LinesTable::pushQuarterCircle(const Vector2<double>& center, double radius, int numOfSegments, int angle)
{
  Vector2<double> p1, p2;
  for (double a = 0; a <= pi_4; a += pi2/numOfSegments)
  {
    p1 = Vector2<double>(sin(a), cos(a)) * radius;
    if (a > 0)
    {
      switch(angle)
      {
        case 0:
          push(center + p1, center + p2,true);
          push(center + Vector2<double>(p1.y,p1.x), center + Vector2<double>(p2.y,p2.x),true);
          break;
        case 1:
          push(center + Vector2<double>(-p1.x,p1.y), center + Vector2<double>(-p2.x,p2.y),true);
          push(center + Vector2<double>(-p1.y,p1.x), center + Vector2<double>(-p2.y,p2.x),true);
          break;
        case 2:
          push(center - p1, center - p2,true);
          push(center + Vector2<double>(-p1.y,-p1.x), center + Vector2<double>(-p2.y,-p2.x),true);
          break;
        case 3:
          push(center + Vector2<double>(p1.x,-p1.y), center + Vector2<double>(p2.x,-p2.y),true);
          push(center + Vector2<double>(p1.y,-p1.x), center + Vector2<double>(p2.y,-p2.x),true);
          break;
      }
    }
    p2 = p1;
  }
}

void FieldDimensions::LinesTable::doubleSided(double width, const FieldDimensions::LinesTable& single)
{
  for(std::vector<Line>::const_iterator i = single.lines.begin(); i != single.lines.end(); ++i)
  {
    push(i->corner + 
          Pose2D(Vector2<double>(-width/2,width/2)), 
          i->length + width);
    push(i->corner + 
          Pose2D(pi, Vector2<double>(i->length,0)) + 
          Pose2D(Vector2<double>(-width/2,width/2)), 
          i->length + width);
  }
}

bool FieldDimensions::LinesTable::isInside(const Vector2<double>& v) const
{
  //note:
  //This function assumes that the point (0,0) is inside and
  //that for any point inside the area the line to (0,0) belongs to the area too.

  Geometry::GTLine testLine(v, -v);
  for(std::vector<Line>::const_iterator i = lines.begin(); i != lines.end(); ++i)
  {
    double factor;
    Geometry::GTLine border(i->corner, i->length);
    if (Geometry::getIntersectionOfRaysFactor(border, testLine, factor))
      return false;
  }
  return true;
}

double FieldDimensions::LinesTable::clip(Vector2<double>& v) const
{
  if(isInside(v))
    return 0;
  else
  {
    Vector2<double> old = v,
                          v2;
    double minDist = 100000;
    for(std::vector<Line>::const_iterator i = lines.begin(); i != lines.end(); ++i)
    {
      Vector2<double> diff = (Pose2D(old) - i->corner).translation;
      if(diff.x < 0)
        v2 = i->corner.translation;

      else if(diff.x > i->length)
        v2 = (i->corner + Pose2D(Vector2<double>(i->length,0))).translation;
      else
        v2 = (i->corner + Pose2D(Vector2<double>(diff.x,0))).translation;
      double dist = (old - v2).abs();
      if(minDist > dist)
      {
        minDist = dist;
        v = v2;
      }
    }
    return (v - old).abs();
  }
}

bool FieldDimensions::LinesTable::getClosestPoint(Vector2<double>& vMin, const Pose2D& p, int numberOfRotations, double minLength) const
{
  int trueNumberOfRotations = numberOfRotations;
  if(numberOfRotations == 2)
    numberOfRotations = 4;

  // target angle -> target index
  double r = p.rotation / pi2 * numberOfRotations + 0.5;
  if(r < 0)
    r += numberOfRotations;
  int targetRot = int(r);
  //ASSERT(targetRot >= 0 && targetRot < numberOfRotations);
  targetRot %= trueNumberOfRotations;
  Vector2<double> v2;
  double minDist = 100000;
  for(std::vector<Line>::const_iterator i = lines.begin(); i != lines.end(); ++i)
    if(i->length >= minLength)
    {
      // angle -> index
      double r = (i->corner.rotation + pi_2) / pi2 * numberOfRotations + 0.5;
      if(r < 0)
        r += numberOfRotations;
      else if(r >= numberOfRotations)
        r -= numberOfRotations;
      int rot = int(r);
      //ASSERT(rot >= 0 && rot < numberOfRotations);
      rot %= trueNumberOfRotations;

      // index must be target index
      if(rot == targetRot)
      {
        Vector2<double> diff = (p - i->corner).translation;
        if(diff.x < 0)
          v2 = i->corner.translation;
        else if(diff.x > i->length)
          v2 = (i->corner + Pose2D(Vector2<double>(i->length,0))).translation;
        else
          v2 = (i->corner + Pose2D(Vector2<double>(diff.x,0))).translation;
        Vector2<double> vDiff = v2 - p.translation;
        double dist = vDiff.abs();
        if(minDist > dist)
        {
          minDist = dist;
          vMin = v2;
        }
      }
    }
  return (minDist<100000);
}

double FieldDimensions::LinesTable::getDistance(const Pose2D& pose) const
{
  double minDist = 100000;
  for(std::vector<Line>::const_iterator i = lines.begin(); i != lines.end(); ++i)
  {
    Vector2<double> v1 = (i->corner - pose).translation,
                    v2 = (i->corner + Pose2D(Vector2<double>(i->length,0)) 
                          - pose).translation;
    if(v1.y < 0 && v2.y > 0)
    {
      double dist = v1.x + (v2.x - v1.x) * -v1.y / (v2.y - v1.y);
      if(dist >= 0 && dist < minDist)
        minDist = dist;
    }
  }
  return minDist == 100000 ? -1 : minDist;
}

const Vector2<int>& FieldDimensions::CornersTable::getClosest(const Vector2<int>& p) const
{
  //ASSERT(!empty());
	int maxDistance2 = std::numeric_limits<int>().max();
  const Vector2<int>* closest = 0;
  for(const_iterator i = begin(); i != end(); ++i)
  {
    Vector2<int> diff = p - *i;
    int distance2 = diff * diff;
    if(maxDistance2 > distance2)
    {
      maxDistance2 = distance2;
      closest = &*i;
    }
  }
  return *closest;
}

const Vector2<double> FieldDimensions::CornersTable::getClosest(const Vector2<double>& p) const
{
  Vector2<int> closest = getClosest(Vector2<int>((int) p.x, (int) p.y));
  return Vector2<double>(closest.x, closest.y);
}

const char* FieldDimensions::getCornerClassName(FieldDimensions::CornerClass cornerClass)
{
  switch(cornerClass)
  {
  case xCorner: return "xCorner";
  case tCorner0: return "tCorner0";
  case tCorner90: return "tCorner90";
  case tCorner180: return "tCorner180";
  case tCorner270: return "tCorner270";
  case lCorner0: return "lCorner0";
  case lCorner90: return "lCorner90";
  case lCorner180: return "lCorner180";
  case lCorner270: return "lCorner270";
  default: return "UNKNOWN";
  }
}
