#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include <QtGui>
#include "Module/Common/VisionHeader.h"
#include "Module/Representations/Perception/VisionInterface.h"
#include "Module/Representations/Perception/BallPercept.h"
#include "Module/Representations/Perception/GoalPercetp.h"
#include "Module/Representations/Perception/LinesPercept.h"
#include "Module/Vision/SensorComm.h"
#include "Module/Representations/BaseProvider.h"
#include "Simulation/NaoProvider/VisionSimulation.h"
#include "Simulation/NaoProvider/SimuNaoProvider.h"
#include "Module/Common/JointData.h"
#include "Module/Core/Thread.h"
#include "Simulation/SignalControl.h"
#include "Simulation/VisionGUI/PlotManager.h"
#include "direct.h"
#include "io.h"




class TJSimRobot;
#define CLIPCOLOR(x) ((x>255)?255:((x<0)?0:x))
const int netVisionInter_size = sizeof(netVisionInterface);
class ImageDialog: public QWidget, BaseProvider
{
	Q_OBJECT

public:
	ImageDialog(TJSimRobot* tjRobot,SmartPtr<Synchronizer> _synchro, QWidget *parent = 0);
	~ImageDialog();

// 		void ProcessNetImage(unsigned char  *buffer);
public:
		SafeJointData getSensoryJointData();
		void ReceiveJointData();
		QImage *getclassifiedImage(){return classifiedImage;}
		void ShowNetClassified(char * netClassified,QImage* image);//add by hu
        //char *visionResult;
		netVisionInterface *visionResult;
		netGoalpost * goalResult;
		netBall * ballResult;
		netCircle * CircleResult;
protected:
	char* imageBuffer;
	char* tempimageBuffer;
	//unsigned char* unclassifyed;
	//int normalSize;//CPlane Size;
	int currentImageWidth_;
	int currentImageHeight_;
	unsigned char* lut_;
	unsigned char* GenLut_;
	TJImage tjImage;
	SafeJointData tjSafeJointData;
	SafeSensorData tjSafeSensorData;
	void paintEvent ( QPaintEvent * event );
	void mouseMoveEvent ( QMouseEvent * event );
signals:
	void EnableSaveLut(bool save);
	void EnableSavePic(bool save);
	void EnableSaveAsPic(bool save);
	void activeThisWindow();
	void IsConnectNaoqi(bool isConnect);
	void updateWindowTitle(const QString & str);
	void ReceiveResultOk(char *buffer);//add by hu
	void LoadNewLut(unsigned char *lut_);
public slots:
		void connectNaoqi(std::string ipStr);
		void closeNaoqi();
private slots:

	void openPic();
	void openLut();
	void run();
	void getNetPic();
	void getStreamPic(bool check_);//add by hu
	void startOdePic(bool check);
	void cameraChange();
	void preImage();
	void nextImage();
	void genLut();
	void saveLut();
	void savePic();
	void getODEPic();
	void saveODEPic();
	void saveAsPic();
private:
	unsigned char image[IMAGE_HEIGHT][IMAGE_WIDTH][3];
	QImage* unclassifiedImage;//unclassifiedImage被Gernerate出来后就不变了，只是图片换了才变
	QImage* classifiedImage;
	QImage* processUnclassifiedImage;//processUnclassifiedImage才是真正被画出来的图片，它在每次执行scanvision前被赋为unclassifiedImage，再执行DrawVisualObjects（）后变成有图片有画线的结合体
	QString lastFileName;
    QString lastLutName;
	QString rawString;
	QString lutString;
	QString saveLutString;
	QString NetPicName;
	QString ODEPicName;
	QTimer *timer;
	int NetFileIndex;
	int ODEFileIndex;
	int m_CurrentFileIndex;

	///=====//
	QDir currentDir;
	QString currentPath;
	QStringList currentList;
	int currentIndex;
	QString currentSavePath;
	void LoadRawStream();
	void LoadLUT(const QString& fileName);
	void initImageParament();
	void creatMousePosLayout();
	SimulationData *selfSimulationData;

	SmartPtr<Synchronizer> theSynchronizer;
	TJSimRobot* behaviorThread;

	void GenUnclassifyImage(TJImage imagetj,QImage* image);
	void ConvertYCbCrToRGB(unsigned char Y, unsigned char Cb, unsigned char Cr,int &finalR,int &finalG,int &finalB) ;
	void GenClassifyImage(unsigned char *lut,TJImage imagetj,QImage* image);
	QRgb LookupRgb(int index);
	void DrawVisualObjects(QImage *image);
	void getDebugPen(QPen& temppen,int penWidth, char penStyle, char penColor);
	void processImage();
	void receiveVisionResult();//add by hu
	void CalCulate(double I[10],double &maxI,int &maxColor,double &SumI);
	void activateWindow();
	void sendData2messageQueue(CMsgID dataID,CMsgThID sendthredID,CMsgThID receiverID,void* src,size_t size);

	//mouseposition
	QLabel *pos;QLabel *posX;QLabel *posY;
	QLabel *yuv;QLabel *Y;QLabel *U;QLabel *V;
	QLabel *id;
	QSpinBox *mouX;QSpinBox *mouY;
	QSpinBox *picY;QSpinBox *picU;QSpinBox *picV;
	QSpinBox *colorid;
	SimuNaoProvider *simuNaoProvider;
    
	void timerEvent(QTimerEvent *event);
	int timerid;
	int Odetimerid;
	bool streamPicOn;
	bool odePicOn;
public:
	bool getStreamPicOn() const{return streamPicOn;}
	bool getOdePicOn() const {return odePicOn;}
};

#endif // IMAGEDIALOG_H
