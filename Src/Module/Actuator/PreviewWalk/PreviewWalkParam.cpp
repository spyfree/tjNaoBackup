#include "PreviewWalkParam.h"
#include "Tools/Math/Common.h"
#include <fstream>
#include <iostream>
PreviewWalkParam::PreviewWalkParam()
{
	setDefaultParams();
}
PreviewWalkParam::~PreviewWalkParam()
{

}
void PreviewWalkParam::setDefaultParams()
{
	g = 9.81;
	dt = 0.02;
	Zc = 0.23;//change to pCOM(3)
	sx = 0.05;
	sy = 0.07;
	srot = 0;
	torsoAngleY = deg2rad(3.0);
	PG = 54;
	numStages = 4;
	double tmplen[4] = {0.3, 0.1, 0.3, 0.1};
	for(int i = 0; i < numStages; i ++)
		sLen[i] = tmplen[i];
	ZmpOffsetRight = Vector2<double>(0.005,0.015);//0.01
	ZmpOffsetLeft = Vector2<double>(0.005,-0.015);//-0.01

	stepHeight = 0.12;//抬脚的高度
	//===set Arms Swing===//
	useArmSwing = DISABLE;
	int startArm = JointData::RShoulderPitch;
	armSwing[JointData::RShoulderPitch - startArm].setParam(true, deg2rad(90), deg2rad(40), 1);
	armSwing[JointData::RShoulderRoll - startArm].setParam(false, deg2rad(-30), 0, 1);
	armSwing[JointData::RElbowYaw - startArm].setParam(false, deg2rad(90), 0, 1);
	armSwing[JointData::RElbowRoll - startArm].setParam(false, deg2rad(90), 0, 1);
	armSwing[JointData::LShoulderPitch - startArm].setParam(true, deg2rad(90), deg2rad(40), -1);
	armSwing[JointData::LShoulderRoll - startArm].setParam(false, deg2rad(30), 0, 1);
	armSwing[JointData::LElbowYaw - startArm].setParam(false, deg2rad(-90), 0, 1);
	armSwing[JointData::LElbowRoll - startArm].setParam(false, deg2rad(-90), 0, 1);
	//================//
	pRHipRollCompensator = deg2rad(4.5);
	pLHipRollCompensator = deg2rad(-4.5);
	useHipRollCompensation = DISABLE;
	useJointsCalibration = DISABLE;
	useHipAnkleRollScale = DISABLE;
	scaleRHipRoll=0; scaleLHipRoll=0;
	scaleRAnkleRoll = 0; scaleLAnkleRoll = 0;
	useHipRollClip = DISABLE;
	clipRHipRollLow = deg2rad(-100); clipRHipRollUp = deg2rad(100); 
	clipLHipRollLow = deg2rad(-100); clipLHipRollUp = deg2rad(100); 
	finalize();
}

void PreviewWalkParam::finalize()
{
	stepX = sx+abs(ZmpOffsetRight.x - ZmpOffsetLeft.x);//注意, 应该用单足支撑和双足支撑下行走的距离来计算
	stepY = sy +abs(ZmpOffsetRight.y - ZmpOffsetLeft.y);

	double sum = 0;
	for(int i = 0; i < numStages; i ++)
	{
		sum += sLen[i];
	}
	for(int i = 0; i < numStages; i ++)
	{
		sLen[i] = sLen[i] / sum;
	}
}

bool PreviewWalkParam::load(const std::string& filename)
{
	bool readOK = true;
	std::ifstream fin(filename.c_str());
	std::cout<<"Reading Preview Walk Config File: "<<filename.c_str()<<" ...       ";
	if(fin){
		char buf[500];
		const int numElements = 78;
		double dbuf[numElements];
		fin >> buf;
		if(strcmp(buf, "#Preview_Walk_Config_Params")  != 0)
		{
			std::cout<<"Now a Preview Walk Config File :"<<filename.c_str()<< std::endl;
			readOK = false;
		}
		for(int i = 0; i < numElements; ++i)
		{
			// ignore space
			while(isspace(fin.peek()) || fin.peek() == ',')
				fin.get();
			// ignore lines beginning with '#'
			if(fin.peek() == '#'){
				while(fin.peek() != '\n')
					fin.get();
				--i;
				continue;
			}
			//read num
			fin >> dbuf[i];
			//check read ok
			if(! fin.good()){
				std::cout<<"Error in Preview Walk Config file "<< filename.c_str()<< " - unexpected EOF" << std::endl;
				readOK = false;
				break;
			}
			//version check 
			if(i == 0 && dbuf[0] != 1.1){
				std::cout<<"Preview Walk Config Version Error, Version "<<1.1<<" is Required"<<std::endl;
				readOK =  false;
				break;
			}
		}
		if(readOK){
			//pop data in buffer into params
			PG =static_cast<int>(dbuf[1]);
			g = dbuf[2];
			dt = dbuf[3];
			Zc = dbuf[4];
			sx = dbuf[5];
			sy = dbuf[6];
			srot = dbuf[7];
			torsoAngleY = deg2rad(dbuf[8]);
			numStages = static_cast<int>(dbuf[9]);
			int i = 0;
			for(i = 0; i < numStages; ++i)
			{
				sLen[i] = dbuf[10 + i];
			}
			ZmpOffsetRight.x = dbuf[10+numStages];
			ZmpOffsetRight.y = dbuf[10+numStages+1];
			ZmpOffsetLeft.x = dbuf[10+numStages +2];
			ZmpOffsetLeft.y = dbuf[10+numStages +3];
			stepHeight = dbuf[10+numStages+4];
			useHipRollCompensation = static_cast<int>(dbuf[10+numStages+5]);
			pRHipRollCompensator = deg2rad(dbuf[10+numStages+6]);
			pLHipRollCompensator = deg2rad(dbuf[10+numStages+7]);
			useHipAnkleRollScale = static_cast<int>(dbuf[10+numStages+8]);
			scaleRHipRoll = dbuf[10+numStages+9];
			scaleLHipRoll = dbuf[10+numStages+10];
			scaleRAnkleRoll = dbuf[10+numStages+11];
			scaleLAnkleRoll = dbuf[10+numStages+12];
			useHipRollClip = static_cast<int>(dbuf[10+numStages+13]);
			clipRHipRollLow = deg2rad(dbuf[10+numStages+14]);
			clipRHipRollUp = deg2rad(dbuf[10+numStages+15]);
			clipLHipRollLow = deg2rad(dbuf[10+numStages+16]);
			clipLHipRollUp = deg2rad(dbuf[10+numStages+17]);
			useJointsCalibration = static_cast<int>(dbuf[10+numStages+18]);
			for(i = 0; i < 12; ++i)
			{
				jointsCalibration[i].offset = deg2rad(dbuf[10+numStages+19 + i]);
			}
			useArmSwing = static_cast<int>(dbuf[10+numStages+31]);
			int startArm = JointData::RShoulderPitch;
			bool swing;
			double offset, amp;
			int swingSign;
			swing = fabs(dbuf[10+numStages+32]-1.0) <0.1?true:false;
			offset = deg2rad(dbuf[10+numStages+33]);
			amp = deg2rad(dbuf[10+numStages+34]);
			swingSign = static_cast<int>(dbuf[10+numStages+35]);
			armSwing[JointData::RShoulderPitch - startArm].setParam(swing, offset, amp, swingSign);
			swing = fabs(dbuf[10+numStages+36]-1.0) <0.1?true:false;
			offset = deg2rad(dbuf[10+numStages+37]);
			amp = deg2rad(dbuf[10+numStages+38]);
			swingSign = static_cast<int>(dbuf[10+numStages+39]);
			armSwing[JointData::RShoulderRoll - startArm].setParam(swing, offset, amp, swingSign);
			swing = fabs(dbuf[10+numStages+40]-1.0) <0.1?true:false;
			offset = deg2rad(dbuf[10+numStages+41]);
			amp = deg2rad(dbuf[10+numStages+42]);
			swingSign = static_cast<int>(dbuf[10+numStages+43]);
			armSwing[JointData::RElbowYaw - startArm].setParam(swing, offset, amp, swingSign);
			swing = fabs(dbuf[10+numStages+44]-1.0) <0.1?true:false;
			offset = deg2rad(dbuf[10+numStages+45]);
			amp = deg2rad(dbuf[10+numStages+46]);
			swingSign = static_cast<int>(dbuf[10+numStages+47]);
			armSwing[JointData::RElbowRoll - startArm].setParam(swing, offset, amp, swingSign);
			swing = fabs(dbuf[10+numStages+48]-1.0) <0.1?true:false;
			offset = deg2rad(dbuf[10+numStages+49]);
			amp = deg2rad(dbuf[10+numStages+50]);
			swingSign = static_cast<int>(dbuf[10+numStages+51]);
			armSwing[JointData::LShoulderPitch - startArm].setParam(swing, offset, amp, swingSign);
			swing = fabs(dbuf[10+numStages+52]-1.0) <0.1?true:false;
			offset = deg2rad(dbuf[10+numStages+53]);
			amp = deg2rad(dbuf[10+numStages+54]);
			swingSign = static_cast<int>(dbuf[10+numStages+55]);
			armSwing[JointData::LShoulderRoll - startArm].setParam(swing, offset, amp, swingSign);
			swing = fabs(dbuf[10+numStages+56]-1.0) <0.1?true:false;
			offset = deg2rad(dbuf[10+numStages+57]);
			amp = deg2rad(dbuf[10+numStages+58]);
			swingSign = static_cast<int>(dbuf[10+numStages+59]);
			armSwing[JointData::LElbowYaw - startArm].setParam(swing, offset, amp, swingSign);
			swing = fabs(dbuf[10+numStages+60]-1.0) <0.1?true:false;
			offset = deg2rad(dbuf[10+numStages+61]);
			amp = deg2rad(dbuf[10+numStages+62]);
			swingSign = static_cast<int>(dbuf[10+numStages+63]);
			armSwing[JointData::LElbowRoll - startArm].setParam(swing, offset, amp, swingSign);
			finalize();
			std::cout<<"DONE!"<<std::endl;
		}
	}else
	{
		std::cout<<"Preview Config File :"<<filename.c_str()<<" NOT EXIST! "<<std::endl;
		readOK = false;
	}
	fin.close();
	return readOK;
}
