/**
* @file ExtendedBallPercepts.h
*/

#ifndef __ExtendedBallPercepts_h_
#define __ExtendedBallPercepts_h_

#include "Tools/Math/Vector2.h"
#include "Module/Representations/Perception/ExtendedBallPercept.h" 

class ExtendedBallPercepts 
{

public:
	ExtendedBallPercepts() 
	{ 
		extendedBallPercepts.reserve(10);
	}


	std::vector<ExtendedBallPercept> extendedBallPercepts;

};
#endif //__ExtendedBallPercepts_h_
