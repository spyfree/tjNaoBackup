#include "SensorComm.h"
SensorComm::SensorComm()
{
	//2 这里非常危险， 如果theTJImage为空，则会引发严重异常
	//2 所以一定要确保KS在blackboard的Representation后初始化

	TJScanVision=new ScanVision(*theTJImage, *theCameraMatrix);
}

SensorComm::~SensorComm()
{
   if(TJScanVision!=NULL)
   {
	   delete TJScanVision;
	   TJScanVision=NULL;
   }
}

void SensorComm::update(Regionpercept* theRegionpercept)
{
	//std::cout<<"+++++++++++++++++  start SensorComm::update  +++++++++++++++++ "<<endl;
	unsigned visionstartTime = SystemCall::getCurrentSystemTime();

	inTJImage();
	// TODO: in camera matrix and so on...


	TJScanVision->ResetScanVision();

	//====Generate Horizontal Line
	horizonLine = GenerateHorizon();
	TJScanVision->setNewHorizon(horizonLine);
	//cameraMatrixCalibration
#ifdef CameraMatrixCalibration
	/* The CameraMatrixCalibration location of the robot
		# 1---------2-----3------4
		# |        |      |       |
		# |         5---- 6       |
		# |            7           |
		# |           ___          |
		# |          /   \         |
		# 9------------8---------- 10|
		# |          \___/         |
		# |                        |
		# |          Robot        
		# |          ----          |
		# |         |     |        |
		# |---------------------4
	*/// 尺寸按实际比赛场地测得
	const Vector3<double> fieldpoint1=Vector3<double>(4185.0,2000.0,0.0);//(4200.0,2000.0,0.0);//The 1th
	Vector2<int> fieldpoint1_pointInImage(0,0);
	Geometry::calculatePointInImage(fieldpoint1,*theCameraMatrix,fieldpoint1_pointInImage);
	DRAW_BIG_DOT(fieldpoint1_pointInImage.x,fieldpoint1_pointInImage.y,Drawings::black,Drawings::red);

	const Vector3<double> fieldpoint2=Vector3<double>(4185.0,1115.0,0.0);//(4200.0,1100.0,0.0);//The 2th
	Vector2<int> fieldpoint2_pointInImage(0,0);
	Geometry::calculatePointInImage(fieldpoint2,*theCameraMatrix,fieldpoint2_pointInImage);
	DRAW_BIG_DOT(fieldpoint2_pointInImage.x,fieldpoint2_pointInImage.y,Drawings::black,Drawings::red);

	const Vector3<double> fieldpoint3=Vector3<double>(4185.0,-1065.0,0.0);//(4200.0,-1100.0,0.0);//The 3th
	Vector2<int> fieldpoint3_pointInImage(0,0);
	Geometry::calculatePointInImage(fieldpoint3,*theCameraMatrix,fieldpoint3_pointInImage);
	DRAW_BIG_DOT(fieldpoint3_pointInImage.x,fieldpoint3_pointInImage.y,Drawings::black,Drawings::red);

	const Vector3<double> fieldpoint4=Vector3<double>(4185.0,-1975.0,0.0);//(4200.0,-2000.0,0.0);//The 4th
	Vector2<int> fieldpoint4_pointInImage(0,0);
	Geometry::calculatePointInImage(fieldpoint4,*theCameraMatrix,fieldpoint4_pointInImage);
	DRAW_BIG_DOT(fieldpoint4_pointInImage.x,fieldpoint4_pointInImage.y,Drawings::black,Drawings::red);

	const Vector3<double> fieldpoint5=Vector3<double>(3585.0,1115.0,0.0);//(3600.0,1100.0,0.0);//The 5th
	Vector2<int> fieldpoint5_pointInImage(0,0);
	Geometry::calculatePointInImage(fieldpoint5,*theCameraMatrix,fieldpoint5_pointInImage);
	DRAW_BIG_DOT(fieldpoint5_pointInImage.x,fieldpoint5_pointInImage.y,Drawings::black,Drawings::red);

	const Vector3<double> fieldpoint6=Vector3<double>(3585.0,-1065.0,0.0);//(3600.0,-1100.0,0.0);//The 6th
	Vector2<int> fieldpoint6_pointInImage(0,0);
	Geometry::calculatePointInImage(fieldpoint6,*theCameraMatrix,fieldpoint6_pointInImage);
	DRAW_BIG_DOT(fieldpoint6_pointInImage.x,fieldpoint6_pointInImage.y,Drawings::black,Drawings::red);

	const Vector3<double> fieldpoint7=Vector3<double>(2400.0,0.0,0.0);//The 7th
	Vector2<int> fieldpoint7_pointInImage(0,0);
	Geometry::calculatePointInImage(fieldpoint7,*theCameraMatrix,fieldpoint7_pointInImage);
	DRAW_BIG_DOT(fieldpoint7_pointInImage.x,fieldpoint7_pointInImage.y,Drawings::black,Drawings::red);

	const Vector3<double> fieldpoint8=Vector3<double>(1175.0,0.0,0.0);//(1200.0,0.0,0.0);//The 8th
	Vector2<int> fieldpoint8_pointInImage(0,0);
	Geometry::calculatePointInImage(fieldpoint8,*theCameraMatrix,fieldpoint8_pointInImage);
	DRAW_BIG_DOT(fieldpoint8_pointInImage.x,fieldpoint8_pointInImage.y,Drawings::black,Drawings::red);

	const Vector3<double> fieldpoint9=Vector3<double>(0.0,2000.0,0.0); //9th
	Vector2<int> fieldpoint9_pointInImage(0,0);
	Geometry::calculatePointInImage(fieldpoint9,*theCameraMatrix,fieldpoint9_pointInImage);
	DRAW_BIG_DOT(fieldpoint9_pointInImage.x,fieldpoint9_pointInImage.y,Drawings::black,Drawings::red);

	const Vector3<double> fieldpoint10=Vector3<double>(0.0,-1975.0,0.0); //10th
	Vector2<int> fieldpoint10_pointInImage(0,0);
	Geometry::calculatePointInImage(fieldpoint10,*theCameraMatrix,fieldpoint10_pointInImage);
	DRAW_BIG_DOT(fieldpoint10_pointInImage.x,fieldpoint10_pointInImage.y,Drawings::black,Drawings::red);


	DRAW_LINE(fieldpoint1_pointInImage.x,fieldpoint1_pointInImage.y,fieldpoint2_pointInImage.x,fieldpoint2_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::red);
	DRAW_LINE(fieldpoint2_pointInImage.x,fieldpoint2_pointInImage.y,fieldpoint3_pointInImage.x,fieldpoint3_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::red);
	DRAW_LINE(fieldpoint3_pointInImage.x,fieldpoint3_pointInImage.y,fieldpoint4_pointInImage.x,fieldpoint4_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::red);
	DRAW_LINE(fieldpoint2_pointInImage.x,fieldpoint2_pointInImage.y,fieldpoint5_pointInImage.x,fieldpoint5_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::red);
	DRAW_LINE(fieldpoint3_pointInImage.x,fieldpoint3_pointInImage.y,fieldpoint6_pointInImage.x,fieldpoint6_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::red);
	DRAW_LINE(fieldpoint5_pointInImage.x,fieldpoint5_pointInImage.y,fieldpoint6_pointInImage.x,fieldpoint6_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::red);
	DRAW_LINE(fieldpoint7_pointInImage.x,fieldpoint7_pointInImage.y,fieldpoint8_pointInImage.x,fieldpoint8_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::red);
	DRAW_LINE(fieldpoint1_pointInImage.x,fieldpoint1_pointInImage.y,fieldpoint9_pointInImage.x,fieldpoint9_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::red);
	DRAW_LINE(fieldpoint4_pointInImage.x,fieldpoint4_pointInImage.y,fieldpoint10_pointInImage.x,fieldpoint10_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::red);
	DRAW_LINE(fieldpoint9_pointInImage.x,fieldpoint9_pointInImage.y,fieldpoint10_pointInImage.x,fieldpoint10_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::red); 
	
	// 	const Vector3<double> fieldpoint1=Vector3<double>(2400.0,0.0,0.0);//The 7th
// 	Vector2<int> fieldpoint1_pointInImage(0,0);
// 	const Vector3<double> fieldpoint2=Vector3<double>(3600.0,1100.0,0.0);//The 5th
// 	Vector2<int> fieldpoint2_pointInImage(0,0);
// 
// 	Geometry::calculatePointInImage(fieldpoint1,*theCameraMatrix,fieldpoint1_pointInImage);
// 	DRAW_BIG_DOT(fieldpoint1_pointInImage.x,fieldpoint1_pointInImage.y,Drawings::red,Drawings::red);
// 
// 	Geometry::calculatePointInImage(fieldpoint2,*theCameraMatrix,fieldpoint2_pointInImage);
// 	DRAW_BIG_DOT(fieldpoint2_pointInImage.x,fieldpoint2_pointInImage.y,Drawings::black,Drawings::red);
#endif



	unsigned processtime=SystemCall::getCurrentSystemTime();
	TJScanVision->proccess();
	unsigned processtimeUsed=SystemCall::getTimeSince(processtime);
	//std::cout<<"vision processtime is "<<processtimeUsed<<" ms "<<std::endl;


	*theRegionpercept = TJScanVision->RegionRun;
	
}

void SensorComm::inTJImage()
{

}

void SensorComm::inCameraMatrix()
{

}
Geometry::GTLine SensorComm::GenerateHorizon() {
	Geometry::GTLine GTHorizon;
    CameraMatrix* cameraMatrix;
	cameraMatrix=theCameraMatrix;
	double r31 = cameraMatrix->rotation(2,0);//cameraMatrix[0][2];
	double r32 = cameraMatrix->rotation(2,1);//cameraMatrix[1][2];
	double r33 = cameraMatrix->rotation(2,2);//cameraMatrix[2][2];
	if(r33 == 0)
		r33 = 0.00001;
	double alpha = (FOVx/2.0);
	double cotAlpha = 1.0/tan(alpha);
	double x1 = 0,
		x2 = IMAGE_WIDTH - 1,
		v2 = IMAGE_WIDTH/2.0,
		v3 = IMAGE_HEIGHT/2.0,
		v1 = FOCAL_LENGTH,//v1*cotAlpha,// FOCAL_LENGTH,
		zr = (v3 * r33 + r31 * v1 - r32 * v2) / r33,
		zl = (v3 * r33 + r31 * v1 + r32 * v2) / r33;

	if (cameraMatrix->rotation(2,2)< 0) {  //2 UnCertain!!
		double t = x1;
		x1 = x2;
		x2 = t;
		t = zr;
		zr = zl;
		zl= t;
	}

 	GTHorizon.base.x = (x1 + x2) / 2.0;
	GTHorizon.base.y = (zr + zl) / 2.0;
 	GTHorizon.direction.x = x2 - x1;
 	GTHorizon.direction.y = zr - zl;
 	GTHorizon.normalizeDirection();



	return GTHorizon;
}
MAKE_KS(SensorComm)
