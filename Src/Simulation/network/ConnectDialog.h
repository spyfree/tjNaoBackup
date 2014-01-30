#ifndef  CONNECTDIALOG_H
#define  CONNECTDIALOG_H
#include "Simulation/SubWindow.h"
#include "ConnectInfo.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QIntValidator>
#include <QComboBox>
#include <QFile>
#include <QTextStream>
#include "Platform/GetSetting.h"
#include "simulation\MenuBar.h"

class ConnectDialog:public SubWindow
{
	Q_OBJECT
public:
	ConnectDialog(QMdiArea* mdiArea);
	void EditItem(ConnectInfo const robotinfo);
	void NewItem();
signals:
	void SendValues(ConnectInfo robotinfo);
private slots:
	void SaveValues();
private:
	void UpdateValues();
	void Show();
	QPushButton *cancelButton;
	QPushButton *okButton;
	QLineEdit *nameEdit;
	QLineEdit *ipEdit;
	QLineEdit *portEdit;
	QGridLayout *layout;
	QWidget *widget;
	ConnectInfo *activeRobot;
};
class ConnectWindow:public SubWindow
{
	Q_OBJECT
public:
	ConnectWindow(QMdiArea *mdiArea);
	~ConnectWindow();
	void Show();
	ConnectInfo GetCurrentInfo();
signals:
	void ConfigInfo(ConnectInfo &robotinfo);
private slots:
	void ReceiveSetting(ConnectInfo const robotinfo);
	void AddItem();
	void DeleteItem();
	void EditItem();
	void SendInfo();
	void ChangeIndex(int index);
private:
	int IntFromString(QString str);
	void writingInfo(QVector<ConnectInfo> * list);
	void readInfo();
	QMdiArea *selfmdi;
	QPushButton *addButton;
	QPushButton *delButton;
	QPushButton *editButton;
	QComboBox *robotList;
	QPushButton *okButton;
	QPushButton *cancelButton;
	QHBoxLayout *infoLayout;
	QHBoxLayout *buttonLayout;
	QVBoxLayout *mainlayout;
	QWidget* widget;
	ConnectDialog *connectDialog;
	QVector<ConnectInfo> *robotInfoList;
	ConnectInfo connectRobot;

};
#endif
