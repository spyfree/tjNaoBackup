#ifndef DrawBuffer_H
#define DrawBuffer_H

/**
*该文件用来存入各种图形
*/

#include "Tools/Math/Vector2.h"
#include "Tools/DrawDebugging/LocMessageData.h"
#include "Platform/SystemCall.h"
#include <vector>

/**
* Class DrawBuffer
*/
class DrawBuffer
{
public:
	/** Default constructor. */
	DrawBuffer();

	/** Destructor. */
	~DrawBuffer();
	
	static DrawBuffer* getInstance()
	{
		if(theInstance==NULL)
		{
			theInstance=new DrawBuffer();
		}
		return theInstance;
	}

	/**The function clear the vector of drawings. */
	void clear();

	/**
	* Adds an arrow to the debug drawing
	* @param start The starting point of the arrow 
	* @param end The target of the arrow
	* @param penStyle Specifies the penStyle of the arrow.
	* @param width The width
	* @param color The color
	*/
	void arrow( 
		Vector2<double> start,
		Vector2<double> end,
		int width,
		LocDrawings::PenStyle penStyle,
		ColorRGBA color
		);

	/**
	* Adds a line to the debug drawing.
	* @param xStart Specifies the x-coordinate of the startpoint for the line. 
	* @param yStart Specifies the y-coordinate of the startpoint for the line.
	* @param xEnd Specifies the x-coordinate of the endpoint for the line.
	* @param yEnd Specifies the y-coordinate of the endpoint for the line.
	* @param penStyle Specifies the penStyle of the Line.
	* @param width Specifies the width of the line.
	* @param color Specifies the color of the line.
	*/
	void line(
		int xStart, 
		int yStart, 
		int xEnd,
		int yEnd,
		int width,
		LocDrawings::PenStyle penStyle,
		ColorRGBA color
		);

	/**
	* Adds a line to the debug drawing. The line is a solid black line with width 1.
	* @param xStart Specifies the x-coordinate of the startpoint for the line. 
	* @param yStart Specifies the y-coordinate of the startpoint for the line.
	* @param xEnd Specifies the x-coordinate of the endpoint for the line.
	* @param yEnd Specifies the y-coordinate of the endpoint for the line.
	*/
	void line(int xStart, int yStart, int xEnd, int yEnd);

	/**
	* Adds a polygon to the debug drawing.
	* @param points Points to an array of points that specifies the vertices of the polygon. Each point in the array is a Point.
	* @param nCount Specifies the number of vertices in the array.
	* @param width Specifies the width of the border.
	* @param penStyle Specifies the penStyle of the border.
	* @param penColor Specifies the color of the border.
	* @param fillStyle Specifies the fillStyle of the polygon.
	* @param fillColor Specifies the color of the polygon.
	*/
	void polygon(
		const Vector2<int>* points,
		int nCount,
		int width,
		LocDrawings::PenStyle penStyle,
		ColorRGBA penColor,
		LocDrawings::FillStyle fillStyle,
		ColorRGBA fillColor
		);

	/**
	* Adds a filled rectangle to the debug drawing.
	* The rectangle extends up to, but does not include, the right and bottom coordinates.
	* @param left Specifies the x-coordinate of the upper-left corner of the rectangle.
	* @param right Specifies the x-coordinate of the lower-right corner of the rectangle.
	* @param top Specifies the y-coordinate of the upper-left corner of the rectangle.
	* @param bottom Specifies the y-coordinate of the lower-right corner of the rectangle.
	* @param width Specifies the width of  the border.
	* @param penStyle Specifies the penStyle of the border.
	* @param penColor Specifies the color of the border.
	* @param fillStyle Specifies the fillStyle of the ellipse.
	* @param fillColor Specifies the color of the ellipse.
	*/  
	void rectangle(
		int left, int right, int top, int bottom, 
		int width,
		LocDrawings::PenStyle penStyle,
		ColorRGBA penColor,
		LocDrawings::FillStyle fillStyle,
		ColorRGBA fillColor
		);
	
	void ellipse(
		int x,int y,int radiusX,int radiusY,
		double rotation,
		LocDrawings::FillStyle fillstyle,
		ColorRGBA fillColor);

	void circle(
		int centerX,int centerY,int radius,
		int penWidth, 
		LocDrawings::PenStyle penStyle, 
		ColorRGBA penColor, 
		LocDrawings::FillStyle fillStyle, 
		ColorRGBA fillColor);
	/**
	* Adds a filled square to the debug drawing. The border of the square is a solid line with width 0.
	* The square is a 3x3 square.
	* @param x Specifies the center of the dot.
	* @param y Specifies the center of the dot.
	* @param penColor Specifies the penColor of the dot.
	* @param fillColor Specifies the fillColor of the dot.
	*/  
	void dot(int x, int y, ColorRGBA penColor, ColorRGBA fillColor);

	/**
	* Adds a filled square to the debug drawing. The border of the square is a solid line with width 5.
	* The square is a 10x10 square.
	* @param x Specifies the center of the dot.
	* @param y Specifies the center of the dot.
	* @param penColor Specifies the penColor of the dot.
	* @param fillColor Specifies the fillColor of the dot.
	*/  
	void largeDot(int x, int y, ColorRGBA penColor, ColorRGBA fillColor);

	/**
	* Adds a filled midsize dot to the debug drawing. The border of the square is a solid line with width 1.
	* @param x Specifies the center of the dot.
	* @param y Specifies the center of the dot.
	* @param penColor Specifies the penColor of the dot.
	* @param fillColor Specifies the fillColor of the dot.
	*/  
	void midDot(int x, int y, ColorRGBA penColor, ColorRGBA fillColor);

	void getLocData();

	/** the kind of the drawing */
	int typeOfDrawing;

	/** Identifier of the process to which this drawing belongs. 'm' (Motion), 'c' (Cognition) etc. */
	char type;

	unsigned timeStamp; /**< The time when this drawing was created. */
	char processIdentifier; /**< The process this drawing was received from. */

	std::vector<Line> lines;
	std::vector<Polygons> polygons;
	std::vector<Ellipses> ellipses;

private:
	PROCESS_WIDE_STORAGE_STATIC DrawBuffer* theInstance; /**< The only instance of this module. */

};

#endif // !defined(AFX_OBJECTTODRAW_H__5BAC2708_35E9_4E3F_9181_92C5C4D671E1__INCLUDED_)
