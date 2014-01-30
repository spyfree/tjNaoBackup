#include"Tools/Math/Common.h"
#include "LinesPercept.h"

Vector2<int> LinesPercept::LineOnField::calculateClosestPointOnLine(const Vector2<int>& p) const
{
	const Vector2<double> p_double = Vector2<double>(p.x, p.y);
	const Vector2<double> normale = Vector2<double>(cos(alpha+pi), sin(alpha+pi)); 
	const Vector2<double> offset = normale * calculateDistToLine(p);
	return p + Vector2<int>((int)offset.x, (int)offset.y);
}


int LinesPercept::getClosestLine(Vector2<int> point, LineOnField& retLine) const
{
	std::list<LinesPercept::LineOnField>::const_iterator closestLine = lineOnField.end();
	int minDist = -1;
	for(std::list<LinesPercept::LineOnField>::const_iterator l1 = lineOnField.begin(); l1 != lineOnField.end(); l1++)
	{
		const int dist = (int)fabs(l1->calculateDistToLine(point));
		if(dist < minDist || minDist == -1)
		{
			closestLine = l1;
			minDist = dist;
		}
	}

	if(minDist != -1)
		retLine = *closestLine;
	return minDist;
}

void LinesPercept::clear()
{
	lineOnField.clear();
	singleSegs.clear();
	intersections.clear();
	circleCenter.found = false;
	circleCenter.pos.x=0.0;
	circleCenter.pos.y=0.0;
}
