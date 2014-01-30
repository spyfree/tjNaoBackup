#include "SensorDataModel.h"
#include "../jointsData.h"
#include "sensorsData.h"
#include <QtGui>

//==========Functions
SensorDataModel::SensorDataModel()
{
	//=======JointSensorModel
	JointsSensorModel = new QStandardItemModel(0,3);

	JointsSensorModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Sensor Name"));
	JointsSensorModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Angle"));
	JointsSensorModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Stiffness"));

	for(int i=0;i<JointsData::numOfJoints;i++)
	{	
		addJointSensorData(JointsSensorModel,
			QObject::tr( JointsData::getJointName(JointsData::Joint(JointsData::numOfJoints-i-1)) ),0,0);
	}

	//=======NaoSensorModel
	NaoSensorModel = new QStandardItemModel(0,3);
	
	NaoSensorModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Sensor Name"));
	NaoSensorModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Value"));
	NaoSensorModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Unit"));

	for (int i=0;i<SensorsData::numOfSensors;i++)
	{
		addNaoSensorData(NaoSensorModel,
			QObject::tr( SensorsData::getSensorName(SensorsData::Sensor(SensorsData::numOfSensors-i-1)) ),0,
			QObject::tr( SensorsData::getSensorUnit(SensorsData::Sensor(SensorsData::numOfSensors-i-1))) );//unit
	}
	//NaoSensorModel->sort(0,Qt::AscendingOrder);
}

SensorDataModel::~SensorDataModel()
{
	delete JointsSensorModel;
	delete NaoSensorModel;
}

void SensorDataModel::addJointSensorData(QAbstractItemModel *model,  QString &sensorName,
							 QVariant angle, QVariant stiffness)
{
	model->insertRow(0); 
	model->setData(model->index(0, 0), sensorName );
	model->setData(model->index(0,1), angle);
	model->setData(model->index(0,2), stiffness);
}

void SensorDataModel::addNaoSensorData(QAbstractItemModel *model, QString &sensorName, QVariant value,QString & sensorUnit)
{
	model->insertRow(0); 
	model->setData(model->index(0, 0), sensorName );
	model->setData(model->index(0,1), value);
	model->setData(model->index(0,2), sensorUnit);
}

void SensorDataModel::setJointSensorDataItem(int row, int col, QVariant value)
{
	JointsSensorModel->setData(JointsSensorModel->index(row,col),		value);
}

void SensorDataModel::setNaoSensorDataItem(int row,QVariant value)
{
	NaoSensorModel->setData(NaoSensorModel->index(row,1),	value);
}