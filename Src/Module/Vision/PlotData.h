/**
* @file Module/Vision/PlotData.h
* This Class for Manager Plot.
* @author Wang WenJia
*/
#ifndef _PLOTDATA_
#define _PLOTDATA_
#include "Tools/Math/Vector2.h"

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
	std::string str;
	int size;
	char color; 
	bool relate;
};
class PlotData  
{
public:
	PlotData(){	};
	~PlotData(){};

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
};
#endif 