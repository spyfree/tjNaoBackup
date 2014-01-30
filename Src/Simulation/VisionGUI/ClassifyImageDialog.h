#ifndef CLASSIFYIMAGEDIALOG_H
#define CLASSIFYIMAGEDIALOG_H
#include "Module/Common/VisionHeader.h"
#include <QtGui>
class ClassifyImageDialog: public QWidget
{
	Q_OBJECT
public:
		ClassifyImageDialog(QWidget *parent = 0);
		~ClassifyImageDialog();
private slots:
		void openPic();
		void openLut();
		void preImage();
		void nextImage();
		void saveAsLut();
		void saveLut();
		void pushUndo();
		void updateYSensitivity(const QString & str);
		void updateUSensitivity(const QString & str);
		void updateVSensitivity(const QString & str);
		void updateCurrentcolor(int index);
		void updateReplace(bool state);
		void updateDontReplace(bool state);
		void updateDelete(bool state);
		void updateHotkey(bool Hotkeyenabled);
		void updateHsiEnabled(bool HsiEnabled);
		void updateHSIRange(const QString &str_rmin,const QString &str_rmax,const QString &str_omin,const QString &str_omax,const QString &str_ymin,const QString &str_ymax,const QString &str_fmin,const QString &str_fmax,const QString &str_cmin,const QString &str_cmax,const QString &str_wymin,const QString &str_wymax);
// 		void update_r_min(const QString &str);
// 		void update_r_max(const QString &str);
// 		void update_o_min(const QString &str);
// 		void update_o_max(const QString &str);
// 		void update_y_min(const QString &str);
// 		void update_y_max(const QString &str);
// 		void update_f_min(const QString &str);
// 		void update_f_max(const QString &str);
// 		void update_c_min(const QString &str);
// 		void update_c_max(const QString &str);
// 		void update_w_min_Y(const QString &str);
// 		void update_w_max_Y(const QString &str);
signals:
		void enableSave(bool save);
		void enableUnDo(bool undo);
		void activeThisWindow();
		void updatePath(const QString & str );
		void sendCurrentColorToToolBox(int color);
		void updateWindowTitle(const QString & str);
protected:
	char* imageBuffer;
	char* tempimageBuffer;
	unsigned char* unclassifyed;
	int normalSize;//CPlane Size;
	int currentImageWidth_;
	int currentImageHeight_;
	unsigned char* lut_;
	unsigned char* undoLut;
	unsigned char* HSI_lut;
	int currentY, currentU, currentV;
	int currentpixelX,currentpixelY;
	int currentColour;
	int ySensitivity,uSensitivity,vSensitivity;
	bool DontReplace,Replace,Delete;
	bool hotkeyEnabled;
	bool hsiEnabled;
	double r_min,r_max,o_min,o_max,y_min,y_max,f_min,f_max,c_min,c_max,w_min_Y,w_max_Y;

	void paintEvent ( QPaintEvent * event );
	void mousePressEvent(QMouseEvent * event);
	void keyPressEvent(QKeyEvent * event);
	void onLeftButton(int pointX,int pointY);
	void onRightButton(int pointX,int pointY);
private:
	QImage* unclassifiedImage;//unclassifiedImage被Gernerate出来后就不变了，只是图片换了才变
	QImage* classifiedImage;
	QImage* tempClassifiedImage;//被画出的临时效果图
	QImage* pureImage;//一张从头到尾干净的图片，在生成临时效果图前赋给tempClassifiedImage，用于clean
	QString lastFileName;
	QString lastLutName;
	QString rawString;
	QString lutString;
	QString saveLutString;
	int m_CurrentFileIndex;

	///=====//
	QDir currentDir;
	QString currentPath;
	QStringList currentList;
	int currentIndex;
	void LoadRawStream();
	void LoadLUT(const QString& fileName);
	void initImageParament();
	void GenUnclassifyImage(unsigned char *buffer,QImage* image);
	void ConvertYCbCrToRGB(unsigned char Y, unsigned char Cb, unsigned char Cr,int &finalR,int &finalG,int &finalB)  ;
	void GenClassifyImage(unsigned char *lut,unsigned char *buffer,QImage* image);
	QRgb LookupRgb(int index);
	void GenTempClassifyImage(unsigned char *buffer,QImage *ToImage,QImage *FromImage);
	void HSI_Process();
	void ConverRGBtoHSI(int R, int G, int B, double& H, double& S, double& I);
	void activateWindow();
	////////////nao来了就不要这个函数了
	void genTempImageBuffer(char *to,char *from);



};

#endif
