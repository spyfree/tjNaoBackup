#include "CameraSettingDialog.h"
CameraSettingDialog::CameraSettingDialog(QMdiArea* mdiArea):SubWindow(mdiArea,"CameraSettingDialog")
{
// 	/setWindowTitle(tr("CameraSettingDialog"));
// 	cameraConfig_inDialog=CameraConfig::getInstance();
// 	thebackupCameraConfig_InDialog=new tempCameraConfig_InDialog;
// 	creatWidget();
// 	creatLayout();
// 	creatConnect();
// 	setInit();//inti thebackupCameraConfig_InDialog
// 	setWidgetParament();//inti 控件初始值，将thebackupCameraConfig_InDialog值设给控件
// 	setCameraConfig(); //在这里做这个先设一下cameraconfig的初值，ok的时候再传出修改的值
	cameraConfig_inDialog=CameraConfig::getInstance();
	thebackupCameraConfig_InDialog=new tempCameraConfig_InDialog;
	creatWidget();
	creatLayout();
	widget=new QWidget;
	widget->setLayout(mmlayout);
	setWidget(widget);
	creatConnect();
	setInit();//inti thebackupCameraConfig_InDialog
	setWidgetParament();//inti 控件初始值，将thebackupCameraConfig_InDialog值设给控件
	setCameraConfig(); //在这里做这个先设一下cameraconfig的初值，ok的时候再传出修改的值
	setVisible(true);
	this->setWindowFlags(Qt::WindowMinimizeButtonHint);

}
CameraSettingDialog::~CameraSettingDialog()
{

}
void CameraSettingDialog::creatWidget()
{
	Framerate_QLabel=new QLabel(tr("Framerate"));
	Framerate_QSpinBox_text=new QSpinBox;
	Framerate_QSpinBox_text->setReadOnly(true);
	Framerate_QSlider=new QSlider(Qt::Horizontal);
	Framerate_QSlider->setRange(5,30);
	Framerate_QSpinBox_text->setRange(5,30);
	
	resolution_QLabel=new QLabel(tr("resolution"));
	resolution_QComboBox=new QComboBox;
	resolution_QComboBox->addItem(tr("160*120"));
	resolution_QComboBox->addItem(tr("320*240"));
	resolution_QComboBox->addItem(tr("640*480"));

	selectBottomCamera_QCheckBox=new QCheckBox(tr("selectBottomCamera"));
	
	autoGain_QCheckBox=new QCheckBox(tr("autoGain"));

	gain_QLabel=new QLabel(tr("gain"));
	gain_QSpinBox_text=new QSpinBox;
	gain_QSpinBox_text->setReadOnly(true);
	gain_QSlider=new QSlider(Qt::Horizontal);
	gain_QSlider->setRange(0,255);
	gain_QSpinBox_text->setRange(0,255);

	autoWhiteBalence_QCheckBox=new QCheckBox(tr("autoWhiteBalence"));

	BlueChroma_QLabel=new QLabel(tr("Blue chroma"));
	BlueChroma_QSpinBox_text=new QSpinBox;
	BlueChroma_QSpinBox_text->setReadOnly(true);
	BlueChroma_QSlider=new QSlider(Qt::Horizontal);
	BlueChroma_QSlider->setRange(0,255);
	BlueChroma_QSpinBox_text->setRange(0,255);

	RedChroma_QLabel=new QLabel(tr("Red chroma"));
	RedChroma_QSpinBox_text=new QSpinBox;
	RedChroma_QSpinBox_text->setReadOnly(true);
	RedChroma_QSlider=new QSlider(Qt::Horizontal);
	RedChroma_QSlider->setRange(0,255);
	RedChroma_QSpinBox_text->setRange(0,255);

	Brightness_QLabel=new QLabel(tr("Brightness"));
	Brightness_QSpinBox_text=new QSpinBox;
	Brightness_QSpinBox_text->setReadOnly(true);
	Brightness_QSlider=new QSlider(Qt::Horizontal);
	Brightness_QSlider->setRange(0,255);
	Brightness_QSpinBox_text->setRange(0,255);

	Contrast_QLabel=new QLabel(tr("Contrast"));
	Contrast_QSpinBox_text=new QSpinBox;
	Contrast_QSpinBox_text->setReadOnly(true);
	Contrast_QSlider=new QSlider(Qt::Horizontal);
	Contrast_QSlider->setRange(0,127);
	Contrast_QSpinBox_text->setRange(0,127);

	Saturation_QLabel=new QLabel(tr("Saturation"));
	Saturation_QSpinBox_text=new QSpinBox;
	Saturation_QSpinBox_text->setReadOnly(true);
	Saturation_QSlider=new QSlider(Qt::Horizontal);
	Saturation_QSlider->setRange(0,255);
	Saturation_QSpinBox_text->setRange(0,255);

	Hue_QLabel=new QLabel(tr("Contrast"));
	Hue_QSpinBox_text=new QSpinBox;
	Hue_QSpinBox_text->setReadOnly(true);
	Hue_QSlider=new QSlider(Qt::Horizontal);
	Hue_QSlider->setRange(-180,180);
	Hue_QSpinBox_text->setRange(-180,180);

	LensCorrectionX_QLabel=new QLabel(tr("Lens Correction  X"));
	LensCorrectionX_QSpinBox_text=new QSpinBox;
	LensCorrectionX_QSpinBox_text->setReadOnly(true);
	LensCorrectionX_QSlider=new QSlider(Qt::Horizontal);
	LensCorrectionX_QSlider->setRange(0,255);
	LensCorrectionX_QSpinBox_text->setRange(0,255);

	LensCorrectionY_QLabel=new QLabel(tr("Lens Correction  Y"));
	LensCorrectionY_QSpinBox_text=new QSpinBox;
	LensCorrectionY_QSpinBox_text->setReadOnly(true);
	LensCorrectionY_QSlider=new QSlider(Qt::Horizontal);
	LensCorrectionY_QSlider->setRange(0,255);
	LensCorrectionY_QSpinBox_text->setRange(0,255);

	AutoExposition_QCheckBox=new QCheckBox(tr("Auto Exposition"));

	Exposure_QLabel=new QLabel(tr("Exposure"));
	Exposure_QSpinBox_text=new QSpinBox;
	Exposure_QSpinBox_text->setReadOnly(true);
	Exposure_QSlider=new QSlider(Qt::Horizontal);
	Exposure_QSlider->setRange(0,1024);
	Exposure_QSpinBox_text->setRange(0,1024);

	int a=Exposure_QSlider->minimum();
	int b=Exposure_QSlider->maximum();
	VerticalFlip_QCheckBox=new QCheckBox(tr("Vertical Flip"));

	HerticalFlip_QCheckBox=new QCheckBox(tr("Horizontal Flip"));

	OK=new QPushButton(tr("OK"));
	Cancel=new QPushButton(tr("Cancel"));

}
void CameraSettingDialog::creatLayout()
{
	mmlayout=new QVBoxLayout;
	QGridLayout *mainlayout=new QGridLayout;//mainlayout不是最外面的layout
	mainlayout->addWidget(Framerate_QLabel,1,1);
	mainlayout->addWidget(Framerate_QSpinBox_text,1,2);
	mainlayout->addWidget(Framerate_QSlider,1,3);
	mainlayout->addWidget(resolution_QLabel,2,1);
	mainlayout->addWidget(resolution_QComboBox,2,2);
	mainlayout->addWidget(selectBottomCamera_QCheckBox,3,1);
	mainlayout->addWidget(autoGain_QCheckBox,4,1);
	mainlayout->addWidget(gain_QLabel,5,1);
	mainlayout->addWidget(gain_QSpinBox_text,5,2);
	mainlayout->addWidget(gain_QSlider,5,3);
	mainlayout->addWidget(autoWhiteBalence_QCheckBox,6,1);
	mainlayout->addWidget(BlueChroma_QLabel,7,1);
	mainlayout->addWidget(BlueChroma_QSpinBox_text,7,2);
	mainlayout->addWidget(BlueChroma_QSlider,7,3);
	mainlayout->addWidget(RedChroma_QLabel,8,1);
	mainlayout->addWidget(RedChroma_QSpinBox_text,8,2);
	mainlayout->addWidget(RedChroma_QSlider,8,3);
	mainlayout->addWidget(Brightness_QLabel,9,1);
	mainlayout->addWidget(Brightness_QSpinBox_text,9,2);
	mainlayout->addWidget(Brightness_QSlider,9,3);
	mainlayout->addWidget(Contrast_QLabel,10,1);
	mainlayout->addWidget(Contrast_QSpinBox_text,10,2);
	mainlayout->addWidget(Contrast_QSlider,10,3);
	mainlayout->addWidget(Saturation_QLabel,11,1);
	mainlayout->addWidget(Saturation_QSpinBox_text,11,2);
	mainlayout->addWidget(Saturation_QSlider,11,3);
	mainlayout->addWidget(Hue_QLabel,12,1);
	mainlayout->addWidget(Hue_QSpinBox_text,12,2);
	mainlayout->addWidget(Hue_QSlider,12,3);
	mainlayout->addWidget(LensCorrectionX_QLabel,13,1);
	mainlayout->addWidget(LensCorrectionX_QSpinBox_text,13,2);
	mainlayout->addWidget(LensCorrectionX_QSlider,13,3);
	mainlayout->addWidget(LensCorrectionY_QLabel,14,1);
	mainlayout->addWidget(LensCorrectionY_QSpinBox_text,14,2);
	mainlayout->addWidget(LensCorrectionY_QSlider,14,3);
	mainlayout->addWidget(AutoExposition_QCheckBox,15,1);
    mainlayout->addWidget(Exposure_QLabel,16,1);
	mainlayout->addWidget(Exposure_QSpinBox_text,16,2);
	mainlayout->addWidget(Exposure_QSlider,16,3);
	mainlayout->addWidget(VerticalFlip_QCheckBox,17,1);
	mainlayout->addWidget(HerticalFlip_QCheckBox,18,1);
    mainlayout->setColumnMinimumWidth(3,200);
	
	QHBoxLayout *button=new QHBoxLayout;
	button->addStretch();
	button->addWidget(OK);
	button->addWidget(Cancel);
	button->setSizeConstraint(QLayout::SetFixedSize);

	mmlayout->addLayout(mainlayout);
	mmlayout->addLayout(button);
   // setLayout(mmlayout);
}
void CameraSettingDialog::creatConnect()
{
	connect(Framerate_QSlider,SIGNAL(valueChanged(int)),Framerate_QSpinBox_text,SLOT(setValue(int)));
	connect(gain_QSlider,SIGNAL(valueChanged(int)),gain_QSpinBox_text,SLOT(setValue(int)));
	connect(BlueChroma_QSlider,SIGNAL(valueChanged(int)),BlueChroma_QSpinBox_text,SLOT(setValue(int)));
	connect(RedChroma_QSlider,SIGNAL(valueChanged(int)),RedChroma_QSpinBox_text,SLOT(setValue(int)));
	connect(Brightness_QSlider,SIGNAL(valueChanged(int)),Brightness_QSpinBox_text,SLOT(setValue(int)));
	connect(Contrast_QSlider,SIGNAL(valueChanged(int)),Contrast_QSpinBox_text,SLOT(setValue(int)));
	connect(Saturation_QSlider,SIGNAL(valueChanged(int)),Saturation_QSpinBox_text,SLOT(setValue(int)));
	connect(Hue_QSlider,SIGNAL(valueChanged(int)),Hue_QSpinBox_text,SLOT(setValue(int)));
	connect(LensCorrectionX_QSlider,SIGNAL(valueChanged(int)),LensCorrectionX_QSpinBox_text,SLOT(setValue(int)));
	connect(LensCorrectionY_QSlider,SIGNAL(valueChanged(int)),LensCorrectionY_QSpinBox_text,SLOT(setValue(int)));
	connect(Exposure_QSlider,SIGNAL(valueChanged(int)),Exposure_QSpinBox_text,SLOT(setValue(int)));
	connect(OK,SIGNAL(clicked()),this,SLOT(pushOK()));
	connect(Cancel,SIGNAL(clicked()),this,SLOT(pushCancel()));
}

void CameraSettingDialog::setInit()//inti thebackupCameraConfig_InDialog
{
	thebackupCameraConfig_InDialog->framerate=15;
	thebackupCameraConfig_InDialog->resolution=1;
	thebackupCameraConfig_InDialog->selectBottomCamera=false;
	thebackupCameraConfig_InDialog->autoGain=true;
	thebackupCameraConfig_InDialog->gain=4;
	thebackupCameraConfig_InDialog->autoWhiteBalence=true;
	thebackupCameraConfig_InDialog->BlueChroma=96;
	thebackupCameraConfig_InDialog->RedChroma=88;
	thebackupCameraConfig_InDialog->Brightness=128;
	thebackupCameraConfig_InDialog->Contrast=64;
	thebackupCameraConfig_InDialog->Saturation=128;
	thebackupCameraConfig_InDialog->Hue=0;
	thebackupCameraConfig_InDialog->LensCorrectionX=0;
	thebackupCameraConfig_InDialog->LensCorrectionY=0;
	thebackupCameraConfig_InDialog->AutoExposition=true;
	thebackupCameraConfig_InDialog->Exposure=504;
	thebackupCameraConfig_InDialog->VerticalFlip=true;
	thebackupCameraConfig_InDialog->HerticalFlip=true;


}
void CameraSettingDialog::setCameraConfig()
{
	cameraConfig_inDialog->framerate=Framerate_QSlider->value();
	cameraConfig_inDialog->resolution=resolution_QComboBox->currentIndex();//+1有待考证
	cameraConfig_inDialog->selectBottomCamera=Saturation_QSlider->value();
	cameraConfig_inDialog->autoGain=autoGain_QCheckBox->isChecked();
	cameraConfig_inDialog->gain=gain_QSlider->value();
	cameraConfig_inDialog->autoWhiteBalence=autoGain_QCheckBox->isChecked();
	cameraConfig_inDialog->BlueChroma=BlueChroma_QSlider->value();
	cameraConfig_inDialog->RedChroma=RedChroma_QSlider->value();
	cameraConfig_inDialog->Brightness=Brightness_QSlider->value();
	cameraConfig_inDialog->Contrast=Contrast_QSlider->value();
	cameraConfig_inDialog->Saturation=Saturation_QSlider->value();
	cameraConfig_inDialog->Hue=Hue_QSlider->value();
	cameraConfig_inDialog->LensCorrectionX=LensCorrectionX_QSlider->value();
	cameraConfig_inDialog->LensCorrectionY=LensCorrectionY_QSlider->value();
	cameraConfig_inDialog->AutoExposition=AutoExposition_QCheckBox->isChecked();
	cameraConfig_inDialog->Exposure=Exposure_QSlider->value();
	cameraConfig_inDialog->VerticalFlip=VerticalFlip_QCheckBox->isChecked();
	cameraConfig_inDialog->HerticalFlip=HerticalFlip_QCheckBox->isChecked();

}
void CameraSettingDialog::pushOK()
{
	setCameraConfig();
	backup_tempCameraConfig_InDialog();
	setVisible(false);
}
void CameraSettingDialog::pushCancel()
{
	setWidgetParament();//回写回控件
	setVisible(false);
}
void CameraSettingDialog::closeEvent()
{
	setWidgetParament();//回写回控件
	setVisible(false);
}
void CameraSettingDialog::backup_tempCameraConfig_InDialog()
{
	thebackupCameraConfig_InDialog->framerate=cameraConfig_inDialog->framerate;
	thebackupCameraConfig_InDialog->resolution=cameraConfig_inDialog->resolution;
	thebackupCameraConfig_InDialog->selectBottomCamera=cameraConfig_inDialog->selectBottomCamera;
	thebackupCameraConfig_InDialog->autoGain=cameraConfig_inDialog->autoGain;
	thebackupCameraConfig_InDialog->gain=cameraConfig_inDialog->gain;
	thebackupCameraConfig_InDialog->autoWhiteBalence=cameraConfig_inDialog->autoWhiteBalence;
	thebackupCameraConfig_InDialog->BlueChroma=cameraConfig_inDialog->BlueChroma;
	thebackupCameraConfig_InDialog->RedChroma=cameraConfig_inDialog->RedChroma;
	thebackupCameraConfig_InDialog->Brightness=cameraConfig_inDialog->Brightness;
	thebackupCameraConfig_InDialog->Contrast=cameraConfig_inDialog->Contrast;
	thebackupCameraConfig_InDialog->Saturation=cameraConfig_inDialog->Saturation;
	thebackupCameraConfig_InDialog->Hue=cameraConfig_inDialog->Hue;
	thebackupCameraConfig_InDialog->LensCorrectionX=cameraConfig_inDialog->LensCorrectionX;
	thebackupCameraConfig_InDialog->LensCorrectionY=cameraConfig_inDialog->LensCorrectionY;
	thebackupCameraConfig_InDialog->AutoExposition=cameraConfig_inDialog->AutoExposition;
	thebackupCameraConfig_InDialog->Exposure=cameraConfig_inDialog->Exposure;
	thebackupCameraConfig_InDialog->VerticalFlip=cameraConfig_inDialog->VerticalFlip;
	thebackupCameraConfig_InDialog->HerticalFlip=cameraConfig_inDialog->HerticalFlip;

}
void CameraSettingDialog::setWidgetParament()
{
	Framerate_QSlider->setValue(thebackupCameraConfig_InDialog->framerate);
	resolution_QComboBox->setCurrentIndex(thebackupCameraConfig_InDialog->resolution);
	selectBottomCamera_QCheckBox->setChecked(thebackupCameraConfig_InDialog->selectBottomCamera);
	autoGain_QCheckBox->setChecked(thebackupCameraConfig_InDialog->autoGain);
	gain_QSlider->setValue(thebackupCameraConfig_InDialog->gain);
	autoWhiteBalence_QCheckBox->setChecked(thebackupCameraConfig_InDialog->autoWhiteBalence);
	BlueChroma_QSlider->setValue(thebackupCameraConfig_InDialog->BlueChroma);
	RedChroma_QSlider->setValue(thebackupCameraConfig_InDialog->RedChroma);
	Brightness_QSlider->setValue(thebackupCameraConfig_InDialog->Brightness);
	Contrast_QSlider->setValue(thebackupCameraConfig_InDialog->Contrast);
	Saturation_QSlider->setValue(thebackupCameraConfig_InDialog->Saturation);
	Hue_QSlider->setValue(thebackupCameraConfig_InDialog->Hue);
	LensCorrectionX_QSlider->setValue(thebackupCameraConfig_InDialog->LensCorrectionX);
	LensCorrectionY_QSlider->setValue(thebackupCameraConfig_InDialog->LensCorrectionY);
	AutoExposition_QCheckBox->setChecked(thebackupCameraConfig_InDialog->AutoExposition);
	Exposure_QSlider->setValue(thebackupCameraConfig_InDialog->Exposure);
	VerticalFlip_QCheckBox->setChecked(thebackupCameraConfig_InDialog->VerticalFlip);
	HerticalFlip_QCheckBox->setChecked(thebackupCameraConfig_InDialog->HerticalFlip);


}
void CameraSettingDialog::	show()
{
	if(!isVisible())
	{	
		setVisible(!isVisible());
		widget->show();
	}
}
