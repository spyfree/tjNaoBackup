#include "ClassifyImageToolBox.h"
ClassifyImageToolBox::ClassifyImageToolBox(QWidget*parent):DockWidget(parent)
{	
	widgetForDock = new QWidget;
	creatWidget();
	setWidget(widgetForDock);
	tjhsidialog=NULL;
}

void ClassifyImageToolBox::creatWidget()
{
	LoadLut = new QPushButton(tr("LoadLut"));
	SaveLut = new QPushButton(tr("SaveLut"));
	SaveLut->setEnabled(false);
	SaveAsLut = new QPushButton(tr("SaveAsLut"));
	Loadimage = new QPushButton(tr("LoadImage"));
	PreImage = new QPushButton(tr("PreImage"));
	NextImage = new QPushButton(tr("NextImage"));
	PathName=new QLineEdit(tr(""));

	HotKey = new QCheckBox(tr("HotKey"));
	HotKey->setChecked(false);
	selectColor = new QComboBox;
	selectColor->addItem(tr("FIELD_GREEN"));
	selectColor->addItem(tr("WHITE"));
	selectColor->addItem(tr("BALL_ORANGE"));
	selectColor->addItem(tr("ROBOT_RED"));
	selectColor->addItem(tr("ROBOT_BLUE"));
	selectColor->addItem(tr("BEACON_GREEN"));
	selectColor->addItem(tr("BEACON_BLUE"));
	selectColor->addItem(tr("BEACON_PINK"));
	selectColor->addItem(tr("BEACON_YELLOW"));
	ylabel = new QLabel(tr("y"));
	ulabel = new QLabel(tr("u"));
	vlabel = new QLabel(tr("v"));
	y = new QLineEdit(tr("10"));//10不能随便修改，应该和ClassifyImage中ysensitivity的初始化的值对应
	u = new QLineEdit(tr("10"));
	v = new QLineEdit(tr("10"));
	yuvRangeSet = new QGroupBox(tr("yuvRange"));

	HSI = new QCheckBox(tr("HSI"));
	HSI->setChecked(false);
	HSIRangeSet=new QPushButton(tr("HSIRangeSet"));

	//hsiRangeSet = new QGroupBox(tr("hsiRangeSet"));





	NotReplace = new QRadioButton(tr("NotReplace"));
	NotReplace->setChecked(true);
	Replace = new QRadioButton(tr("Replace"));
	Delete = new QRadioButton(tr("Delete"));
	ReplaceGroupBox = new QGroupBox(tr("ReplaceGroupBox"));

	useSoftColor = new QCheckBox(tr("useSoftColor"));
	UnDo = new QPushButton(tr("UnDo"));
	UnDo->setEnabled(false);
	//layout
	QHBoxLayout *yLayout = new QHBoxLayout;
	yLayout->addWidget(ylabel);
	yLayout->addWidget(y);
	QHBoxLayout *uLayout = new QHBoxLayout;
	uLayout->addWidget(ulabel);
	uLayout->addWidget(u);
	QHBoxLayout *vLayout = new QHBoxLayout;
	vLayout->addWidget(vlabel);
	vLayout->addWidget(v);
	QVBoxLayout *yuvLayout = new QVBoxLayout;
	yuvLayout->addLayout(yLayout);
	yuvLayout->addLayout(uLayout);
	yuvLayout->addLayout(vLayout);
	yuvRangeSet->setLayout(yuvLayout);
	

	QVBoxLayout *replaceLayout = new QVBoxLayout;
	replaceLayout->addWidget(NotReplace);
	replaceLayout->addWidget(Replace);
	replaceLayout->addWidget(Delete);
	replaceLayout->setSizeConstraint(QLayout::SetFixedSize);
	ReplaceGroupBox->setLayout(replaceLayout);
    //set size
	PathName->setMaximumWidth(replaceLayout->sizeHint().width());
	y->setMaximumWidth(replaceLayout->sizeHint().width()-30);
	u->setMaximumWidth(replaceLayout->sizeHint().width()-30);
	v->setMaximumWidth(replaceLayout->sizeHint().width()-30);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(LoadLut);
	mainLayout->addWidget(SaveLut);
	mainLayout->addWidget(SaveAsLut);
	mainLayout->addWidget(Loadimage);
	mainLayout->addWidget(PreImage);
	mainLayout->addWidget(NextImage);
	mainLayout->addWidget(PathName);
	mainLayout->addWidget(HotKey);
	mainLayout->addWidget(selectColor);
	mainLayout->addWidget(yuvRangeSet);
	mainLayout->addWidget(HSI);
	mainLayout->addWidget(HSIRangeSet);

	//mainLayout->addWidget(hsiRangeSet);
	mainLayout->addWidget(ReplaceGroupBox);
	mainLayout->addWidget(useSoftColor);
	mainLayout->addWidget(UnDo);
	mainLayout->addStretch();
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	widgetForDock->setLayout(mainLayout);
	connect(HSIRangeSet,SIGNAL(clicked()),this,SLOT(pushHSIRangeSetButton()));
}
void ClassifyImageToolBox::pushHSIRangeSetButton()
{
	if (!tjhsidialog)
	{
		tjhsidialog=new HSIDialog(this);
		connect(tjhsidialog,SIGNAL(sendHSIRangeToToolBox(const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &)),this,SIGNAL(updateHSIRange(const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &)));
	}	
	tjhsidialog->setModal(true);//set the dialog as the modal dialog
	tjhsidialog->show();
	tjhsidialog->raise();
	tjhsidialog->activateWindow();
// 	HSIDialog *tjhsidialog=new HSIDialog(this);
// 	if (tjhsidialog->exec())
// 	{
// 		connect(tjhsidialog,SIGNAL(sendHSIRangeToToolBox(const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &)),this,SIGNAL(updateHSIRange(const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &)));
// 	}
// 	delete tjhsidialog;
}
HSIDialog::HSIDialog(QWidget*parent):QDialog(parent)
{	
	setWindowTitle(tr("HSIDialog"));
	//////////init parament
	rmin_backup=QString("0");
	rmax_backup=QString("23");
	omin_backup=QString("23");
	omax_backup=QString("49");
	ymin_backup=QString("53");
	ymax_backup=QString("70");
	fmin_backup=QString("78");
	fmax_backup=QString("120");
	cmin_backup=QString("120");
	cmax_backup=QString("166");
	wymin_backup=QString("200");
	wymax_backup=QString("0");

	rmin=QString("0");
	rmax=QString("23");
	omin=QString("23");
	omax=QString("49");
	ymin=QString("53");
	ymax=QString("70");
	fmin=QString("78");
	fmax=QString("120");
	cmin=QString("120");
	cmax=QString("166");
	wymin=QString("200");
	wymax=QString("0");

	/////////creat widget
	OK=new QPushButton(tr("OK"));
	Cancel=new QPushButton(tr("Cancel"));
	R=new QLabel(tr("<R<"));
	O=new QLabel(tr("<O<"));
	Y=new QLabel(tr("<Y<"));
	F=new QLabel(tr("<F<"));
	C=new QLabel(tr("<C<"));
	W_y=new QLabel(tr("<W_y<"));
	R_min=new QLineEdit(tr("0"));
	R_max=new QLineEdit(tr("23"));
	O_min=new QLineEdit(tr("23"));
	O_max=new QLineEdit(tr("49"));
	Y_min=new QLineEdit(tr("53"));
	Y_max=new QLineEdit(tr("70"));
	F_min=new QLineEdit(tr("78"));
	F_max=new QLineEdit(tr("120"));
	C_min=new QLineEdit(tr("120"));
	C_max=new QLineEdit(tr("166"));
	W_y_min=new QLineEdit(tr("200"));
	W_y_max=new QLineEdit(tr("0"));
	/////////////Layout
	QHBoxLayout *Layout1=new QHBoxLayout;
	Layout1->addWidget(R_min);
	Layout1->addWidget(R);
	Layout1->addWidget(R_max);
	Layout1->addWidget(F_min);
	Layout1->addWidget(F);
	Layout1->addWidget(F_max);
	QHBoxLayout *Layout2=new QHBoxLayout;
	Layout2->addWidget(O_min);
	Layout2->addWidget(O);
	Layout2->addWidget(O_max);
	Layout2->addWidget(C_min);
	Layout2->addWidget(C);
	Layout2->addWidget(C_max);
	QHBoxLayout *Layout3=new QHBoxLayout;
	Layout3->addWidget(Y_min);
	Layout3->addWidget(Y);
	Layout3->addWidget(Y_max);
	Layout3->addWidget(W_y_min);
	Layout3->addWidget(W_y);
	Layout3->addWidget(W_y_max);
	QVBoxLayout *wholeHSILayout=new QVBoxLayout;
	wholeHSILayout->addLayout(Layout1);
	wholeHSILayout->addLayout(Layout2);
	wholeHSILayout->addLayout(Layout3);
	QVBoxLayout *rightLayout=new QVBoxLayout;
	rightLayout->addWidget(OK);
	rightLayout->addWidget(Cancel);
	QHBoxLayout *final=new QHBoxLayout;
	final->addLayout(wholeHSILayout);
	final->addLayout(rightLayout);
	setLayout(final);
	/////////connect
	connect(R_min,SIGNAL(textChanged(const QString & )),this,SLOT(update_r_min(const QString &)));
	connect(R_max,SIGNAL(textChanged(const QString & )),this,SLOT(update_r_max(const QString &)));
	connect(O_min,SIGNAL(textChanged(const QString & )),this,SLOT(update_o_min(const QString &)));
	connect(O_max,SIGNAL(textChanged(const QString & )),this,SLOT(update_o_max(const QString &)));
	connect(Y_min,SIGNAL(textChanged(const QString & )),this,SLOT(update_y_min(const QString &)));
	connect(Y_max,SIGNAL(textChanged(const QString & )),this,SLOT(update_y_max(const QString &)));
	connect(F_min,SIGNAL(textChanged(const QString & )),this,SLOT(update_f_min(const QString &)));
	connect(F_max,SIGNAL(textChanged(const QString & )),this,SLOT(update_f_max(const QString &)));
	connect(C_min,SIGNAL(textChanged(const QString & )),this,SLOT(update_c_min(const QString &)));
	connect(C_max,SIGNAL(textChanged(const QString & )),this,SLOT(update_c_max(const QString &)));
	connect(W_y_min,SIGNAL(textChanged(const QString & )),this,SLOT(update_w_min_Y(const QString &)));
	connect(W_y_max,SIGNAL(textChanged(const QString & )),this,SLOT(update_w_max_Y(const QString &)));
	connect(Cancel,SIGNAL(clicked()),this,SLOT(clickCancel()));
	connect(OK,SIGNAL(clicked()),this,SLOT(clickedOK()));
}
void HSIDialog::clickCancel()
{
	//if click cancel ,set the lineedit as the backup,let it be the last (time after click OK button) value
	R_min->setText(rmin_backup);
	R_max->setText(rmax_backup);
	O_min->setText(omin_backup);
	O_max->setText(omax_backup);
	Y_min->setText(ymin_backup);
	Y_max->setText(ymax_backup);
	F_min->setText(fmin_backup);
	F_max->setText(fmax_backup);
	C_min->setText(cmin_backup);
	C_max->setText(cmax_backup);
	W_y_min->setText(wymin_backup);
	W_y_max->setText(wymax_backup);
	emit close();


}
void HSIDialog::clickedOK()
{
	emit sendHSIRangeToToolBox(rmin,rmax,omin,omax,ymin,ymax,fmin,fmax,cmin,cmax,wymin,wymax);
// 	emit sendrminToToolBox(rmin);
// 	emit sendrmaxToToolBox(rmax);
// 	emit sendominToToolBox(omin);
// 	emit sendominToToolBox(omin);
// 	emit sendyminToToolBox(ymin);
// 	emit sendymaxToToolBox(ymax);
// 	emit sendfminToToolBox(fmin);
// 	emit sendfmaxToToolBox(fmax);
// 	emit sendcminToToolBox(cmin);
// 	emit sendcmaxToToolBox(cmax);
// 	emit sendwyminToToolBox(wymin);
// 	emit sendwymaxToToolBox(wymax);
	R_min->setText(rmin);
	R_max->setText(rmax);
	O_min->setText(omin);
	O_max->setText(omax);
	Y_min->setText(ymin);
	Y_max->setText(ymax);
	F_min->setText(fmin);
	F_max->setText(fmax);
	C_min->setText(cmin);
	C_max->setText(cmax);
	W_y_min->setText(wymin);
	W_y_max->setText(wymax);
	//backup the final text
	rmin_backup=rmin;
	rmax_backup=rmax;
	omin_backup=omin;
	omax_backup=omax;
	ymin_backup=ymin;
	ymax_backup=ymax;
	fmin_backup=fmin;
	fmax_backup=fmax;
	cmin_backup=cmin;
	cmax_backup=cmax;
	wymin_backup=wymin;
	wymax_backup=wymax;

	emit close();
}
void HSIDialog::update_r_min(const QString &str)
{
	rmin=str;
}
void HSIDialog::update_r_max(const QString &str)
{
	rmax=str;
}
void HSIDialog::update_o_min(const QString &str)
{
	omin=str;
}
void HSIDialog::update_o_max(const QString &str)
{
	omax=str;
}
void HSIDialog::update_y_min(const QString &str)
{
	ymin=str;
}
void HSIDialog::update_y_max(const QString &str)
{
	ymax=str;
}
void HSIDialog::update_f_min(const QString &str)
{
	fmin=str;
}
void HSIDialog::update_f_max(const QString &str)
{
	fmax=str;
}
void HSIDialog::update_c_min(const QString &str)
{
	cmin=str;
}
void HSIDialog::update_c_max(const QString &str)
{
	cmax=str;
}
void HSIDialog::update_w_min_Y(const QString &str)
{
	wymin=str;
}
void HSIDialog::update_w_max_Y(const QString &str)
{
	wymax=str;
}
