// DebugDrawings.h: interface for the ScanVision class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __DEBUG_DRAWINGS__
#define __DEBUG_DRAWINGS__

 #ifdef NAO_DEBUG
 #include <Module/Vision/ObjectToDraw.h>
// #include <Simulation/VisionGUI/Gloabaltodraw.h>
 #endif

class Drawings
{
public:
  /** The pen style that is used for basic shapes*/
  enum PenStyle
  {
    ps_NoPen,ps_SolidLine, ps_DashLine, ps_DotLine, ps_DashDotLine,ps_DashDotDotLine,ps_CustomDashLine
  };
  /** The color that is used for basic shapes*/
  enum Color
  {
	  color0,color1,black,white,darkGray,gray,lightGray,red,green,blue,cyan,magenta,yellow,darkRed,darkGreen,darkBlue,darkCyan,darkMagenta,darkYellow,transparent
  };
};

#ifndef NAO_DEBUG

//#define NDECLARE_DEBUGDRAWING( id, type, description) /**/
#define DRAW_CIRCLE(center_x, center_y, radius,penWidth, penStyle, penColor) /**/
#define DRAW_DOT(x, y,penColor, fillColor) /**/
#define DRAW_LINE(x1, y1, x2, y2,penWidth, penStyle, penColor) /**/
#define DRAW_HARECTANGLE(x1,y1,x2,y2,penWidth, penStyle,  penColor) /**/
#define DRAW_BIG_DOT(x, y,penColor, fillColor) /**/
#define DRAW_XYH_POSE(x, y, rot, pencolor, fillcolor) /**/
#define DRAW_SQUARE(x, y, pencolor, fillcolor) /**/
#define  OBJECT_TO_DRAW_CLEAR()/**/
#define FIELD_CIRCLE(center_x, center_y, radius,penWidth, penStyle, penColor) /**/
#define FIELD_DOT(x, y,penColor, fillColor) /**/
#define FIELD_LINE(x1, y1, x2, y2,penWidth, penStyle, penColor) /**/
//#define FIELD_HARECTANGLE(x1,y1,x2,y2,penWidth, penStyle,  penColor) /**/
#define FIELD_BIG_DOT(x, y,penColor, fillColor) /**/
#define DRAW_CROSS(x,y,size,penWidth,penStyle,penColor)
//#define NLARGE_DOT(id, x, y, penColor, fillColor) /**/
//#define NARROW(id, x1, y1, x2, y2, penWidth, penStyle, penColor) /**/
//#define NCOLORED_LINE(id, x1, y1, x2, y2, penWidth, penStyle, penColorR, penColorG, penColorB) /**/
//#define NPOSE_2D_SAMPLE(id, p, color) /**/
//#define NOCTANGLE(id, x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,x7,y7,x8,y8, color, fill) /**/
//#define NQUADRANGLE(id, x1,y1,x2,y2,x3,y3,x4,y4, penWidth, penStyle, penColor) /**/
//#define NCROSS(id, x, y, size, penWidth, penStyle, penColor) /**/
//#define NCOMPLEX_DRAWING(id,expression) /**/
//#define NDEBUG_DRAWING_FINISHED(id) /**/

#else//WIN32
/**
* A macro that sends a dot
* @param x The x coordinate of the center of the box
* @param y The y coordinate of the center of the box
* @param penColor The color of the border of the dot (Drawings::Color)
* @param fillColor The color of the dot (Drawings::Color)
*/
#define DRAW_DOT(x, y,penColor, fillColor) \
	ObjectToDraw::getInstance()->AddDebugPoints((int)x, (int)y,(char)penColor, (char)fillColor);

/*A macro to draw cross*/
#define DRAW_CROSS(x,y,size,penWidth,penStyle,penColor)\
	ObjectToDraw::getInstance()->AddDebugLines((int)(x+size), (int)(y+size), (int)(x-size), (int)(y-size),(int)penWidth,(char)penStyle, (char)penColor);\
	ObjectToDraw::getInstance()->AddDebugLines((int)(x+size), (int)(y-size), (int)(x-size), (int)(y+size),(int)penWidth,(char)penStyle, (char)penColor);

/**
* A macro that sends a line
* @param x1 The x coordinate of the starting point.
* @param y1 The y coordinate of the starting point.
* @param x2 The x coordinate of the end point.
* @param y2 The y coordinate of the end point.
* @param penWidth The width of the arc of the circle
* @param penStyle The pen style of the arc of the circle (Drawings::PenStyle)
* @param penColor The color of the line (Drawings::Color)
*/
#define DRAW_LINE(x1, y1, x2, y2,penWidth, penStyle, penColor) \
	ObjectToDraw::getInstance()->AddDebugLines((int)x1, (int)y1, (int)x2, (int)y2,(int)penWidth,(char)penStyle, (char)penColor);

/**
* A macro that sends an Horizontal-Aligned Rectangle
* @param x1,y1,x2,y2 The coordinates of 2 opposite corners
* @param penWidth The width of the arc of the circle
* @param penStyle The pen style of the arc of the circle (Drawings::PenStyle)
* @param penColor The color of the quadrangle
*/
#define DRAW_HARECTANGLE(x1,y1,x2,y2,penWidth, penStyle,  penColor) \
	ObjectToDraw::getInstance()->AddDebugHARectangles((int)x1, (int)y1, (int)x2, (int)y2, (int)penWidth, (char)penStyle, (char)penColor);

/**
* A macro that sends a circle
* @param center_x The x coordinate of the center of the circle
* @param center_y The y coordinate of the center of the circle
* @param radius The radius of the circle
* @param penWidth The width of the arc of the circle
* @param penStyle The pen style of the arc of the circle (Drawings::PenStyle)
* @param penColor The color of the arc of the circle (Drawings::Color)
*/
#define DRAW_CIRCLE(center_x, center_y, radius,penWidth, penStyle, penColor) \
	ObjectToDraw::getInstance()->AddDebugCircles((int)center_x, (int)center_y, (int)radius,(int)penWidth, (char)penStyle, (char)penColor);

/**
* A macro that sends a big point
* @param x, y: coordinate of the dot
* @param penColor The color of the border of the dot (Drawings::Color)
* @param fillColor: Color of the dot
*/
#define DRAW_BIG_DOT(x, y,penColor, fillColor) \
	ObjectToDraw::getInstance()->AddDebugBigDot((int)x, (int)y, (char)penColor, (char)fillColor);

/************************************************************************/
/* to clear object_to_draw                                                                     */
/************************************************************************/

#define  OBJECT_TO_DRAW_CLEAR()\
	ObjectToDraw::getInstance()->clear();
//////////////////////////////////////////////////////////////////////////
//           The following is for Field Debug Drawing              //////
/**
* A macro that sends a Sample pose
* @param x, y: coordinate of the pose, in cm
* @param rot : coordinate of the rotation
* @param penColor The color of the border of the dot (Drawings::Color)
* @param fillColor: Color of the dot
*/
#define DRAW_XYH_POSE(x, y, rot, pencolor, fillcolor) \
Field_object_draw->AddDebugXYH((int) x, (int) y,(double) rot,(char) pencolor, (char) fillcolor);

#define DRAW_SQUARE(x, y, sidelength, pencolor, fillcolor) \
Field_object_draw->AddDebugSquare((int) x, (int) y, (int) sidelength,(char) pencolor, (char) fillcolor);

/**
* A macro that sends a dot
* @param x The x coordinate of the center of the box, in cm
* @param y The y coordinate of the center of the box, in cm
* @param penColor The color of the border of the dot (Drawings::Color)
* @param fillColor The color of the dot (Drawings::Color)
*/
#define FIELD_DOT(x, y,penColor, fillColor) \
	Field_object_draw->AddDebugPoints((int)x, (int)y,(char)penColor, (char)fillColor);

/**
* A macro that sends a line
* @param x1 The x coordinate of the starting point, in cm
* @param y1 The y coordinate of the starting point, in cm
* @param x2 The x coordinate of the end point, in cm
* @param y2 The y coordinate of the end point, in cm
* @param penWidth The width of the arc of the circle
* @param penStyle The pen style of the arc of the circle (Drawings::PenStyle)
* @param penColor The color of the line (Drawings::Color)
*/
#define FIELD_LINE(x1, y1, x2, y2,penWidth, penStyle, penColor) \
	Field_object_draw->AddDebugLines((int)x1, (int)y1, (int)x2, (int)y2,(int)penWidth,(char)penStyle, (char)penColor);

/**
* A macro that sends a circle
* @param center_x The x coordinate of the center of the circle, in cm
* @param center_y The y coordinate of the center of the circle, in cm
* @param radius The radius of the circle, in cm
* @param penWidth The width of the arc of the circle
* @param penStyle The pen style of the arc of the circle (Drawings::PenStyle)
* @param penColor The color of the arc of the circle (Drawings::Color)
*/
#define FIELD_CIRCLE(center_x, center_y, radius,penWidth, penStyle, penColor) \
	Field_object_draw->AddDebugCircles((int)center_x, (int)center_y, (int)radius,(int)penWidth, (char)penStyle, (char)penColor);

/**
* A macro that sends a big point
* @param x, y: coordinate of the dot, in cm
* @param penColor The color of the border of the dot (Drawings::Color)
* @param fillColor: Color of the dot
*/
#define FIELD_BIG_DOT(x, y,penColor, fillColor) \
	Field_object_draw->AddDebugBigDot((int)x, (int)y, (char)penColor, (char)fillColor);
/**
* A macro that output a stirng of text on the field
* @param str: a char* or QString or just a string format text
* @param x,y: position of the text start on the field
* @param size: font size of the text;
* @param color: color of text;
* @param relate: true, cooridinate change with team number; false, abusolute coordinate in pixels unit of the dialog panel
*/
#define FIELD_TEXTOUT(str, x, y, size, color, relate)\
	Field_object_draw->AddDebugFieldText((QString)str, (int)x, (int)y, (int)size, (char)color, (bool)relate);




//////////////////////////////////////////////////////////////////////////
#endif//TJ_DEBUGGER

#endif
