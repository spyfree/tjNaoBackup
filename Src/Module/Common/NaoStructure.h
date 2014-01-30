/**
* @file NaoStructure.h
*
* The file defines a class to represent NaoStructure.
* 
* @author XU Tao
*/
#ifndef __NaoStructure_h_
#define __NaoStructure_h_

#include "NaoConfig.h"
#include "JointData.h"
#include "Tools/Math/Vector3.h"
#include "Tools/Math/Matrix.h"
#include <string>
#include <vector>
class NaoStructure
{
public:
	struct Linkage 
	{
		Linkage();
		std::string name;                     		//名字              
		int jointID;                          		//连杆对应的关节ID                     
		int sister;                           		//姐妹连杆编号                           
		int child;                            		//子连杆编号                             
		int mother;                           		//母连杆编号                             
		Vector3<double> p;                    		 //在世界坐标系中位置(m)                 
		RotationMatrix R;                     		//在世界坐标系中姿态, 旋转               
		Vector3<double> v;                    		 //在世界坐标系中速度(m/s)               
		Vector3<double> w;                    		 //在世界坐标系中角速度(rad/s)           
		double q;                             		//关节角                                 
		double dq;                            		//关节速度                               
		double ddq;                           		//关节角加速度                           
		Vector3<double> a;                    		 //关节轴矢量(相对于母杆? 具体见骨架图)  
		Vector3<double> aw;                   		//关节轴矢量(世界坐标系中)               
		Vector3<double> b;                    		 //相对位置(相对于母连杆)                
		double m;                             		//质量(kg)                               
		Vector3<double> c;                    		 //质心位置(在连杆局部坐标系中)          
		Matrix3x3<double> I;                  		//惯性张量(在连杆局部坐标系中)           
		std::vector< Vector3<double> > vertex;		//形状(顶点信息,在局部坐标系中)          
		//std::vector< Vector3<double> > face;		//形状(连接情况)
		int chainID;
		/*Linkage& operator=(Linkage& other);*/
	};

	enum
	{
		noneLink = 0,	//the first one of the uLink array will not be used, to keep with Matlab code;
		bodyLink = 1,
		headLink = 4,
		headEndLink = 5,
		rHandLink = 10,
		lHandLink = 15,
		rFootLink = 22,
		lFootLink = 29,
		numULinks = 30
	};
	enum Chain{
		CBody = 0, 
		CHead,
		CRArm, 
		CLArm, 
		CRLeg, 
		CLLeg,
		NumChains,
		NonChain = NumChains
	};
	enum TASK_SPACE
	{
		Space_Body = 0, 
		Space_Supprot_Leg, 
		Space_World
	};
	Linkage uLink[numULinks]; /**< The Linkages sturcture of the rotbot. */
	Vector3<double> pCOM;	/**< The COM relative to supporting foot */
	Vector3<double> supportFootPosWorld;/**< The world position of supporting foot, x, y, z, in m*/
	Vector3<double> supportFootRotWorld;/**< The world (x, y, z) rotation angles of supporting foot */
	int supportingMode;/**<  Current Supporting Mode, See NaoConfig class*/
	int lastSupportingMode; /**< The Suppporting mode of last frame*/
	Vector3<double> bodyAngles;//body Angles(Rollx, tilty, 0) from sensor data...
	NaoStructure();

	void setLinksJoints(const JointData& joints);
	int getLinkIndex(int jointID) const { return linkFromJoints[jointID]; }
	void switchSupportFoot(int supFoot);
	void setBodyAngles(const Vector3<double>& angles)
	{
		bodyAngles = angles;
	}
	Vector3<double> getWorldPos() const
	{
		return supportFootPosWorld;
	}
	Vector3<double> getWorldCOMPos() const
	{
		RotationMatrix R;
		R.fromKardanRPY(supportFootRotWorld.x, supportFootRotWorld.y, supportFootRotWorld.z);
		return (supportFootPosWorld + R * pCOM);
	}
	Vector3<double> getWorldPosition(int link) const
	{
		RotationMatrix R;
		R.fromKardanRPY(supportFootRotWorld.x, supportFootRotWorld.y, supportFootRotWorld.z);
		return (supportFootPosWorld + R * uLink[link].p);
	}
	Vector3<double> getWorldRotation(int link) const
	{
		RotationMatrix R;
		R.fromKardanRPY(supportFootRotWorld.x, supportFootRotWorld.y, supportFootRotWorld.z);
		return RotationMatrix(R * uLink[link].R).getXYZAngles();
	}

	Vector3<double> getCoM(int space) const
	{
		RotationMatrix R(bodyAngles.x, bodyAngles.y, 0);
		switch(space)
		{
		case Space_Body:
			return uLink[bodyLink].R.invert() * (pCOM - uLink[bodyLink].p);
			break;
		case Space_World:
			/*R = R * (uLink[bodyLink].R.invert());*/
			return supportFootPosWorld + (R.rotateZ(supportFootRotWorld.z)) * pCOM;
			//return 
			break;
		default://Space_Supprot_Leg
			return pCOM;
			break;
		}
	}

	Vector3<double> getRotAngles(int link, int space) const
	{
		RotationMatrix R;
		switch(space)
		{
		case Space_Body:
			R = (uLink[bodyLink].R.invert() * uLink[link].R);
			break;
		case Space_World:
			R = RotationMatrix(bodyAngles.x, bodyAngles.y, 0)/* * (uLink[bodyLink].R.invert())*/;
			R = ((R.rotateZ(supportFootRotWorld.z)) * uLink[link].R);
			break;
		default://Space_Supprot_Leg
			R = uLink[link].R;
			break;
		}
		return R.getXYZAngles();
	}

	Vector3<double> getPosition(int link, int space) const
	{
		RotationMatrix R;
		switch(space)
		{
		case Space_Body:
			return uLink[bodyLink].R.invert() * (uLink[link].p - uLink[bodyLink].p);
			break;
		case Space_World:
			R = RotationMatrix(bodyAngles.x, bodyAngles.y, 0)/* * (uLink[bodyLink].R.invert())*/;
			return supportFootPosWorld + (R.rotateZ(supportFootRotWorld.z)) * uLink[link].p;
			//return 
			break;
		default://Space_Supprot_Leg
			return uLink[link].p;
			break;
		}
	}

	const RotationMatrix& getRotationMatrix(int link) const{
		return uLink[link].R;
	}
	Vector3<double> getRotationAxis(int link, int space) const 
	{
		RotationMatrix R;
		Vector3<double> a = uLink[link].a;
		switch(space)
		{
		case Space_Body:
			return uLink[bodyLink].R.invert() * (a.normalize());
			break;
		case Space_World:
			R = RotationMatrix(bodyAngles.x, bodyAngles.y, 0)/* * (uLink[bodyLink].R.invert())*/;
			return supportFootPosWorld + (R.rotateZ(supportFootRotWorld.z)) * (a.normalize());
			break;
		default://Space_Supprot_Leg
			return uLink[link].a;
			break;
		}
	}
// 	NaoStructure& operator=(const NaoStructure& other)
// 	{
// 		for(int i = 0; i < numULinks; ++i)
// 		{
// 			uLink[i] = other.uLink[i];
// 		}
// 		pCOM  = other.pCOM;
// 		return *this;
// 	}
private:
	void buildNaoStructure();
	int linkFromJoints[JointData::numOfJoints];
	/** Get Inertia Matrix a cylinder
	* @param m mass
	* @param r radius
	* @param h height
	* @param rotA rotation angles of the cylinder, original cylinder refer to List Moment of Inertia Matrix.pdf, or wikipedia
	* @return Inertia Matrix
	*/
	Matrix3x3<double> createCylinderIM(double m, double r, double h, Vector3<double> rotA);
	Matrix3x3<double> createBoxIM(double m, double x, double y,double z, Vector3<double> rotA);
	Matrix3x3<double> createBoxIM(double m, double x, double y,double z)
	{
		return createBoxIM(m, x, y, z, Vector3<double>(0,0,0));
	}
	Matrix3x3<double> createSphereIM(double m, double r, Vector3<double> rotA);
	Matrix3x3<double> createSphereIM(double m, double r)
	{
		return createSphereIM(m, r, Vector3<double>(0,0,0));
	}
	Matrix3x3<double> createZeroIM();
	//Vector3<double> getPosition(const Vector3<double>& target, int space)
};
#endif
