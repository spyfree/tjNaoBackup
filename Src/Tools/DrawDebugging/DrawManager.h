#ifndef DrawManager_H
#define DrawManager_H

/**
*该文件用来绘制粒子
*/

#include "Tools/DrawDebugging/LocMessageData.h"
#include "Tools/Math/Vector2.h"
class QPainter;
class QBrush;
class QPen;

class DrawManager
{
public:
	DrawManager();
	~DrawManager();
	/** 
	* Paints a DebugDrawings to a QPainter. 
	* @param painter The graphics context the DebugDrawing is painted to.
	* @param debugDrawing The DebugDrawing to paint.
	* @param baseTrans A basic transformation.
	*/
	void paintDebugDrawing(QPainter& painter, const LocMessageData& theLocMessageData);
	void SetPenStyle(QPen& pen, char penStyle);

};

#endif