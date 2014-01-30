#include "Geometry.h"
#include "Module/Common/VisionHeader.h"

bool Geometry::clipPointInsideRectange(
									   const Vector2<int>& bottomLeftCorner, 
									   const Vector2<int>& topRightCorner,
									   Vector2<int>& point
									   )
{
	bool clipped = false;
	if (point.x < bottomLeftCorner.x)
	{
		point.x = bottomLeftCorner.x;
		clipped = true;
	}
	if (point.x > topRightCorner.x)
	{
		point.x = topRightCorner.x;
		clipped = true;
	}
	if (point.y < bottomLeftCorner.y)
	{
		point.y = bottomLeftCorner.y;
		clipped = true;
	}
	if (point.y > topRightCorner.y)
	{
		point.y = topRightCorner.y;
		clipped = true;
	}
	return clipped;
}

bool Geometry::clipPointInsideRectange(
									   const Vector2<int>& bottomLeftCorner, 
									   const Vector2<int>& topRightCorner,
									   Vector2<double>& point
									   )
{
	bool clipped = false;
	if (point.x < bottomLeftCorner.x)
	{
		point.x = (double)bottomLeftCorner.x;
		clipped = true;
	}
	if (point.x > topRightCorner.x)
	{
		point.x = (double)topRightCorner.x;
		clipped = true;
	}
	if (point.y < bottomLeftCorner.y)
	{
		point.y = (double)bottomLeftCorner.y;
		clipped = true;
	}
	if (point.y > topRightCorner.y)
	{
		point.y = (double)topRightCorner.y;
		clipped = true;
	}
	return clipped;
}

bool Geometry::getIntersectionPointsOfLineAndRectangle(
													   const Vector2<int>& bottomLeft, 
													   const Vector2<int>& topRight,
													   const Geometry::GTLine line,
													   Vector2<int>& point1, 
													   Vector2<int>& point2
													   )
{
	int foundPoints=0;
	Vector2<double> point[2];
	if (line.direction.x!=0)
	{
		double y1=line.base.y+(bottomLeft.x-line.base.x)*line.direction.y/line.direction.x;
		if ((y1>=bottomLeft.y)&&(y1<=topRight.y))
		{
			point[foundPoints].x=bottomLeft.x;
			point[foundPoints++].y=y1;
		}
		double y2=line.base.y+(topRight.x-line.base.x)*line.direction.y/line.direction.x;
		if ((y2>=bottomLeft.y)&&(y2<=topRight.y))
		{
			point[foundPoints].x=topRight.x;
			point[foundPoints++].y=y2;
		}
	}
	if (line.direction.y!=0)
	{
		double x1=line.base.x+(bottomLeft.y-line.base.y)*line.direction.x/line.direction.y;
		if ((x1>=bottomLeft.x)&&(x1<=topRight.x)&&(foundPoints<2))
		{
			point[foundPoints].x=x1;
			point[foundPoints].y=bottomLeft.y;
			if ((foundPoints==0)||((point[0]-point[1]).abs()>0.1))
			{
				foundPoints++;
			}
		}
		double x2=line.base.x+(topRight.y-line.base.y)*line.direction.x/line.direction.y;
		if ((x2>=bottomLeft.x)&&(x2<=topRight.x)&&(foundPoints<2))
		{
			point[foundPoints].x=x2;
			point[foundPoints].y=topRight.y;
			if ((foundPoints==0)||((point[0]-point[1]).abs()>0.1))
			{
				foundPoints++;
			}
		}
	}
	switch (foundPoints)
	{
	case 1:
		point1.x=(int)point[0].x;
		point2.x=point1.x;
		point1.y=(int)point[0].y;
		point2.y=point1.y;
		foundPoints++;
		return true;
	case 2:
		if ((point[1]-point[0])*line.direction >0)
		{
			point1.x=(int)point[0].x;
			point1.y=(int)point[0].y;
			point2.x=(int)point[1].x;
			point2.y=(int)point[1].y;
		}
		else
		{
			point1.x=(int)point[1].x;
			point1.y=(int)point[1].y;
			point2.x=(int)point[0].x;
			point2.y=(int)point[0].y;
		}
		return true;
	default:
		return false;
	}
}

double Geometry::getDistanceToLine
(
 const GTLine line,
 const Vector2<double>& point
 )
{
  if (line.direction.x == 0 && line.direction.y == 0) 
    return distance(point, line.base);

  Vector2<double> normal;
  normal.x = line.direction.y;
  normal.y = -line.direction.x;
  normal.normalize();

  double c = normal * line.base;

  return normal * point - c;
}

double Geometry::distance
(
 const Vector2<double>& point1,
 const Vector2<double>& point2
 )
{
  return (point2 - point1).abs();
}

/******for Ball**********/
void Geometry::getRotationAndTranslationFromMatrix(const Matrix4x4& cameraMatrix, Matrix3x3<double>& rotation, Vector3<double>& translation)
{
	int i=0;
	for(;i < 3; i++) {
		rotation.c[i].x = cameraMatrix[i][0];
		rotation.c[i].y = cameraMatrix[i][1];
		rotation.c[i].z = cameraMatrix[i][2];
	}
	translation.x = cameraMatrix[3][0];
	translation.y = cameraMatrix[3][1];
	translation.z = cameraMatrix[3][2];
}


void Geometry::getImagePointCoordinatinRobotAxis(double x, double y, const CameraMatrix& cameraMatrix, Vector3<double>& coordinate )
{
	Vector3<double> tempcoordinate = Vector3<double>(FOCAL_LENGTH,
		                                             IMAGE_WIDTH/2-x,
												     IMAGE_HEIGHT/2-y);
	coordinate = cameraMatrix.rotation * tempcoordinate;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

bool Geometry::calculatePointOnField
(
 const double x,
 const double y,
 const CameraMatrix& cameraMatrix,
 Vector2<double>& pointOnField
 )
{
	/*double xFactor = 1/FOCAL_LENGTH,
           yFactor = 1/FOCAL_LENGTH;*/
	Vector3<double> 
    vectorToCenter(FOCAL_LENGTH,(IMAGE_WIDTH/2-x),(IMAGE_HEIGHT/2 - y));
  
	Vector3<double> 
    vectorToCenterWorld = cameraMatrix.rotation * vectorToCenter;

  //Is the point above the horizon ? - return
  if(vectorToCenterWorld.z > -5) return false;
  double a1 = cameraMatrix.translation.x,
         a2 = cameraMatrix.translation.y,
         a3 = cameraMatrix.translation.z,
         b1 = vectorToCenterWorld.x,
         b2 = vectorToCenterWorld.y,
         b3 = vectorToCenterWorld.z;
  
  pointOnField.x = (a1 * b3 - a3 * b1) / b3 + 0.5;
  pointOnField.y = (a2 * b3 - a3 * b2) / b3 + 0.5;

  return fabs(pointOnField.x) < 10000 && fabs(pointOnField.y) < 10000;
}
/***********************************************************************/
/* function to calculate a Point on height h
/*********************************************************************/
bool Geometry::calculatePointOnH
(
const double x,
const double y,
const double h,
const CameraMatrix& cameraMatrix,
Vector2<double>& pointOnH
)
{
	Vector3<double> 
		vectorToCenter(FOCAL_LENGTH,(IMAGE_WIDTH/2-x),(IMAGE_HEIGHT/2 - y));

	Vector3<double> 
		vectorToCenterWorld = cameraMatrix.rotation * vectorToCenter;

	//Is the point above the horizon ? - return
	if(vectorToCenterWorld.z > -5) return false;
	double a1 = cameraMatrix.translation.x,
		a2 = cameraMatrix.translation.y,
		a3 = cameraMatrix.translation.z-h*10,
		b1 = vectorToCenterWorld.x,
		b2 = vectorToCenterWorld.y,
		b3 = vectorToCenterWorld.z;
	pointOnH.x = (a1 * b3 - a3 * b1) / b3 + 0.5;
	pointOnH.y = (a2 * b3 - a3 * b2) / b3 + 0.5;

	return fabs(pointOnH.x) < 10000 && fabs(pointOnH.y) < 10000;
}
/************************************************************************/
/* function to get the image size of a object                                                                     */
/************************************************************************/
double Geometry::getSizeByDistance
(
 double sizeInReality,
 double distance
)
{
  double xFactor = FOCAL_LENGTH;

  return sizeInReality / distance * xFactor;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int Geometry::calculateLineSize
(
 const Vector2<int>& pointInImage, 
 const CameraMatrix& cameraMatrix
)
{
  Vector2<double> pointOnField; //position on field, relative to robot
  if(Geometry::calculatePointOnField(pointInImage.x, pointInImage.y, cameraMatrix, pointOnField))
  {
    int distance = (int) sqrt(SQUARE(cameraMatrix.translation.z) + SQUARE(pointOnField.abs()));
    return (int)Geometry::getSizeByDistance(25*2, distance);
  }
  else
  {
    return 0;
  }
}

int Geometry::ccw(const Vector2<double>& p0, const Vector2<double>& p1, const Vector2<double>& p2)
{
	double dx1(p1.x - p0.x); 
	double dy1(p1.y - p0.y);
	double dx2(p2.x - p0.x);
	double dy2(p2.y - p0.y);
	if(dx1*dy2 > dy1*dx2)
		return 1;
	if(dx1*dy2 < dy1*dx2)
		return -1;
	// Now (dx1*dy2 == dy1*dx2) must be true:
	if((dx1*dx2 < 0.0) || (dy1*dy2 < 0.0))
		return -1;
	if((dx1*dx1 + dy1*dy1) >= (dx2*dx2 + dy2*dy2))
		return 0;
	return 1;
}

bool Geometry::getIntersectionOfLines
(
 const GTLine line1,
 const GTLine line2,
 Vector2<double>& intersection
 )
{
  if(line1.direction.y * line2.direction.x == line1.direction.x * line2.direction.y)
  {
    return false;
  }
  
  intersection.x = 
    line1.base.x +
    line1.direction.x * 
    (
    line1.base.y * line2.direction.x - 
    line2.base.y * line2.direction.x + 
    (-line1.base.x + line2.base.x) * line2.direction.y
    )
    /
    ( (-line1.direction.y) * line2.direction.x + line1.direction.x * line2.direction.y );
  
  intersection.y = 
    line1.base.y + 
    line1.direction.y *
    (
    -line1.base.y * line2.direction.x + 
    line2.base.y * line2.direction.x + 
    (line1.base.x - line2.base.x) * line2.direction.y
    )
    /
    (line1.direction.y * line2.direction.x - line1.direction.x * line2.direction.y);
  
  return true;
}

bool Geometry::calculatePointInImage
( 
 const Vector2<int>& point,
 const CameraMatrix& cameraMatrix,
 Vector2<int>& pointInImage
 )
{
  Vector2<double> offset(point.x - cameraMatrix.translation.x,
                         point.y - cameraMatrix.translation.y);
  return calculatePointByAngles(
    Vector2<double>(atan2(offset.y,offset.x),
                    atan2(-cameraMatrix.translation.z,offset.abs())),
    cameraMatrix,
    pointInImage
  );
}
void Geometry::calculatePointInImage
(
 const Vector3<double>& fieldpoint,
 const CameraMatrix& cameraMatrix,
 Vector2<int>& pointInImage 
 )
{
	Vector3<double>vectorToPoint;
	Matrix3x3<double> rotationMatrix = cameraMatrix.rotation;
	vectorToPoint = rotationMatrix.invert()*(fieldpoint-cameraMatrix.translation);
	double scale;
	scale = fabs(FOCAL_LENGTH/vectorToPoint.x);
	pointInImage.x = (int)(0.5 + (IMAGE_WIDTH/2) - vectorToPoint.y * scale);
	pointInImage.y = (int)(0.5 + (IMAGE_HEIGHT/2) - vectorToPoint.z * scale);


}
/*
void Geometry::calculatePointInImage
( 
 const Vector2<int>& point,
 const CameraMatrix4X3& cameraMatrix,
 Vector2<int>& pointInImage
)
{
	Vector2<double> offset(point.x - cameraMatrix.translation.x,
                         point.y - cameraMatrix.translation.y);
	Vector3<double> vectorToPointWorld(offset.x, offset.y, -cameraMatrix.translation.z );
	Vector3<double> vectorToPoint = cameraMatrix.rotation.invert() * vectorToPointWorld;
	double factor = FOCAL_LENGTH;  
    double scale = factor / vectorToPoint.y;
	pointInImage.x = (int)(0.5 + (IMAGE_WIDTH/2) + vectorToPoint.x * scale);
    pointInImage.y = (int)(0.5 + (IMAGE_HEIGHT/2) - vectorToPoint.z * scale);
}
*/

bool Geometry::calculatePointByAngles
(
 const Vector2<double>& angles,
 const CameraMatrix& cameraMatrix, 
 Vector2<int>& point
 )
{
	Vector3<double> vectorToPointWorld, vectorToPoint;
	vectorToPointWorld.x = cos(angles.x);
	vectorToPointWorld.y = sin(angles.x);
	vectorToPointWorld.z = tan(angles.y);

	Matrix3x3<double> rotationMatrix = cameraMatrix.rotation;
	vectorToPoint = rotationMatrix.invert() * vectorToPointWorld;

	double factor = FOCAL_LENGTH;

	double scale = fabs(factor / vectorToPoint.x);

	//point.x = (int)(0.5 + (IMAGE_WIDTH/2) + vectorToPoint.x * scale);
	//point.y = (int)(0.5 + (IMAGE_HEIGHT/2) - vectorToPoint.z * scale);
	point.x = (int)(0.5 + (IMAGE_WIDTH/2) - vectorToPoint.y * scale);
	point.y = (int)(0.5 + (IMAGE_HEIGHT/2) - vectorToPoint.z * scale);
	return vectorToPoint.x > 0;
}


void Geometry::calculateAnglesForPoint
(
 const Vector2<int>& point, 
 const CameraMatrix& cameraMatrix,  
 Vector2<double>& angles
 )
{
  double factor = FOCAL_LENGTH;
  Vector3<double> vectorToPointWorld;
  getImagePointCoordinatinRobotAxis((double)point.x, (double)point.y, cameraMatrix, vectorToPointWorld );  
  
  angles.x =
     atan2(vectorToPointWorld.y,vectorToPointWorld.x);
  
  angles.y =
    atan2(
    vectorToPointWorld.z,
    sqrt(vectorToPointWorld.x * vectorToPointWorld.x +  vectorToPointWorld.y * vectorToPointWorld.y) 
    );
}

void Geometry::calculateAnglesForPoint
(
 const Vector2<double>& point, 
 const CameraMatrix& cameraMatrix,  
 Vector2<double>& angles
 )
{
	double factor = FOCAL_LENGTH;
	Vector3<double> vectorToPointWorld;
	getImagePointCoordinatinRobotAxis(point.x, point.y, cameraMatrix, vectorToPointWorld );  

	angles.x =
		atan2(vectorToPointWorld.y,vectorToPointWorld.x);

	angles.y =
		atan2(
		vectorToPointWorld.z,
		sqrt(vectorToPointWorld.x * vectorToPointWorld.x +  vectorToPointWorld.y * vectorToPointWorld.y) 
		);
}
double Geometry::getBallDistanceByAngleSize
(
 double sizeInReality,
 double sizeAsAngle
 )
{
	return (sizeInReality / 2.0) / sin(sizeAsAngle / 2.0 + 0.00000001);
}


double Geometry::pixelSizeToAngleSize(double pixelSize)
{
	return atan(pixelSize * FOCAL_LENGTH_INV);  
}


double Geometry::angleTo(const Pose2D& from, 
						 const Vector2<double>& to)
{
	Pose2D relPos = Pose2D(to) - from;
	return atan2(relPos.translation.y,relPos.translation.x);
}

double Geometry::distanceTo(const Pose2D& from, 
							const Vector2<double>& to)
{
	return (Pose2D(to) - from).translation.abs();
}

Vector2<double> Geometry::vectorTo(const Pose2D& from, const Vector2<double>& to)
{
	return (Pose2D(to) - from).translation;
}

Vector2<double> Geometry::rotate(const Vector2<double>& v, double a)
{
	Vector2<double> rV;
	double cos_a(cos(a));
	double sin_a(sin(a));
	rV.x = cos_a*v.x - sin_a*v.y;
	rV.y = sin_a*v.x + cos_a*v.y;
	return rV;
}

Vector2<int> Geometry::rotate(const Vector2<int>& v, double a)
{
	Vector2<int> rV;
	double cos_a(cos(a));
	double sin_a(sin(a));
	rV.x = static_cast<int>(cos_a*v.x - sin_a*v.y);
	rV.y = static_cast<int>(sin_a*v.x + cos_a*v.y);
	return rV;
}



int Geometry::getIntersectionOfCircles(
									   const Circle &c0,
									   const Circle &c1,
									   Vector2<double> &p1,
									   Vector2<double> &p2
									   )
{
	double a, dx, dy, d, h, rx, ry;
	double x2, y2;

	/* dx and dy are the vertical and horizontal distances between
	* the circle centers.
	*/
	dx = c1.center.x - c0.center.x;
	dy = c1.center.y - c0.center.y;

	/* Determine the straight-line distance between the centers. */
	d = sqrt((dy*dy) + (dx*dx));

	/* Check for solvability. */
	if (d > (c0.radius + c1.radius))
	{
		/* no solution. circles do not intersect. */
		return 0;
	}
	if (d < fabs(c0.radius - c1.radius))
	{
		/* no solution. one circle is contained in the other */
		return 0;
	}

	/* 'point 2' is the point where the line through the circle
	* intersection points crosses the line between the circle
	* centers.  
	*/

	/* Determine the distance from point 0 to point 2. */
	a = ((c0.radius * c0.radius) - (c1.radius * c1.radius) + (d * d)) / (2.0 * d) ;

	/* Determine the coordinates of point 2. */
	x2 = c0.center.x + (dx * a/d);
	y2 = c0.center.y + (dy * a/d);

	/* Determine the distance from point 2 to either of the
	* intersection points.
	*/
	h = sqrt((c0.radius * c0.radius) - (a*a));

	/* Now determine the offsets of the intersection points from
	* point 2.
	*/
	rx = -dy * (h/d);
	ry =  dx * (h/d);

	/* Determine the absolute intersection points. */
	p1.x = x2 + rx;
	p2.x = x2 - rx;
	p1.y = y2 + ry;
	p2.y = y2 - ry;

	return 1;
}


bool Geometry::checkIntersectionOfLines(
										const Vector2<double>& l1p1,
										const Vector2<double>& l1p2,
										const Vector2<double>& l2p1,
										const Vector2<double>& l2p2)
{
	return (((ccw(l1p1, l1p2, l2p1) * ccw(l1p1,l1p2,l2p2)) <= 0)
		&& ((ccw(l2p1, l2p2, l1p1) * ccw(l2p1,l2p2,l1p2)) <= 0));
}


bool Geometry::getIntersectionOfRaysFactor
(
 const GTLine& ray1,
 const GTLine& ray2,
 double& factor
 )
{
	double divisor = ray2.direction.x * ray1.direction.y - ray1.direction.x * ray2.direction.y;
	if(divisor==0)
	{
		return false;
	}
	double k=(ray2.direction.y*ray1.base.x - ray2.direction.y*ray2.base.x - ray2.direction.x*ray1.base.y + ray2.direction.x*ray2.base.y)/divisor;
	double l=(ray1.direction.y*ray1.base.x - ray1.direction.y*ray2.base.x - ray1.direction.x*ray1.base.y + ray1.direction.x*ray2.base.y)/divisor;
	if ((k>=0)&&(l>=0)&&(k<=1)&&(l<=1))
	{
		factor=k;
		return true;
	}
	return false;
}
