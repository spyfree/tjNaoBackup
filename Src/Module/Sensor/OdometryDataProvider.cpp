#include "OdometryDataProvider.h"
//#include "Module/Actuator/MotionDebugging.h"
//#include "Module/Actuator/MatlabHelp/MatlabHelp.h"
#include <iostream>
#include <fstream>
using namespace std;
//void OdometryDataProvider::update(InertiaMatrix* theInertiaMatrix)
//{
	//const Vector3<double> axis(theSafeSensorData->data[SensorData::angleX],theSafeSensorData->data[SensorData::angleY],theNaoStructure->uLink[NaoStructure::bodyLink].R.getZAngle());
 //   RotationMatrix inertiaRotation;
	//inertiaRotation.fromKardanRPY(axis.x,axis.y,axis.z);
	//const double newBody2SupZAngle = theNaoStructure->uLink[NaoStructure::bodyLink].R.getZAngle();
 //
	//int supfootLink = 0;
	//int swingfootLink = 0;
	//bool useLeft;
	//if(theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_LEFT || theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	//{
	//	supfootLink = NaoStructure::lFootLink;
	//	swingfootLink = NaoStructure::rFootLink;
	//	useLeft = true;
	//}else
	//{
	//	supfootLink = NaoStructure::rFootLink;
	//	swingfootLink = NaoStructure::lFootLink;
	//	useLeft = false;
	//}
	//RotationMatrix tmprotz;
	//tmprotz.fromKardanRPY(0.0,0.0,lastBody2SupZAngle);
 //   if(lastsupmod != theNaoStructure->supportingMode && (theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_LEFT || theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_RIGHT))
	//{
	//		theInertiaMatrix->translation = lastSwingFoot.rotation.invert() * theInertiaMatrix->translation;
	//		theInertiaMatrix->rotation = lastSwingFoot.rotation.invert() * theInertiaMatrix->rotation;
 //           lastFootSpan = lastSwingFoot.rotation.invert() * (Vector3<double>(0.0,0.0,0.0)-lastFootSpan);
 //           tmprotz = lastSwingFoot.rotation.invert() * tmprotz;
	//		lastBody2SupZAngle = tmprotz.getZAngle();
	//}
 //   Vector3<double> newFootSpan(theNaoStructure->uLink[supfootLink].p - theNaoStructure->uLink[swingfootLink].p);
	//Pose3D newInertiaMatrix;
	//newInertiaMatrix.translate(newFootSpan.x/2.0,newFootSpan.y/2.0,0.0);
	//newInertiaMatrix.translate(theNaoStructure->uLink[NaoStructure::bodyLink].p);
	//newInertiaMatrix.rotation = inertiaRotation;
	////newInertiaMatrix.translate(0.08,0.06,0.27);
 //
	//if (theInertiaMatrix->translation.z!=0)
	//{
	//	/*Pose3D& inertiaOffset(theInertiaMatrix->inertiaOffset);
	//	inertiaOffset = theInertiaMatrix->invert();
	//	inertiaOffset.translate(lastFootSpan.x/2.0,lastFootSpan.y/2.0,0.0);
	//	inertiaOffset.translate(newFootSpan.x/-2.0,newFootSpan.y/-2.0,0.0);
	//	inertiaOffset.translate(newInertiaMatrix.translation);
	//	inertiaOffset.rotateZ(newBody2SupZAngle - lastBody2SupZAngle);
	//	std::cout<<"deltabodyangle  "<<(newBody2SupZAngle - lastBody2SupZAngle)*180/3.14<<"  ";
	//	inertiaOffset.rotate(newInertiaMatrix.rotation);
	//	std::cout<<"--iOffset--  "<<inertiaOffset.rotation.getXYZAngles().x*180/3.14<<"   "<<inertiaOffset.rotation.getXYZAngles().y*180/3.14<<"   ";*/
 //       theInertiaMatrix->inertiaOffset.rotation = theNaoStructure->uLink[supfootLink].R;
	//	theInertiaMatrix->inertiaOffset.translation = (Vector3<double>(0.0,0.0,0.0)-theInertiaMatrix->translation);
 //
 //       theInertiaMatrix->inertiaOffset.translate(lastFootSpan.x/2.0,lastFootSpan.y/2.0,0.0);
 //       theInertiaMatrix->inertiaOffset.translate(newFootSpan.x/-2.0,newFootSpan.y/-2.0,0.0);
 //       theInertiaMatrix->inertiaOffset.translate(newInertiaMatrix.translation);
 //       theInertiaMatrix->inertiaOffset.rotateZ(newBody2SupZAngle - lastBody2SupZAngle);
	//}
 //
 //   Pose3D spanoffset;
	//spanoffset.translate(lastFootSpan.x/2.0,lastFootSpan.y/2.0,0.0);
	//spanoffset.translate(newFootSpan.x/-2.0,newFootSpan.y/-2.0,0.0);
	////std::cout<<"--InertiaMatrix.y--  "<<theNaoStructure->supportingMode<<"   "<<theInertiaMatrix->translation.y<<"    "<<newInertiaMatrix.translation.y<<std::endl;
	////std::cout<<"--delta_bodyangle--  "<<theNaoStructure->supportingMode<<"   "<<lastBody2SupZAngle*180/3.14<<"   "<<newBody2SupZAngle*180/3.14<<"   "<<(newBody2SupZAngle - lastBody2SupZAngle)*180/3.14<<"     ";/*std::endl;*/
	////std::cout<<"--spanoffset--  "<<spanoffset.translation.x<<"   ";
	////std::cout<<"--IMOffset--  "<<theInertiaMatrix->inertiaOffset.rotation.getZAngle()*180/3.14<<"   ";
 //   theInertiaMatrix->translation = newInertiaMatrix.translation;
	//theInertiaMatrix->rotation = newInertiaMatrix.rotation;
	//
 //   lastSwingFoot.rotation = theNaoStructure->uLink[swingfootLink].R;
 //   lastFootSpan = newFootSpan;
 //   lastBody2SupZAngle = newBody2SupZAngle;
	//lastsupmod = theNaoStructure->supportingMode;

	//tmpInertiaMatrix.translation = theInertiaMatrix->translation;
	//tmpInertiaMatrix.rotation = theInertiaMatrix->rotation;
	//tmpInertiaMatrix.inertiaOffset = theInertiaMatrix->inertiaOffset;

	//
	//Pose2D testoffset;
	//testoffset.translation.x = theInertiaMatrix->inertiaOffset.translation.x;
	//testoffset.translation.y = theInertiaMatrix->inertiaOffset.translation.y;
	//test = test + testoffset.translation;
	/*Vector2<double> savaOdoData(test);
	MDEBUG_SAVE(MDebug::idpOdometryData, savaOdoData)*/

//}
//void OdometryDataProvider::update(OdometryData* theOdometryData)
//{
//	//std::cout<<"--ODOData_IMOffset--  "<<tmpInertiaMatrix.inertiaOffset.rotation.getZAngle()*180/3.14<<std::endl;
//	Pose3D tmp1;
//    Pose2D odometryOffset;
//	Pose3D odometryOffset3D/*(lastInertiaMatrix)*/;
//	odometryOffset3D.translate(lastInertiaMatrix.translation);
//	odometryOffset3D.translate(tmpInertiaMatrix.inertiaOffset.translation);
//	odometryOffset3D.translate(Vector3<double>(0.0,0.0,0.0)-tmpInertiaMatrix.translation);
//	odometryOffset3D.rotation = tmpInertiaMatrix.inertiaOffset.rotation;
//
//	odometryOffset.translation.x = 1000*odometryOffset3D.translation.x;//unit of translation.x is mm
//	odometryOffset.translation.y = 1000*odometryOffset3D.translation.y;
//	odometryOffset.rotation = odometryOffset3D.rotation.getZAngle();
//
//	//std::cout<<"--odometryOffset.rot--   "<<odometryOffset.rotation*180/3.14<<std::endl;
//	//std::cout<<"--odoOffset--  "<<odometryOffset.translation.x<<std::endl;
//    *theOdometryData += odometryOffset;
//	//std::cout<<"-----------------------odoAngle  "<<theOdometryData->rotation*180/3.14<<std::endl;
//	//std::cout<<"-----------------------odoX  "<<theOdometryData->translation.x<<std::endl;
//	//std::cout<<"-----------------------odoY  "<<theOdometryData->translation.y<<std::endl;
//	lastInertiaMatrix.translation = tmpInertiaMatrix.translation;
//	lastInertiaMatrix.rotation = tmpInertiaMatrix.rotation;
//
//	//Vector2<double> savaOdoData(theOdometryData->translation);
// //   MDEBUG_SAVE(MDebug::idpOdometryData, savaOdoData)
//	selfMessageQueue->SetMessageData(idOdometryData,idActuatorThread,idVisionThread,
//		sizeof(OdometryData),(char*)theOdometryData);
//
//}
void OdometryDataProvider::update(OdometryData* theOdometryData)
{
	int supfootLink = 0;
	int swingfootLink = 0;
	//bool useLeft;
	if(theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_LEFT || theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		supfootLink = NaoStructure::lFootLink;
		swingfootLink = NaoStructure::rFootLink;
		//useLeft = true;
	}else
	{
		supfootLink = NaoStructure::rFootLink;
		swingfootLink = NaoStructure::lFootLink;
		//useLeft = false;
	}
	bool changed;
	changed = false;
	if(lastsupmod != theNaoStructure->supportingMode && (theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_LEFT || theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_RIGHT))
	{
		lastFootSpan = lastrSwg2Supfoot.invert()*(Vector3<double>(0.0,0.0,0.0)-lastFootSpan);
		lastrBody2Supfoot = lastrSwg2Supfoot.invert()*lastrBody2Supfoot;
		lastrSwg2Supfoot = lastrSwg2Supfoot.invert();
		changed = true;
		    //lastrBody2Supfoot = theNaoStructure->uLink[NaoStructure::bodyLink].R;
	}
	Vector3<double> newFootSpan(theNaoStructure->uLink[swingfootLink].p - theNaoStructure->uLink[supfootLink].p);
	if (changed)
	{
		newFootSpan = lastrSwg2Supfoot*newFootSpan;
	}
	
	if (flag)
	{
		lastFootSpan =newFootSpan;
		flag = 0;
	}
	SpanOffset = (newFootSpan - lastFootSpan)/2.0;
	RotationMatrix rSwg2Supfoot(theNaoStructure->uLink[swingfootLink].R);
	RotationMatrix rBody2Supfoot(theNaoStructure->uLink[NaoStructure::bodyLink].R);

	Pose2D odometryOffset;

    //odometryOffset.rotation = (rSwg2Supfoot.getZAngle() - lastrSwg2Supfoot.getZAngle())/2.0;
    odometryOffset.rotation = rBody2Supfoot.getZAngle() - lastrBody2Supfoot.getZAngle();//rad
	odometryOffset.translation.x = 1000*SpanOffset.x;//in mm
	odometryOffset.translation.y = 1000*SpanOffset.y;//in mm
	if (changed)
	{
		odometryOffset.translation.x = 0;
		odometryOffset.translation.y = 0;
	}

// 	//add a filter here
// 	if abs(odometryOffset.translation.x)>20
// 		odometryOffset.translation.x = 0;


	//std::cout<<"#######offsetXXXX===="<<odometryOffset.translation.x<<"#######OffsetYYYY====="<<odometryOffset.translation.y<<std::endl;
    *theOdometryData +=odometryOffset;
	//theOdometryData->translation.x += odometryOffset.translation.x;
	//theOdometryData->translation.y += odometryOffset.translation.y;
	//theOdometryData->rotation += odometryOffset.rotation;
	//std::cout<<"-------------------------odoY  "<<theNaoStructure->supportingMode<<"  "<<theNaoStructure->supportFootPosWorld.x<<std::endl;
	lastFootSpan = newFootSpan;
	//nextrsupfoot = theNaoStructure->uLink[swingfootLink].R;
    lastsupmod = theNaoStructure->supportingMode;
	//lastrsupfoot2world = rsupfoot2world;
	lastrSwg2Supfoot=rSwg2Supfoot;
	lastrBody2Supfoot = rBody2Supfoot;
	selfMessageQueue->SetMessageData(idOdometryData,idActuatorThread,idRobotThread,
		sizeof(OdometryData),(char*)theOdometryData);
	outOdometryData();
// 	double test1;
// 	selfMessageQueue->SearchMyMessage(idTest,idODESimulation,idActuatorThread,(char*)&test1,sizeof(double));
// 	if (abs(test1)<=360)
// 	{
// 		MATLABHELP_PLOT(test1,"oderot");
// 		MATLABHELP_PLOT(theOdometryData->rotation*180/3.14,"odorot");
// 	}
/*	std::cout<<"!!!!!!!!Odorot before = "<<theOdometryData->rotation<<std::endl;*/

/*	theOdometryData->rotation =theOdometryData->rotation*0.8522 + 0.3195*3.14/180;*/
// 	double testX;
// 	selfMessageQueue->SearchMyMessage(idTestX,idODESimulation,idActuatorThread,(char*)&testX,sizeof(double));
// 	if (abs(testX)<=5000)
// 	{
 		//MATLABHELP_PLOT(testX,"odeX");
// 		MATLABHELP_PLOT(theOdometryData->translation.x,"odoX");
// 	}
// 	double testY;
// 	selfMessageQueue->SearchMyMessage(idTestY,idODESimulation,idActuatorThread,(char*)&testY,sizeof(double));
// 	if (abs(testY)<=5000)
// 	{
// 		MATLABHELP_PLOT(testY,"odeY");
// 		MATLABHELP_PLOT(theOdometryData->translation.y,"odoY");
// 	}


	//std::cout<<"!!!!!!!!Odorot bef = "<<theOdometryData->rotation*180/3.14<<std::endl;
// 	OdometryData mmm;
// 	mmm = *theOdometryData;
//   	mmm.rotation =theOdometryData->rotation*0.8522 + 0.3195*3.14/180;
//   	mmm.rotation = normalize(mmm.rotation );
// 
//  	if (theMotionPlan->walkCmd.speed.translation.x!=0&&theMotionPlan->walkCmd.speed.translation.y==0&&theMotionPlan->walkCmd.speed.rotation==0)
//  	{
//  		//向前校准
//  		mmm.translation.x = theOdometryData->translation.x*1.0238+8.2005;
//  		mmm.translation.y = theOdometryData->translation.y*theOdometryData->translation.y*(-0.5827)-6.7878*theOdometryData->translation.y-31.5220;
//  	}
//  	else if (theMotionPlan->walkCmd.speed.translation.x==0&&theMotionPlan->walkCmd.speed.translation.y!=0&&theMotionPlan->walkCmd.speed.rotation==0)
//  	{
//  		//平移校准
//  		mmm.translation.x = theOdometryData->translation.x*theOdometryData->translation.x*1.98673303826669+theOdometryData->translation.x*(-5.67609629032795)+4.28445177536678;
//  		mmm.translation.y = 0.978682011234416*theOdometryData->translation.y+106.869854456775;
//  	}
//  	else
//  	{
//  		mmm.translation.x = theOdometryData->translation.x*1.0238+8.2005;
//  		mmm.translation.y = 0.978682011234416*theOdometryData->translation.y+106.869854456775;
// 
//  	}
 //	//向前校准
 //    mmm.translation.x = theOdometryData->translation.x*1.0238+8.2005;
 ////  	mmm.translation.y = theOdometryData->translation.y*theOdometryData->translation.y*(-0.5827)-6.7878*theOdometryData->translation.y-31.5220;
 //    //
 //	//平移校准
 //	//mmm.translation.x = theOdometryData->translation.x*theOdometryData->translation.x*1.98673303826669+theOdometryData->translation.x*(-5.67609629032795)+4.28445177536678;
 //	mmm.translation.y = 0.978682011234416*theOdometryData->translation.y+106.869854456775;
 //	//

// 	double testX;
// 	selfMessageQueue->SearchMyMessage(idTestX,idODESimulation,idActuatorThread,(char*)&testX,sizeof(double));
// 	if (abs(testX)<=5000)
// 	{
// 		//MATLABHELP_PLOT(testX,"odeX");
// 		//MATLABHELP_PLOT(mmm.translation.x,"odoX");
// 	}
// 	double testY;
// 	selfMessageQueue->SearchMyMessage(idTestY,idODESimulation,idActuatorThread,(char*)&testY,sizeof(double));
// 	if (abs(testY)<=5000)
// 	{
//MATLABHELP_PLOT(testY,"odeY");
// 		//MATLABHELP_PLOT(mmm.translation.y,"odoY");
// 	}
	//std::cout<<"!!!!!!!!Odorot after = "<<mmm.rotation*180/3.14<<std::endl;

	//std::cout<<"!!!!!!!!OdoX="<<theOdometryData->translation.x<<std::endl;
	//std::cout<<"!!!!!!!!OdoY="<<theOdometryData->translation.y<<std::endl;
	//selfMessageQueue->SetMessageData(idOdometryData,idActuatorThread,idVisionThread,
	//			sizeof(OdometryData),(char*)(&mmm));
// 	selfMessageQueue->SetMessageData(idOdometryData,idActuatorThread,idVisionThread,
// 		sizeof(OdometryData),(char*)(theOdometryData));

	//if(dataDebugcounter>=50/*&&dataDebugcounter<=1050*/)
	//{
// 		std::string path = "/opt/naoqi/lib/naoqi/OdometryData.txt";
// 		std::ofstream outFile(path.c_str(),ios::app);
// 		outFile<<"#Odata#  "<<theOdometryData->translation.x<<"   "<<theOdometryData->translation.y<<"   "<<theOdometryData->rotation<<"   "<<
// 			"   "<<odometryOffset.translation.x<<"   "<<odometryOffset.translation.y<<"   "<<odometryOffset.rotation<<"   "<<theNaoStructure->supportingMode<<"   "<<changed<<"    "<<newFootSpan.x<<"    "<<"\n";/*"   "<<supfootLink<<
// 			"   "<<theNaoStructure->uLink[swingfootLink].p.x<<"   "<<theNaoStructure->uLink[swingfootLink].p.y
// 			<<"   "<<theNaoStructure->uLink[supfootLink].p.x<<"   "<<theNaoStructure->uLink[supfootLink].p.y<<*/
// 		outFile.close();
	/*}
	dataDebugcounter++;*/
}


OdometryDataProvider::OdometryDataProvider()
{
	flag  =1;
	dataDebugcounter = 0;

}
void OdometryDataProvider::inNaoStructure()
{

}
void OdometryDataProvider::inSafeSensorData()
{

}
//void OdometryDataProvider::outInertiaMatrix()
//{
//
//}
void OdometryDataProvider::outOdometryData()
{

}
// void OdometryDataProvider::inMotionPlan()
// {
// 
// }
MAKE_KS(OdometryDataProvider)
