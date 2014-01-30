/**
 * @file Math/Common.h
 *
 * This contains some often used mathematical definitions and functions.
 *
 * @author <a href="mailto:martin.kallnik@gmx.de">Martin Kallnik</a>
 * @author Max Risler
 */


#ifndef __Math_Common_h__
#define __Math_Common_h__

#include <cmath>
#include <cstdlib>


/**
* defines the sign of a
*/
#ifndef sign
#define sign(a)   ( (a) < 0 ? -1 : ((a)==0) ? 0 : 1 )
#endif

/**
* defines the square of a value
*/
#ifndef sqr
#define sqr(a) ( (a) * (a) )
#endif

inline double sec(const double a){return 1/cos(a);}

inline double cosec(const double a){return 1/sin(a);}

static const int VERY_LARGE_INT = 2000000000; //2 billions

/** @name constants for some often used angles */
///@{
/** constant for a half circle*/
const double pi = 3.1415926535897932384626433832795;
/** constant for a full circle*/
const double pi2 = 2.0*3.1415926535897932384626433832795;
/** constant for three quater circle*/
const double pi3_2 = 1.5*3.1415926535897932384626433832795;
/** constant for a quarter circle*/
const double pi_2 = 0.5*3.1415926535897932384626433832795;
/** constant for a 1 degree*/
const double pi_180 = 3.1415926535897932384626433832795/180;
/** constant for a 1/8 circle*/
const double pi_4 = 3.1415926535897932384626433832795*0.25;
/** constant for a 3/8 circle*/
const double pi3_4 = 3.1415926535897932384626433832795*0.75;
/** constant for an expression used by the gaussian function*/
const double sqrt2pi = sqrt(2.0*pi);
///@}

static const double DEG_OVER_RAD = 180 / pi;

/*
* spits out the 95% confidence factor of the variance of
* of a given value
**/
inline double get95CF(double standardDeviation) {
	return sqr(standardDeviation) / 4;
}

/**
 * Converts angle from rad to degrees.
 * \param angle code in rad
 * \return angle coded in degrees
 */
inline double rad2deg(double angle){return angle * 180.0 / pi;}

/** Converts angle from degrees to rad.
 * \param degrees angle coded in degrees
 * \return angle coded in rad
 */
inline double deg2rad(double degrees){return degrees * pi_180;}

/**
* reduce angle to [-pi..+pi[
* \param data angle coded in rad
* \return normalized angle coded in rad
*/
inline double normalize(double data)
{
  if (data < pi && data >= -pi) return data;
  double ndata = data - ((int )(data / pi2))*pi2;
  if (ndata >= pi)
  {
    ndata -= pi2;
  }
  else if (ndata < -pi)
  {
    ndata += pi2;
  }
  return ndata;
}

/**
* The function returns a random number in the range of [0..1].
* @return The random number.
*/
inline double randomDouble() {return double(rand()) / RAND_MAX;}

/**
* The function returns a random integer number in the range of [0..n-1].
* @param n the number of possible return values (0 ... n-1)
* @return The random number.
*/
inline int random(int n) {return (int)(randomDouble()*n*0.999999);}

/** constant, cast before execution*/
const double RAND_MAX_DOUBLE = static_cast<double>(RAND_MAX);

/**
* The function returns a random integer number in the range of [0..n].
* @param n the number of possible return values (0 ... n)
* @return The random number.
*/
inline int randomFast(int n)
{
  return static_cast<int>((rand()*n) / RAND_MAX_DOUBLE);
}

/**
* Round to the next integer
* @param d A number
* @return The number as integer
*/
inline int roundNumberToInt(double d)
{
  return static_cast<int>(floor(d+0.5));
}

/**
* Round to the next integer but keep type
* @param d A number
* @return The number
*/
inline double roundNumber(double d)
{
  return floor(d+0.5);
}

inline double NormalizeAngle_180(double angle) 
{
	if (angle < -180) 
	{
		angle += 360;
	}
	if (angle >= 180) 
	{
		angle -= 360;
	}
	return angle;
}

template <class T>
inline static T ABS(const T &x) {
	return (x > 0 ? x : -x);
}

/**
* CLIP a number within a range
* @param x A number
* @param bound [-bound, bound]
*/
template <class T>
inline T CLIP(const T &x, const T &bound) {
	/* clips (+/-) x to bound, assume bound is positive */
	return ((x <-bound || x> bound) ? bound * sign(x) : x);
}
/**
* CLIP a number within a range
* @param x A number
* @param lowerBound The lower bound
* @param upperBound The upper bound
*/
template <class T>
inline T CLIP(const T& x, const T& lowerBound, const T& upperBound) {
	// similar with CLIP, but distinguish lower and upper bounds
	if (lowerBound <= upperBound) {
		// if bounds are in order
		if (x < lowerBound)
			return lowerBound;
		else if (x > upperBound)
			return upperBound;
		else
			return x;
	} else {
		// if upperBound < lowerBound
		if (x < upperBound)
			return upperBound;
		else if (x > lowerBound)
			return lowerBound;
		else
			return x;
	}
}

enum{
	Interpolate_Linear,
	Interpolate_Smooth
};
/**
* Interpolation function
* @param p1 first point,
* @param p2 The second point
* @param ratio The ratio from the p1 to p2
* @param inerp_mod Interpolate_Linear or Interpolate_Smooth
*/
template<class T>
inline T interpolate(const T& p1, const T& p2, double ratio, int inerp_mod)
{
	// TODO: add smooth interpolation
	return (p2 - p1) * ratio + p1;
}


  typedef int             int32;
  typedef unsigned int    uint32;
  typedef short           int16;
  typedef unsigned short  uint16;
  typedef char            int8;
  typedef unsigned char   uint8;
#else
  typedef int             int32;
  typedef unsigned int    uint32;
  typedef short           int16;
  typedef unsigned short  uint16;
  typedef char            int8;
  typedef unsigned char   uint8;
#endif // __Math_Common_h__
