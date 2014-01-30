#include "ImageDialog.h"
#include "Module/Behavior/TJSimRobot.h"
ImageDialog::ImageDialog(TJSimRobot* tjRobot,SmartPtr<Synchronizer> _synchro, QWidget *parent)
:QWidget(parent),behaviorThread(tjRobot),theSynchronizer(_synchro)
{
	setWindowTitle(tr("Image Window"));
	setWindowIcon(QIcon(":/icons/Simulator.png"));
	setMouseTracking(1);
	initImageParament();
	simuNaoProvider=SimuNaoProvider::getInstance();
	selfSimulationData=SimulationData::getInstance(); //added by qiang
	//timer = new QTimer(this);
	//connect(timer, SIGNAL(timeout()),this, SLOT(getODEPic()));
	//timer->start(0);
	
}

ImageDialog::~ImageDialog()
{
	if (unclassifiedImage!=NULL)
	{
		delete unclassifiedImage;
		unclassifiedImage=NULL;
	}
	if (classifiedImage!=NULL)
	{
		delete classifiedImage;
		classifiedImage=NULL;
	}
	if (processUnclassifiedImage!=NULL)
	{
		delete processUnclassifiedImage;
		processUnclassifiedImage=NULL;
	}
// 	if (unclassifyed!=NULL)
// 	{
// 		delete[] unclassifyed;
// 		unclassifyed=NULL;
// 	}
	if (lut_!=NULL)
	{
		delete[] lut_;
		lut_ = NULL;
	}
	if (GenLut_!=NULL)
	{
		delete[] GenLut_;
		GenLut_=NULL;
	}
	if (visionResult!=NULL)
	{
		delete visionResult;
		visionResult=NULL;
	}
	if(goalResult!=NULL)
	{
		delete goalResult;
		goalResult=NULL;
	}
	if(ballResult!=NULL)
	{
		delete ballResult;
		ballResult=NULL;
	}
	if(CircleResult!=NULL)
	{
		delete CircleResult;
		CircleResult=NULL;
	}

}
void ImageDialog::initImageParament()
{
	//////////parament init
	lastFileName = tr("D:/Netpics/");
	lastLutName= tr("D:/Netpics/");
	tjImage.imageSize = FULL_IMAGE_WIDTH*FULL_IMAGE_HEIGHT*3;
	currentIndex=0;
	currentImageWidth_=FULL_IMAGE_WIDTH;
	currentImageHeight_=FULL_IMAGE_HEIGHT;
	NetFileIndex=0;
	ODEFileIndex=0;
	NetPicName= tr("D:/NetPics/RawH0.raw");

	unclassifiedImage=NULL;
	unclassifiedImage=new QImage(FULL_IMAGE_WIDTH, FULL_IMAGE_HEIGHT, QImage::Format_RGB32);

	classifiedImage=NULL;
	classifiedImage=new QImage(FULL_IMAGE_WIDTH, FULL_IMAGE_HEIGHT, QImage::Format_RGB32);
	
	processUnclassifiedImage=NULL;
	processUnclassifiedImage=new QImage(FULL_IMAGE_WIDTH, FULL_IMAGE_HEIGHT, QImage::Format_RGB32);

	imageBuffer=NULL;

	tempimageBuffer=NULL;

	//visionResult = (char*)malloc(netVisionInter_size);//add by hu
	visionResult = new netVisionInterface;
    goalResult= new netGoalpost;
	ballResult=new netBall;
	CircleResult=new netCircle;

// 	unclassifyed=NULL;
// 	unclassifyed=new unsigned char[normalSize];
// 	memset(unclassifyed,0,currentImageWidth_*currentImageHeight_*sizeof(unsigned char));

	lut_=NULL;
	lut_ = new unsigned char[64*64*64]; // Stores the look up table
	memset(lut_,0,64*64*64);

	GenLut_=NULL;
	GenLut_=new unsigned char[64*64*64];
	memset(GenLut_,0,64*64*64);

	creatMousePosLayout();

} 
void ImageDialog::creatMousePosLayout()
{
	//mouse pos
	pos=new QLabel(tr("MouPos:"));posX=new QLabel(tr("posX:"));posY=new QLabel(tr("posY:"));
	yuv=new QLabel(tr("YUV:"));Y=new QLabel(tr("Y:"));U=new QLabel(tr("U:"));V=new QLabel(tr("V:"));
	id=new QLabel(tr("ColorID:"));
	mouX=new QSpinBox;mouY=new QSpinBox;
	picY=new QSpinBox;picU=new QSpinBox;picV=new QSpinBox;
	colorid=new QSpinBox;
	pos->setParent(this);posX->setParent(this);posY->setParent(this);
	yuv->setParent(this);Y->setParent(this);U->setParent(this);V->setParent(this);
	id->setParent(this);
	mouX->setParent(this);mouY->setParent(this);
	picY->setParent(this);picU->setParent(this);picV->setParent(this);
	colorid->setParent(this);
	const double m=1.75;
	pos->setGeometry(0,FULL_IMAGE_HEIGHT*m+10,50,15);
	posX->setGeometry(50,FULL_IMAGE_HEIGHT*m+10,30,15);
	mouX->setGeometry(100,FULL_IMAGE_HEIGHT*m+10,100,15);
	posY->setGeometry(200,FULL_IMAGE_HEIGHT*m+10,30,15);
	mouY->setGeometry(250,FULL_IMAGE_HEIGHT*m+10,100,15);
	yuv->setGeometry(0,FULL_IMAGE_HEIGHT*m+50,50,15);
	Y->setGeometry(50,FULL_IMAGE_HEIGHT*m+50,5,15);
	picY->setGeometry(60,FULL_IMAGE_HEIGHT*m+50,85,15);
	U->setGeometry(150,FULL_IMAGE_HEIGHT*m+50,5,15);
	picU->setGeometry(160,FULL_IMAGE_HEIGHT*m+50,85,15);
	V->setGeometry(250,FULL_IMAGE_HEIGHT*m+50,5,15);
	picV->setGeometry(260,FULL_IMAGE_HEIGHT*m+50,85,15);
	id->setGeometry(0,FULL_IMAGE_HEIGHT*m+90,50,15);
	colorid->setGeometry(50,FULL_IMAGE_HEIGHT*m+90,100,15);
	mouX->setReadOnly(1);mouY->setReadOnly(1);
	picY->setReadOnly(1);picU->setReadOnly(1);picV->setReadOnly(1);
	colorid->setReadOnly(1);
	mouX->setMaximum(FULL_IMAGE_WIDTH);mouY->setMaximum(FULL_IMAGE_HEIGHT);
	picY->setMaximum(256);picU->setMaximum(256);picV->setMaximum(256);


}
//==============slot function====================//
void ImageDialog::run()
{
	processImage();
	DrawVisualObjects(unclassifiedImage);
	update();
	show();
	raise();
	activateWindow();

}
void ImageDialog::openPic()
{
	QString filename = QFileDialog::getOpenFileName(this,tr("open Pic"),lastFileName,tr("Images (*.raw)"));
	currentDir=QFileInfo(filename).absoluteDir();
	currentPath=QFileInfo(filename).absolutePath();
	currentList=currentDir.entryList(QStringList(tr("*.raw")),QDir::Files,QDir::Name);
	currentIndex=currentList.indexOf(QFileInfo(filename).fileName());
	if (!filename.isEmpty())
	{
		lastFileName = filename;
		//rawString= filename.toStdWString();
		rawString = filename;
		LoadRawStream();
		GenUnclassifyImage(tjImage,unclassifiedImage);
		(*processUnclassifiedImage)=(*unclassifiedImage);

		if(lut_ != NULL)
		{
			GenClassifyImage(lut_,tjImage,classifiedImage);
			processImage();
			DrawVisualObjects(processUnclassifiedImage);
		}
	}		
	update();
	show();
	raise();
	activateWindow();
}
void ImageDialog::openLut()
{
	(*processUnclassifiedImage)=(*unclassifiedImage);//如果不将unclassifiedImage赋给processUnclassifiedImage，而直接用unclassifiedImage代入DrawVisualObjects（）函数中，在换一张色表的时候原来unclassifiedImage已经是被画上了线点的图片了，再换一个色表就在这个不纯的图片上继续画图，所以会发现图片上物体叠加。
	lastLutName=lastFileName;
	QString filename = QFileDialog::getOpenFileName(this,tr("open Lut"),lastLutName, tr("Lut (*.dat)"));
	if (!filename.isEmpty())
	{
		lastLutName = filename;
		//lutString = filename.toStdString();
		lutString = filename;
		LoadLUT(lutString);
		if (tjImage.unCorrectedRaw!=NULL)
		{		
			GenClassifyImage(lut_,tjImage,classifiedImage);
			processImage();
			DrawVisualObjects(processUnclassifiedImage);
		}
	}		
	update();
	show();
	raise();
	activateWindow();
}
void ImageDialog::preImage()
{
	if (currentIndex-1>=0)
	{
		QString nextfile(currentPath);
		QString currentFile_onlyname=currentList.at(--currentIndex);
		nextfile.append(tr("/")).append(currentFile_onlyname);
		lastFileName=nextfile;
		//rawString= nextfile.toStdString();
		rawString = nextfile;
		LoadRawStream();
		GenUnclassifyImage(tjImage,unclassifiedImage);
		(*processUnclassifiedImage)=(*unclassifiedImage);
		if(lut_ != NULL)
		{
			GenClassifyImage(lut_,tjImage,classifiedImage);
			processImage();
			DrawVisualObjects(processUnclassifiedImage);
		}
		update();
		show();
		raise();
		activateWindow();
	}
	else
		QMessageBox::information(this,tr("Warning"),tr("TO THE START"));
} 
void ImageDialog::nextImage()
{
	if (currentIndex+1<currentList.size())
	{
		QString nextfile(currentPath);
		QString currentFile_onlyname=currentList.at(++currentIndex);
		nextfile.append(tr("/")).append(currentFile_onlyname);
		lastFileName=nextfile;
		//rawString= nextfile.toStdString();
		rawString = nextfile;
		LoadRawStream();
		GenUnclassifyImage(tjImage,unclassifiedImage);
		(*processUnclassifiedImage)=(*unclassifiedImage);
		if(lut_ != NULL)
		{
			GenClassifyImage(lut_,tjImage,classifiedImage);
			processImage();
			DrawVisualObjects(processUnclassifiedImage);
		}
		update();
		show();
		raise();
		activateWindow();
	}
	else
		QMessageBox::information(this,tr("Warning"),tr("TO THE END"));
}
void ImageDialog::genLut()
{
	///////nao来了要调试这些权值，以及标色表程序每种颜色的范围
		//lamda,Ibk,tau,Bi[10]为控制权值///////
		const double lambda=0.5,Ibk=1,tau=0.001;
		double Bi[10];
		Bi[c_UNKNOWN]=0;
		Bi[c_FIELD_GREEN]=0.8;
		Bi[c_WHITE]=0.8;
		Bi[c_BALL_ORANGE]=0.8;
		Bi[c_ROBOT_RED]=0.8;
		Bi[c_ROBOT_BLUE]=0.8;
		Bi[c_BEACON_GREEN]=0.8;
		Bi[c_BEACON_BLUE]=0.8;
		Bi[c_BEACON_PINK]=0.8;
		Bi[c_BEACON_YELLOW]=0.8;
		double I[10]={0},SumI,maxI;
		int maxColor;
		int offsetOld,offsetNew;
		memset(GenLut_,0,64*64*64);
	
		emit EnableSaveLut(false);
		for(int Ny=0;Ny<=255;Ny+=4){
			for(int Nu=0;Nu<=255;Nu+=4){
				for(int Nv=0;Nv<=255;Nv+=4){
					//计算搜索范围!!!/////////
					int CyLow=CLIPCOLOR(Ny-10);
					int CyHigh=CLIPCOLOR(Ny+10);
					int CuLow=CLIPCOLOR(Nu-10);
					int CuHigh=CLIPCOLOR(Nu+10);
					int CvLow=CLIPCOLOR(Nv-10);
					int CvHigh=CLIPCOLOR(Nv+10);
					/////清I[color]////////
					for(int i=0;i<10;i++)
						I[i]=0;
					offsetNew=(Ny >> 2 << 12)+(Nu >> 2 << 6)+(Nv >> 2);
					for(int Oy=CyLow;Oy<=CyHigh;Oy+=4){
						for(int Ou=CuLow;Ou<=CuHigh;Ou+=4){
							for(int Ov=CvLow;Ov<=CvHigh;Ov+=4){
								offsetOld=(Oy >> 2 << 12)+(Ou >> 2 << 6)+(Ov >> 2);
								int ColorOld=lut_[offsetOld];
								if((Oy==Ny&&Ou==Nu&&Ov==Nv)||(ColorOld==0))
									continue;
								int DisManhatun=abs(Ny-Oy)+abs(Nu-Ou)+abs(Nv-Ov);
								I[ColorOld]+=pow(lambda,DisManhatun);
							}
						}
					}
					for(int i=0;i<10;i++){
						I[i]*=Bi[i];
					}
					CalCulate(I,maxI,maxColor,SumI);
					if(maxI/(Ibk+SumI)>tau)
						GenLut_[offsetNew]=(unsigned char)maxColor;
				}
			}
		}	
	
		emit EnableSaveLut(true);
}
void ImageDialog::saveLut()
{
	QString filename = QFileDialog::getSaveFileName(this,tr("save Lut"),lastLutName, tr("Lut (*.dat)"));
	if (!filename.isEmpty())
	{
		saveLutString = filename;
		QFile f(saveLutString);
		f.open(QIODevice::WriteOnly);
		f.write((char*)GenLut_, 64*64*64);
		f.close();
	}
	update();
	show();
	raise();
	activateWindow();
}

void ImageDialog::cameraChange()
{
  simuNaoProvider->changeCam();
}

void ImageDialog::getNetPic()
{

	EnableSavePic(false);
	EnableSaveAsPic(false);
	simuNaoProvider->outSafeJointData(tjSafeJointData);
	// TODO:need to get sensordata from nao when connect to nao
	simuNaoProvider->outSafeSensorData(tjSafeSensorData);

		simuNaoProvider->getPic(tjImage);
	sendData2messageQueue(idSimSensorData,idImageDialog,idVisionThread,&tjSafeSensorData,sizeof(SafeSensorData));
	sendData2messageQueue(idSensoryJointData,idImageDialog,idVisionThread,&tjSafeJointData,sizeof(SafeJointData));	
	sendData2messageQueue(idUncorrectedRaw,idImageDialog,idVisionThread,tjImage.unCorrectedRaw,tjImage.imageSize);
	GenUnclassifyImage(tjImage,unclassifiedImage);
	(*processUnclassifiedImage)=(*unclassifiedImage);
	if(lut_ != NULL)
	{
		GenClassifyImage(lut_,tjImage,classifiedImage);
		processImage();
		DrawVisualObjects(processUnclassifiedImage);
	}
	update();
	show();
	raise();
    activateWindow();

	EnableSavePic(true);
	EnableSaveAsPic(true);


}

void ImageDialog::getStreamPic(bool check_)
{
	streamPicOn=check_;
    if (check_)
    {
	   timerid = startTimer(300);
	   //std::cout<<"we are in getandsaveStreamPic 11111111"<<std::endl;
    }
	else
	{
       killTimer(timerid);
	}

}

void ImageDialog::startOdePic(bool check)
{
	odePicOn = check;
	if(check)
	{
		Odetimerid = startTimer(150);
	}
	else
	{
		killTimer(Odetimerid);
	}
}

void ImageDialog::timerEvent(QTimerEvent *event)
{
	if (event->timerId()==timerid)
	{
		getNetPic();
		//savePic();
	}
 	if (event->timerId()==Odetimerid)
 	{
 		getODEPic();
 	}
	
	
}
// 
void ImageDialog::savePic()
{
	//Change NetPicName
	if(currentSavePath.isNull()) 
	{
		if(access("D:/Netpics", 0) == -1)
		{
	        mkdir("D:\\Netpics");
			mkdir("D:\\Netpics\\MyPics");
			 currentSavePath="D:/Netpics/MyPics";
		}
		else
		{  
		    mkdir("D:\\Netpics\\MyPics");
			currentSavePath="D:/Netpics/MyPics";
		}
		//return 0;

	}
	QString tempcurrentSavePath(currentSavePath);
	NetPicName=tempcurrentSavePath.append(QString("/RawH%1.raw").arg(QString::number(NetFileIndex), 3, QLatin1Char('0')));
	QFile streamfile(NetPicName);
	bool OpenOk=streamfile.open(QIODevice::WriteOnly);
	if(!OpenOk)
	{
		QMessageBox::information(this,tr("file"),tr("Can't open The File"));
		return;
	}
	streamfile.write((char*)tjImage.unCorrectedRaw, tjImage.imageSize);
// 	streamfile.write((char*)(&tjSafeJointData),sizeof(SafeJointData));
// 	streamfile.write((char*)(&tjSafeSensorData),sizeof(SafeSensorData));

	/*******************************************/
	for (int i=0;i<SafeJointData::numOfJoints;i++)
	{
		streamfile.write((char*)(&tjSafeJointData.angles[i]),sizeof(double));
	}
	streamfile.write((char*)(&tjSafeJointData.timeStamp),sizeof(unsigned));
	/********************************************/
	for (int i=0;i<SafeSensorData::numOfSensors;i++)
	{
		streamfile.write((char*)(&tjSafeSensorData.data[i]),sizeof(float));
	}
	for(int i = 0; i < SafeJointData::numOfJoints; ++i)
	{
		streamfile.write((char*)(&tjSafeSensorData.loads[i]),sizeof(float));
		streamfile.write((char*)(&tjSafeSensorData.temperatures[i]),sizeof(float));
	}
	streamfile.write((char*)(&tjSafeSensorData.timeStamp),sizeof(unsigned));
	/**********************************************/

	emit updateWindowTitle(tr("Image  ***NetPic***   ").append(NetPicName));
	//存动作值
	streamfile.close();
	NetFileIndex++;
	update();
	show();
	raise();
	activateWindow();
}

void ImageDialog::getODEPic()
{
	if(SignalControl::getInstance()->visionSignal == SignalControl::VODE)
	{
	#ifdef ODESimulationActuator
		if(behaviorThread && behaviorThread->isRunning())
			theSynchronizer->getEventbyName("Behavior->ImageView")->wait();
	#endif
		selfMessageQueue->SearchMyMessage(idSimImage,idVisionThread,idImageDialog,
			(char*)&tjImage, sizeof(TJImage));
		selfMessageQueue->SearchMyMessage(idSensoryJointData,idVisionThread,idImageDialog,
			(char*)(&tjSafeJointData),sizeof(SafeJointData));
		selfMessageQueue->SearchMyMessage(idSimSensorData,idVisionThread,idImageDialog,
			(char*)(&tjSafeSensorData),sizeof(SafeSensorData));
		GenUnclassifyImage(tjImage,unclassifiedImage);
		(*processUnclassifiedImage)=(*unclassifiedImage);
		PlotManager::getInstance()->getObjectToDraw();
		if(lut_ != NULL&&tjImage.unCorrectedRaw!=NULL)
		{
			GenClassifyImage(tjImage.lut_,tjImage,classifiedImage);
			DrawVisualObjects(processUnclassifiedImage);
		    receiveVisionResult();
		}
		update();
		show();
		raise();
		activateWindow();
	}
}

void ImageDialog::saveODEPic()
{	
	QString tempcurrentSavePath("D:/Netpics");
	ODEPicName=tempcurrentSavePath.append(QString("/RawH%1.raw").arg(QString::number(ODEFileIndex), 3, QLatin1Char('0')));
	QFile ODEfile(ODEPicName);
	bool OpenOk=ODEfile.open(QIODevice::WriteOnly);
	if(!OpenOk)
	{
		QMessageBox::information(this,tr("file"),tr("Can't open The File"));
		return;
	}
	else
	{
		ODEfile.write((char*)tjImage.unCorrectedRaw, IMAGE_WIDTH*IMAGE_HEIGHT*3);
// 		ODEfile.write((char*)(&tjSafeJointData),sizeof(SafeJointData));
// 		ODEfile.write((char*)(&tjSafeSensorData),sizeof(SafeSensorData));
		/*******************************************/
		for (int i=0;i<SafeJointData::numOfJoints;i++)
		{
			ODEfile.write((char*)(&tjSafeJointData.angles[i]),sizeof(double));
		}
		ODEfile.write((char*)(&tjSafeJointData.timeStamp),sizeof(unsigned));
		/********************************************/
		for (int i=0;i<SafeSensorData::numOfSensors;i++)
		{
			ODEfile.write((char*)(&tjSafeSensorData.data[i]),sizeof(float));
		}
		for(int i = 0; i < SafeJointData::numOfJoints; ++i)
		{
			ODEfile.write((char*)(&tjSafeSensorData.loads[i]),sizeof(float));
			ODEfile.write((char*)(&tjSafeSensorData.temperatures[i]),sizeof(float));
		}
		ODEfile.write((char*)(&tjSafeSensorData.timeStamp),sizeof(unsigned));
		/**********************************************/

	}
	emit updateWindowTitle(tr("Image  ***ODEPic***   ").append(ODEPicName));
	//存动作值
	ODEfile.close();
	ODEFileIndex++;

// 	update();
// 	show();
// 	raise();
// 	activateWindow();
}

void ImageDialog::saveAsPic()
{
	currentSavePath=QFileDialog::getExistingDirectory(this, tr("Select Save File Path"),lastFileName);
	NetFileIndex=0;
}
//============= end of slot function=================//

//=============algorithm function=================//
void ImageDialog::LoadLUT(const QString& fileName)
{
	QFile lutFile(fileName);
	if(!lutFile.open(QIODevice::ReadOnly))
	{
		QMessageBox::information(this,tr("lut"),tr("Unable to load lookup table!"));
		return;
	}
	long lSize = lutFile.size();
	lutFile.read((char*)lut_, lSize);
	lutFile.close();
	tjImage.setLUT(lut_);
	sendData2messageQueue(idLut,idImageDialog,idVisionThread,&(tjImage.TJlut),sizeof(tjImage.TJlut));
	emit LoadNewLut(lut_);
}
void ImageDialog::LoadRawStream()
{
	if (imageBuffer != NULL) {
		delete[] imageBuffer;
		imageBuffer=NULL;
	}
	QFile file(rawString);
	if(!file.open(QIODevice::ReadOnly))
	{
		m_CurrentFileIndex=0;
		QMessageBox::information(this,tr("file"),tr("File not found"));
		return;
	}
	emit updateWindowTitle(tr("Image   ***LocalPic***  ").append(rawString));

	int filesize = file.size();
	imageBuffer=new char[filesize];
	if (imageBuffer == NULL) {
		imageBuffer = NULL;
		QMessageBox::information(this,tr("file"),tr("Not enough memory to load file"));
		return;
	}
	file.read(imageBuffer, filesize);
	file.close();
	//memcpy(unclassifyed, imageBuffer, normalSize);
	//Set TJImage Buffer!
	memcpy(tjImage.unCorrectedRaw, imageBuffer,tjImage.imageSize);
// 	memcpy((char*)(&tjSafeJointData),imageBuffer+tjImage.imageSize,sizeof(SafeJointData));
// 	memcpy((char*)(&tjSafeSensorData),imageBuffer+tjImage.imageSize+sizeof(SafeJointData),sizeof(SafeSensorData));
	int i;
	/*************************************************/
	for (i=0;i<SafeJointData::numOfJoints;i++)
	{
		memcpy(&(tjSafeJointData.angles[i]),imageBuffer+tjImage.imageSize+i*sizeof(double),sizeof(double));
	}
	int size=tjImage.imageSize+SafeJointData::numOfJoints*sizeof(double);
	memcpy(&tjSafeJointData.timeStamp,imageBuffer+size,sizeof(unsigned));
	size=size+sizeof(unsigned);
	/***************************************************/
	for (i=0;i<SafeSensorData::numOfSensors;i++)
	{
		memcpy(&(tjSafeSensorData.data[i]),imageBuffer+size+i*sizeof(float),sizeof(float));
	}
	size=size+SafeSensorData::numOfSensors*sizeof(float);
	for(i = 0; i < 2*SafeJointData::numOfJoints; ++i)
	{
		memcpy(&(tjSafeSensorData.loads[i]),imageBuffer+size+i*sizeof(float),sizeof(float));
		i=i+1;
		memcpy(&(tjSafeSensorData.temperatures[i]),imageBuffer+size+i*sizeof(float),sizeof(float));
	}
	size=size+2*SafeJointData::numOfJoints*sizeof(float);
	memcpy(&(tjSafeSensorData.timeStamp),imageBuffer+size,sizeof(unsigned));
	sendData2messageQueue(idUncorrectedRaw,idImageDialog,idVisionThread,tjImage.unCorrectedRaw,tjImage.imageSize);
	sendData2messageQueue(idSensoryJointData,idImageDialog,idVisionThread,&tjSafeJointData,sizeof(SafeJointData));
	sendData2messageQueue(idSimSensorData,idImageDialog,idVisionThread,&tjSafeSensorData,sizeof(SafeSensorData));

}
void ImageDialog::GenClassifyImage(unsigned char *lut,TJImage imagetj,QImage* image)
{
	unsigned char* lutFile;
	lutFile = lut;
	unsigned char y = imagetj.unCorrectedRaw[0][0][0];
	unsigned char u = imagetj.unCorrectedRaw[0][0][1];
	unsigned char v = imagetj.unCorrectedRaw[0][0][2];

	int h = 0;
	int w = 0;
	while (h < FULL_IMAGE_HEIGHT) {
		w = 0;
		while (w < FULL_IMAGE_WIDTH) {
			int offset = ( y >> 2 << 12)+( u>> 2 << 6)+( v>> 2);
			int temp=*(lutFile+offset);
			image->setPixel(w,h,LookupRgb(temp));

			w++;
			y=imagetj.unCorrectedRaw[h][w][0];
			u=imagetj.unCorrectedRaw[h][w][1];
			v=imagetj.unCorrectedRaw[h][w][2];
		}
		h++;
	}

}

void ImageDialog::ShowNetClassified(char * netClassified,QImage* image)
{
	OBJECT_TO_DRAW_CLEAR();
	int h=0;
	int w=0;
	while (h<FULL_IMAGE_HEIGHT)
	{
		w=0;
		while (w<FULL_IMAGE_WIDTH)
		{
			int temp = netClassified[h*FULL_IMAGE_WIDTH+w];
			image->setPixel(w,h,LookupRgb(temp));
			w++;
		}
		h++;
	}

	//netVisionInterface netResult;

	memcpy(visionResult,netClassified+IMAGE_SIZE,sizeof(netVisionInterface));
	//std::cout<<"in ImageDialog, is the ball in the image ? "<<(int)netResult.ballSeen<<std::endl;
	//std::cout<<"in ImageDialog, the rightGoalB  is  "<<(float)netResult.rightGoalB<<std::endl;
    emit ReceiveResultOk((char*)visionResult);
	std::cout<<"in ImageDialog,  netVisionInterface is "<<sizeof(netVisionInterface)<<std::endl;//is 56

		CameraMatrix camMax;
		RotationMatrix rot(-double(visionResult->row)/1000000000,-double(visionResult->pitch)/1000000000,-double(visionResult->yaw)/1000000000);
		camMax.translation.x = double(visionResult->camX)/100000;
		camMax.translation.y = double(visionResult->camY)/100000;
		camMax.translation.z = double(visionResult->camZ)/100000;
		camMax.rotation = rot;
		DRAW_LINE(visionResult->horizonStartX,visionResult->horizonStartY,
			visionResult->horizonEndX,visionResult->horizonEndY,
			3,Drawings::ps_SolidLine, Drawings::darkBlue);
		const Vector3<double> fieldpoint1=Vector3<double>(4185.0,2000.0,0.0);//(4200.0,2000.0,0.0);//The 1th
		Vector2<int> fieldpoint1_pointInImage(0,0);
		Geometry::calculatePointInImage(fieldpoint1,camMax,fieldpoint1_pointInImage);
		DRAW_BIG_DOT(fieldpoint1_pointInImage.x,fieldpoint1_pointInImage.y,Drawings::black,Drawings::red);

		const Vector3<double> fieldpoint2=Vector3<double>(4185.0,1115.0,0.0);//(4200.0,1100.0,0.0);//The 2th
		Vector2<int> fieldpoint2_pointInImage(0,0);
		Geometry::calculatePointInImage(fieldpoint2,camMax,fieldpoint2_pointInImage);
		DRAW_BIG_DOT(fieldpoint2_pointInImage.x,fieldpoint2_pointInImage.y,Drawings::black,Drawings::red);

		const Vector3<double> fieldpoint3=Vector3<double>(4185.0,-1065.0,0.0);//(4200.0,-1100.0,0.0);//The 3th
		Vector2<int> fieldpoint3_pointInImage(0,0);
		Geometry::calculatePointInImage(fieldpoint3,camMax,fieldpoint3_pointInImage);
		DRAW_BIG_DOT(fieldpoint3_pointInImage.x,fieldpoint3_pointInImage.y,Drawings::black,Drawings::red);

		const Vector3<double> fieldpoint4=Vector3<double>(4185.0,-1975.0,0.0);//(4200.0,-2000.0,0.0);//The 4th
		Vector2<int> fieldpoint4_pointInImage(0,0);
		Geometry::calculatePointInImage(fieldpoint4,camMax,fieldpoint4_pointInImage);
		DRAW_BIG_DOT(fieldpoint4_pointInImage.x,fieldpoint4_pointInImage.y,Drawings::black,Drawings::red);

		const Vector3<double> fieldpoint5=Vector3<double>(3585.0,1115.0,0.0);//(3600.0,1100.0,0.0);//The 5th
		Vector2<int> fieldpoint5_pointInImage(0,0);
		Geometry::calculatePointInImage(fieldpoint5,camMax,fieldpoint5_pointInImage);
		DRAW_BIG_DOT(fieldpoint5_pointInImage.x,fieldpoint5_pointInImage.y,Drawings::black,Drawings::red);

		const Vector3<double> fieldpoint6=Vector3<double>(3585.0,-1065.0,0.0);//(3600.0,-1100.0,0.0);//The 6th
		Vector2<int> fieldpoint6_pointInImage(0,0);
		Geometry::calculatePointInImage(fieldpoint6,camMax,fieldpoint6_pointInImage);
		DRAW_BIG_DOT(fieldpoint6_pointInImage.x,fieldpoint6_pointInImage.y,Drawings::black,Drawings::red);

		const Vector3<double> fieldpoint7=Vector3<double>(2400.0,0.0,0.0);//The 7th
		Vector2<int> fieldpoint7_pointInImage(0,0);
		Geometry::calculatePointInImage(fieldpoint7,camMax,fieldpoint7_pointInImage);
		DRAW_BIG_DOT(fieldpoint7_pointInImage.x,fieldpoint7_pointInImage.y,Drawings::black,Drawings::red);

		const Vector3<double> fieldpoint8=Vector3<double>(1175.0,0.0,0.0);//(1200.0,0.0,0.0);//The 8th
		Vector2<int> fieldpoint8_pointInImage(0,0);
		Geometry::calculatePointInImage(fieldpoint8,camMax,fieldpoint8_pointInImage);
		DRAW_BIG_DOT(fieldpoint8_pointInImage.x,fieldpoint8_pointInImage.y,Drawings::black,Drawings::red);

		const Vector3<double> fieldpoint9=Vector3<double>(0.0,2000.0,0.0); //9th
		Vector2<int> fieldpoint9_pointInImage(0,0);
		Geometry::calculatePointInImage(fieldpoint9,camMax,fieldpoint9_pointInImage);
		DRAW_BIG_DOT(fieldpoint9_pointInImage.x,fieldpoint9_pointInImage.y,Drawings::black,Drawings::red);

		const Vector3<double> fieldpoint10=Vector3<double>(0.0,-1975.0,0.0); //10th
		Vector2<int> fieldpoint10_pointInImage(0,0);
		Geometry::calculatePointInImage(fieldpoint10,camMax,fieldpoint10_pointInImage);
		DRAW_BIG_DOT(fieldpoint10_pointInImage.x,fieldpoint10_pointInImage.y,Drawings::black,Drawings::red);


		DRAW_LINE(fieldpoint1_pointInImage.x,fieldpoint1_pointInImage.y,fieldpoint2_pointInImage.x,fieldpoint2_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::black);
		DRAW_LINE(fieldpoint2_pointInImage.x,fieldpoint2_pointInImage.y,fieldpoint3_pointInImage.x,fieldpoint3_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::black);
		DRAW_LINE(fieldpoint3_pointInImage.x,fieldpoint3_pointInImage.y,fieldpoint4_pointInImage.x,fieldpoint4_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::black);
		DRAW_LINE(fieldpoint2_pointInImage.x,fieldpoint2_pointInImage.y,fieldpoint5_pointInImage.x,fieldpoint5_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::black);
		DRAW_LINE(fieldpoint3_pointInImage.x,fieldpoint3_pointInImage.y,fieldpoint6_pointInImage.x,fieldpoint6_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::black);
		DRAW_LINE(fieldpoint5_pointInImage.x,fieldpoint5_pointInImage.y,fieldpoint6_pointInImage.x,fieldpoint6_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::black);
		DRAW_LINE(fieldpoint7_pointInImage.x,fieldpoint7_pointInImage.y,fieldpoint8_pointInImage.x,fieldpoint8_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::black);
		DRAW_LINE(fieldpoint1_pointInImage.x,fieldpoint1_pointInImage.y,fieldpoint9_pointInImage.x,fieldpoint9_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::black);
		DRAW_LINE(fieldpoint4_pointInImage.x,fieldpoint4_pointInImage.y,fieldpoint10_pointInImage.x,fieldpoint10_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::black);
		DRAW_LINE(fieldpoint9_pointInImage.x,fieldpoint9_pointInImage.y,fieldpoint10_pointInImage.x,fieldpoint10_pointInImage.y,1,Drawings::ps_SolidLine,Drawings::black); 


	
	DrawVisualObjects(image);
	update();
	show();
	raise();
	activateWindow();
}

void ImageDialog::GenUnclassifyImage(TJImage imagetj,QImage* image)
{
	QRgb value;
	unsigned char Y,Cb,Cr;
	int r,g,b;
	int index=0;
	for (int j=0;j<FULL_IMAGE_HEIGHT;j++){
		for (int i=0;i<FULL_IMAGE_WIDTH;i++){

				Y=imagetj.unCorrectedRaw[j][i][0];
				Cb=imagetj.unCorrectedRaw[j][i][1];
				Cr=imagetj.unCorrectedRaw[j][i][2];
				ImageDialog::ConvertYCbCrToRGB(Y,Cb,Cr,r,g,b);
				value = qRgb(r, g, b);
				image->setPixel(i,j,value);
		}
	}
}
const static bool BDrawScanLines = true;
void ImageDialog::DrawVisualObjects(QImage *image)
{
	QPainter painter(image);
	painter.setRenderHints(QPainter::HighQualityAntialiasing);
	/***********************draw horizonline*****************************/
	Vector2<int> bottomleft(0, 0), topright(IMAGE_WIDTH-1, IMAGE_HEIGHT-1);
	Vector2<int> point1,point2;
	// 	if(SComm->TJScanVision->HorizonExist)
	// 	{
	// 		if(	Geometry::getIntersectionPointsOfLineAndRectangle(bottomleft,topright,
	// 			SComm->TJScanVision->HorizonLine,point1,point2))
	// 		{		QPen temppen;
	// 				getDebugPen(temppen,1,Drawings::ps_SolidLine,Drawings::red);
	// 				painter.setPen(temppen);
	// 				painter.drawLine(point1.x,point1.y,point2.x,point2.y);
	// 		}
	// 	}


	if(BDrawScanLines){
		//////////////===========New Draw===========////////////
		DebugLine tempLine;
		//Draw Lines
		std::vector< DebugLine >::const_iterator DebugLineIter = PlotManager::getInstance()->DrawdataBuffer.DebugLinesBuffer.begin();
		while (DebugLineIter != PlotManager::getInstance()->DrawdataBuffer.DebugLinesBuffer.end())	
		{
			QPen temppen;
			tempLine = *DebugLineIter;
			getDebugPen(temppen,tempLine.penWidth,tempLine.penStyle,tempLine.pencolor);
			painter.setPen(temppen);
			painter.drawLine(tempLine.startp.x,tempLine.startp.y,tempLine.endp.x,tempLine.endp.y);
			DebugLineIter++;
		}
		//Draw Points
		DebugPoint tempPoint;
		std::vector< DebugPoint >::const_iterator DebugPointIter = PlotManager::getInstance()->DrawdataBuffer.DebugFeaturePoint.begin();
		while (DebugPointIter != PlotManager::getInstance()->DrawdataBuffer.DebugFeaturePoint.end())	
		{
			QPen temppen;
			int pradius = 1;
			tempPoint = *DebugPointIter;
			QBrush tempb(Qt::SolidPattern);
			tempb.setColor(QColor(Qt::GlobalColor (tempPoint.fillColor)));
			getDebugPen(temppen,1,Drawings::ps_SolidLine,tempPoint.pencolor);
			painter.setPen(temppen);
			painter.setBrush(tempb);
			painter.drawEllipse(QPoint(tempPoint.point.x,tempPoint.point.y),pradius,pradius);
			DebugPointIter++;
		}
		//Draw Big Dots
		std::vector< DebugPoint >::const_iterator DebugBigDotIter = PlotManager::getInstance()->DrawdataBuffer.DebugBigDotBuff.begin();
		while (DebugBigDotIter != PlotManager::getInstance()->DrawdataBuffer.DebugBigDotBuff.end())	
		{
			QPen temppen;
			int pradius = 2;
			tempPoint = *DebugBigDotIter;
			QBrush tempb(Qt::SolidPattern);
			tempb.setColor(QColor(Qt::GlobalColor(tempPoint.fillColor)));
			getDebugPen(temppen,1,Drawings::ps_SolidLine,tempPoint.pencolor);
			painter.setPen(temppen);
			painter.setBrush(tempb);
			painter.drawEllipse(QPoint(tempPoint.point.x,tempPoint.point.y),pradius,pradius);
			DebugBigDotIter++;
		}
		//Draw Rectangles
		// 		DebugRectangle tempHARect;
		// 		Vector2<int>point3,point4,point5,point6;
		// 		std::vector< DebugRectangle >::const_iterator DebugHARectIter = object_to_draw->DebugHARectangleBuffer.begin();
		// 		while (DebugHARectIter != object_to_draw->DebugHARectangleBuffer.end())
		// 		{
		// 			QPen temppen;
		// 			tempHARect = *DebugHARectIter;
		// 			point3=SComm->imagetj.CoordinateHA2Image(Vector2<double>(static_cast<double>(tempHARect.topLeftpoint.x),static_cast<double>(tempHARect.topLeftpoint.y)),
		// 				SComm->TJScanVision->HorizonLine);
		// 			point4=SComm->imagetj.CoordinateHA2Image(Vector2<double>(static_cast<double>(tempHARect.bottomRightpoint.x),static_cast<double>(tempHARect.topLeftpoint.y)),
		// 				SComm->TJScanVision->HorizonLine);
		// 			point5=SComm->imagetj.CoordinateHA2Image(Vector2<double>(static_cast<double>(tempHARect.bottomRightpoint.x),static_cast<double>(tempHARect.bottomRightpoint.y)),
		// 				SComm->TJScanVision->HorizonLine);
		// 			point6=SComm->imagetj.CoordinateHA2Image(Vector2<double>(static_cast<double>(tempHARect.topLeftpoint.x),static_cast<double>(tempHARect.bottomRightpoint.y)),
		// 				SComm->TJScanVision->HorizonLine);
		// 
		// 			getDebugPen(temppen,tempHARect.penWidth,tempHARect.penStyle,tempHARect.pencolor);
		// 			painter.setPen(temppen);
		// 			painter.drawLine(QPoint(point3.x,point3.y)QPoint(point4.x,point4.y));
		// 			painter.drawLine(QPoint(point4.x,point4.y),QPoint(point5.x,point5.y));
		// 			painter.drawLine(QPoint(point5.x,point5.y),QPoint(point6.x,point6.y));
		// 			painter.drawLine(QPoint(point6.x,point6.y),QPoint(point3.x,point3.y));
		// 			DebugHARectIter++;
		// 		}
		//Draw Cirecles
		DebugCircle tempCircle;
		std::vector< DebugCircle >::const_iterator DebugCircelIter = PlotManager::getInstance()->DrawdataBuffer.DebugCircleBuffer.begin();
		while(DebugCircelIter != PlotManager::getInstance()->DrawdataBuffer.DebugCircleBuffer.end())
		{
			QPen temppen;
			tempCircle = *DebugCircelIter;
			getDebugPen(temppen,tempCircle.penWidth,tempCircle.penStyle,tempCircle.pencolor);
			painter.setPen(temppen);
			painter.setBrush(Qt::NoBrush);
			painter.drawEllipse(QPoint(tempCircle.Center.x,tempCircle.Center.y),tempCircle.radius,tempCircle.radius);
			DebugCircelIter++;
		}
	}//if(BDrawScanLines)
}
//==========end of algorithm function=================//

void ImageDialog::processImage()
{
	if(SignalControl::getInstance()->visionSignal == SignalControl::VOFFLINE)
		theSynchronizer->getEventbyName("VOffline->Behavior")->signal();

	if(SignalControl::getInstance()->visionSignal == SignalControl::VOFFLINE)
	{
		if(behaviorThread && behaviorThread->isRunning())
		{SmartPtr<Event> temp=theSynchronizer->getEventbyName("Behavior->VOffline");
		temp->wait();}
			//theSynchronizer->getEventbyName("Behavior->VOffline")->wait();
		int a=0;
		PlotManager::getInstance()->getObjectToDraw();
	}
   receiveVisionResult();
}
void ImageDialog::receiveVisionResult()
{
	if (selfMessageQueue->SearchMyMessage(idGoalPercept,idRobotThread,idNetworkThread,
		(char*)goalResult,sizeof(netGoalpost)))
	{
		//std::cout<<"in TcpConnect, inTJImage sizeof netGoalpost is "<<sizeof(netGoalpost)<<std::endl;
		visionResult->HorizenInImage=goalResult->HorizenInImage;
		visionResult->horizonEndX=goalResult->horizonEndX;
		visionResult->horizonEndY=goalResult->horizonEndY;
		visionResult->horizonStartX=goalResult->horizonStartX;
		visionResult->horizonStartY=goalResult->horizonStartY;
		visionResult->oppleftgoalSeen=goalResult->oppleftgoalSeen;
		visionResult->oppleftGoalX=goalResult->oppleftGoalX;
		visionResult->oppleftGoalY=goalResult->oppleftGoalY;
		visionResult->opprightgoalSeen=goalResult->opprightgoalSeen;
		visionResult->opprightGoalX=goalResult->opprightGoalX;
		visionResult->opprightGoalY=goalResult->opprightGoalY;
		visionResult->ownleftgoalSeen=goalResult->ownleftgoalSeen;
		visionResult->ownleftGoalX=goalResult->ownleftGoalX;
		visionResult->ownleftGoalY=goalResult->ownleftGoalY;
		visionResult->ownrightgoalSeen=goalResult->ownrightgoalSeen;
		visionResult->ownrightGoalX=goalResult->ownrightGoalX;
		visionResult->ownrightGoalY=goalResult->ownrightGoalY;
		visionResult->unknowoppgoalSeen=goalResult->unknowoppgoalSeen;
		visionResult->unknowoppGoalX=goalResult->unknowoppGoalX;
		visionResult->unknowoppGoalY=goalResult->unknowoppGoalY;
		visionResult->unknowowngoalSeen=goalResult->unknowowngoalSeen;
		visionResult->unknowownGoalX=goalResult->unknowownGoalX;
		visionResult->unknowownGoalY=goalResult->unknowownGoalY;
	}

	if (selfMessageQueue->SearchMyMessage(idBallresult,idRobotThread,idNetworkThread,
		(char*)ballResult,sizeof(netBall)))
	{
		//std::cout<<"in TcpConnect, inTJImage sizeof netBall is "<<sizeof(netBall)<<std::endl;
	    visionResult->ballSeen=ballResult->ballSeen;
		visionResult->balldis=ballResult->balldis;
		visionResult->ballx=ballResult->ballx;
		visionResult->bally=ballResult->bally;
	}
	if(selfMessageQueue->SearchMyMessage(idCircle,idRobotThread,idNetworkThread,
		(char*)CircleResult,sizeof(netBall)))
	{  
       visionResult->CircleSeen=CircleResult->CircleSeen;
	   visionResult->CircleCenterX=CircleResult->CircleCenterX;
	   visionResult->CircleCenterY=CircleResult->CircleCenterY;
	}
		
	emit ReceiveResultOk((char*)visionResult);
}
void ImageDialog::sendData2messageQueue(CMsgID dataID,CMsgThID sendthredID,CMsgThID receiverID,void* src,size_t size)
{
	selfMessageQueue->SetMessageData(dataID,sendthredID,receiverID, size,(char*)src);
}

//=====================event function============================//
void ImageDialog::paintEvent ( QPaintEvent * event ) 
{
	QPainter painter(this);
	painter.drawImage(QRect(0,0,FULL_IMAGE_WIDTH*1.75,FULL_IMAGE_HEIGHT*1.75),*processUnclassifiedImage,QRect(0,0,FULL_IMAGE_WIDTH,FULL_IMAGE_HEIGHT));
	painter.drawImage(QRect(FULL_IMAGE_WIDTH*1.75+10,0,FULL_IMAGE_WIDTH*1.75,FULL_IMAGE_HEIGHT*1.75),*classifiedImage,QRect(0,0,FULL_IMAGE_WIDTH,FULL_IMAGE_HEIGHT));
	//painter.drawImage(QRect(FULL_IMAGE_WIDTH*1.75+10,0,FULL_IMAGE_WIDTH*1.75,FULL_IMAGE_HEIGHT*1.75),*netclassifiedImage,QRect(0,0,FULL_IMAGE_WIDTH,FULL_IMAGE_HEIGHT));
}
void ImageDialog::mouseMoveEvent ( QMouseEvent * event )
{
	QPointF point=event->pos();
	double x=point.x();
	double y=point.y();
	int xx,yy;
	unsigned char* lutFile;
	lutFile = lut_;
	const double m=1.75;
 	if (x/m>=int(x)/m+0.5)
 	{xx=x/m+1;}
 	else{xx=x/m;}
 	if (y/m>=int(y)/m+0.5)
 	{yy=y/m+1;}
	else{yy=y/m;}

	if (xx<FULL_IMAGE_WIDTH&&xx>=0&&yy<FULL_IMAGE_HEIGHT&&yy>=0)
	{
		mouX->setValue(xx);
		mouY->setValue(yy);
		if (tjImage.unCorrectedRaw!=NULL)
		{
// 			unsigned char Y=unclassifyed[960*yy+3*xx]; // y*3*320+x*3
// 			unsigned char U=unclassifyed[960*yy+3*xx+1];
// 			unsigned char V=unclassifyed[960*yy+3*xx+2];
			unsigned char Y=tjImage.unCorrectedRaw[yy][xx][0];
			unsigned char U=tjImage.unCorrectedRaw[yy][xx][1];
			unsigned char V=tjImage.unCorrectedRaw[yy][xx][2];

			picY->setValue(Y);
			picU->setValue(U);
			picV->setValue(V);
			if(lut_!=NULL)
			{
				int offset = (Y>> 2 << 12)+(U>> 2 << 6)+(V>> 2);
				int temp=*(lutFile+offset);
				colorid->setValue(temp);
			}


		}

	}
	else{
		mouX->setValue(0);
		mouY->setValue(0);
		picY->setValue(0);
		picU->setValue(0);
		picV->setValue(0);
		colorid->setValue(0);

	}


}
void ImageDialog::activateWindow()
{
	emit activeThisWindow();
}

//=================end of event function============================//

//==================math tool function=============================//
void ImageDialog::CalCulate(double I[10],double &maxI,int &maxColor,double &SumI)
{
	maxColor=0;
	maxI=0;
	SumI=0;
	for(int i=0;i<10;i++){
		if(I[i]>maxI){
			maxI=I[i];
			maxColor=i;
		}
		SumI+=I[i];
	}
}

QRgb ImageDialog::LookupRgb(int index)
{
	switch (index)
	{
	case c_UNKNOWN:
		return qRgb(0,0,0);break;
	case c_FIELD_GREEN:
		return qRgb(0,255,0);break;
	case c_WHITE:
		return qRgb(255,255,255);break;
	case c_BALL_ORANGE:
		return qRgb(220,150,0);break;
	case c_ROBOT_RED:
		return qRgb(255,0,0);break;
	case c_ROBOT_BLUE:
		return qRgb(50,0,160);break;
	case c_BEACON_GREEN:
		return qRgb(128,128,128);break;
	case c_BEACON_BLUE:
		return qRgb(57,134,210);break;
	case c_BEACON_PINK:
		return qRgb(220,60,220);break;
	case c_BEACON_YELLOW:
		return qRgb(245,235,50);break;
	default :
		break;
	}
	return qRgb(0,0,0);
}
void ImageDialog::ConvertYCbCrToRGB(unsigned char Y, unsigned char Cb, unsigned char Cr,int &finalR,int &finalG,int &finalB) 
{
	///////////////GT,it is right///////////////////////////
	int r = Y + ((1436 * (Cr - 128))>> 10),
		g = Y - ((354 * (Cb - 128) + 732 * (Cr - 128))>> 10),
		b = Y + ((1814 * (Cb - 128))>> 10);
	if(r < 0) r = 0; else if(r > 255) r = 255;
	if(g < 0) g = 0; else if(g > 255) g = 255;
	if(b < 0) b = 0; else if(b > 255) b = 255;
	finalR=r;
	finalG=g;
	finalB=b;
//////////////////////////////////
	///////////sony,it is not right/////////////
// 	int r,g,b;
// 	//RGBColour ResultRGB;
// 	double R = 255.0 * (Y + Cb);
// 	double G = 255.0 * (Y - 0.51*Cb - 0.19*Cr);
// 	double B = 255.0 * (Y + Cr);
// 	r = ( int)R;
// 	if (R > 255.0) {
// 		r = 255;
// 	} else if (R < 0.0) {
// 		r = 0;
// 	}
// 	g = (int)G;
// 	if (G > 255.0) {
// 		g = 255;
// 	} else if (G < 0.0) {
// 		g = 0;
// 	}
// 	b = (int)B;
// 	if (B > 255.0) {
// 		b = 255;
// 	} else if (B < 0.0) {
// 		b = 0;
// 	}
// 	finalR=r;
// 	finalG=g;
// 	finalB=b;
}
void ImageDialog::getDebugPen(QPen& temppen,int penWidth, char penStyle, char penColor)
{
	temppen.setColor(QColor(Qt::GlobalColor(penColor)));
	temppen.setWidth(penWidth);
	temppen.setStyle(Qt::PenStyle(penStyle));
}

//==================end of math tool function=============================//




SafeJointData ImageDialog::getSensoryJointData()
{
	simuNaoProvider->outSafeJointData(tjSafeJointData);
	return tjSafeJointData;

}
void ImageDialog::connectNaoqi(std::string ipStr)
{
	if(simuNaoProvider->connectNao(ipStr)==false)
		emit IsConnectNaoqi(false);
	else 
		emit IsConnectNaoqi(true);
	
}
void ImageDialog::closeNaoqi()
{
	simuNaoProvider->closeConnect();
}
void ImageDialog::ReceiveJointData()
{
 simuNaoProvider->sendSimJoints(selfSimulationData->theMotionJointData,selfSimulationData->portID,2000);
}