// ObjectToDraw.h: interface for the ObjectToDraw class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __OBJECT_TO_DRAW__
#define __OBJECT_TO_DRAW__

// #include <QtGui>
#include "Tools/Math/Vector2.h"
#include "Module/Vision/PlotData.h"
#include "Module/Representations/BaseProvider.h"

/*
struct DebugPoint 
{
	Vector2<int> point;
	char pencolor;
	char fillColor;
};
struct DebugLine 
{
	Vector2<int> startp;
	Vector2<int> endp;
	int penWidth;
	char penStyle;
	char pencolor;
};

struct DebugRectangle 
{
	Vector2<int> topLeftpoint;
	Vector2<int> bottomRightpoint;
	int penWidth;
	char penStyle;
	char pencolor;
};
struct DebugCircle 
{
	Vector2<int> Center;
	int radius;
	int penWidth;
	char penStyle;
	char pencolor;
};
struct DebugRobot{
	Vector2<int> point;
	double rotation;
	char pencolor;
	char fillcolor;
};
struct DebugSquare{
	Vector2<int> point;
	int sidelength;
	char pencolor;
	char fillcolor;
};
struct DebugText{
	Vector2<int> start;
	QString str;
	int size;
	char color; 
	bool relate;
};
*/
/**
* Class ObjectToDraw
*/
class ObjectToDraw:public BaseProvider
{
protected:
	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	ObjectToDraw():
		MAX_PONITS(2000),
		MAX_LINES(1000),
		MAX_RECTANGLES(50),
		MAX_CIRCLES(100),
		MAX_BIG_DOTS(200),
		MAX_SAMPLES(220),
		MAX_CORNER(100)
	  {
		  clear();
	  };
	~ObjectToDraw()
	{
		theInstance = 0;
	}
private:
	PlotData PlotData_buffer;

public:
	const int MAX_PONITS;
	const int MAX_LINES;
	const int MAX_RECTANGLES;
	const int MAX_CIRCLES;
	const int MAX_BIG_DOTS;
	const int MAX_SAMPLES;
	const int MAX_CORNER;
public:
	/**
	* Get an instance of the PlotData module,
	* AT MOST ONE INSTANCE IS EXSIT!
	*/
	static void InitInstance()
	{
		if(theInstance == 0)
		{
			theInstance = new ObjectToDraw();
		}
	}
	static ObjectToDraw* getInstance()
	{
		if(theInstance==NULL)
		{
			theInstance=new ObjectToDraw();
		}
		return theInstance;
	}
	static void FinishPlot()
	{
		theInstance->setMessageQueue();
	}
	void setMessageQueue()
	{
		//MessageQueue
		selfMessageQueue->SetMessageData(idObjectDraw,idActuatorThread,idMainWindow,sizeof(PlotData),(char*)(&PlotData_buffer));
		clear();
	}
private:
	static ObjectToDraw* theInstance; /**< Not Thread Storage mode, one copy globally*/
	/*
	std::vector< DebugPoint > DebugFeaturePoint;
	std::vector< DebugPoint > DebugBigDotBuff;
	//vector< pair< Vector2<int> , Vector2<int> > > DebugScanLines;
	std::vector< DebugLine > DebugLinesBuffer;
	//vector< pair< Vector2<int> , Vector2<int> > > DebugHARectangle;
	std::vector< DebugRectangle > DebugHARectangleBuffer;
	//void AddDebugPoints(Vector2<int> point);

	std::vector< DebugCircle > DebugCircleBuffer;
	//
	std::vector<DebugRobot> DebugXYHPose;
	std::vector<DebugSquare> DebugSquareBuff;

	std::vector<DebugText> DebugFieldTextBuffer;
	*/
	/**
	* Clear All Drawings' Buffer!!
	*/
public:
	static void clear()
	{
		if (theInstance)
		{
			theInstance->PlotData_buffer.DebugFeaturePoint.clear();
			theInstance->PlotData_buffer.DebugLinesBuffer.clear();
			theInstance->PlotData_buffer.DebugHARectangleBuffer.clear();
			theInstance->PlotData_buffer.DebugCircleBuffer.clear();
			theInstance->PlotData_buffer.DebugBigDotBuff.clear();
			theInstance->PlotData_buffer.DebugXYHPose.clear();
			theInstance->PlotData_buffer.DebugSquareBuff.clear();
			theInstance->PlotData_buffer.DebugFieldTextBuffer.clear();
		}

	};

	/**
	* Add Debug-Drawing  Lines
	* @param x1, y1: startpoint
	* @param x2, y2: endpoint
	*/
	static void AddDebugLines(int x1, int y1, int x2, int y2, int penWidth, char penStyle, char pencolor)
	{
		if (theInstance)
		{
			DebugLine temp;
			temp.startp.x = x1;
			temp.startp.y = y1;
			temp.endp.x = x2;
			temp.endp.y = y2;
			temp.penWidth=penWidth;
			temp.penStyle = penStyle;
			temp.pencolor = pencolor;
			if(theInstance->PlotData_buffer.DebugLinesBuffer.size()<theInstance->MAX_LINES){
				theInstance->PlotData_buffer.DebugLinesBuffer.push_back(temp);
				//DebugScanLines.push_back(topendpoint);
				//DebugScanLines.push_back(bottomendpoint);
			}
		}


	};

	/**
	* Add Debug-Drawing Points
	* @param x1,y1: point
	* @param fillcolor: fill in color of this point!
	*/
	static void AddDebugPoints(int x1, int y1,char penColor, char fillcolor)
	{
		if (theInstance)
		{	
			DebugPoint temp;
			temp.point.x = x1;
			temp.point.y = y1;
			temp.pencolor = penColor;
			temp.fillColor= fillcolor;
			if(theInstance->PlotData_buffer.DebugFeaturePoint.size() < theInstance->MAX_PONITS){
				theInstance->PlotData_buffer.DebugFeaturePoint.push_back(temp);
			}
		}
	};


	/**
	* Add the Horizontal-Aligned Rectangle
	* @param topLeftpointX,topLeftpointY: top Left point of the rectangle
	* @param bottomRightpointX,bottomRightpointY: bottom Right point of the rectangle
	*/
	static void AddDebugHARectangles(int topLeftpointX,int topLeftpointY, int bottomRightpointX, int bottomRightpointY, int penWidth, char penStyle,char pencolor)
	{
		if (theInstance)
		{
			DebugRectangle temp;
			temp.topLeftpoint.x = topLeftpointX;
			temp.topLeftpoint.y = topLeftpointY;
			temp.bottomRightpoint.x = bottomRightpointX;
			temp.bottomRightpoint.y = bottomRightpointY;
			temp.penWidth = penWidth;
			temp.penStyle =penStyle;
			temp.pencolor = pencolor;
			if(theInstance->PlotData_buffer.DebugHARectangleBuffer.size()<theInstance->MAX_RECTANGLES){
				theInstance->PlotData_buffer.DebugHARectangleBuffer.push_back(temp);
			}
		}

	};

	/**
	* Add a Circle
	* @param Center_x, Center_y: Center point of the circle
	* @param radius: radius of the circle
	* @param pencolor: color of this circle
	*/
	static void AddDebugCircles(int Center_x, int Center_y, int radius,int penWidth,char penStyle, char pencolor)
	{
		if (theInstance)
		{
			DebugCircle temp;
			temp.Center.x = Center_x;
			temp.Center.y = Center_y;
			temp.radius = radius;
			temp.penWidth = penWidth;
			temp.penStyle = penStyle;
			temp.pencolor = pencolor;
			if(theInstance->PlotData_buffer.DebugCircleBuffer.size() < theInstance->MAX_CIRCLES)
			{
				theInstance->PlotData_buffer.DebugCircleBuffer.push_back(temp);	
			}
		}

	};
	
	/**
	* Add a Big Dot
	*
	*/
	static void AddDebugBigDot(int x, int y, char penColor, char fillColor)
	{
		if (theInstance)
		{
			DebugPoint temp;
			temp.point.x = x;
			temp.point.y = y;
			temp.pencolor = penColor;
			temp.fillColor = fillColor;
			if(theInstance->PlotData_buffer.DebugBigDotBuff.size() < theInstance->MAX_BIG_DOTS){
				theInstance->PlotData_buffer.DebugBigDotBuff.push_back(temp);
			}

		}

	};
	/**
	* Add a string of text to show on the Field
	* @param: x,y position of the field coordinate
	*/
	static void AddDebugFieldText(std::string str, int x, int y, int size, char penColor, bool relate)
	{
		if (theInstance)
		{
			DebugText temp;
			temp.start.x = x;
			temp.start.y = y;
			temp.str = str;
			temp.size = size;
			temp.color = penColor;
			temp.relate = relate;
			theInstance->PlotData_buffer.DebugFieldTextBuffer.push_back(temp);
		}

	};
    /**
	* Add a Sample Pose 
	*
	*/

	static void AddDebugXYH(int x, int y,double rot,char pencolor, char fillcolor)
	{
		if (theInstance)
		{
			DebugRobot temp;
			temp.point.x = x;
			temp.point.y = y;
			temp.rotation = rot;
			temp.pencolor = pencolor;
			temp.fillcolor= fillcolor;
			if(theInstance->PlotData_buffer.DebugXYHPose.size() < theInstance->MAX_SAMPLES){
				theInstance->PlotData_buffer.DebugXYHPose.push_back(temp);
			}
		}

	};
	static void AddDebugSquare(int x, int y, int sidelength, char pencolor, char fillcolor)
	{
		if (theInstance)
		{
			DebugSquare temp;
			temp.point.x = x;
			temp.point.y = y;
			temp.sidelength = sidelength;
			temp.pencolor = pencolor;
			temp.fillcolor= fillcolor;
			if(theInstance->PlotData_buffer.DebugSquareBuff.size() < theInstance->MAX_CORNER ){
				theInstance->PlotData_buffer.DebugSquareBuff.push_back(temp);
			}
		}

	}
;

    
};

#endif // !defined(AFX_OBJECTTODRAW_H__5BAC2708_35E9_4E3F_9181_92C5C4D671E1__INCLUDED_)
