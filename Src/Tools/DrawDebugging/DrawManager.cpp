#include "DrawManager.h"
#include <QtGui>
#include <vector>
#include <iostream>

DrawManager::DrawManager()
{
	//setGlobals();
}
DrawManager::~DrawManager()
{

}

void DrawManager::paintDebugDrawing(QPainter& painter, const LocMessageData& theLocMessageData)
{
	static QBrush brush(Qt::SolidPattern);
	static QBrush noBrush(Qt::NoBrush);
	static QPen pen;
	static QPen noPen(Qt::NoPen);
	//draw lines
	Line tempLine;
	std::vector<Line>::const_iterator lineIter;
	for(lineIter = theLocMessageData.lines.begin(); lineIter != theLocMessageData.lines.end(); ++lineIter)
	{
		tempLine = *lineIter;
		if(tempLine.penStyle != LocDrawings::ps_null)
		{
			pen.setColor(QColor(tempLine.penColor.r, tempLine.penColor.g, tempLine.penColor.b, tempLine.penColor.a));
			// A line width of zero indicates a cosmetic pen. This means that the pen width is always drawn one pixel wide, independent of the transformation set on the painter.
			pen.setWidth(tempLine.width < 10 ? 0 : tempLine.width);
			switch (tempLine.penStyle)
			{
			case LocDrawings::ps_dash:
				pen.setStyle(Qt::DashLine);
				break;
			case LocDrawings::ps_dot:
				pen.setStyle(Qt::DotLine);
				break;
			case LocDrawings::ps_solid:
			default:
				pen.setStyle(Qt::SolidLine);
			}
			painter.setPen(pen);

			painter.drawLine(tempLine.xStart, tempLine.yStart, tempLine.xEnd, tempLine.yEnd);
		}

	}

	//draw polygons
	Polygons tempPolygon;
	std::vector<Polygons>::const_iterator polygonIter;
	for(polygonIter = theLocMessageData.polygons.begin(); polygonIter != theLocMessageData.polygons.end(); ++polygonIter)
	{
		tempPolygon = *polygonIter;
		if(tempPolygon.penStyle != LocDrawings::ps_null)
		{
			pen.setColor(QColor(tempPolygon.penColor.r, tempPolygon.penColor.g, tempPolygon.penColor.b, tempPolygon.penColor.a));
			// A line width of zero indicates a cosmetic pen. This means that the pen width is always drawn one pixel wide, independent of the transformation set on the painter.
			pen.setWidth(tempPolygon.width < 0 ? 0 : tempPolygon.width);
			switch (tempPolygon.penStyle)
			{
			case LocDrawings::ps_dash:
				pen.setStyle(Qt::DashLine);
				break;
			case LocDrawings::ps_dot:
				pen.setStyle(Qt::DotLine);
				break;
			case LocDrawings::ps_solid:
			default:
				pen.setStyle(Qt::SolidLine);
			}
			painter.setPen(pen);
			
			// copy vector2 to QPoints
			static QPoint points[16];
			for(int n = tempPolygon.nCount - 1; n >= 0 ; --n)
				points[n] = QPoint(tempPolygon.points[n].x, tempPolygon.points[n].y);

			painter.drawPolygon(points, tempPolygon.nCount);
		}

	}

	//draw ellipses
	Ellipses tempEllipse;
	std::vector<Ellipses>::const_iterator ellipseIter;
	for(ellipseIter = theLocMessageData.ellipses.begin(); ellipseIter != theLocMessageData.ellipses.end(); ++ellipseIter)
	{
		tempEllipse = *ellipseIter;
		// select brush
		if(tempEllipse.fillStyle == LocDrawings::bs_solid)
		{
			brush.setColor(QColor(tempEllipse.fillColor.r, tempEllipse.fillColor.g, tempEllipse.fillColor.b, tempEllipse.fillColor.a));
			painter.setBrush(brush);
		}
		else
			painter.setBrush(noBrush);

		// select pen
		if(tempEllipse.penStyle != LocDrawings::ps_null)
		{
			pen.setColor(QColor(tempEllipse.penColor.r, tempEllipse.penColor.g, tempEllipse.penColor.b, tempEllipse.penColor.a));
			// A line width of zero indicates a cosmetic pen. This means that the pen width is always drawn one pixel wide, independent of the transformation set on the painter.
			pen.setWidth(tempEllipse.width < 0 ? 0 : tempEllipse.width);
			switch (tempEllipse.penStyle)
			{
			case LocDrawings::ps_dash:
				pen.setStyle(Qt::DashLine);
				break;
			case LocDrawings::ps_dot:
				pen.setStyle(Qt::DotLine);
				break;
			case LocDrawings::ps_solid:
			default:
				pen.setStyle(Qt::SolidLine);
			}
			painter.setPen(pen);
		}
		else
			painter.setPen(noPen);

		if(tempEllipse.rotation != 0.0)
		{
			QTransform trans(painter.transform());
			QTransform transBack(painter.transform());
			trans.translate(qreal(tempEllipse.x), qreal(tempEllipse.y));
			trans.rotateRadians(qreal(tempEllipse.rotation));
			painter.setTransform(trans);
			painter.drawEllipse(-tempEllipse.radiusX, -tempEllipse.radiusY, 2*tempEllipse.radiusX, 2*tempEllipse.radiusY);
			painter.setTransform(transBack);
		}
		else 
		{
			painter.drawEllipse(tempEllipse.x-tempEllipse.radiusX, tempEllipse.y-tempEllipse.radiusY, 2*tempEllipse.radiusX, 2*tempEllipse.radiusY);
		}

	}
}

