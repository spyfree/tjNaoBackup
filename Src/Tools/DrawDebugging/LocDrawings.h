#ifndef LocDrawings_H
#define LocDrawings_H

#include "Tools/ColorClasses.h"
#include "Tools/Math/Vector2.h"

class LocDrawings
{
public:
	/** IDs for shape types 
	* shapes are the basic drawings that can be sent.
	*/
	enum ShapeType
	{
		circle, polygon, ellipse, line, dot, midDot, largeDot, 
		arrow, text, tip, origin, gridRGBA, gridMono
	};

	/** The pen style that is used for basic shapes*/
	enum PenStyle
	{
		ps_solid, ps_dash, ps_dot, ps_null
	};

	/** The fill style that is used for basic shapes*/
	enum FillStyle
	{
		bs_solid, bs_null
	};
};

class ColorRGBA
{
public:
	ColorRGBA()
	{
		r = g = b = 0;
		a = 255;
	}

	ColorRGBA(unsigned char r, 
		unsigned char g, 
		unsigned char b, 
		unsigned char a = 255):
	r(r), g(g), b(b), a(a)
	{
	}

	ColorRGBA(const ColorRGBA& other)
	{
		*this = other;
	}

	ColorRGBA(ColorClasses::Color colorClass)
	{
		switch(colorClass) 
		{
		case ColorClasses::orange:      r = 255; g = 128; b = 64;  break;
		case ColorClasses::yellow:      r = 255; g = 255; b = 0;   break;
		case ColorClasses::blue:        r = 0;   g = 0;   b = 128; break;
		case ColorClasses::green:       r = 0;   g = 255; b = 0;   break;
		case ColorClasses::white:       r = 255; g = 255; b = 255; break;
		case ColorClasses::black:       r = 0; g = 0; b = 0;       break;
		case ColorClasses::robotBlue:   r = 0; g = 255; b = 255;   break;
		case ColorClasses::red:         r = 255; g = 0; b = 0;     break;
		default: r = 0; g = 96; b = 128;                           break;
		};
		a = 255;
	}

	ColorRGBA operator*(double scale) const
	{
		unsigned char r2 = static_cast<unsigned char>(scale*r);
		unsigned char g2 = static_cast<unsigned char>(scale*g);
		unsigned char b2 = static_cast<unsigned char>(scale*b);
		unsigned char a2 = static_cast<unsigned char>(scale*a);
		return ColorRGBA(r2,g2,b2,a2);
	}

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

/** the maximum number of points in a polygon */
enum { MAX_NUMBER_OF_POINTS=16, MAX_NUMBER_OF_GRID_CELLS=1600 };

/** base class for all drawing elements */
class Element
{
public:
	LocDrawings::PenStyle penStyle;
	ColorRGBA penColor;
	int width;
};

/** Stores a polygon */
class Polygons:public Element
{
public: 
	Vector2<int> points[MAX_NUMBER_OF_POINTS];
	int nCount;
	LocDrawings::FillStyle fillStyle;
	ColorRGBA fillColor;
};

/** Stores a line */
class Line :public Element
{
public:
	int xStart, yStart, xEnd, yEnd;
};

/** Stores an ellipse */
class Ellipses :public Element
{
public:
	int x, y, radiusX, radiusY;
	double rotation;
	LocDrawings::FillStyle fillStyle;
	ColorRGBA fillColor;
};


#endif
