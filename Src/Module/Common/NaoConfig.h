#ifndef __NaoConfig_h_
#define __NaoConfig_h_

//#include "Tools/Streams/Streamable.h"
#include "Platform/SystemCall.h"
#include "Tools/Math/Vector3.h"
//#include "Tools/Range.h"
/**
* @class NaoConfig
* A Singleton class represent the configuration of Nao.V2(old version)
* This Class cannot keep only one allocation, but we can make sure there is
* only one theNaoCfg in one thread.
*/
class NaoConfig
{
public:
	/**
	* Get an instance of the NaoConfig module,
	* AT MOST ONE INSTANCE IS EXSIT!
	*/
	static NaoConfig* getInstance();
	//static void initNaoConfig();
	/**
	* Destroy the NaoConfig Module
	* DONT FORGET TO CALL THIS FUNCTION!
	* TODO: Use smart pointer.
	*/
	//static void destroyNaoConfig();

//protected:
	NaoConfig();
	~NaoConfig();
private:
	PROCESS_WIDE_STORAGE_STATIC NaoConfig* theNaoCfg;
	void defaultInit();
public:
	enum{
		SUPPORT_MODE_LEFT = 0,
		SUPPORT_MODE_DOUBLE_LEFT,
		SUPPORT_MODE_RIGHT,
		SUPPORT_MODE_DOUBLE_RIGHT
	};
	/**LegID represents Legs*/
	enum LegID{
		LegIDLeft = -1,
		LegIDRight = 1
	};
	//static const int LegIDRight;
	//static const int LegIDLeft;
	//Limb Length (mm)
	double NeckOffsetZ;
	double HipOffsetZ;
	double ShoulderOffsetY;
	double HipOffsetY;
	double UpperArmLength;
	double ThighLength;
	double LowerArmLength;
	double TibiaLength;
	double ShoulderOffsetZ;
	double FootHeight;
	double HandOffsetX;  //newly add
	double HandOffsetZ;   //newly add
	//==Joints limitation
	double JUpHeadYaw,JDownHeadYaw;
	double JUpHeadPitch,JDownHeadPitch;
	double JUpRShoulderPitch,JDownRShoulderPitch;
	double JUpRShoulderRoll,JDownRShoulderRoll;
	double JUpRElbowYaw,JDownRElbowYaw;
	double JUpRElbowRoll,JDownRElbowRoll;
	double JUpLShoulderPitch,JDownLShoulderPitch;
	double JUpLShoulderRoll,  JDownLShoulderRoll;
	double JUpLElbowYaw,JDownLElbowYaw;
	double JUpLElbowRoll,JDownLElbowRoll;
	double JUpRHipYawPitch,JDownRHipYawPitch;
	double JUpRHipRoll,JDownRHipRoll;
	double JUpRHipPitch,JDownRHipPitch;
	double JUpRKneePitch,JDownRKneePitch;
	double JUpRAnklePitch,JDownRAnklePitch;
	double JUpRAnkleRoll,JDownRAnkleRoll;
	double JUpLHipYawPitch, JDownLHipYawPitch;
	double JUpLHipRoll,JDownLHipRoll;
	double JUpLHipPitch,JDownLHipPitch;
	double JUpLKneePitch,JDownLKneePitch;
	double JUpLAnklePitch,JDownLAnklePitch;
	double JUpLAnkleRoll,JDownLAnkleRoll;

	//Link Masses(kg)
	double MassChest;
	double MassHead;
	double MassUpperArm;
	double MassLowerArm;
	double MassThigh;
	double MassTibia;
	double MassFoot;
	double MassTotal;
	double MassNeck;  //newly add
	double MassShoulder;  //newly add
	double MassElbow;  //newly add
	double MassPelvis;
	double MassHip;
    double MassAnkle;  //newly add
	//==Mass center (m), relative to the parent link, in initial Robot coordinate
	Vector3<double> McChest;
	Vector3<double> McHead;
	Vector3<double> McUpperArmRight;
	Vector3<double> McUpperArmLeft;
	Vector3<double> McLowerArmRight;
	Vector3<double> McLowerArmLeft;
	Vector3<double> McThighRight;
	Vector3<double> McThighLeft;
	Vector3<double> McTibiaRight;
	Vector3<double> McTibiaLeft;
	Vector3<double> McFootRight;
	Vector3<double> McFootLeft;
	Vector3<double> McNeck;  //newly add
	Vector3<double> McRightShoulder;  //newly add
    Vector3<double> McLeftShoulder;   //newly add
	Vector3<double> McRightElbow;  //newly add
    Vector3<double> McLeftElbow;  //newly add
	Vector3<double> McRightPelvis;   //newly add
    Vector3<double> McLeftPelvis;   //newly add
	Vector3<double> McRightHip;   //newly add
	Vector3<double> McLeftHip;   //newly add
    Vector3<double> McRightAnkle;  //newly add
	Vector3<double> McLeftAnkle;  //newly add

};

Out& operator<<(Out& stream, const NaoConfig* naoCfg);
#endif //__NaoConfig_h_
