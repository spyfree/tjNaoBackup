
#ifndef __LinesPercept_h_
#define __LinesPercept_h_

#include "Tools/Math/Vector2.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include <list>

/**
* @class LinePercept
* A class that represents the found fieldlines, center circle and intersections.
*/
class LinesPercept
{
public:
	/**
	* @class Linesegment
	*
	* This class represents a linesegment generated from a lienspot.
	*/
	class LineSegment
	{
	public:
		double alpha; /**< direction of representation in Hesse norm form of this linesegment */
		double d; /**< distance of representation in Hesse norm form of this linesegment */
		Vector2<int> p1, /**< start/end point of this linesegment in field coordinates */
			p2, /**< start/end point of this linesegment in field coordinates */
			p1Img, /**< start/end point of this linesegment in image coordinates */
			p2Img; /**< start/end point of this linesegment in image coordinates */
	};

	/**
	* @class Line
	*
	* This class represents a found fieldline.
	*/
	class LineOnField
	{
	public:
		double alpha; /**< direction of this line in Hesse norm form */
		double d; /**< distance of this line in Hesse norm form */
		bool dead; /**< This is needed for merging lines */
		bool midLine; /**< Whether this is the line throught the center circle */
		std::vector<LineSegment> segments; /**< The linesegments forming this line */
		Vector2<int> first, last; /**< The starting/end point of this line in field coordinates */

		/**
		* Calculates the distance of a point p the this line
		* @param p a point
		* @return the distance
		*/
		double calculateDistToLine(const Vector2<int>& p) const
		{
			return p.x * cos(alpha) + p.y * sin(alpha) - d;
		}

		/**
		* Calculates the closest point to a point on this line
		* @param p a point
		* @return the closest point on this line
		*/
		Vector2<int> calculateClosestPointOnLine(const Vector2<int>& p) const;
	};

	/**
	* @class CircleSpot
	*
	* This class represents circle spots. A circle spot
	* is a point calculated from a linesegment where the
	* center circle would be if the linesegment is part
	* of the center circle. 
	* This is also used for the found circle.
	*/
	class CircleSpot
	{
	public:
		CircleSpot():found(false),lastSeen(0) {}

		Vector2<int> pos; /**< The position of the center of the center circle in field coordinates */
		bool found; /**< Whether the center circle was found in this frame */
		unsigned long lastSeen; /**< The last time the center circle was seen */
		std::list<LineSegment>::iterator iterator; /**< An iterator pointing to the according segment
												   in the singleSegs list */
	};

	/**
	* @class Intersection
	* A class representing a intersection of two fieldlines
	*/
	class Intersection 
	{
	private:
		void serialize(In* in, Out* out);

	public:
		enum IntersectionType
		{
			L,
			T,
			X,
		};
		IntersectionType intersectionType;
		Vector2<int> intersectionOnField; /**< The fieldcoordinates of the intersection */
		Vector2<double> dir1, dir2; /**< The directions of the lines intersected. */
	};
public:
	std::list<LineOnField> lineOnField; /**<The found fieldlines */
	std::vector<Intersection> intersections; /**< The intersections of the lines */
	std::list<LineSegment> singleSegs; /**< Line segments which could not be clustered to a line */
	CircleSpot circleCenter; /**< The position of the center circle if found */

	/**Clear the percept */
	void clear();

	/** Determines the closest line to a given point 
	* @param point the given point
	* @param retLine the closest line
	* @return the distance from point to retLine
	* */
	int getClosestLine(Vector2<int> point, LineOnField& retLine) const;
};
struct netCircle
{
	uint8 CircleSeen;
	int32 CircleCenterX;
	int32 CircleCenterY;
};
#endif //__LinePercept_h_

