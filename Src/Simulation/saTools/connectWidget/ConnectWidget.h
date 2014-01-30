#ifndef CONNECTWIDGET_H
#define CONNECTWIDGET_H

#include <QWidget>
#include <vector>
using std::vector;
//--
#include "ConnectInformation.h"
#include "Platform/GetSetting.h"
//=========
class QPushButton;
class QComboBox;
class QWidget;
class QLineEdit;
//=========
class ConnectWidget : public QWidget
{
	Q_OBJECT

	private slots:
		void connectButton(bool);
		void disConnectButton(bool);

		void addRobotInfoButton();
		void deleRobotInfoButton();
		void editRobotInfoButton();

		void robotListBox(int);
		void saveEditInfo();
	public:
		ConnectWidget(QWidget *parent );
		~ConnectWidget();

	private:
		void createRobotInfoEditWidget();
		void writeInfo(vector<ConnectInformation>  robotVector);
		void readInfo();
		void refreshInfo(ConnectInformation  curConnectInfo);
		void resizeConnectWidget();

	private:		//Qt Components
		QPushButton * connectPushButton;
		QPushButton * disConnectPushButton;

		QPushButton * addRobotInfoPushButton;
		QPushButton * deleRobotInfoPushButton;
		QPushButton * editRobotInfoPushButton;
		//--
		QComboBox * robotListComboBox;

		//--
		QWidget * robotInfoEditWidget;
		QLineEdit * robotNameLineEdit;
		QLineEdit * ipLineEdit;
		QLineEdit * portLineEdit;
		//---Layout
		
	private:		//C++  Components
		ConnectInformation currentRobotConnectInfo;
		vector<ConnectInformation>	robotListInfo;
};

#endif // CONNECTWIDGET_H
