#ifndef SAPLAYERWIDGET_H
#define SAPLAYERWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include <QMap>
#include <QVariant>
#include <QVector>
//TJArkNaoPro_New\Config\SpecActions
//system("start SpecActions/Stand.act");
#include "Simulation/saTools/jointsData.h"
//@todo:可以考虑用QMap			//QMap<int, QVariant> itemOfRoot;
class QDirModel;
class QComboBox;
class QPushButton;
class QTextEdit;
class QTreeView;
class QStandardItemModel;
class QSpinBox;
class QGroupBox;
//=========
class SaPlayerWidget : public QWidget
{
	Q_OBJECT

		signals:
			void saPlayerCallSaToolsSignal(int playType,int curStep,QVector<JointsData> comInfo);
	
		private slots:
			void currentFileNumChange(int);
			void currentStepChange(int);
			void startStepChange(int);
			void endStepChange(int);
			void recordNowJointsDataButton();
			void freshDocumentFilesButton();
			void openSaConfigFileButton();
			void openFloderButton();
			void playCurrentStepButton();
			void playAllStepsButton();
			void playRangeStepsButton();
			void activeButton();

public:
	SaPlayerWidget(QWidget *parent );

private:
	void createDirModel();
	void createFileList();
	void createInfoDisplayScreen();
	void createControlPlane();
	void createPlayerPlane();
	void readFileData();
	void processFileData();
	void addItemToModel(QAbstractItemModel *, QString &, QVariant);
	void setItemOfModel(QAbstractItemModel *,int , QVariant );
	void freshDocumentFile();
	void freshCurrentFile();
	void freshModelData();
	void freshCurrentStep();

private:
	QDirModel *fileDirModel;
	QComboBox * saFilesComboBox;
	//-------Information Plane
	QTreeView * infoDisplayScreenTreeView;
	QStandardItemModel * infoDisplayModel;
	//------Control Plane
	QPushButton * recordNowJointsDataPushButton;
	QPushButton * freshDocumentFilesPushButton;
	QPushButton * openSaConfigFilePushButton;
	QPushButton * openContainerPushButton;
	QSpinBox * currentStepSpinBox;
	QPushButton * activePushButton;
	//------Player Plane
	QPushButton * playCurrentStepPushButton;
	QPushButton * playAllStepsPushButton;
	QPushButton * playRangeStepsPushButton;
	QSpinBox * startStepSpinBox;
	QSpinBox * endStepSpinBox;
	QGroupBox * playPlaneGroupBox;
public:
	int startStep;
	int endStep;

private:
	QModelIndex saConfigPathModelIndex;
	QVector<QString> namesOfFile;
	int currentFileNum;
	QVector<QString> fileDataString;

private:		//fileInfo
	QString saConfigFilePathString;
	int numOfStepToAct;
	QVector<JointsData> jointValueVector;
	int currentStep;

//------以下成员为暂时使用
private:
	//QTextEdit * cfgTextEdit;

//------Enum
public:
	enum playType
	{
		playSingleStep = 0,
		playAllStep ,
		playRangeStep
	};
};

#endif	//SAPLAYERWIDGET_H