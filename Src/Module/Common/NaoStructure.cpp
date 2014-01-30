#include "NaoStructure.h"
#include "NaoConfig.h"
NaoStructure::Linkage::Linkage()
	:name(""), jointID(JointData::NoneJoint), sister(noneLink),
	child(noneLink), mother(noneLink), q(0), dq(0),ddq(0),
	m(0)
{

}

// NaoStructure::Linkage& NaoStructure::Linkage::operator=(Linkage& other);


/************************************************************************/
/*            Class NaoStructure                                                                     */
/************************************************************************/
NaoStructure::NaoStructure()
:supportingMode(NaoConfig::SUPPORT_MODE_RIGHT),
lastSupportingMode(NaoConfig::SUPPORT_MODE_RIGHT)
{
	//supportFootPosWorld = Vector3<double>(0,0,0);
	//supportFootRotWorld = Vector3<double>(0,0,0);
	buildNaoStructure();
	linkFromJoints[JointData::NoneJoint] = noneLink;
	for(int i = noneLink; i < numULinks; ++i)
	{
		if(uLink[i].jointID != JointData::NoneJoint)
		{
			linkFromJoints[uLink[i].jointID] = i;
		}
	}
}
void NaoStructure::setLinksJoints(const JointData& joints)
{
	for(int i = noneLink; i < numULinks; ++i)
	{
		if(uLink[i].jointID != JointData::NoneJoint)
		{
			uLink[i].q = joints.angles[uLink[i].jointID];
		}
	}
}
void NaoStructure::buildNaoStructure()
{
	NaoConfig* theNaoCfg = NaoConfig::getInstance();
	uLink[1].name = "BodyRollX";					//名字
	uLink[1].jointID = JointData::NoneJoint;
	uLink[1].sister = 0;							//姐妹连杆编号
	uLink[1].child = 2;								//子连杆编号
	uLink[1].mother = 1;							//母连杆编号
	uLink[1].p = Vector3<double>(0,0,0);           //在世界坐标系中位置(m)
	uLink[1].R = RotationMatrix();					//在世界坐标系中姿态, 旋转
	uLink[1].v = Vector3<double>(0,0,0);           //在世界坐标系中速度(m/s)
	uLink[1].w = Vector3<double>(0,0,0);			//在世界坐标系中角速度(rad/s)
	uLink[1].q = 0;									 //关节角
	uLink[1].dq = 0;								//关节速度
	uLink[1].ddq = 0;								//关节角加速度
	uLink[1].a = Vector3<double>(1,0,0);         //关节轴矢量(相对于母杆? 具体见骨架图)
	uLink[1].aw = Vector3<double>(1,0,0);        //关节轴矢量(世界坐标系中)
	uLink[1].b = Vector3<double>(0,0,0);           //相对位置(相对于母连杆)
	uLink[1].m = 0;									 //质量(kg)
	uLink[1].c = Vector3<double>(0,0,0);           //质心位置(在连杆局部坐标系中)
	uLink[1].I = createZeroIM();             //惯性张量(在连杆局部坐标系中)
	uLink[1].vertex.resize(0);							//形状(顶点信息,在局部坐标系中)
	//uLink[1].face = 0;								//形状(连接情况)
	uLink[1].chainID = CBody;
	//==
	uLink[2].name = "BodyPitchY";
	uLink[2].jointID = JointData::NoneJoint;
	uLink[2].sister = 0;
	uLink[2].child = 3;
	uLink[2].mother = 1;
	uLink[2].p = Vector3<double>(0, 0, 0);
	uLink[2].R = RotationMatrix();
	uLink[2].v = Vector3<double>(0, 0, 0);
	uLink[2].w = Vector3<double>(0, 0, 0);
	uLink[2].q = 0;
	uLink[2].dq = 0;
	uLink[2].ddq = 0;
	uLink[2].a = Vector3<double>(0, 1, 0);
	uLink[2].aw = Vector3<double>(0, 1, 0);
	uLink[2].b = Vector3<double>(0, 0, 0);
	uLink[2].m = 0;
	uLink[2].c = Vector3<double>(0, 0, 0);
	uLink[2].I = createZeroIM();
	uLink[2].vertex.resize(0);
	//uLink[2].face = 0;
	uLink[2].chainID = CBody;
	//==
	uLink[3].name = "BodyYawZ";   //躯干
	uLink[3].jointID = JointData::NoneJoint;
	uLink[3].sister = 6;
	uLink[3].child = 4;
	uLink[3].mother = 2;
	uLink[3].p = Vector3<double>(0, 0, 0);
	uLink[3].R = RotationMatrix();
	uLink[3].v = Vector3<double>(0, 0, 0);
	uLink[3].w = Vector3<double>(0, 0, 0);
	uLink[3].q = 0;
	uLink[3].dq = 0;
	uLink[3].ddq = 0;
	uLink[3].a = Vector3<double>(0, 0, 1);
	uLink[3].aw = Vector3<double>(0, 0, 1);
	uLink[3].b = Vector3<double>(0, 0, 0);
	uLink[3].m = theNaoCfg->MassChest;
	uLink[3].c = theNaoCfg->McChest;
	uLink[3].I = createBoxIM(uLink[3].m, 0.1, 0.1, 0.18, Vector3<double>(0,0,0));
	uLink[3].vertex.resize(0);
	//uLink[3].face = 0;
	uLink[3].chainID = CBody;
	//==
	uLink[4].name = "HeadYawZ";     //Neck  颈部
	uLink[4].jointID = JointData::HeadYaw;
	uLink[4].sister = 0;
	uLink[4].child = 5;
	uLink[4].mother = 3;
	uLink[4].p = Vector3<double>(0,0,0);
	uLink[4].R = RotationMatrix();
	uLink[4].v = Vector3<double>(0,0,0);
	uLink[4].w = Vector3<double>(0,0,0);
	uLink[4].q = 0;
	uLink[4].dq = 0;
	uLink[4].ddq = 0;
	uLink[4].a = Vector3<double>(0,0,1);
	uLink[4].aw = Vector3<double>(0,0,1);
	uLink[4].b = Vector3<double>(0, 0, theNaoCfg->NeckOffsetZ);
	uLink[4].m = theNaoCfg->MassNeck;
	uLink[4].c = theNaoCfg->McNeck;
	uLink[4].I = createZeroIM();
	uLink[4].vertex.resize(0);
	//uLink[4].face = 0;
	uLink[4].chainID = CHead;
	//==
	uLink[5].name = "HeadPitchY";    //Head  头部
	uLink[5].jointID = JointData::HeadPitch;
	uLink[5].sister = 0;
	uLink[5].child = 0;
	uLink[5].mother = 4;
	uLink[5].p = Vector3<double>(0,0,0);
	uLink[5].R = RotationMatrix();
	uLink[5].v = Vector3<double>(0,0,0);
	uLink[5].w = Vector3<double>(0,0,0);
	uLink[5].q = 0;
	uLink[5].dq = 0;
	uLink[5].ddq = 0;
	uLink[5].a = Vector3<double>(0,1,0);
	uLink[5].aw = Vector3<double>(0,1,0);
	uLink[5].b = Vector3<double>(0,0,0);
	uLink[5].m = theNaoCfg->MassHead;
	uLink[5].c = theNaoCfg->McHead; /*- Vector3<double>(0, 0, theNaoCfg->NeckOffsetZ);*///要减去脖子到身体的向量
	uLink[5].I = createSphereIM(uLink[5].m, 0.065);
	uLink[5].vertex.resize(0);
	//uLink[5].face = 0;
	uLink[5].chainID = CHead;
	//==
	uLink[6].name = "RShoulderPitchY";  //RShoulder  右肩
	uLink[6].jointID = JointData::RShoulderPitch;
	uLink[6].sister = 11;
	uLink[6].child = 7;
	uLink[6].mother = 1;
	uLink[6].p = Vector3<double>(0,0,0);
	uLink[6].R = RotationMatrix();
	uLink[6].v = Vector3<double>(0,0,0);
	uLink[6].w = Vector3<double>(0,0,0);
	uLink[6].q = 0;
	uLink[6].dq = 0;
	uLink[6].ddq = 0;
	uLink[6].a = Vector3<double>(0,1,0);
	uLink[6].aw = Vector3<double>(0,1,0);
	uLink[6].b = Vector3<double>(0, -theNaoCfg->ShoulderOffsetY, theNaoCfg->ShoulderOffsetZ);
	uLink[6].m = theNaoCfg->MassShoulder;
	uLink[6].c = theNaoCfg->McRightShoulder;
	uLink[6].I = createZeroIM();
	uLink[6].vertex.resize(0);
	//uLink[6].face = 0;
	uLink[6].chainID = CRArm;
	//==
	uLink[7].name = "RShoulderRollZ";   //RUpperArm  右上臂
	uLink[7].jointID = JointData::RShoulderRoll;
	uLink[7].sister = 0;
	uLink[7].child = 8;
	uLink[7].mother = 6;
	uLink[7].p = Vector3<double>(0,0,0);
	uLink[7].R = RotationMatrix();
	uLink[7].v = Vector3<double>(0,0,0);
	uLink[7].w = Vector3<double>(0,0,0);
	uLink[7].q = 0;
	uLink[7].dq = 0;
	uLink[7].ddq = 0;
	uLink[7].a = Vector3<double>(0,0,1);
	uLink[7].aw = Vector3<double>(0,0,1);
	uLink[7].b = Vector3<double>(0,0,0);
	uLink[7].m = theNaoCfg->MassUpperArm;
	uLink[7].c = theNaoCfg->McUpperArmRight;    /*-Vector3<double>(0, -theNaoCfg->ShoulderOffsetY, theNaoCfg->ShoulderOffsetZ);*/
	uLink[7].I = createBoxIM(uLink[7].m, 0.08, 0.07, 0.06);
	uLink[7].vertex.resize(0);
	//uLink[7].face = 0;
	uLink[7].chainID = CRArm;
	//==
	uLink[8].name = "RElbowYawX";   //RElbow  右肘
	uLink[8].jointID = JointData::RElbowYaw;
	uLink[8].sister = 0;
	uLink[8].child = 9;
	uLink[8].mother = 7;
	uLink[8].p = Vector3<double>(0,0,0);
	uLink[8].R = RotationMatrix();
	uLink[8].v = Vector3<double>(0,0,0);
	uLink[8].w = Vector3<double>(0,0,0);
	uLink[8].q = 0;
	uLink[8].dq = 0;
	uLink[8].ddq = 0;
	uLink[8].a = Vector3<double>(1,0,0);
	uLink[8].aw = Vector3<double>(1,0,0);
	uLink[8].b = Vector3<double>(theNaoCfg->UpperArmLength, 0, 0);
	uLink[8].m = theNaoCfg->MassElbow;
	uLink[8].c = theNaoCfg->McRightElbow;
	uLink[8].I = createZeroIM();
	uLink[8].vertex.resize(0);
	//uLink[8].face = 0;
	uLink[8].chainID = CRArm;
	//==
	uLink[9].name = "RElbowRollZ";  //RLowerArm  右前臂
	uLink[9].jointID = JointData::RElbowRoll;
	uLink[9].sister = 0;
	uLink[9].child = 10;
	uLink[9].mother = 8;
	uLink[9].p = Vector3<double>(0,0,0);
	uLink[9].R = RotationMatrix();
	uLink[9].v = Vector3<double>(0,0,0);
	uLink[9].w = Vector3<double>(0,0,0);
	uLink[9].q = 0;
	uLink[9].dq = 0;
	uLink[9].ddq = 0;
	uLink[9].a = Vector3<double>(0,0,1);
	uLink[9].aw = Vector3<double>(0,0,1);
	uLink[9].b = Vector3<double>(0,0,0);
	uLink[9].m = theNaoCfg->MassLowerArm;
	uLink[9].c = theNaoCfg->McLowerArmRight;  /*-Vector3<double>(theNaoCfg->UpperArmLength, -theNaoCfg->ShoulderOffsetY, theNaoCfg->ShoulderOffsetZ);*/
	uLink[9].I = createBoxIM(uLink[9].m, 0.11, 0.05, 0.05);
	uLink[9].vertex.resize(0);
	//uLink[9].face = 0;
	uLink[9].chainID = CRArm;
	//==
	uLink[10].name = "RHand";
	uLink[10].jointID = JointData::NoneJoint;
	uLink[10].sister = 0;
	uLink[10].child = 0;
	uLink[10].mother = 9;
	uLink[10].p = Vector3<double>(0,0,0);
	uLink[10].R = RotationMatrix();
	uLink[10].v = Vector3<double>(0,0,0);
	uLink[10].w = Vector3<double>(0,0,0);
	uLink[10].q = 0;
	uLink[10].dq = 0;
	uLink[10].ddq = 0;
	uLink[10].a = Vector3<double>(0,0,0);
	uLink[10].aw = Vector3<double>(0,0,0);
	uLink[10].b = Vector3<double>(theNaoCfg->LowerArmLength+theNaoCfg->HandOffsetX, 0, theNaoCfg->HandOffsetZ);
	uLink[10].m = 0;
	uLink[10].c = Vector3<double>(0,0,0);
	uLink[10].I = createZeroIM();
	uLink[10].vertex.resize(0);
	//uLink[10].face = 0;
	uLink[10].chainID = CRArm;
	//==
	uLink[11].name = "LShoulderPitchY";   //LShoulder
	uLink[11].jointID = JointData::LShoulderPitch;
	uLink[11].sister = 16;
	uLink[11].child = 12;
	uLink[11].mother = 1;
	uLink[11].p = Vector3<double>(0,0,0);
	uLink[11].R = RotationMatrix();
	uLink[11].v = Vector3<double>(0,0,0);
	uLink[11].w = Vector3<double>(0,0,0);
	uLink[11].q = 0;
	uLink[11].dq = 0;
	uLink[11].ddq = 0;
	uLink[11].a = Vector3<double>(0,1,0);
	uLink[11].aw = Vector3<double>(0,1,0);
	uLink[11].b = Vector3<double>(0, theNaoCfg->ShoulderOffsetY, theNaoCfg->ShoulderOffsetZ);
	uLink[11].m = theNaoCfg->MassShoulder;
	uLink[11].c = theNaoCfg->McLeftShoulder;
	uLink[11].I = createZeroIM();
	uLink[11].vertex.resize(0);
	//uLink[11].face = 0;
	uLink[11].chainID = CLArm;
	//==
	uLink[12].name = "LShoulderRollZ";    //LUpperArm
	uLink[12].jointID = JointData::LShoulderRoll;
	uLink[12].sister = 0;
	uLink[12].child = 13;
	uLink[12].mother = 11;
	uLink[12].p = Vector3<double>(0,0,0);
	uLink[12].R = RotationMatrix();
	uLink[12].v = Vector3<double>(0,0,0);
	uLink[12].w = Vector3<double>(0,0,0);
	uLink[12].q = 0;
	uLink[12].dq = 0;
	uLink[12].ddq = 0;
	uLink[12].a = Vector3<double>(0,0,1);
	uLink[12].aw = Vector3<double>(0,0,1);
	uLink[12].b = Vector3<double>(0,0,0);
	uLink[12].m = theNaoCfg->MassUpperArm;
	uLink[12].c = theNaoCfg->McUpperArmLeft;    /*-Vector3<double>(0, theNaoCfg->ShoulderOffsetY, theNaoCfg->ShoulderOffsetZ);*/
	uLink[12].I = createBoxIM(uLink[12].m, 0.08, 0.07, 0.06);
	uLink[12].vertex.resize(0);
	//uLink[12].face = 0;
	uLink[12].chainID = CLArm;
	//==
	uLink[13].name = "LElbowYawX";   //LElbow
	uLink[13].jointID = JointData::LElbowYaw;
	uLink[13].sister = 0;
	uLink[13].child = 14;
	uLink[13].mother = 12;
	uLink[13].p = Vector3<double>(0,0,0);
	uLink[13].R = RotationMatrix();
	uLink[13].v = Vector3<double>(0,0,0);
	uLink[13].w = Vector3<double>(0,0,0);
	uLink[13].q = 0;
	uLink[13].dq = 0;
	uLink[13].ddq = 0;
	uLink[13].a = Vector3<double>(1,0,0);
	uLink[13].aw = Vector3<double>(1,0,0);
	uLink[13].b = Vector3<double>(theNaoCfg->UpperArmLength, 0, 0);
	uLink[13].m = theNaoCfg->MassElbow;
	uLink[13].c = theNaoCfg->McLeftElbow;
	uLink[13].I = createZeroIM();
	uLink[13].vertex.resize(0);
	//uLink[13].face = 0;
	uLink[13].chainID = CLArm;
	//==
	uLink[14].name = "LElbowRollZ";   //LLowerArm
	uLink[14].jointID = JointData::LElbowRoll;
	uLink[14].sister = 0;
	uLink[14].child = 15;
	uLink[14].mother = 13;
	uLink[14].p = Vector3<double>(0,0,0);
	uLink[14].R = RotationMatrix();
	uLink[14].v = Vector3<double>(0,0,0);
	uLink[14].w = Vector3<double>(0,0,0);
	uLink[14].q = 0;
	uLink[14].dq = 0;
	uLink[14].ddq = 0;
	uLink[14].a = Vector3<double>(0,0,1);
	uLink[14].aw = Vector3<double>(0,0,1);
	uLink[14].b = Vector3<double>(0,0,0);
	uLink[14].m = theNaoCfg->MassLowerArm;
	uLink[14].c = theNaoCfg->McLowerArmLeft;   /*-Vector3<double>(theNaoCfg->UpperArmLength, theNaoCfg->ShoulderOffsetY, theNaoCfg->ShoulderOffsetZ);*/
	uLink[14].I = createBoxIM(uLink[14].m, 0.11, 0.05, 0.05);
	uLink[14].vertex.resize(0);
	//uLink[14].face = 0;
	uLink[14].chainID = CLArm;
	//==
	uLink[15].name = "LHand";
	uLink[15].jointID = JointData::NoneJoint;
	uLink[15].sister = 0;
	uLink[15].child = 0;
	uLink[15].mother = 14;
	uLink[15].p = Vector3<double>(0,0,0);
	uLink[15].R = RotationMatrix();
	uLink[15].v = Vector3<double>(0,0,0);
	uLink[15].w = Vector3<double>(0,0,0);
	uLink[15].q = 0;
	uLink[15].dq = 0;
	uLink[15].ddq = 0;
	uLink[15].a = Vector3<double>(0,0,0);
	uLink[15].aw = Vector3<double>(0,0,0);
	uLink[15].b = Vector3<double>(theNaoCfg->LowerArmLength+theNaoCfg->HandOffsetX, 0, theNaoCfg->HandOffsetZ);
	uLink[15].m = 0;
	uLink[15].c = Vector3<double>(0,0,0);
	uLink[15].I = createZeroIM();
	uLink[15].vertex.resize(0);
	//uLink[15].face = 0;
	uLink[15].chainID = CLArm;
	//==
	uLink[16].name = "RHipYawPitch";   //RPelvis  右骨盆
	uLink[16].jointID = JointData::RHipYawPitch;
	uLink[16].sister = 23;
	uLink[16].child = 17;
	uLink[16].mother = 1;
	uLink[16].p = Vector3<double>(0,0,0);
	uLink[16].R = RotationMatrix();
	uLink[16].v = Vector3<double>(0,0,0);
	uLink[16].w = Vector3<double>(0,0,0);
	uLink[16].q = 0;
	uLink[16].dq = 0;
	uLink[16].ddq = 0;
	uLink[16].a = Vector3<double>(0,1,1);     //这里要注意, 原来是[0 1 1],但我是按RotX(-pi/4)*RotZ(q)*RotX(pi/4)来处理这个关节的
	uLink[16].aw = Vector3<double>(0,0,1);
	uLink[16].b = Vector3<double>(0, -theNaoCfg->HipOffsetY, -theNaoCfg->HipOffsetZ);
	uLink[16].m = theNaoCfg->MassPelvis;
	uLink[16].c = theNaoCfg->McRightPelvis;
	uLink[16].I = createZeroIM();
	uLink[16].vertex.resize(0);
	//uLink[16].face = 0;
	uLink[16].chainID = CRLeg;
	//==
	uLink[17].name = "RHipRollX";   //RHip    右髋
	uLink[17].jointID = JointData::RHipRoll;
	uLink[17].sister = 0;
	uLink[17].child = 18;
	uLink[17].mother = 16;
	uLink[17].p = Vector3<double>(0,0,0);
	uLink[17].R = RotationMatrix();
	uLink[17].v = Vector3<double>(0,0,0);
	uLink[17].w = Vector3<double>(0,0,0);
	uLink[17].q = 0;
	uLink[17].dq = 0;
	uLink[17].ddq = 0;
	uLink[17].a = Vector3<double>(1,0,0);
	uLink[17].aw = Vector3<double>(1,0,0);
	uLink[17].b = Vector3<double>(0,0,0);
	uLink[17].m = theNaoCfg->MassHip;
	uLink[17].c = theNaoCfg->McRightHip;
	uLink[17].I = createZeroIM();
	uLink[17].vertex.resize(0);
	//uLink[17].face = 0;
	uLink[17].chainID = CRLeg;
	//==
	uLink[18].name = "RHipPitchY";  //RThigh  右大腿
	uLink[18].jointID = JointData::RHipPitch;
	uLink[18].sister = 0;
	uLink[18].child = 19;
	uLink[18].mother = 17;
	uLink[18].p = Vector3<double>(0,0,0);
	uLink[18].R = RotationMatrix();
	uLink[18].v = Vector3<double>(0,0,0);
	uLink[18].w = Vector3<double>(0,0,0);
	uLink[18].q = 0;
	uLink[18].dq = 0;
	uLink[18].ddq = 0;
	uLink[18].a = Vector3<double>(0,1,0);
	uLink[18].aw = Vector3<double>(0,1,0);
	uLink[18].b = Vector3<double>(0,0,0);
	uLink[18].m = theNaoCfg->MassThigh;
	uLink[18].c = theNaoCfg->McThighRight;  /*-Vector3<double>(0, -theNaoCfg->HipOffsetY, -theNaoCfg->HipOffsetZ);*/
	uLink[18].I = createBoxIM(uLink[18].m, 0.07, 0.07, 0.14);
	uLink[18].vertex.resize(0);
	//uLink[18].face = 0;
	uLink[18].chainID = CRLeg;
	//==
	uLink[19].name = "RKneePitchY";   //RTibia  右小腿
	uLink[19].jointID = JointData::RKneePitch;
	uLink[19].sister = 0;
	uLink[19].child = 20;
	uLink[19].mother = 18;
	uLink[19].p = Vector3<double>(0,0,0);
	uLink[19].R = RotationMatrix();
	uLink[19].v = Vector3<double>(0,0,0);
	uLink[19].w = Vector3<double>(0,0,0);
	uLink[19].q = 0;
	uLink[19].dq = 0;
	uLink[19].ddq = 0;
	uLink[19].a = Vector3<double>(0,1,0);
	uLink[19].aw = Vector3<double>(0,1,0);
	uLink[19].b = Vector3<double>(0, 0, -theNaoCfg->ThighLength);
	uLink[19].m = theNaoCfg->MassTibia;
	uLink[19].c = theNaoCfg->McTibiaRight;   /*-Vector3<double>(0, -theNaoCfg->HipOffsetY, -theNaoCfg->HipOffsetZ - theNaoCfg->ThighLength);*/
	uLink[19].I = createBoxIM(uLink[19].m, 0.07, 0.08, 0.11);
	uLink[19].vertex.resize(0);
	//uLink[19].face = 0;
	uLink[19].chainID = CRLeg;
	//==
	uLink[20].name = "RAnklePitchY";  //RAnkle  右踝
	uLink[20].jointID = JointData::RAnklePitch;
	uLink[20].sister = 0;
	uLink[20].child = 21;
	uLink[20].mother = 19;
	uLink[20].p = Vector3<double>(0,0,0);
	uLink[20].R = RotationMatrix();
	uLink[20].v = Vector3<double>(0,0,0);
	uLink[20].w = Vector3<double>(0,0,0);
	uLink[20].q = 0;
	uLink[20].dq = 0;
	uLink[20].ddq = 0;
	uLink[20].a = Vector3<double>(0,1,0);
	uLink[20].aw = Vector3<double>(0,1,0);
	uLink[20].b = Vector3<double>(0, 0, -theNaoCfg->TibiaLength);
	uLink[20].m = theNaoCfg->MassAnkle;
	uLink[20].c = theNaoCfg->McRightAnkle;
	uLink[20].I = createZeroIM();
	uLink[20].vertex.resize(0);
	//uLink[20].face = 0;
	uLink[20].chainID = CRLeg;
	//==
	uLink[21].name = "RAnkleRollX";   //RFoot  右足
	uLink[21].jointID = JointData::RAnkleRoll;
	uLink[21].sister = 0;
	uLink[21].child = 22;
	uLink[21].mother = 20;
	uLink[21].p = Vector3<double>(0,0,0);
	uLink[21].R = RotationMatrix();
	uLink[21].v = Vector3<double>(0,0,0);
	uLink[21].w = Vector3<double>(0,0,0);
	uLink[21].q = 0;
	uLink[21].dq = 0;
	uLink[21].ddq = 0;
	uLink[21].a = Vector3<double>(1,0,0);
	uLink[21].aw = Vector3<double>(1,0,0);
	uLink[21].b = Vector3<double>(0,0,0);
	uLink[21].m = theNaoCfg->MassFoot;
	uLink[21].c =theNaoCfg->McFootRight; /*- Vector3<double>(0, -theNaoCfg->HipOffsetY, -theNaoCfg->HipOffsetZ - theNaoCfg->ThighLength - theNaoCfg->TibiaLength);*/
	uLink[21].I = createBoxIM(uLink[21].m, 0.16, 0.08, 0.02);
	uLink[21].vertex.resize(0);
	//uLink[21].face = 0;
	uLink[21].chainID = CRLeg;
	//==
	uLink[22].name = "RFoot";
	uLink[22].jointID = JointData::NoneJoint;
	uLink[22].sister = 0;
	uLink[22].child = 0;
	uLink[22].mother = 21;
	uLink[22].p = Vector3<double>(0,0,0);
	uLink[22].R = RotationMatrix();
	uLink[22].v = Vector3<double>(0,0,0);
	uLink[22].w = Vector3<double>(0,0,0);
	uLink[22].q = 0;
	uLink[22].dq = 0;
	uLink[22].ddq = 0;
	uLink[22].a = Vector3<double>(0,1,0);
	uLink[22].aw = Vector3<double>(0,1,0);
	uLink[22].b = Vector3<double>(0, 0, -theNaoCfg->FootHeight);
	uLink[22].m = 0;
	uLink[22].c =Vector3<double>(0,0,0);
	uLink[22].I = createZeroIM();
	uLink[22].vertex.resize(0);
	uLink[22].vertex.push_back(Vector3<double>(-0.057, 0.042,0));
	uLink[22].vertex.push_back(Vector3<double>(0.102, 0.042,0));
	uLink[22].vertex.push_back(Vector3<double>(0.102, -0.06,0));
	uLink[22].vertex.push_back(Vector3<double>(-0.057, -0.06,0));
	//uLink[22].face = 0;
	uLink[22].chainID = CRLeg;
	//==
	uLink[23].name = "LHipYawPitch";  //LPelvis
	uLink[23].jointID = JointData::LHipYawPitch;
	uLink[23].sister = 0;
	uLink[23].child = 24;
	uLink[23].mother = 1;
	uLink[23].p = Vector3<double>(0,0,0);
	uLink[23].R = RotationMatrix();
	uLink[23].v = Vector3<double>(0,0,0);
	uLink[23].w = Vector3<double>(0,0,0);
	uLink[23].q = 0;
	uLink[23].dq = 0;
	uLink[23].ddq = 0;
	uLink[23].a = Vector3<double>(0,1,-1);     //这里要注意, 原来是[0 1 1],但我是按RotX(-pi/4)*RotZ(q)*RotX(pi/4)来处理这个关节的
	uLink[23].aw = Vector3<double>(0,0,1);
	uLink[23].b = Vector3<double>(0, theNaoCfg->HipOffsetY, -theNaoCfg->HipOffsetZ);
	uLink[23].m = theNaoCfg->MassPelvis;
	uLink[23].c = theNaoCfg->McLeftPelvis;
	uLink[23].I = createZeroIM();
	uLink[23].vertex.resize(0);
	//uLink[23].face = 0;
	uLink[23].chainID = CLLeg;
	//==
	uLink[24].name = "LHipRollX";  //LHip
	uLink[24].jointID = JointData::LHipRoll;
	uLink[24].sister = 0;
	uLink[24].child = 25;
	uLink[24].mother = 23;
	uLink[24].p = Vector3<double>(0,0,0);
	uLink[24].R = RotationMatrix();
	uLink[24].v = Vector3<double>(0,0,0);
	uLink[24].w = Vector3<double>(0,0,0);
	uLink[24].q = 0;
	uLink[24].dq = 0;
	uLink[24].ddq = 0;
	uLink[24].a = Vector3<double>(1,0,0);
	uLink[24].aw = Vector3<double>(1,0,0);
	uLink[24].b = Vector3<double>(0,0,0);
	uLink[24].m = theNaoCfg->MassHip;
	uLink[24].c = theNaoCfg->McLeftHip;
	uLink[24].I = createZeroIM();
	uLink[24].vertex.resize(0);
	//uLink[24].face = 0;
	uLink[24].chainID = CLLeg;
	//==
	uLink[25].name = "LHipPitchY";   //LThigh
	uLink[25].jointID = JointData::LHipPitch;
	uLink[25].sister = 0;
	uLink[25].child = 26;
	uLink[25].mother = 24;
	uLink[25].p = Vector3<double>(0,0,0);
	uLink[25].R = RotationMatrix();
	uLink[25].v = Vector3<double>(0,0,0);
	uLink[25].w = Vector3<double>(0,0,0);
	uLink[25].q = 0;
	uLink[25].dq = 0;
	uLink[25].ddq = 0;
	uLink[25].a = Vector3<double>(0,1,0);
	uLink[25].aw = Vector3<double>(0,1,0);
	uLink[25].b = Vector3<double>(0,0,0);
	uLink[25].m = theNaoCfg->MassThigh;
	uLink[25].c = theNaoCfg->McThighLeft;  /*-Vector3<double>(0, theNaoCfg->HipOffsetY, -theNaoCfg->HipOffsetZ);*/
	uLink[25].I = createBoxIM(uLink[25].m, 0.07, 0.07, 0.14);
	uLink[25].vertex.resize(0);
	//uLink[25].face = 0;
	uLink[25].chainID = CLLeg;
	//==
	uLink[26].name = "LKneePitchY";  //LTibia
	uLink[26].jointID = JointData::LKneePitch;
	uLink[26].sister = 0;
	uLink[26].child = 27;
	uLink[26].mother = 25;
	uLink[26].p = Vector3<double>(0,0,0);
	uLink[26].R = RotationMatrix();
	uLink[26].v = Vector3<double>(0,0,0);
	uLink[26].w = Vector3<double>(0,0,0);
	uLink[26].q = 0;
	uLink[26].dq = 0;
	uLink[26].ddq = 0;
	uLink[26].a = Vector3<double>(0,1,0);
	uLink[26].aw = Vector3<double>(0,1,0);
	uLink[26].b = Vector3<double>(0, 0, -theNaoCfg->ThighLength);
	uLink[26].m = theNaoCfg->MassTibia;
	uLink[26].c = theNaoCfg->McTibiaLeft;  /*-Vector3<double>(0, theNaoCfg->HipOffsetY, -theNaoCfg->HipOffsetZ - theNaoCfg->ThighLength);*/
	uLink[26].I = createBoxIM(uLink[26].m, 0.07, 0.08, 0.11);
	uLink[26].vertex.resize(0);
	//uLink[26].face = 0;
	uLink[26].chainID = CLLeg;
	//==
	uLink[27].name = "LAnklePitchY";   //LAnkle
	uLink[27].jointID = JointData::LAnklePitch;
	uLink[27].sister = 0;
	uLink[27].child = 28;
	uLink[27].mother = 26;
	uLink[27].p = Vector3<double>(0,0,0);
	uLink[27].R = RotationMatrix();
	uLink[27].v = Vector3<double>(0,0,0);
	uLink[27].w = Vector3<double>(0,0,0);
	uLink[27].q = 0;
	uLink[27].dq = 0;
	uLink[27].ddq = 0;
	uLink[27].a = Vector3<double>(0,1,0);
	uLink[27].aw = Vector3<double>(0,1,0);
	uLink[27].b = Vector3<double>(0, 0, -theNaoCfg->TibiaLength);
	uLink[27].m = theNaoCfg->MassAnkle;
	uLink[27].c = theNaoCfg->McLeftAnkle;
	uLink[27].I = createZeroIM();
	uLink[27].vertex.resize(0);
	//uLink[27].face = 0;
	uLink[27].chainID = CLLeg;
	//==
	uLink[28].name = "LAnkleRollX";  //LFoot
	uLink[28].jointID = JointData::LAnkleRoll;
	uLink[28].sister = 0;
	uLink[28].child = 29;
	uLink[28].mother = 27;
	uLink[28].p = Vector3<double>(0,0,0);
	uLink[28].R = RotationMatrix();
	uLink[28].v = Vector3<double>(0,0,0);
	uLink[28].w = Vector3<double>(0,0,0);
	uLink[28].q = 0;
	uLink[28].dq = 0;
	uLink[28].ddq = 0;
	uLink[28].a = Vector3<double>(1,0,0);
	uLink[28].aw = Vector3<double>(1,0,0);
	uLink[28].b = Vector3<double>(0,0,0);
	uLink[28].m = theNaoCfg->MassFoot;
	uLink[28].c =theNaoCfg->McFootLeft; /*- Vector3<double>(0, theNaoCfg->HipOffsetY, -theNaoCfg->HipOffsetZ - theNaoCfg->ThighLength - theNaoCfg->TibiaLength);*/
	uLink[28].I = createBoxIM(uLink[28].m, 0.16, 0.08, 0.02);
	uLink[28].vertex.resize(0);
	//uLink[28].face = 0;
	uLink[28].chainID = CLLeg;
	//==
	uLink[29].name = "LFoot";
	uLink[29].jointID = JointData::NoneJoint;
	uLink[29].sister = 0;
	uLink[29].child = 0;
	uLink[29].mother = 28;
	uLink[29].p = Vector3<double>(0,0,0);
	uLink[29].R = RotationMatrix();
	uLink[29].v = Vector3<double>(0,0,0);
	uLink[29].w = Vector3<double>(0,0,0);
	uLink[29].q = 0;
	uLink[29].dq = 0;
	uLink[29].ddq = 0;
	uLink[29].a = Vector3<double>(0,1,0);
	uLink[29].aw = Vector3<double>(0,1,0);
	uLink[29].b = Vector3<double>(0, 0, -theNaoCfg->FootHeight);
	uLink[29].m = 0;
	uLink[29].c =Vector3<double>(0,0,0);
	uLink[29].I = createZeroIM();
	uLink[29].vertex.resize(0);
	uLink[29].vertex.push_back(Vector3<double>(-0.057, -0.042,0));
	uLink[29].vertex.push_back(Vector3<double>(0.102, -0.042,0));
	uLink[29].vertex.push_back(Vector3<double>(0.102, 0.06,0));
	uLink[29].vertex.push_back(Vector3<double>(-0.057, 0.06,0));
	//uLink[29].face = 0;
	uLink[29].chainID = CLLeg;
}
// TODO: Modify Inertia Matrix calculation to fit with central of mass of each link...
Matrix3x3<double> NaoStructure::createCylinderIM(double m, double r, double h, Vector3<double> rotA)
{
	Matrix3x3<double> a;
	a(0,0)=m*r*r/4+m*h*h/3;
	a(0,1)=0;
	a(0,2)=0;
	a(1,0)=0;
	a(1,1)=m*r*r/4+m*h*h/3;
	a(1,2)=0;
	a(2,0)=0;
	a(2,1)=0;
	a(2,2)=m*r*r/2;
	RotationMatrix rot(rotA.x, rotA.y, rotA.z);
	a = rot * a * rot.transpose();
	return a;
}

Matrix3x3<double> NaoStructure::createBoxIM(double m, double x, double y,double z, Vector3<double> rotA)
{
	Matrix3x3<double> a;
	a(0,0)=m*(y*y+z*z)/12;
	a(0,1)=0;
	a(0,2)=0;
	a(1,0)=0;
	a(1,1)=m*(x*x+z*z)/12;
	a(1,2)=0;
	a(2,0)=0;
	a(2,1)=0;
	a(2,2)=m*(y*y+x*x)/12;
	RotationMatrix rot(rotA.x, rotA.y, rotA.z);
	a = rot * a * rot.transpose();
	return a;
}
Matrix3x3<double> NaoStructure::createSphereIM(double m, double r, Vector3<double> rotA)
{
	Matrix3x3<double> a;
	a(0,0)=2*m*r*r/5;
	a(0,1)=0;
	a(0,2)=0;
	a(1,0)=0;
	a(1,1)=2*m*r*r/5;
	a(1,2)=0;
	a(2,0)=0;
	a(2,1)=0;
	a(2,2)=2*m*r*r/5;
	RotationMatrix rot(rotA.x, rotA.y, rotA.z);
	a = rot * a * rot.transpose();
	return a;
}
Matrix3x3<double> NaoStructure::createZeroIM()
{
	Matrix3x3<double> a;
	a(0,0)=0;
	a(0,1)=0;
	a(0,2)=0;
	a(1,0)=0;
	a(1,1)=0;
	a(1,2)=0;
	a(2,0)=0;
	a(2,1)=0;
	a(2,2)=0;
	return a;
}
