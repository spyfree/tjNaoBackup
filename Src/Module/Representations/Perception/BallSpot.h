/**
* @file BallSpot.h
* Declaration of a class that represents a spot that might be an indication of a ball.
* @author <a href="mailto:Thomas.Roefer@dfki.de">Thomas Röfer</a>
*/

#ifndef __BallSpot_h_
#define __BallSpot_h_

#include "Tools/Math/Vector2.h"

/**
* @class BallSpot
* A class that represents a spot that might be an indication of a ball.
*/
class BallSpot
{

public:
  BallSpot()/*:found(false)*/{};
  Vector2<int> position;
  bool found;
  double eccentricity;


};

#endif //__BallSpot_h_
