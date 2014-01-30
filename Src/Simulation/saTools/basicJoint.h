#ifndef BASICJOINT_H
#define BASICJOINT_H

#include <QWidget>

class QGroupBox;
class QSlider;
class QSpinBox;
class QCheckBox;

//===============		@todo: 改成doubleSpin，并且增加一个设置精度的spinBox
class	BasicJoint : public QWidget
{
	Q_OBJECT

signals:
	void angleChanged(int portId, int angleValue);
	void stiffnessChanged(int portId,int flag);

	public slots:
		void angleChangedSignalEmitter(int angleValue);
	
	private slots:
		void setJointStiffness( int stiffness);
//--------------
public:
	BasicJoint(const QString & jointName, int id, int minJ, int maxJ,QWidget *parent);		//construct_2
	void setAngleValue( int );
	void setPower(bool);

private:
	void setJointRange();

public:
	QGroupBox * basicJointGroupBox;
	int  myId;
	//--
	double jointStiffness;
	//--static	variable
	static int numOfBasicJoint;

private:
	QSlider * basicJointSlider;
	QSpinBox * basicJointSpinBox;
	QCheckBox * basicJointPowerCheckBox;

	//--
	int maxJoint;
	int minJoint;
};

#endif	//BASICJOINT_H