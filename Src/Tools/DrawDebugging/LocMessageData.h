#ifndef LocMessageData_H
#define LocMessageData_H

/**
*LocMessageData���ݰ���ͨ��MessageQueue�ɶ�λ�̴߳������߳�
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