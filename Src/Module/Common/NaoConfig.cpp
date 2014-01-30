#include "NaoConfig.h"
#include "Tools/Math/Common.h"
#include "Tools/Streams/OutStreams.h"
//problem in linux
PROCESS_WIDE_STORAGE NaoConfig* NaoConfig::theNaoCfg = NULL;
//const int NaoConfig::LegIDRight = 1;
//const int NaoConfig::LegIDLeft = -1;
NaoConfig::NaoConfig()
{
	theNaoCfg = this;
	defaultInit();
}

NaoConfig::~NaoConfig()
{
	theNaoCfg = NULL;
}

NaoConfig* NaoConfig::getInstance()
{
	OutTextCmd out;
	if(theNaoCfg == NULL)
	{
//		out << "Allocate a new NaoConfig()..., THIS should not  happen!!!!" << endl;
		new NaoConfig();
	}
	//out << "getting a NaoConfig instance..." << endl;
	return theNaoCfg;
}
// void NaoConfig::initNaoConfig()
// {
// 	getInstance();
// }
// void NaoConfig::destroyNaoConfig()
// {
// 	if(theNaoCfg != NULL)
// 	{
// 		delete theNaoCfg;
// 		theNaoCfg = NULL;
// 	}
// }

void NaoConfig::defaultInit()
{
	//Limbs Length(mm)
	NeckOffsetZ =126.5/1000; HipOffsetZ =85.0/1000;
	ShoulderOffsetY =98.0/1000; HipOffsetY= 50.0/1000;
	UpperArmLength =105.0/1000; ThighLength =100.0/1000;
	LowerArmLength =55.95/1000; TibiaLength =102.90/1000;  //LowerArmLength =135.0/1000;old
	ShoulderOffsetZ =100.0/1000; FootHeight=45.19/1000;
	HandOffsetX = 57.75/1000; HandOffsetZ = 12.31/1000;
	//ElbowOffsetY = 15.00/1000;
	//==Joints limitation
	JUpHeadYaw=deg2rad(-119.5);  JDownHeadYaw=deg2rad(119.5);
	JUpHeadPitch=deg2rad(-38.5);  JDownHeadPitch=deg2rad(29.5);
	JUpRShoulderPitch=deg2rad(-119.5);  JDownRShoulderPitch=deg2rad(119.5);
	JUpRShoulderRoll=deg2rad(-76.0);  JDownRShoulderRoll=deg2rad(18.0);
	JUpRElbowYaw=deg2rad(-119.5);  JDownRElbowYaw=deg2rad(119.5);
	JUpRElbowRoll=deg2rad(2.0);  JDownRElbowRoll=deg2rad(88.5);
	JUpLShoulderPitch=deg2rad(-119.5);  JDownLShoulderPitch= deg2rad(119.5);
	JUpLShoulderRoll=deg2rad(-18.0);  JDownLShoulderRoll=deg2rad(76.0);
	JUpLElbowYaw=deg2rad(-119.5);  JDownLElbowYaw=deg2rad(119.5);
	JUpLElbowRoll=deg2rad(-88.5);  JDownLElbowRoll=deg2rad(-2);
	JUpRHipYawPitch=deg2rad(-65.62);  JDownRHipYawPitch=deg2rad(42.44);
	JUpRHipRoll=deg2rad(-42.30);  JDownRHipRoll=deg2rad(23.76);
	JUpRHipPitch=deg2rad(-101.54);  JDownRHipPitch=deg2rad(27.82);
	JUpRKneePitch=deg2rad(-5.9);  JDownRKneePitch=deg2rad(121.47);
	JUpRAnklePitch=deg2rad(-67.97);  JDownRAnklePitch=deg2rad(53.40);
	JUpRAnkleRoll=deg2rad(-45.03);  JDownRAnkleRoll=deg2rad(22.27);
	JUpLHipYawPitch=deg2rad(-65.62);  JDownLHipYawPitch=deg2rad(42.44);
	JUpLHipRoll=deg2rad(-21.74);  JDownLHipRoll=deg2rad(45.29);
	JUpLHipPitch=deg2rad(-101.63);  JDownLHipPitch=deg2rad(27.73);
	JUpLKneePitch=deg2rad(-5.29);  JDownLKneePitch=deg2rad(121.04);
	JUpLAnklePitch=deg2rad(-68.15);  JDownLAnklePitch=deg2rad(52.86);
	JUpLAnkleRoll=deg2rad(-22.79);  JDownLAnkleRoll=deg2rad(44.06);
	//Link Masses(kg)5
	MassChest = 1049.6/1000;
	MassHead =605.33/1000;
	MassUpperArm= 157.94/1000;
	MassLowerArm =184.05/1000;
	MassThigh =389.76/1000;
	MassTibia =291.63/1000;
	MassFoot =161.71/1000;
	MassTotal =5029.339/1000;
	MassNeck = 64.42/1000;           //newly add
	MassShoulder = 69.84/1000;            //newly add
	MassElbow=64.83/1000;               //newly add
	MassPelvis = 71.18/1000;
	MassHip = 130.53/1000;
	MassAnkle = 134.15/1000;              //newly add
	//==Mass center (m), relative to the parent link, in initial Robot coordinate
	McChest =Vector3<double>(-4.13/1000, 0.09/1000, 43.42/1000);
	McHead =Vector3<double>(1.12/1000,-0.03/1000,52.58/1000);
	McUpperArmRight =Vector3<double>(24.29/1000,-9.52/1000,3.2/1000);
	McUpperArmLeft =Vector3<double>(24.55/1000,5.63/1000,3.3/1000);
	McLowerArmRight =Vector3<double>(65.3/1000,1.14/1000,0.51/1000);
	McLowerArmLeft =Vector3<double>(65.3/1000,1.14/1000,0.51/1000);
	McThighRight=Vector3<double>(1.39/1000,-2.25/1000,-53.74/1000);
	McThighLeft =Vector3<double>(1.38/1000,2.21/1000,-53.73/1000);
	McTibiaRight=Vector3<double>(3.94/1000,-2.21/1000,-49.38/1000);
	McTibiaLeft=Vector3<double>(4.53/1000,2.25/1000,-49.36/1000);
	McFootRight=Vector3<double>(25.4/1000,-3.32/1000,-32.39/1000);
	McFootLeft=Vector3<double>(25.42/1000,3.30/1000,-32.39/1000);
	McNeck = Vector3<double>(-0.01/1000, 0.14/1000, -27.42/1000);  //newly add
	McRightShoulder = Vector3<double>(-1.65/1000,26.63/1000,0.14/1000) ;  //newly add
	McLeftShoulder = Vector3<double>(-1.65/1000,-26.63/1000,0.14/1000) ;  //newly add
	McRightElbow = Vector3<double>(-27.44/1000,0.00/1000,-0.14/1000);  //newly add
	McLeftElbow = Vector3<double>(-27.44/1000,-0.00/1000,-0.14/1000);  //newly add
	McRightPelvis = Vector3<double>(-7.66/1000, 12.00/1000, 27.16/1000);  //newly add
    McLeftPelvis = Vector3<double>(-7.81/1000, -11.14/1000, 26.61/1000);  //newly add
	McRightHip = Vector3<double>(-15.49/1000, -0.29/1000, -5.16/1000);  //newly add
	McLeftHip = Vector3<double>(-15.49/1000, 0.29/1000, -5.15/1000);  //newly add
	McRightAnkle = Vector3<double>(0.45/1000,-0.3/1000,6.84/1000);  //newly add
	McLeftAnkle = Vector3<double>(0.45/1000,0.29/1000,6.85/1000);  //newly add


}

Out& operator<<(Out& stream, const NaoConfig* naoCfg)
{
	if(naoCfg != NULL)
	{
		stream << naoCfg->McChest << endl;
		stream << naoCfg->McHead << endl;
		stream << naoCfg->McUpperArmRight << endl;
		stream << naoCfg->McUpperArmLeft << endl;
		stream << naoCfg->McLowerArmRight << endl;
		stream << naoCfg->McLowerArmLeft << endl;
		stream << naoCfg->McThighRight << endl;
		stream << naoCfg->McThighLeft << endl;
		stream << naoCfg->McTibiaRight << endl;
        stream << naoCfg->McTibiaLeft << endl;  //newly add
		stream << naoCfg->McFootRight << endl;  //newly add
		stream << naoCfg->McFootLeft << endl;  //newly add
		stream << naoCfg->McNeck << endl;  //newly add
		stream << naoCfg->McRightShoulder << endl;  //newly add
		stream << naoCfg->McLeftShoulder << endl;  //newly add
		stream << naoCfg->McRightElbow << endl;  //newly add
		stream << naoCfg->McLeftElbow << endl;  //newly add
		stream << naoCfg->McRightPelvis << endl;  //newly add
		stream << naoCfg->McLeftPelvis << endl;  //newly add
		stream << naoCfg->McRightHip << endl;  //newly add
		stream << naoCfg->McLeftHip << endl;  //newly add
		stream << naoCfg->McRightAnkle << endl;  //newly add
		stream << naoCfg->McLeftAnkle << endl;  //newly add

	}
	return stream;
}
