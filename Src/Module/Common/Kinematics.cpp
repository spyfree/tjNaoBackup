#include "Kinematics.h"
#include "Module/Common/NaoConfig.h"
#include "Platform/GTAssert.h"
Vector3<double> Kinematics::calcCOM(const NaoStructure& naoStruct)
{
	Vector3<double> com;
	NaoConfig* theNaoCfg = NaoConfig::getInstance();
	double TotalMass = theNaoCfg->MassTotal;
	com = calcMC(naoStruct, NaoStructure::bodyLink) / TotalMass;
	return com;
}

Vector3<double> Kinematics::calcMC(const NaoStructure& naoStruct, int j)
{
	Vector3<double> mc;
	if(j == NaoStructure::noneLink)
		mc = Vector3<double>(0,0,0);
	else
	{
		mc = ( naoStruct.uLink[j].p + naoStruct.uLink[j].R * naoStruct.uLink[j].c) * naoStruct.uLink[j].m;
		mc = mc + calcMC(naoStruct, naoStruct.uLink[j].sister) + calcMC(naoStruct, naoStruct.uLink[j].child);
	}
	return mc;
}

void Kinematics::execForwardKinematics(NaoStructure& naoStruct, int j)
{
	int i = 0;
	RotationMatrix Rj;
	//NaoConfig* theNaoCfg = NaoConfig::getInstance();
	if(j == NaoStructure::noneLink)
	{
		return; 
	}
	if(j >= 23 && j <= 29){
		j = j;
	}

	if (j != NaoStructure::bodyLink)
	{
		i = naoStruct.uLink[j].mother;
		if (naoStruct.uLink[j].jointID == JointData::LHipYawPitch 
			|| naoStruct.uLink[j].jointID == JointData::RHipYawPitch)
		{
			naoStruct.uLink[j].p = naoStruct.uLink[i].R * naoStruct.uLink[j].b + naoStruct.uLink[i].p;
			
			if (naoStruct.uLink[j].jointID == JointData::RHipYawPitch){
				Rj = RotationMatrix::getRotationX(-pi/4) * RotationMatrix::getRotationZ(naoStruct.uLink[j].q)*RotationMatrix::getRotationX(pi/4);
				naoStruct.uLink[j].aw = naoStruct.uLink[i].R * RotationMatrix::getRotationX(-pi/4)* naoStruct.uLink[j].a;
			}
			else
			{
				Rj = RotationMatrix::getRotationX(-pi3_4)*RotationMatrix::getRotationZ(naoStruct.uLink[j].q)*RotationMatrix::getRotationX(pi3_4);
				naoStruct.uLink[j].aw = naoStruct.uLink[i].R * RotationMatrix::getRotationX(-3*pi/4)* naoStruct.uLink[j].a;
			}
			naoStruct.uLink[j].R = naoStruct.uLink[i].R * Rj;
		}
		else
		{
			naoStruct.uLink[j].p = naoStruct.uLink[i].R * naoStruct.uLink[j].b + naoStruct.uLink[i].p;
			if (naoStruct.uLink[j].a[0] == 1)
				Rj = RotationMatrix::getRotationX(naoStruct.uLink[j].q);
			else if(naoStruct.uLink[j].a[1] == 1)
				Rj = RotationMatrix::getRotationY(naoStruct.uLink[j].q);
			else if(naoStruct.uLink[j].a[2] == 1)
				Rj = RotationMatrix::getRotationZ(naoStruct.uLink[j].q);
			else
				Rj = RotationMatrix();
			naoStruct.uLink[j].aw = naoStruct.uLink[i].R * naoStruct.uLink[j].a;
			naoStruct.uLink[j].R = naoStruct.uLink[i].R * Rj;
		}
	}
	execForwardKinematics(naoStruct, naoStruct.uLink[j].sister);
	execForwardKinematics(naoStruct, naoStruct.uLink[j].child);
}


Vector3<double> Kinematics::recalcBodyPos2SupFoot(NaoStructure& naoStruct, const Vector3<double>& supFootPos, int sup_mode)
{
	if (sup_mode == NaoConfig::SUPPORT_MODE_LEFT || sup_mode == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		naoStruct.uLink[NaoStructure::bodyLink].R = (naoStruct.uLink[NaoStructure::lFootLink].R).invert() * naoStruct.uLink[NaoStructure::bodyLink].R;
		naoStruct.uLink[NaoStructure::bodyLink].p = ((naoStruct.uLink[NaoStructure::lFootLink].R).invert() * (naoStruct.uLink[NaoStructure::bodyLink].p - naoStruct.uLink[NaoStructure::lFootLink].p))+supFootPos;
	}
	//elseif sup_mode == NaoCfg.SUPPORT_MODE_DOUBLE_LEFT
	//elseif sup_mode == NaoCfg.SUPPORT_MODE_DOUBLE_RIGHT
	else	//SUPPORT_MODE_RIGHT
	{
		naoStruct.uLink[NaoStructure::bodyLink].R = (naoStruct.uLink[NaoStructure::rFootLink].R).invert() * naoStruct.uLink[NaoStructure::bodyLink].R;
		naoStruct.uLink[NaoStructure::bodyLink].p = ((naoStruct.uLink[NaoStructure::rFootLink].R).invert() * (naoStruct.uLink[NaoStructure::bodyLink].p - naoStruct.uLink[NaoStructure::rFootLink].p))+supFootPos;
	}
	return getSupFootWorldRotationAngle(naoStruct, sup_mode);
}

Vector3<double> Kinematics::getSupFootWorldRotationAngle(const NaoStructure& naoStruct, int sup_mode)
{
	RotationMatrix Rsf;
	if (sup_mode == NaoConfig::SUPPORT_MODE_LEFT)
		Rsf = naoStruct.uLink[NaoStructure::lFootLink].R;
	else
		Rsf = naoStruct.uLink[NaoStructure::rFootLink].R;

	//return Vector3<double>(Rsf.getXAngle(), Rsf.getYAngle(),Rsf.getZAngle());
	return Rsf.getXYZAngles();
}

void Kinematics::doNaoLegInvKinematics(JointCmd& jointCmd, int ileg, const Vector3<double>& Pf, const RotationMatrix& Rf, 
						   const Vector3<double>& Pb, const RotationMatrix& Rb, double l0, double w0, double l1, double l2, double l3)
{
	double q0 = 0, q1 = 0, q2 = 0, q3 = 0, q4 = 0, q5 = 0;
	int ilegSign = 1, i = 0;
	ASSERT((ileg == NaoConfig::LegIDRight) || (ileg == NaoConfig::LegIDLeft));
	if(ileg == NaoConfig::LegIDRight)
		ilegSign = 1;
	else
		ilegSign = -1;
	Vector3<double> p2 = (Pb + Rb * Vector3<double>(0, -w0 * ilegSign, -l0));
	Vector3<double> r = Rf.invert() * (p2 - Pf);
	double C = r.abs();
	double c3 = (sqr(C) - sqr(l1) - sqr(l2))/(2*l1*l2);
	if(c3 >= 1)
		q3 = 0;
	else if(c3 <= -1)
		q3 = pi;
	else
		q3 = acos(c3);
	double alpha = -asin(l1 * sin(pi - q3) / C);
	q5 = atan2(r.y, r.z);
	if(q5 > pi_2)
		q5 = q5 - pi;
	else if(q5 < - pi_2)
		q5 = q5 + pi;
	q4 = -(atan2(r.x, sign(r.z) * sqrt(sqr(r.y) + sqr(r.z))) - alpha);
	RotationMatrix R;
	if(ileg == NaoConfig::LegIDRight)
	{
		R = RotationMatrix::getRotationX(-pi_4).invert() * Rb.invert() * Rf 
			* RotationMatrix::getRotationX(q5).invert() * RotationMatrix::getRotationY(q3+q4).invert(); 
		//这里没有考虑奇异点情况: c1 == s1时,即q1 == 45 或-135时
		if(R(0,1) == 0 && R(1,1) == 0)
		{
			//"Warning: Leg InversKinematic singularity occur! q1 should be 45 or -135 d"
		}
		q0 = atan2(-R(0,1), R(1,1));
		q1 = atan2(R(2,1)-(-R(0,1)*sin(q0) + R(1,1) * cos(q0)), 
					R(2,1) + (- R(0,1) * sin(q0) + R(1,1) * cos(q0)));
		q2 = atan2(-R(2,0), R(2,2));
	}else{
		R = RotationMatrix::getRotationX(-pi3_4).invert() * Rb.invert() * Rf 
			* RotationMatrix::getRotationX(q5).invert() * RotationMatrix::getRotationY(q3+q4).invert(); 
		//这里没有考虑奇异点情况: c1 == s1时,即q1 == 45 或-135时
		if(R(0,1) == 0 && R(1,1) == 0)
		{
			//"Warning: Leg InversKinematic singularity occur! q1 should be 45 or -135 d"
		}
		q0 = atan2(R(0,1), -R(1,1));
		q1 = atan2(-R(2,1)+(R(0,1)*sin(q0) - R(1,1) * cos(q0)), 
					R(2,1) + ( R(0,1) * sin(q0) -R(1,1) * cos(q0)));
		q2 = atan2(R(2,0), -R(2,2));	
	}
	if(ileg == NaoConfig::LegIDRight)
	{
		jointCmd.angles[JointData::RHipYawPitch] = q0;
		jointCmd.angles[JointData::RHipRoll] = q1;
		jointCmd.angles[JointData::RHipPitch] = q2;
		jointCmd.angles[JointData::RKneePitch] = q3;
		jointCmd.angles[JointData::RAnklePitch] = q4;
		jointCmd.angles[JointData::RAnkleRoll] = q5;
	}else{
		jointCmd.angles[JointData::LHipYawPitch] = q0;
		jointCmd.angles[JointData::LHipRoll] = q1;
		jointCmd.angles[JointData::LHipPitch] = q2;
		jointCmd.angles[JointData::LKneePitch] = q3;
		jointCmd.angles[JointData::LAnklePitch] = q4;
		jointCmd.angles[JointData::LAnkleRoll] = q5;
	}
		
}

static const bool bUseJointsIncreasingLimit = false;
static const double jointIncreasingLimit = 0.06;//radians
void Kinematics::calculateSafeLegJoints(JointCmd& jointCmd, int ileg, const Vector3<double>& psfoot, const RotationMatrix& frz, 
								  const Vector3<double>& bodypos, const RotationMatrix& bodyR)
{
	JointCmd jtmp = jointCmd;
	double djoint;
	int i = 0;
	Kinematics::doNaoLegInvKinematics(jtmp, ileg, psfoot, frz, 	bodypos, bodyR, 
		NaoConfig::getInstance()->HipOffsetZ, NaoConfig::getInstance()->HipOffsetY, NaoConfig::getInstance()->ThighLength, NaoConfig::getInstance()->TibiaLength, NaoConfig::getInstance()->FootHeight);
	if(ileg == NaoConfig::LegIDRight){
		for(i = JointData::RHipYawPitch; i <= JointData::RAnkleRoll; ++i)
		{
			djoint = jtmp.angles[i] - jointCmd.angles[i];
			if(bUseJointsIncreasingLimit == true){
				djoint = abs(djoint) > jointIncreasingLimit ? sign(djoint) * jointIncreasingLimit : djoint;
			}
			jointCmd.angles[i] += djoint;
		}
	}else
	{
		for(i = JointData::LHipYawPitch; i <= JointData::LAnkleRoll; ++i)
		{
			djoint = jtmp.angles[i] - jointCmd.angles[i];
			if(bUseJointsIncreasingLimit == true){
				djoint = abs(djoint) > jointIncreasingLimit ?sign(djoint) * jointIncreasingLimit : djoint;
			}
			jointCmd.angles[i] += djoint;
		}
	}
}

//===============
void Kinematics::doNaoLegInvKinematics(JointCmd& jointCmd, int ileg, const Vector3<double>& Pf, const RotationMatrix& Rf, 
									   const Vector3<double>& Pb, const RotationMatrix& Rb, double rotRHipYawPitch, double deltaRRollX, double deltaRPitchY, double l0, double w0, double l1, double l2, double l3)
{
	double q0 = 0, q1 = 0, q2 = 0, q3 = 0, q4 = 0, q5 = 0;
	int ilegSign = 1, i = 0;
	ASSERT((ileg == NaoConfig::LegIDRight) || (ileg == NaoConfig::LegIDLeft));
	if(ileg == NaoConfig::LegIDRight)
		ilegSign = 1;
	else
		ilegSign = -1;
	Vector3<double> p2 = (Pb + Rb * Vector3<double>(0, -w0 * ilegSign, -l0));
	Vector3<double> r = Rf.invert() * (p2 - Pf);
	double C = r.abs();
	double c3 = (sqr(C) - sqr(l1) - sqr(l2))/(2*l1*l2);
	if(c3 >= 1)
		q3 = 0;
	else if(c3 <= -1)
		q3 = pi;
	else
		q3 = acos(c3);
	double alpha = -asin(l1 * sin(pi - q3) / C);
	q5 = atan2(r.y, r.z);
	if(q5 > pi_2)
		q5 = q5 - pi;
	else if(q5 < - pi_2)
		q5 = q5 + pi;
	q4 = -(atan2(r.x, sign(r.z) * sqrt(sqr(r.y) + sqr(r.z))) - alpha);
	RotationMatrix R;
	if(ileg == NaoConfig::LegIDRight)
	{
		R = Rb.invert() * Rf * RotationMatrix::getRotationX(q5).invert() * RotationMatrix::getRotationY(q3+q4).invert(); 
		//这里没有考虑奇异点情况: c1 == s1时,即q1 == 45 或-135时
		if(R(0,1) == 0 && R(1,1) == 0)
		{
			//"Warning: Leg InversKinematic singularity occur! q1 should be 45 or -135 d"
		}
		q0 = atan2(-R(0,1), R(1,1));
		q1 = atan2(R(2,1), -R(0,1)*sin(q0) + R(1,1) * cos(q0));
		q2 = atan2(-R(2,0), R(2,2));
	}else{
		R = Rb.invert() * Rf * RotationMatrix::getRotationX(q5).invert() * RotationMatrix::getRotationY(q3+q4).invert(); 
		//这里没有考虑奇异点情况: c1 == s1时,即q1 == 45 或-135时
		if(R(0,1) == 0 && R(1,1) == 0)
		{
			//"Warning: Leg InversKinematic singularity occur! q1 should be 45 or -135 d"
		}
		q0 = atan2(-R(0,1), R(1,1));
		q1 = atan2(R(2,1), -R(0,1)*sin(q0) + R(1,1) * cos(q0));
		q2 = atan2(-R(2,0), R(2,2));	
	}
	if(ileg == NaoConfig::LegIDRight)
	{
		jointCmd.angles[JointData::RHipYawPitch] = rotRHipYawPitch;//q0;
		jointCmd.angles[JointData::RHipRoll] = q1 - deltaRRollX;
		jointCmd.angles[JointData::RHipPitch] = q2 - deltaRPitchY;
		jointCmd.angles[JointData::RKneePitch] = q3;
		jointCmd.angles[JointData::RAnklePitch] = q4;
		jointCmd.angles[JointData::RAnkleRoll] = q5;
	}else{
		jointCmd.angles[JointData::LHipYawPitch] = rotRHipYawPitch;//q0;
		jointCmd.angles[JointData::LHipRoll] = q1 + deltaRRollX;
		jointCmd.angles[JointData::LHipPitch] = q2 - deltaRPitchY;
		jointCmd.angles[JointData::LKneePitch] = q3;
		jointCmd.angles[JointData::LAnklePitch] = q4;
		jointCmd.angles[JointData::LAnkleRoll] = q5;
	}

}

void Kinematics::calculateSafeLegJoints(JointCmd& jointCmd, int ileg, const Vector3<double>& psfoot, const RotationMatrix& frz, 
										const Vector3<double>& bodypos, const RotationMatrix& bodyR, double rotRHipYawPitch, double deltaRRollX, double deltaRPitchY)
{
	JointCmd jtmp = jointCmd;
	double djoint;
	int i = 0;
	Kinematics::doNaoLegInvKinematics(jtmp, ileg, psfoot, frz, 	bodypos, bodyR, rotRHipYawPitch, deltaRRollX, deltaRPitchY,
		NaoConfig::getInstance()->HipOffsetZ, NaoConfig::getInstance()->HipOffsetY, NaoConfig::getInstance()->ThighLength, NaoConfig::getInstance()->TibiaLength, NaoConfig::getInstance()->FootHeight);
	if(ileg == NaoConfig::LegIDRight){
		for(i = JointData::RHipYawPitch; i <= JointData::RAnkleRoll; ++i)
		{
			djoint = jtmp.angles[i] - jointCmd.angles[i];
			if(bUseJointsIncreasingLimit == true){
				djoint = abs(djoint) > jointIncreasingLimit ? sign(djoint) * jointIncreasingLimit : djoint;
			}
			jointCmd.angles[i] += djoint;
		}
	}else
	{
		for(i = JointData::LHipYawPitch; i <= JointData::LAnkleRoll; ++i)
		{
			djoint = jtmp.angles[i] - jointCmd.angles[i];
			if(bUseJointsIncreasingLimit == true){
				djoint = abs(djoint) > jointIncreasingLimit ?sign(djoint) * jointIncreasingLimit : djoint;
			}
			jointCmd.angles[i] += djoint;
		}
	}
}

RotationMatrix Kinematics::rodrigues(Vector3<double> a, double radians)
{
	a = a.normalize();
	RotationMatrix ac(Vector3<double>(0, a.z, -a.y), 
								Vector3<double>(-a.z, 0, a.x),
								Vector3<double>(a.y, -a.x, 0));
	return (RotationMatrix() + ac * sin(radians) + ac * ac * (1-cos(radians)));

}