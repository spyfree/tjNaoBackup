/**
 * @file Matrix.cpp
 * Implements RotationMatrix
 *
 * @author <a href="mailto:martin.kallnik@gmx.de" > Martin Kallnik</a>
 * @author Max Risler
 */

#include "Matrix.h"
#include "Common.h"
#include "Tools/Streams/Streamable.h"

RotationMatrix::RotationMatrix(const double& yaw, const double& pitch, const double& roll)
{
	const double cy=cos(yaw);
	const double sy=sin(yaw);
	const double cp=cos(pitch);
	const double sp=sin(pitch);
	const double cr=cos(roll);
	const double sr=sin(roll);

	c[0].x=cr*cp ;
	c[0].y=-sr*cy+cr*sp*sy ;
	c[0].z=sr*sy+cr*sp*cy ;
	c[1].x=sr*cp ;
	c[1].y=cr*cy+sr*sp*sy ;
	c[1].z=-cr*sy+sr*sp*cy ;
	c[2].x=-sp ;
	c[2].y=cp*sy ;
	c[2].z=cp*cy ;
}
RotationMatrix& RotationMatrix::fromKardanRPY
(const double yaw, const double pitch, const double roll){

  double cy=cos(yaw);
  double sy=sin(yaw);
  double cp=cos(pitch);
  double sp=sin(pitch);
  double cr=cos(roll);
  double sr=sin(roll);

  c[0].x=cr*cp ;
  c[0].y=-sr*cy+cr*sp*sy ;
  c[0].z=sr*sy+cr*sp*cy ;
  c[1].x=sr*cp ;
  c[1].y=cr*cy+sr*sp*sy ;
  c[1].z=-cr*sy+sr*sp*cy ;
  c[2].x=-sp ;
  c[2].y=cp*sy ;
  c[2].z=cp*cy ;

  return *this;
}
			
RotationMatrix& RotationMatrix::rotateX(const double angle)
{
  double c = cos(angle),
         s = sin(angle);
  *this *= RotationMatrix(Vector3<double>(1,0,0),
                          Vector3<double>(0,c,s),
                          Vector3<double>(0,-s,c));
  return *this;
}

RotationMatrix& RotationMatrix::rotateY(const double angle)
{
  double c = cos(angle),
         s = sin(angle);
  *this *= RotationMatrix(Vector3<double>(c,0,-s),
                          Vector3<double>(0,1,0),
                          Vector3<double>(s,0,c));
  return *this;
}

RotationMatrix& RotationMatrix::rotateZ(const double angle)
{
  double c = cos(angle),
         s = sin(angle);
  *this *= RotationMatrix(Vector3<double>(c,s,0),
                          Vector3<double>(-s,c,0),
                          Vector3<double>(0,0,1));
  return *this;
}

double RotationMatrix::getXAngle() const
{
  double h = sqrt(c[2].y * c[2].y + c[2].z * c[2].z);
  return h ? acos(c[2].z / h) * (c[2].y > 0 ? -1 : 1) : 0;
}

double RotationMatrix::getYAngle() const
{
  double h = sqrt(c[0].x * c[0].x + c[0].z * c[0].z);
  return h ? acos(c[0].x / h) * (c[0].z > 0 ? -1 : 1) : 0;
}

double RotationMatrix::getZAngle() const
{
  double h = sqrt(c[0].x * c[0].x + c[0].y * c[0].y);
  return h ? acos(c[0].x / h) * (c[0].y < 0 ? -1 : 1) : 0;
}
Vector3<double> RotationMatrix::getXYZAngles() const
{
	return Vector3<double>(getXAngle(), getYAngle(), getZAngle());
}
template <class V> In& operator>>(In& stream, Matrix3x3<V>& matrix3x3)
{
  //STREAM_REGISTER_BEGIN_EXT( matrix3x3);
  STREAM_EXT( stream, matrix3x3.c[0]);
  STREAM_EXT( stream, matrix3x3.c[1]);
  STREAM_EXT( stream, matrix3x3.c[2]);
  //STREAM_REGISTER_FINISH();
  return stream;
}

template <class V> Out& operator<<(Out& stream, const Matrix3x3<V>& matrix3x3)
{
  //STREAM_REGISTER_BEGIN_EXT( matrix3x3);
  STREAM_EXT( stream, matrix3x3.c[0]);
  STREAM_EXT( stream, matrix3x3.c[1]);
  STREAM_EXT( stream, matrix3x3.c[2]);
  //STREAM_REGISTER_FINISH();
  return stream;
}

In& operator>>(In& stream, RotationMatrix& rotationMatrix)
{
  //STREAM_REGISTER_BEGIN_EXT( rotationMatrix);
  STREAM_EXT( stream, rotationMatrix.c[0]);
  STREAM_EXT( stream, rotationMatrix.c[1]);
  STREAM_EXT( stream, rotationMatrix.c[2]);
  //STREAM_REGISTER_FINISH();
  return stream;
}

Out& operator<<(Out& stream, const RotationMatrix& rotationMatrix)
{
  //STREAM_REGISTER_BEGIN_EXT( rotationMatrix);
  STREAM_EXT( stream, rotationMatrix.c[0]);
  STREAM_EXT( stream, rotationMatrix.c[1]);
  STREAM_EXT( stream, rotationMatrix.c[2]);
  //STREAM_REGISTER_FINISH();
  return stream;
}
