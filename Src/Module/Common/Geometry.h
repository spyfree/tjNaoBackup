#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <math.h>

#include "Tools/Math/Vector2.h"
#include "Tools/Math/Pose2D.h"
#include "Tools/Math/Matrix2x2.h"
#include <vector>
#include "Module/Representations/Perception/cameramatrix.h"
/**
* defines the square of a value
*/
#ifndef SQUARE
#define SQUARE(a) ( (a) * (a) )
#endif
/**
*I don't which stupid guy used max and min at first, which conflict with the std::max and std::min
* It caused a lot of troubles!!!!!!!!!!!!!!!!!!
*/
// #ifndef Max
// #define Max(a,b)            (((a) > (b)) ? (a) : (b))
// #endif
//
// #ifndef Min
// #define Min(a,b)            (((a) < (b)) ? (a) : (b))
// #endif

class Vector3f: std::vector<double> {
 public:
  Vector3f(): std::vector<double>(3, 0.0) {}
};

template <class T>
class BaseMatrix
{
 public:
  BaseMatrix (int rows=1, int cols=1) :
    nRows_(rows), nCols_(cols), data_(rows*cols) { }
  int numberRows () const {return nRows_;}
  int numberCols () const {return nCols_;}

  const T* operator [] (int k) const {return &data_[k*nCols_];}
  T* operator [] (int k) {return &data_[k*nCols_];}
 protected:
  int nRows_;
  int nCols_;
  std::vector<T> data_;
};

// 4x4 affine transformation matrix
class Matrix4x4 : public BaseMatrix<double> {
public:
  Matrix4x4() : BaseMatrix<double>(4,4) {clear();}
  void clear() {
    for (int i = 0; i < 16; i++) data_[i] = 0.0;
    data_[0] = data_[5] = data_[10] = data_[15] = 1.0;
  }
  void translate(double v[3]) {
    data_[12] += v[0];
    data_[13] += v[1];
    data_[14] += v[2];
  }
  void translate(double x, double y, double z) {
    data_[12] += x;
    data_[13] += y;
    data_[14] += z;
  }
  void rotateX(double angle) {
    double dxx,dxy,dxz,dyx,dyy,dyz,dzx,dzy,dzz;
    double dx,dy,dz;
    dxx = data_[0]; dxy = data_[1]; dxz = data_[2];
    dyx = data_[4]; dyy = data_[5]; dyz = data_[6];
    dzx = data_[8]; dzy = data_[9]; dzz = data_[10];
    dx = data_[12]; dy = data_[13]; dz = data_[14];
    data_[1] = dxy*cos(angle)-dxz*sin(angle); // dxy
    data_[5] = dyy*cos(angle)-dyz*sin(angle); // dyy
    data_[9] = dzy*cos(angle)-dzz*sin(angle); // dzy
    data_[2] = dxy*sin(angle)+dxz*cos(angle); // dxz
    data_[6] = dyy*sin(angle)+dyz*cos(angle); // dyz
    data_[10] = dzy*sin(angle)+dzz*cos(angle); // dzz
    data_[13] = dy*cos(angle)-dz*sin(angle); // dy
    data_[14] = dy*sin(angle)+dz*cos(angle); // dz
  }
  void rotateY(double angle) {
    double dxx,dxy,dxz,dyx,dyy,dyz,dzx,dzy,dzz;
    double dx,dy,dz;
    dxx = data_[0]; dxy = data_[1]; dxz = data_[2];
    dyx = data_[4]; dyy = data_[5]; dyz = data_[6];
    dzx = data_[8]; dzy = data_[9]; dzz = data_[10];
    dx = data_[12]; dy = data_[13]; dz = data_[14];
    data_[0] = dxx*cos(angle)+dxz*sin(angle); // dxx
    data_[4] = dyx*cos(angle)+dyz*sin(angle); // dyx
    data_[8] = dzx*cos(angle)+dzz*sin(angle); // dzx
    data_[2] = -dxx*sin(angle)+dxz*cos(angle); // dxz
    data_[6] = -dyx*sin(angle)+dyz*cos(angle); // dyz
    data_[10] = -dzx*sin(angle)+dzz*cos(angle); // dzz
    data_[12] = dx*cos(angle)+dz*sin(angle); // dx
    data_[14] = -dx*sin(angle)+dz*cos(angle); // dz
  }
  void rotateZ(double angle) {
    double dxx,dxy,dxz,dyx,dyy,dyz,dzx,dzy,dzz;
    double dx,dy,dz;
	dxx = data_[0]; dxy = data_[1]; dxz = data_[2];
    dyx = data_[4]; dyy = data_[5]; dyz = data_[6];
    dzx = data_[8]; dzy = data_[9]; dzz = data_[10];
    dx = data_[12]; dy = data_[13]; dz = data_[14];
    data_[0] = dxx*cos(angle)-dxy*sin(angle); // dxx
    data_[4] = dyx*cos(angle)-dyy*sin(angle); // dyx
    data_[8] = dzx*cos(angle)-dzy*sin(angle); // dzx
    data_[1] = dxx*sin(angle)+dxy*cos(angle); // dxy
    data_[5] = dyx*sin(angle)+dyy*cos(angle); // dyy
    data_[9] = dzx*sin(angle)+dzy*cos(angle); // dzy
    data_[12] = dx*cos(angle)-dy*sin(angle); // dx
    data_[13] = dx*sin(angle)+dy*cos(angle); // dy


  }
};

#include <Tools/Math/Vector3.h>
#include "Tools/Math/Matrix.h"
class CameraMatrix4X3
{
public:
	Matrix3x3<double> rotation;
	Vector3<double> translation;
	CameraMatrix4X3(){};
	CameraMatrix4X3(const Matrix4x4& cameraMatrix){
		get(cameraMatrix);
	};
	~CameraMatrix4X3(){};

/////////the old get ,the matrix is not right for now coordinate;
// 	void get(const Matrix4x4& cameraMatrix)
// 	{
// 		for(int i=0;i < 3; i++) {
// 			rotation.c[i].x = cameraMatrix[i][0];
// 			rotation.c[i].y = cameraMatrix[i][1];
// 			rotation.c[i].z = cameraMatrix[i][2];
// 		}
// 		translation.x = cameraMatrix[3][0];
// 		translation.y = cameraMatrix[3][1];
// 		translation.z = cameraMatrix[3][2];
// 	}
	void get(const Matrix4x4& cameraMatrix)
	{
		for(int i=0;i < 3; i++) {
			rotation.c[i].x = cameraMatrix[0][i];
			rotation.c[i].y = cameraMatrix[1][i];
			rotation.c[i].z = cameraMatrix[2][i];
		}
		translation.x = cameraMatrix[3][0];
		translation.y = cameraMatrix[3][1];
		translation.z = cameraMatrix[3][2];
	}
};
/************************************************************************/
/*                  GTGeometry                                          */
/************************************************************************/
/**
* The class Geometry defines representations for geometric objects and Methods
* for calculations with such object.
*
*/
class Geometry
{
public:
	  /** Defines a circle by its center and its radius*/
  struct Circle
  {
    Circle():radius(0) {}
    Circle(const Vector2<double>& c, double r)
    {
      center = c;
      radius = r;
    }
    Vector2<double> center;
    double radius;
  };
  
/** Defines a line by two vectors*/
	struct GTLine
	{
		Vector2<double> base;
		Vector2<double> direction;

		GTLine() {};
		GTLine(Vector2<double> base, Vector2<double> direction) :
		base(base),
			direction(direction)
		{};

		GTLine(Vector2<int> base, Vector2<double> direction) :
		direction(direction)
		{
			this->base.x = base.x;
			this->base.y = base.y;
		};

		GTLine(Vector2<int> base, Vector2<int> direction)
		{
			this->base.x = base.x;
			this->base.y = base.y;
			this->direction.x = direction.x;
			this->direction.y = direction.y;
		};


		GTLine(double baseX, double baseY, double directionX, double directionY)
		{
			base.x = baseX;
			base.y = baseY;
			direction.x = directionX;
			direction.y = directionY;
		};

		GTLine(const Pose2D& base, double length = 1.0)
		{
			this->base = base.translation;
			this->direction = (Pose2D(base.rotation) + Pose2D(Vector2<double>(length,0))).translation;
		}

		void normalizeDirection(){
			double distance =
				sqrt(SQUARE(direction.x) + SQUARE(direction.y));
			direction.x = direction.x / distance;
			direction.y = direction.y / distance;
		}
	};

  /** 
  * Calculates the angle between a pose and a position
  * @param from The base pose.
  * @param to The other position.
  * @return the angle from the pose to the position.
  */
  static double angleTo(const Pose2D& from, 
    const Vector2<double>& to);

  /** 
  * Calculates the distance from a pose to a position
  * @param from The base pose.
  * @param to The other position.
  * @return the distance from the pose to the position.
  */
  static double distanceTo(const Pose2D& from, 
    const Vector2<double>& to);

  /** 
  * Calculates the relative vector from a pose to a position
  * @param from The base pose.
  * @param to The other position.
  * @return the vector from the pose to the position.
  */
  static Vector2<double> vectorTo(const Pose2D& from, 
    const Vector2<double>& to);

  /**
  * Rotates a vector 
  * @param v The vector
  * @param a The angle
  * @return The rotated vector
  */
  static Vector2<double> rotate(const Vector2<double>& v, double a);

  /**
  * Rotates a vector 
  * @param v The vector
  * @param a The angle
  * @return The rotated vector
  */
  static Vector2<int> rotate(const Vector2<int>& v, double a);

  static int getIntersectionOfCircles(
    const Circle &c1,
    const Circle &c2,
    Vector2<double> &p1,
    Vector2<double> &p2
    );
    
    static bool checkIntersectionOfLines(
    const Vector2<double>& l1p1,
    const Vector2<double>& l1p2,
    const Vector2<double>& l2p1,
    const Vector2<double>& l2p2
    );

  static bool getIntersectionOfRaysFactor(
    const GTLine& ray1,
    const GTLine& ray2,
    double& intersection
    );

private:
  static int ccw(
    const Vector2<double>& p0, 
    const Vector2<double>& p1, 
    const Vector2<double>& p2);
 
public:
	static bool clipPointInsideRectange(
		const Vector2<int>& bottomLeftCorner,
		const Vector2<int>& topRightCorner,
		Vector2<int>& point
		);

	static bool clipPointInsideRectange(
		const Vector2<int>& bottomLeftCorner,
		const Vector2<int>& topRightCorner,
		Vector2<double>& point
    );
	/**
	* Clips a line with a rectangle
	* Be careful!! the coordinate of the bottomleft is (0,0) in a normal coordinate system;
	* While here, the bottomleft is actually topleft!!
	*
	* @param bottomLeft The bottom left corner of the rectangle
	* @param topRight The top right corner of the rectangle
	* @param line The line to be clipped
	* @param point1 The starting point of the resulting line
	* @param point2 The end point of the resulting line
	* @return states whether clipping was necessary (and done)
	*/
	static bool getIntersectionPointsOfLineAndRectangle(
		const Vector2<int>& bottomLeft,
		const Vector2<int>& topRight,
		const Geometry::GTLine line,
		Vector2<int>& point1,
		Vector2<int>& point2
   );
  static double getDistanceToLine
(
 const GTLine line,
 const Vector2<double>& point
 );
   static double distance(
    const Vector2<double>& point1,
    const Vector2<double>& point2
    );
/**
*function to get the rotation Matrix and translation vector from a cameraMartix.
*@param cameraMatrix The cameraMatrix of this image.
*@param rotation The rotation Matrix found in the cameraMatrix.
*@param translation The translation vector found in the cameraMatrix.
*/
	static void getRotationAndTranslationFromMatrix(const Matrix4x4& cameraMatrix, Matrix3x3<double>& rotation, Vector3<double>& translation);
/**
* function to calculate the coordinate of image point in RobotAxis, without scale and translation.
* @param x, y The coordinate in image.
* @param rotation The rotationMatrix in CameraMatrix.
* @param coordinate used to store the coordinate of image point.
*/
	static void getImagePointCoordinatinRobotAxis(double x, double y, const CameraMatrix & cameraMatrix, Vector3<double>& coordinate);

/************************************************************************/
/* function to calculate a Point on field                                                                     */
/************************************************************************/
	static bool calculatePointOnField(
    const double x,
    const double y,
    const CameraMatrix& cameraMatrix,
    Vector2<int>& pointOnField
    )
	{
		Vector2<double> pointOnFieldDouble;
		bool onField = calculatePointOnField(x,y,cameraMatrix,pointOnFieldDouble);
		pointOnField.x = (int)pointOnFieldDouble.x;
		pointOnField.y = (int)pointOnFieldDouble.y;
		return onField;
	}

	static bool calculatePointOnField
	(
	const double x,
	const double y,
	const CameraMatrix& cameraMatrix,
	Vector2<double>& pointOnField
	);
/***********************************************************************/
/* function to calculate a Point on height h cm
/*********************************************************************/
	static bool calculatePointOnH
	(
	const double x,
	const double y,
	const double h,
	const CameraMatrix& cameraMatrix,
	Vector2<double>& pointOnH
	);
/************************************************************************/
/*                                                                      */
/************************************************************************/
	static double getSizeByDistance(double sizeInReality, double distance);
/************************************************************************/
/*                                                                      */
/************************************************************************/
	static int calculateLineSize
(
 const Vector2<int>& pointInImage,
 const CameraMatrix& cameraMatrix
);
/************************************************************************/
/*                                                                      */
/************************************************************************/
static bool getIntersectionOfLines
(
 const GTLine line1,
 const GTLine line2,
 Vector2<double>& intersection
 );
/************************************************************************/
/*                                                                      */
/************************************************************************/
static bool calculatePointInImage
(
 const Vector2<int>& point,
 const CameraMatrix& cameraMatrix,
 Vector2<int>& pointInImage
);
static void calculatePointInImage
(const Vector3<double>& point,
 const CameraMatrix& cameraMatrix,
 Vector2<int>& pointInImage
 );

static void calculatePointInImage
(
 const Vector2<double>& point,
 const CameraMatrix& cameraMatrix,
 Vector2<int>& pointInImage
)
{
	Vector2<int> pointOnFieldInt;
	pointOnFieldInt.x = (int)point.x;
	pointOnFieldInt.y = (int)point.y;
	calculatePointInImage(pointOnFieldInt, cameraMatrix, pointInImage);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
static bool calculatePointByAngles
(
 const Vector2<double>& angles,
 const CameraMatrix& cameraMatrix,
 Vector2<int>& point
 );

static void calculateAnglesForPoint
(
 const Vector2<int>& point,
 const CameraMatrix& cameraMatrix,

 Vector2<double>& angles
 );
static void calculateAnglesForPoint
(
 const Vector2<double>& point, 
 const CameraMatrix& cameraMatrix,  
 Vector2<double>& angles
 );

//////////////////////////////////////////////////////////////////////////
////   This Function is used to convert the coordinate in          ///////
////  SelfLocator to coordinate of TJArk's Location Coordinate System/////
////  AS Original Point of Coordinate in SelfLocator Module is the///////
////  Center of the field, in mm. The direction of the robot is its right side
////////////////////////////////////////////////////////////////////////
// static Vector2<double> SelfLoc2TJLocVector(const Vector2<double> p){
// 	Vector2<double> temp;
// 	temp = p / 10.0;//from mm to cm
// 	temp += Vector2<double>(FIELD_WIDTH /2, FIELD_LENGTH/2);
// 	return temp;
// }
// static Pose2D SelfLoc2TJLocPos2D(const Pose2D pos){
// 	Pose2D temp = pos;
// 	temp.translation /= 10.0;
// 	temp.translation += Vector2<double>(FIELD_WIDTH /2, FIELD_LENGTH/2);
// 	temp.rotation += pi/2;
// 	normalize(temp.rotation);
// 	return temp;
// }


/** 
* The function determines how far the ball is away depending on its real size and the size in the image.
* @param sizeInReality The real size of the ball.
* @param sizeInPixels The size in the image.
* @return The distance between the camera and the ball.
*/
static double getBallDistanceByAngleSize(
	double sizeInReality, 
	double sizeInPixels
	);

static double pixelSizeToAngleSize(double pixelSize);
};

#endif


