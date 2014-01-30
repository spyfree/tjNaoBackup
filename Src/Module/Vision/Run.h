#ifndef _RUNS_H_
#define _RUNS_H_
#include "Tools/Math/Vector2.h"
class Run
{

public:
	Run(){};
	~Run(){};
	Vector2<int> start;
	Vector2<int> end;
	int length; 
	//int parent;
	//int next;
	unsigned char colour;
	/**< The length of the segment line in the image. */
	// 		enum RunType
	// 		{
	// 		YellowGoal,
	// 		CyanGoal
	// 		}; 
};
#endif