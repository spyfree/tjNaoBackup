#ifndef PaintMethods_H
#define PaintMethods_H

#include "Tools/DrawDebugging/DrawBuffer.h"

#define LINE(startx,starty,endx,endy,penWidth, penStyle, penColor)\
	DrawBuffer::getInstance()->line((int)startx,(int)starty,(int)endx,(int)endy,(int)penWidth,(LocDrawings::PenStyle)penStyle,(ColorRGBA)penColor)

#define DOT(x,y,penColor,fillColor)\
	DrawBuffer::getInstance()->dot((int)x,(int)y,(ColorRGBA)penColor,(ColorRGBA)fillColor)

#define LARGE_DOT(x,y,penColor,fillColor)\
	DrawBuffer::getInstance()->largeDot((int)x,(int)y,(ColorRGBA)penColor,(ColorRGBA)fillColor)

#define ARROW(start,end,penWidth,penStyle,penColor)\
	DrawBuffer::getInstance()->arrow((Vector2<double>)start,(Vector2<double>)end,(int)penWidth,(LocDrawings::PenStyle)penStyle,(ColorRGBA)penColor)

#define ELLIPSE(x,y,radiusX,radiusY,rotation,fillStyle,fillColor)\
	DrawBuffer::getInstance()->ellipse((int)x,(int)y,(int)radiusX,(int)radiusY,double(rotation),(LocDrawings::FillStyle)fillStyle,(ColorRGBA)fillColor)

/** 
* A macro that sends a circle
* @param center_x The x coordinate of the center of the circle
* @param center_y The y coordinate of the center of the circle
* @param radius The radius of the circle
* @param penWidth The width of the arc of the circle
* @param penStyle The pen style of the arc of the circle (Drawings::PenStyle)
* @param penColor The color of the arc of the circle 
* @param fillStyle The fill style of the circle
* @param fillColor The fill color of the circle 
*/
#define CIRCLE(center_x, center_y, radius, penWidth, penStyle, penColor, fillStyle, fillColor)\
	DrawBuffer::getInstance()->circle((int)center_x,(int)center_y,(int)radius,(int)penWidth,(LocDrawings::PenStyle)penStyle,(ColorRGBA)penColor,\
	(LocDrawings::FillStyle)fillStyle,(ColorRGBA)fillColor)

/** 
* A macro that sends a polygon
* @param id A drawing id (Drawings::FieldDrawing or Drawings::ImageDrawing)
* @param numberOfPoints The number the points of the polygon
* @param points A list which contains the points of the polygon
* @param penWidth The width of the pen
* @param penStyle The pen style of the arc of the circle (Drawings::PenStyle)
* @param penColor The color of the arc of the circle 
* @param fillStyle The fill style of the polygon
* @param fillColor The fill color of the polygon 
*/
#define POLYGON(numberOfPoints, points, penWidth, penStyle, penColor, fillStyle, fillColor) \
	for(int i = 0; i < numberOfPoints-1; ++i) \
	LINE(points[i].x,points[i].y,points[i+1].x,points[i+1].y,penWidth, penStyle, penColor);\
	LINE(points[numberOfPoints-1].x,points[numberOfPoints-1].y,points[0].x,points[0].y,penWidth, penStyle, penColor)

/** 
* A macro that sends an rectangle
* @param x1,y1,x2,y2 The coordinates of 2 opposite corners
* @param penWidth The line width of the rectangle
* @param penStyle The line style, e.g. dotted
* @param penColor The color of the quadrangle
*/
#define RECTANGLE(x1,y1,x2,y2, penWidth, penStyle, penColor)\
	LINE(x1, y1, x1, y2, penWidth, penStyle, penColor); \
	LINE(x1, y2, x2, y2, penWidth, penStyle, penColor); \
	LINE(x2, y2, x2, y1, penWidth, penStyle, penColor); \
	LINE(x2, y1, x1, y1, penWidth, penStyle, penColor) 


/** 
* A macro that sends an quadrangle
* @param x1,y1,x2,y2,x3,y3,x4,y4 The coordinates of the 4 quadrangle vertices
* @param penWidth The line width of the quadrangle
* @param penStyle The line style, e.g. dotted
* @param penColor The color of the quadrangle
*/
#define QUADRANGLE(x1,y1,x2,y2,x3,y3,x4,y4, penWidth, penStyle, penColor)\
	LINE(x1,y1,x2,y2,penWidth, penStyle, penColor);\
	LINE(x2,y2,x3,y3,penWidth, penStyle, penColor);\
	LINE(x3,y3,x4,y4,penWidth, penStyle, penColor);\
	LINE(x4,y4,x1,y1,penWidth, penStyle, penColor)

/** 
* A macro that sends a cross
* @param x,y The center of the cross
* @param size Half of the height of the rectangle enclosing the cross
* @param penWidth The line width of the rectangle
* @param penStyle The line style, e.g. dotted
* @param penColor The color of the quadrangle
*/
#define CROSS(x, y, size, penWidth, penStyle, penColor) \
	LINE(x+size, y+size, x-size, y-size, penWidth, penStyle, penColor); \
	LINE(x+size, y-size, x-size, y+size, penWidth, penStyle, penColor)

#endif
