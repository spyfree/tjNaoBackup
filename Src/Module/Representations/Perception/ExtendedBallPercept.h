/**
* @file ExtendedBallPercept.h
*
* Very simple representation of a seen ball
*

*/

#ifndef __ExtendedBallPercept_h_
#define __ExtendedBallPercept_h_

#include "Tools/Math/Vector2.h"

class ExtendedBallPercept 
{


public:
  /** Constructor */
  ExtendedBallPercept():
  positionInImage(0,0),
  radiusInImage(0),
  relativePositionOnField(0,0,0),
  projectedBallRadius(0),
  levenbergMarquardtResult(false),
  couldBeBallPart(false),
  isValidBallPercept(true),
  validity(-1)
  {};
  ~ExtendedBallPercept(){};
  /** Draws the ball*/

  Vector2<int> positionInImage;         /**< The position of the ball in the current image */
  double radiusInImage;                    /**< The radius of the ball in the current image */
  
  Vector3<double> relativePositionOnField; /**< Ball position relative to the robot. */
  double projectedBallRadius;
  double percentOfOrange;
  bool levenbergMarquardtResult;
  bool couldBeBallPart;
  bool isValidBallPercept;

  Vector2<int> centerTopLeft;
  Vector2<int> centerDownRight;
  Vector2<int> outerTopLeft;
  Vector2<int> outerDownRight;
  Vector2<int> middleTopLeft ;
  Vector2<int> middleDownRight;
  // 0=COUNTER; 1=ORANGE; 2=GREEN; 3=WHITE; 4=RED; 5=YELLOW; 6=BLUE; 7=NONE; 8=OTHER
  int centerArea[9];
  int outerArea[9];
  int middleArea[9];
  double centerResult;
  double outerResult;
  double distanceCheckResult;
  double validity;

  double deviationOfBallPoints; // the average deviation of non-border ballpoints from the calculated circle
  double durabilityOfBallPoints; // 0..1; factor considering the relation of nearGreen points, nearYellow points and hardEdge points to the number of non-border points
  double eccentricity;
};

#endif// __ExtendedBallPercept_h_
