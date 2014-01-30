#ifndef __GroundContactState_h_
#define __GroundContactState_h_
class GroundContactState
{
public:
	GroundContactState() : contact(false), contactSafe(false) { }
	bool contact;
	bool contactSafe;
};
#endif