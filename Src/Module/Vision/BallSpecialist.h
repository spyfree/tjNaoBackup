/**
*
*@file BallSpecialist.h  the algorithm of detecting ball based on pixel information
* @author
*/

#ifndef _BALL_SPECIALIST_H_
#define _BALL_SPECIALIST_H_

#include "Module/Vision/DebugDrawings.h"
#include "Tools/Math/Vector2.h"
#include "Module/Vision/Run.h"
#include "Module/Vision/TJImage.h"
#include "Module/Common/VisionHeader.h"
#include "Module/Common/Geometry.h"
#include "Module/Representations/KS.h"
#include "Module/Representations/Blackboard.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Vision/Regionpercept.h"
#include "Module/Representations/Perception/BallPercept.h"
#include "Module/Actuator/ProxyContainer.h"
#include <vector>
#include "Tools/Math/Common.h"
#include "Module/Common/FrameInfo.h"
#include "Module/Representations/Perception/BallSpots.h"
#include "Module/Network/RoboCupGameControlData.h"
#include "Tools/TJAssign.h"
#include <iostream>
#include<string>
#include "Platform/GetSetting.h"
using namespace std;
DEFINE_KS(BallSpecialist)
REQUIRES(TJImage)
REQUIRES(CameraMatrix)
REQUIRES(BallSpots)
REQUIRES(FrameInfo)
PROVIDES(BallPercept)
USES(BallPercept)
USES(TeamCfgInfo)
USES(RoboCupGameControlData)
END_KS
/**
The BallSpecialist finds a single ball in an image. 找到一个球（通过算法检查的色块）立即返回
*/

class BallSpecialist:public BallSpecialistBase,BaseProvider
{

	/**
	* Searches for the ball in the image, starting from the specified point.
	*/
public:
	BallSpecialist();
	~BallSpecialist();
	class Paraments:public paramAssign
	{
	public:

        double sqrMaxBallDistance;//球相对于机器人的最大距离
		double clippingApproxRadiusScale;//2.0
		double clippingApproxRadiusPixelBonus;//2.5
		double scanMaxColorDistance;//30
		double checkMaxRadiusDifference;//1.6
		double scanPixelTolerance;//2
	    double refineMaxPixelCount;//2
        double refineMaxColorDistance;//35
        double checkMinRadiusDifference;//0.9
		double checkMinRadiusPixelBonus;//6.0
		double checkOutlineRadiusScale;//1.1
		double checkOutlineRadiusPixelBonus;//2.0
		double minRadiusthreshold;//6.5
		double minApproDiameterforScan;//10.0当球很小时，计算近似半径时会有较大误差，将扫描length设为一个稍大的值
		void massAssign()
		{
			ASSIGN(sqrMaxBallDistance);
			ASSIGN(clippingApproxRadiusScale);
			ASSIGN(clippingApproxRadiusPixelBonus);
			ASSIGN(scanMaxColorDistance);
			ASSIGN(checkMaxRadiusDifference);
			ASSIGN(scanPixelTolerance);
			ASSIGN(refineMaxPixelCount);
			ASSIGN(refineMaxColorDistance);
			ASSIGN(checkMinRadiusDifference);
			ASSIGN(checkMinRadiusPixelBonus);
			ASSIGN(checkOutlineRadiusScale);
			ASSIGN(checkOutlineRadiusPixelBonus);
			ASSIGN(minRadiusthreshold);
			ASSIGN(minApproDiameterforScan);

		}
	};



	void update(BallPercept* theBallPercept);
	void inTJImage(){}
	void inCameraMatrix(){}
	void inBallSpots(){}
	void inFrameInfo(){}
	void outBallPercept();




private:
	/**
	* The class represents a ball point.
	*/
	class BallPoint
	{
	public:
		Vector2<int> step;
		Vector2<int> start;
		Vector2<int> point;
		Vector2<double> pointf;
		bool atBorder;
		bool isValid;

		BallPoint() : atBorder(false), isValid(false) {}
	};
	struct PixelYUV
	{
		unsigned char y,u,v;
	};

    Paraments BallParaments;

    BallPoint ballPoints[8];
    Vector2<int> approxCenter2;
    int totalPixelCount;
    int totalCbSum;
    int totalCrSum;
    PixelYUV startPixel;
    int approxRadius1;/*caculate the approximate radius of the ball according to the bearing of ballspot*/
    unsigned int validBallPoints; /**< Count of usable points on the outer edge of the ball. */
	Vector2<int> center; /**< Center of the ball in image. */
	double radius; /**< Radius of the ball in image. */
    Vector3<double> sizeBasedCenterOnField;//基于size计算的球在场地上的位置
	Vector3<double>bearingBasedCenterOnField;//基于bearing计算的球在场地上的位置
    Vector3<double> usedCenterOnField;

	BallPercept lastPercept;
	Vector2<double> velocity; // the ball position offset divided by time
	ProxyContainer myProxyContainer;


	 /* private fuction for ball cognition*/
	bool checkBallSpot(const BallSpot& ballSpot);

	/** Scan for the ball starting at a given trigger point */
	bool searchBallPoints(const BallSpot& ballSpot);

	/* fine the end of orange region*/
	bool findEndOfBall(const Vector2<int>&start,PixelYUV pixel,const Vector2<int> &step,int maxLength,BallPoint&ballPoint);

	/*find the end of orange region based on color table */
	bool findEndOfBall(const Vector2<int>& start,unsigned char ballColor,const Vector2<int> &step,int maxLength,BallPoint&ballPoint);

	/*check ballPoints whether it is ok*/
	bool checkBallPoints();

	/*the check after computer the center and radius of the ball*/
	bool evaluateBallInImage();

	/*calculate the center and radius of the ball in the image*/
	bool calculateBallInImage();

	/*calculate the point position of ball on the field according to the coordinate system*/
	bool calculateBallOnField();

	/**
	* The function tries to calculate the ball percept by using the Levenberg-Marquardt
	* algorithm. The function fails if less than 3 points are available.
	* @return true if ball percept was created
	*/
	bool createBallPerceptLevenbergMarquardt(Vector2<int>& center,double& radius);

	void sendBallResult();

	void keeperDive();

};

#endif
