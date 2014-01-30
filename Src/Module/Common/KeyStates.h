#ifndef __KeyStates_h_
#define __KeyStates_h_

#include "Tools/Streams/Streamable.h"

/**
* The class represents the states of the keys.
*/
class KeyStates : public Streamable
{
private:
	void serialize(In* in, Out* out)
	{
		//STREAM_REGISTER_BEGIN();
		STREAM_ARRAY(pressed);
		//STREAM_REGISTER_FINISH();
	}

public:
	mutable int numChestClick;
	mutable int numLeftClick;
	mutable int numRightClick;
	enum Key
	{
		RBumperRight,   /**< Right Bumper, Right Button*/
		RBumperLeft,    /**< Right Bumper, Left Button*/
		LBumperRight, /**< Left Bumper, Right Button*/
		LBumperLeft,    /**< Left Bumper, Left Button*/
		ChestButton,    /**<Chest Button*/
		numberOfKeys
	};

	bool pressed[numberOfKeys];

	KeyStates()
	{
		for(int i = 0; i < numberOfKeys; ++i)
		{
			pressed[i] = false;
		}
	}

	static const char* getKeyName(Key key)
	{
		switch(key)
		{
		case RBumperRight: return "RBumperRight";
		case RBumperLeft: return "RBumperLeft";
		case LBumperRight: return "LBumperRight";
		case LBumperLeft: return "LBumperLeft";
		case ChestButton: return "ChestButton";
		default: return "unknown";
		}
	}
};
#endif //__KeyStates_h_
