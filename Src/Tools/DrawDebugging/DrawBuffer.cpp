#include "DrawBuffer.h"
#include "Platform/SystemCall.h"
#include <iostream>

PROCESS_WIDE_STORAGE DrawBuffer* DrawBuffer::theInstance = 0;
DrawBuffer::DrawBuffer()
{
	DrawBuffer::theInstance = this;
	timeStamp = SystemCall::getCurrentSystemTime();
}


DrawBuffer::~DrawBuffer()
{
	clear();
	DrawBuffer::theInstance = NULL;
}

void DrawBuffer::clear()
{
	lines.clear();
	polygons.clear();
	ellipses.clear();
}


void DrawBuffer::arrow(Vector2<double> start, Vector2<double> end, int width,
						 LocDrawings::PenStyle penStyle, ColorRGBA color)
{
	Vector2<double> startToEnd((end.x - start.x)/4, (end.y - start.y)/4);
	Vector2<double> perpendicular(startToEnd.y, -1*startToEnd.x);
	// start to endpoint
	line((int)start.x, (int)start.y, (int)(end.x), (int)(end.y),width,penStyle,color);
	// endpoint to left and right
	line((int)(end.x), (int)(end.y), (int)(end.x - startToEnd.x + perpendicular.x), (int)(end.y - startToEnd.y + perpendicular.y), width,penStyle, color);
	line((int)(end.x), (int)(end.y), (int)(end.x - startToEnd.x - perpendicular.x), (int)(end.y - startToEnd.y - perpendicular.y), width, penStyle, color);
}


void DrawBuffer::line
(
 int xStart,
 int yStart,
 int xEnd,
 int yEnd,
 int width,
 LocDrawings::PenStyle penStyle,
 ColorRGBA penColor
 )
{
	Line element;
	element.xStart = xStart;
	element.yStart = yStart;
	element.xEnd = xEnd;
	element.yEnd = yEnd;
	element.penStyle = penStyle;
	element.width = width;
	element.penColor = penColor;
	lines.push_back(element);
}

void DrawBuffer::line
(
 int xStart,
 int yStart,
 int xEnd,
 int yEnd
 )
{
	line(xStart, yStart, xEnd, yEnd, 10, LocDrawings::ps_solid, ColorRGBA(0,0,0));
}

void DrawBuffer::polygon
(
 const Vector2<int>* points,
 int nCount,
 int width,
 LocDrawings::PenStyle penStyle,
 ColorRGBA penColor,
 LocDrawings::FillStyle fillStyle,
 ColorRGBA fillColor
 )
{

	//ASSERT(nCount <= MAX_NUMBER_OF_POINTS);
	Polygons element;
	memcpy(element.points, points, sizeof(element.points[0]) * nCount);
	element.nCount = nCount;
	element.width = width;
	element.penStyle = penStyle;
	element.penColor = penColor;
	element.fillStyle = fillStyle;
	element.fillColor = fillColor;
	polygons.push_back(element);


}


void DrawBuffer::dot(int x, int y, ColorRGBA penColor, ColorRGBA fillColor)
{
	Vector2<int> points[4];
	points[0].x = x - 1;
	points[0].y = y - 1;
	points[1].x = x + 1;
	points[1].y = y - 1;
	points[2].x = x + 1;
	points[2].y = y + 1;
	points[3].x = x - 1;
	points[3].y = y + 1;
	polygon(
		points,
		4,
		0,
		LocDrawings::ps_solid,
		penColor,
		LocDrawings::bs_solid,
		fillColor);
}

void DrawBuffer::largeDot(int x, int y, ColorRGBA penColor, ColorRGBA fillColor)
{
	Vector2<int> points[4];
	points[0].x = x - 25;
	points[0].y = y - 25;
	points[1].x = x + 25;
	points[1].y = y - 25;
	points[2].x = x + 25;
	points[2].y = y + 25;
	points[3].x = x - 25;
	points[3].y = y + 25;
	polygon(
		points,
		4,
		3,
		LocDrawings::ps_solid,
		penColor,
		LocDrawings::bs_solid,
		fillColor);
}

void DrawBuffer::midDot(int x, int y, ColorRGBA penColor, ColorRGBA fillColor)
{
	Vector2<int> points[4];
	points[0].x = x - 2;
	points[0].y = y - 2;
	points[1].x = x + 2;
	points[1].y = y - 2;
	points[2].x = x + 2;
	points[2].y = y + 2;
	points[3].x = x - 2;
	points[3].y = y + 2;
	polygon(
		points,
		4,
		10,
		LocDrawings::ps_solid,
		penColor,
		LocDrawings::bs_solid,
		fillColor);
}

void DrawBuffer::rectangle
(
 int left,
 int right,
 int top,
 int bottom,
 int width,
 LocDrawings::PenStyle penStyle,
 ColorRGBA penColor,
 LocDrawings::FillStyle fillStyle,
 ColorRGBA fillColor
 )
{
	Vector2<int> points[4];
	points[0].x = left;
	points[0].y = top;
	points[1].x = right;
	points[1].y = top;
	points[2].x = right;
	points[2].y = bottom;
	points[3].x = left;
	points[3].y = bottom;
	polygon(
		points,
		4,
		width,
		penStyle,
		penColor,
		fillStyle,
		fillColor);
}

void DrawBuffer::ellipse(int x,int y,int radiusX,int radiusY,
			 double rotation,
			 LocDrawings::FillStyle fillstyle,
			 ColorRGBA fillColor)
{
	Ellipses element;
	element.x = x;
	element.y = y;
	element.radiusX = radiusX;
	element.radiusY = radiusY;
	element.rotation = rotation;
	element.fillStyle = fillstyle;
	element.fillColor = fillColor;
	ellipses.push_back(element);
}

void DrawBuffer::circle(int centerX, int centerY, int radius, 
						int penWidth, 
						LocDrawings::PenStyle penStyle, 
						ColorRGBA penColor, 
						LocDrawings::FillStyle fillStyle, 
						ColorRGBA fillColor)
{
	Ellipses element;
	element.x = centerX;
	element.y = centerY;
	element.radiusX = radius;
	element.radiusY = radius;
	element.rotation = 0;
	element.fillStyle = fillStyle;
	element.fillColor = fillColor;
	element.penColor = penColor;
	element.penStyle = penStyle;
	ellipses.push_back(element);

}



