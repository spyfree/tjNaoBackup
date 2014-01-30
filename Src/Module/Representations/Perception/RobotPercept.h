/**
* @file RobotPercept.h
* @repersentation of Robot needed to be detected
*/
#ifndef RobotPercept_H
#define RobotPercept_H

#include "Tools/Math/Vector2.h"
#include "Module/Vision/DebugDrawings.h"
#include "Tools/ColorClasses.h"
#include "Module/Common/VisionHeader.h"
#include "Module/Network/RoboCupGameControlData.h"
#include <vector>

class RobotPercept 
{
	
public:
	class Robot 
	{
	public:
		Vector2<int> lowestPx; /**< coordinates of the lowest seen pixel */
		Vector2<int> middlePx; /**< cooodinates of the middle seen pixel */
		int robotStandHeight;
		int team; /**< the team color of the seen robot (either TEAM_BLUE or TEAM_RED) */
		bool standing; /**< whether the robot is upright or lying on the field */
		Vector2<double> relativePositionOnField; /** < robot loc position */

		Robot() {}
		Robot(const Vector2<int>& lowestPx,const Vector2<int>& middlePx, bool standing, unsigned char color) : lowestPx(lowestPx),middlePx(middlePx),standing(standing),robotStandHeight(20)
		{
			if(color == CYAN)
				team = TEAM_BLUE;
			else if(color == RED)
				team = TEAM_RED;
			else
				team = -1;
		}
	};
	typedef std::vector<Robot>::const_iterator RCIt;
	typedef std::vector<Robot>::iterator RIt;

	std::vector<Robot> robots;
	std::vector<Robot> unlocalizableRobots;

	/*void draw()
	{
		DECLARE_DEBUG_DRAWING("representation:RobotPercept:pixels", "drawingOnImage");
		COMPLEX_DRAWING("representation:RobotPercept:pixels",
			for(RCIt r(robots.begin()); r != robots.end(); r++)
			{
				ColorClasses::Color color(r->team == TEAM_BLUE ? ColorClasses::robotBlue :
					r->team == TEAM_RED ? ColorClasses::red : ColorClasses::black);
		MID_DOT("representation:RobotPercept:pixels", (int) r->lowestPx.x, (int) r->lowestPx.y, color, color);
			}
			);
	}*/
};

#endif

















