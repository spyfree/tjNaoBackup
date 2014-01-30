/**
* @file RobotKinematics.h
*
* The file provider with some kinematics related functions.
* 
* @author XU Tao
*/

#ifndef __Kinematics_h_
#define __Kinematics_h_
#include "Tools/Math/Vector3.h"
#include "Module/Common/NaoStructure.h"
namespace Kinematics
{
	/**calculate the center of mass
	* @param naoStruct the structure of robot, (links)
	* @return the position of the COM.
	*/
	extern Vector3<double> calcCOM(const NaoStructure& naoStruct);
	/**A recursive function to help the calcCOM to calculate the m*COM of each children link
	* @param naoStruct the structure of robot, (links)
	* @param j from the j link to calculate.
	* @return mc
	*/
	extern Vector3<double> calcMC(const NaoStructure& naoStruct, int j);

	/**A recursive function to calculate posture of each link, relative to link j coordinate
	* @param naoStruct the structure of robot, (links)
	*/
	extern void execForwardKinematics(NaoStructure& naoStruct, int j);
	
	/**
	* 根据当前脚掌的姿态和世界位置, 重新计算身体位置和姿态
	* 主要目的是以支撑脚为原心, 支撑脚脚面水平接触地面
	* @param naoStruct the structure of robot, (links)
	* @param supFootPos position of supporting leg in world coordinate.
	* @param sup_mode supporting mode.
	* @return rotation angles (rx, ry, rz) of supporting leg 
	*/
	extern Vector3<double> recalcBodyPos2SupFoot(NaoStructure& naoStruct, const Vector3<double>& supFootPos, int sup_mode);
	
	/**
	* Get the rx, ry, rz rotation angles of supporting foot in world coordinate.
	* @return a vector [rx, ry, rz]
	*/
	extern Vector3<double> getSupFootWorldRotationAngle(const NaoStructure& naoStruct, int sup_mode);

	/**
	* Get the rx, ry, rz rotation angles from rotation matrix
	* @return a vector [rx, ry, rz]
	*/
	//extern Vector3<double> getRotAngleFromRotMatrix(const RotationMatrix& rMatrix);

	/**Nao invert-kinematic
	* @param jointCmd store result joints data, 
	* @param leg: right:1; left:-1;
	* @param Pf, Rf, position and rotation of left foot in world coordinate,Pf:1 by 3, Rf: 3 by 3
	* @param Pb, Rb, position and rotation of body in world coordinate
	* @param l0,w0,l1,l2,l3, robot leg limb configuration, in m
	*/
	extern void doNaoLegInvKinematics(JointCmd& jointCmd, int ileg, const Vector3<double>& Pf, const RotationMatrix& Rf, 
		const Vector3<double>& Pb, const RotationMatrix& Rb, double l0, double w0, double l1, double l2, double l3);
	/**
	* Calculate joints value from leg destination posture and body posture, with clip of joints for safety
	* @param jointCmd buffer to store joints, it contains the joints of last frame when input, and contains the result joints when output
	* @param ileg Which leg
	* @param psfoot Position of foot Ankle
	* @param frz Rotation Matrix of foot
	* @param bodypos Position of body
	* @param bodyR Rotation Matrix of body
	*/
	extern void calculateSafeLegJoints(JointCmd& jointCmd, int ileg, const Vector3<double>& psfoot, const RotationMatrix& frz, 
		const Vector3<double>& bodypos, const RotationMatrix& bodyR);
	/**Nao invert-kinematic
	* @param jointCmd store result joints data, 
	* @param leg: right:1; left:-1;
	* @param Pf, Rf, position and rotation of left foot in world coordinate,Pf:1 by 3, Rf: 3 by 3
	* @param Pb, Rb, position and rotation of body in world coordinate
	* @param l0,w0,l1,l2,l3, robot leg limb configuration, in m
	*/
	extern void doNaoLegInvKinematics(JointCmd& jointCmd, int ileg, const Vector3<double>& Pf, const RotationMatrix& Rf, 
		const Vector3<double>& Pb, const RotationMatrix& Rb, double rotRHipYawPitch, double deltaRRollX, double deltaRPitchY, double l0, double w0, double l1, double l2, double l3);
	/**
	* Calculate joints value from leg destination posture and body posture, with clip of joints for safety
	* @param jointCmd buffer to store joints, it contains the joints of last frame when input, and contains the result joints when output
	* @param ileg Which leg
	* @param psfoot Position of foot Ankle
	* @param frz Rotation Matrix of foot
	* @param bodypos Position of body
	* @param bodyR Rotation Matrix of body
	*/
	extern void calculateSafeLegJoints(JointCmd& jointCmd, int ileg, const Vector3<double>& psfoot, const RotationMatrix& frz, 
		const Vector3<double>& bodypos, const RotationMatrix& bodyR, double rotRHipYawPitch, double deltaRRollX, double deltaRPitchY);

	/**
	* Rorgigues Fomulation
	* @param a The Vector of rotation axis, unit vector.
	* @param radians Angles of rotation around this axis
	* @return Rotation Matrix
	*/
	extern RotationMatrix rodrigues(Vector3<double> a, double radians);

}

#endif