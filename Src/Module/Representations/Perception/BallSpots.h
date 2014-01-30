/**
* @file BallSpots.h
* Declaration of a class that represents a spot that might be an indication of a ball.
*/
#ifndef __BallSpots_h_
#define __BallSpots_h_

#include "Tools/Math/Vector2.h"
#include "BallSpot.h"
#include <vector>
/**
* @class BallSpots
* A class that represents a spot that might be an indication of a ball.
*/
class BallSpots
{
public:
	BallSpots() 
	{ 
		ballSpots.reserve(50); 
	}

	std::vector<BallSpot> ballSpots;
	//BallSpot bs;

	void addBallSpot(int x, int y, double eccentricity)
	{
		BallSpot bs;
		bs.position.x = x;
		bs.position.y = y;
		bs.eccentricity = eccentricity;
		//bs.found=true;
		ballSpots.push_back(bs);
	}
};
#endif //__BallSpots_h_
