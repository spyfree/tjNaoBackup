/**
* @file Module/Motion/ObjectDetection.h
* This file declares the base class ObjectDetection.
*/

#ifndef __ObjectDetection_h_
#define __ObjectDetection_h_

#include "ProxyContainer.h"
#include "Tools/TJAssign.h"
#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <list>

using namespace std;
using namespace boost;
using namespace boost::python;


class ObjectDetection
{
public:
	ObjectDetection();
	~ObjectDetection();
	void updateSonarsData();
	void walkBalanceCheck();
    bool isObjectThere();
	
private:

	void updateBodyAnlge();
	void updateBumperData();

private:
	ProxyContainer myProxyContainer;
	std::list<int> stateList;



public:
	class Parameters:public paramAssign
	{
	public:
		void massAssign();
	public:
		int ULimt;
		int DLimt;
		int UnableLimt;

		double angleXmax;
		double angleXmin;
		double angleYmax;
		double angleYmin;

		double newAngleXmax;
		double newAngleXmin;
		double newAngleYmax;
		double newAngleYmin;

		double leftSonarData;
		double rightSonarData;
		double angleX;
		double angleY;

		double rightBumper;
		double leftBumper;
	};
	Parameters params;
};

//void c_init_ObjectDetection();
boost::python::list getObjectState();

#endif
