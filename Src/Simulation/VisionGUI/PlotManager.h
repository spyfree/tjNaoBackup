/**
* @file Simulation/VisionGUI/PlotManager.h
* This Class for Manager Plot.
* @author Wang WenJia
*/
#ifndef PLOTMANAGER_H
#define PLOTMANAGER_H
#include "Tools/Math/Vector2.h"
//#include <QObject>
#include "Module/Representations/BaseProvider.h"
#include "Module/Vision/PlotData.h"


class PlotManager:/*public QObject,*/BaseProvider
{
	//Q_OBJECT
protected:
	PlotManager()
	{
		clear();
	}
	~PlotManager()
	{
		theInstance=0;
	}
private:
	static PlotManager* theInstance; /**< Not Thread Storage mode, one copy globally*/
public:
	/**
	* Get an instance of the PlotManager module,
	* AT MOST ONE INSTANCE IS EXSIT!
	*/
// 	static void InitInstance()
// 	{
// 		if(theInstance == 0)
// 		{
// 			theInstance = new PlotManager();
// 		}
// 	}
	static PlotManager* getInstance()
	{
		if(theInstance==NULL)
		{
			theInstance=new PlotManager();
		}
		return theInstance;
	}
	static void clear()
	{
		if (theInstance)
		{
			theInstance->DrawdataBuffer.DebugFeaturePoint.clear();
			theInstance->DrawdataBuffer.DebugLinesBuffer.clear();
			theInstance->DrawdataBuffer.DebugHARectangleBuffer.clear();
			theInstance->DrawdataBuffer.DebugCircleBuffer.clear();
			theInstance->DrawdataBuffer.DebugBigDotBuff.clear();
			theInstance->DrawdataBuffer.DebugXYHPose.clear();
			theInstance->DrawdataBuffer.DebugSquareBuff.clear();
			theInstance->DrawdataBuffer.DebugFieldTextBuffer.clear();
		}

	};
public:
	PlotData DrawdataBuffer;
public:
	void getObjectToDraw ()
	{
		selfMessageQueue->SearchMyMessage(idObjectDraw,idActuatorThread,idMainWindow,(char*)(&DrawdataBuffer),sizeof(PlotData));
	};

};

#endif