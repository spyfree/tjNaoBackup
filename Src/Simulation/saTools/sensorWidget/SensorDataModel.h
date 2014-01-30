#ifndef SENSORDATAMODEL_H
#define SENSORDATAMODEL_H

class QStandardItemModel;
class QAbstractItemModel;
class QString;
class QVariant;
//======
class SensorDataModel
{
	public:
		SensorDataModel();
		~SensorDataModel();

	private:
		void addJointSensorData(QAbstractItemModel *model,  QString &sensorName, QVariant angle, QVariant stiffness);
		void addNaoSensorData(QAbstractItemModel *model,  QString &sensorName, QVariant value,QString & sensorUnit);
	
	public:
		void setJointSensorDataItem(int row, int col, QVariant value);
		void setNaoSensorDataItem(int row,QVariant value);

	public:
		QStandardItemModel * JointsSensorModel;
		QStandardItemModel * NaoSensorModel;
};

#endif	//SENSORDATAMODEL_H