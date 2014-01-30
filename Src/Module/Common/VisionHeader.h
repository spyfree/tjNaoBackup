#ifndef VISIONHEADER_H
#define VISIONHEADER_H
#ifndef PI
#define PI 3.14159265358979323846
#endif
#include "Tools/Math/Common.h"
//image parament
#define   FULL_IMAGE_WIDTH 320
#define FULL_IMAGE_HEIGHT 240
const unsigned int IMAGE_WIDTH = 320;
const unsigned int IMAGE_HEIGHT = 240;
const unsigned int IMAGE_SIZE = IMAGE_WIDTH*IMAGE_HEIGHT;
//==================================
// TODO: The Camera Related Information definition is temporary, move them to a CameraInfo Class!!!
//Use A Camera Information Class to replace the Config here
// From camera docs, in mm:
const float IMAGE_WIDTH_MM = 2.46f;//2.36f;
const float IMAGE_HEIGHT_MM = 1.85f;//1.76f;


////////////以下的FOV_X_DEG 和FOVx两个参数来由不详，实际计算中已经不用这两个参数
#define FOV_X_DEG 46.4f//but Behuman's FOV_X_DEG is 47.8 degree
#define FOVx (deg2rad(FOV_X_DEG))
// Calculated from numbers in camera docs, in mm:


/////////NOTICE  :Update By WWJ:2011-1-5/////////////////////////////////
/*****************************************************************/
/*******************The algrithom of Focal_Length************************/
/*****************************************************************/
/*In the aldebaran forum ,someone has done the measurements:

In the distance of 1 meter to the camera (as orthogonal to this distance as i could manage):
width: 83cm, height: 62cm, diagonal: 102cm.
so the degree for the width /height/diagonal are as below:

2 * atan(83/100/2) = 45°
2 * atan(62/100/2) = 34.45°
2 * atan(102/100/2) = 54.04°

The detail for the information can be access from the website:
http://robocup.aldebaran-robotics.com/index.php?option=com_fireboard&Itemid=31&func=view&id=1281&catid=36

*/
/*In GT2010,the openingAngleWidth and the openingAngleHeight is the final value of the camera angle,these can be calculate the Focal_Length
//openingAngleWidth = 0.78674f; // 45.08?//60.97 1.064127
//openingAngleHeight  = 0.60349f; // 34.58?//47.64 0.831475

Because the IMAGE_WIDTH_MM and IMAGE_HEIGHT_MM is the actual value of the camera plate(感光片的实际尺寸),
so Focal_Length=(IMAGE_WIDTH_MM/2)/tan(openingAngleWidth/2)*320/IMAGE_WIDTH_MM(此处焦距采用的是像素点的单位，而不是mm的单位)
			=385.54
*/
/*****************************************************************/
/**************The end of algrithom of Focal_Length************************/
/*****************************************************************/

const float FOCAL_LENGTH = 271.7889;//385.54;//361.06;//(float)(IMAGE_WIDTH_MM/2) * MM_TO_PIX_X / tan(FOV_X_DEG/2);// MM_TO_PIX_X=135.59 , and FOCAL_LENGTH = 361.06 pix;FOCAL_LENGTH is in pix unit
const float FOCAL_LENGTH_INV=1/FOCAL_LENGTH;
const double BALL_DIAMETER = 65.0;
const double BALL_RADIUS=65.0/2.0;
const double GoalPostWidthInMM=100;
const double GoalHeightInMM=800;
const double GoalWidthInMM=1400;
enum Colour {
	c_UNKNOWN = 0,
	c_FIELD_GREEN = 1,
	c_WHITE = 2,
	c_BALL_ORANGE = 3,
	c_ROBOT_RED = 4,
	c_ROBOT_BLUE = 5,
	c_BEACON_GREEN = 6,//GARY
	c_BEACON_BLUE = 7,
	c_BEACON_PINK = 8,
	c_BEACON_YELLOW = 9
};
enum ColorID {
	BLACK = 0,
	FIELD = 1,
	WHITE = 2,
	ORANGE = 3,
	RED = 4,
	BLUE = 5,
	GRAY = 6,
	CYAN = 7,
	PINK = 8,
	YELLOW = 9,
	CYANGREEN=10,
	YELLOWWHITE=11,
	REDORANGE=12,
	ORANGEYELLOW=13
};
#endif // VISIONHEADER_H
