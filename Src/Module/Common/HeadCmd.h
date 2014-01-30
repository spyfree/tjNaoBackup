#ifndef __HeadCmd_h_
#define __HeadCmd_h_

class HeadCmd
{
public:
	HeadCmd():pan(0), tilt(0), speed(1){}
	double pan; /**< left,right. +CCW, -CW*/
	double tilt; /**< up and down. +down, -up*/
	double speed; /**< Max joint speed, rad/s*/
};
#endif