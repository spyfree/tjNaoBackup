#ifndef LocMessageData_H
#define LocMessageData_H

/**
*LocMessageData数据包是通过MessageQueue由定位线程传给主线程
*/

#include "Tools/DrawDebugging/LocDrawings.h"

class LocMessageData
{
public:
	LocMessageData(){}
	~LocMessageData(){}

	std::vector<Line> lines;
	std::vector<Polygons> polygons;
	std::vector<Ellipses> ellipses;
	
};

#endif