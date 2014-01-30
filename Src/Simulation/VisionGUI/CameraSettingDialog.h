#ifndef CAMERASETTINGDIALOG_H
#define CAMERASETTINGDIALOG_H
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QComboBox>
#include <QPushButton>
#include<QCheckBox>
#include <QLayout>
#include <QDialog>
#include "Module/Common/CameraConfig.h"
#include "Simulation/SubWindow.h"
/*******************************
the class tempCameraConfig_InDialog is just used in cameraSettingDialog
it is write for push Cancel button
*******************************/
class tempCameraConfig_InDialog
{
public:
	tempCameraConfig_InDialog(){};
	~tempCameraConfig_InDialog(){};
public:
	int framerate;
	int resolution;
	bool selectBottomCamera;
	bool autoGain;
	int  gain;
	bool autoWhiteBalence;
	int BlueChroma;
	int RedChroma;
	int Brightness;
	int Contrast;
	int Saturation;
	int Hue;
	int LensCorrectionX;
	int LensCorrectionY;
	bool AutoExposition;
	int Exposure;
	bool VerticalFlip;
	bool HerticalFlip;
};
class CameraSettingDialog:public SubWindow
{
	Q_OBJECT
public:
	CameraSettingDialog(QMdiArea* mdiArea=0);
	~CameraSettingDialog();
private:
	//framerate
	QLabel *Framerate_QLabel;
	QSpinBox *Framerate_QSpinBox_text;
	QSlider *Framerate_QSlider;
	//resolution
	QLabel *resolution_QLabel;
	QComboBox *resolution_QComboBox;
	//selectBottomCamera
	QCheckBox *selectBottomCamera_QCheckBox;
	//autoGain
	QCheckBox *autoGain_QCheckBox;
	//gain
	QLabel *gain_QLabel;
	QSlider *gain_QSlider;
	QSpinBox *gain_QSpinBox_text;
	//autoWhiteBalence
	QCheckBox *autoWhiteBalence_QCheckBox;
	//Blue chroma
	QLabel *BlueChroma_QLabel;
	QSlider *BlueChroma_QSlider;
	QSpinBox *BlueChroma_QSpinBox_text;
	//Red Chroma
	QLabel *RedChroma_QLabel;
	QSlider *RedChroma_QSlider;
	QSpinBox *RedChroma_QSpinBox_text;
	//Brightness
	QLabel *Brightness_QLabel;
	QSlider *Brightness_QSlider;
	QSpinBox *Brightness_QSpinBox_text;
	//Contrast
	QLabel *Contrast_QLabel;
	QSlider *Contrast_QSlider;
	QSpinBox *Contrast_QSpinBox_text;
	//Saturation
	QLabel *Saturation_QLabel;
	QSlider *Saturation_QSlider;
	QSpinBox *Saturation_QSpinBox_text;
	//Hue
	QLabel *Hue_QLabel;
	QSlider *Hue_QSlider;
	QSpinBox *Hue_QSpinBox_text;
	//Lens Correction  X
	QLabel *LensCorrectionX_QLabel;
	QSlider *LensCorrectionX_QSlider;
	QSpinBox *LensCorrectionX_QSpinBox_text;
	//Correction Lens Y
	QLabel *LensCorrectionY_QLabel;
	QSlider *LensCorrectionY_QSlider;
	QSpinBox *LensCorrectionY_QSpinBox_text;
	//Auto Exposition
	QCheckBox *AutoExposition_QCheckBox;
	//Exposure
	QLabel *Exposure_QLabel;
	QSlider *Exposure_QSlider;
	QSpinBox *Exposure_QSpinBox_text;
	//Vertical Flip
	QCheckBox *VerticalFlip_QCheckBox;
	//Horizontal Flip
	QCheckBox *HerticalFlip_QCheckBox;


	QPushButton *OK;
	QPushButton *Cancel;

	QVBoxLayout *mmlayout;

	QWidget *widget;

	
	void creatWidget();
	void creatLayout();
	void creatConnect();
	void setInit();
	void setCameraConfig();
	void backup_tempCameraConfig_InDialog();
	void setWidgetParament();
	CameraConfig *cameraConfig_inDialog;
	tempCameraConfig_InDialog *thebackupCameraConfig_InDialog;






	
private slots:
	void pushOK();
	void pushCancel();
	void show();
	void closeEvent();
};

#endif