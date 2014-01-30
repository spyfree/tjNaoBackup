#ifndef CLASSIFYIMAGETOOLBOX_H
#define CLASSIFYIMAGETOOLBOX_H
#include "../DockWidget.h"
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QLineEdit>
#include <QWidget>
#include <QDialog>
class HSIDialog:public QDialog
{
	Q_OBJECT
public:
	HSIDialog(QWidget *parent=0);
private:
	QLabel *R,*O,*Y,*F,*C,*W_y;
	QLineEdit *R_min,*R_max,*O_min,*O_max,*Y_min,*Y_max,*F_min,*F_max,*C_min,*C_max,*W_y_min,*W_y_max;
	QPushButton *OK;
	QPushButton *Cancel;
	QString rmin,rmax,omin,omax,ymin,ymax,fmin,fmax,cmin,cmax,wymin,wymax;
	QString rmin_backup,rmax_backup,omin_backup,omax_backup,ymin_backup,ymax_backup,fmin_backup,fmax_backup,cmin_backup,cmax_backup,wymin_backup,wymax_backup;

signals:
	void sendHSIRangeToToolBox(const QString &str_rmin,const QString &str_rmax,const QString &str_omin,const QString &str_omax,const QString &str_ymin,const QString &str_ymax,const QString &str_fmin,const QString &str_fmax,const QString &str_cmin,const QString &str_cmax,const QString &str_wymin,const QString &str_wymax);
	// 	void sendrminToToolBox(const QString &str);
	// 	void sendrmaxToToolBox(const QString &str);
	// 	void sendominToToolBox(const QString &str);
	// 	void sendominToToolBox(const QString &str);
	// 	void sendyminToToolBox(const QString &str);
	// 	void sendymaxToToolBox(const QString &str);
	// 	void sendfminToToolBox(const QString &str);
	// 	void sendfmaxToToolBox(const QString &str);
	// 	void sendcminToToolBox(const QString &str);
	// 	void sendcmaxToToolBox(const QString &str);
	// 	void sendwyminToToolBox(const QString &str);
	// 	void sendwymaxToToolBox(const QString &str);
private slots:
		void update_r_min(const QString &str);
		void update_r_max(const QString &str);
		void update_o_min(const QString &str);
		void update_o_max(const QString &str);
		void update_y_min(const QString &str);
		void update_y_max(const QString &str);
		void update_f_min(const QString &str);
		void update_f_max(const QString &str);
		void update_c_min(const QString &str);
		void update_c_max(const QString &str);
		void update_w_min_Y(const QString &str);
		void update_w_max_Y(const QString &str);
		void clickedOK();
		void clickCancel();

};

class ClassifyImageToolBox:public DockWidget
{	
	Q_OBJECT
public:
	ClassifyImageToolBox(QWidget*parent=0);
	//the small widget
	QPushButton *SaveLut;
	QPushButton *LoadLut;
	QPushButton *SaveAsLut;
	QPushButton *Loadimage;
	QPushButton *PreImage;
	QPushButton *NextImage;
	QLineEdit *PathName;
	QCheckBox *HotKey;
	QComboBox *selectColor;
	QLabel *ylabel;
	QLabel *ulabel;
	QLabel *vlabel;
	QLineEdit *y;
	QLineEdit *u;
	QLineEdit *v;
	QGroupBox *yuvRangeSet;

	QCheckBox *HSI;
	//QGroupBox *hsiRangeSet;
	QPushButton *HSIRangeSet;
	HSIDialog *tjhsidialog;
	QRadioButton *Replace;
	QRadioButton *NotReplace;
	QRadioButton *Delete;
	QGroupBox *ReplaceGroupBox;

	QCheckBox *useSoftColor;
	QPushButton *UnDo;

private slots:
	void pushHSIRangeSetButton();
signals:
	void updateHSIRange(const QString &str_rmin,const QString &str_rmax,const QString &str_omin,const QString &str_omax,const QString &str_ymin,const QString &str_ymax,const QString &str_fmin,const QString &str_fmax,const QString &str_cmin,const QString &str_cmax,const QString &str_wymin,const QString &str_wymax);
private:
	//the small widget

	//the whole widget
	QWidget *widgetForDock;
	void creatWidget();
};

#endif