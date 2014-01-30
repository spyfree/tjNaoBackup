#ifndef __FieldDrawing_h_ 
#define __FieldDrawing_h_ 

#include <QWidget>
#include <QPainter>
#include "Tools/DrawDebugging/DrawManager.h"
#include "Tools/DrawDebugging/DrawBuffer.h"
#include "Module/Representations/Configuration/FieldDimensions.h"
#include "Module/Representations/BaseProvider.h"

class FieldDrawing:public QWidget,public BaseProvider
{
	Q_OBJECT
public:
	FieldDrawing(QWidget *parent =0);
	~FieldDrawing();
	void readDimensions();
	void drawField(QPainter& painter);
	void drawGoals(QPainter& painter);
	void drawLocDrawings(QPainter& painter);
	void updateLocWidget();
	DrawBuffer* theDrawBuffer;
	DrawManager* drawManager;
	LocMessageData* theLocMessageData;
	QPainter painter;
	static FieldDrawing *fieldDrawing;
	static FieldDrawing* getInstance();
	double positionX;
	double positionY;
	double rotation;
protected:
	void paintEvent(QPaintEvent* event);

private:
	FieldDimensions* fieldDimensions;
	

private:
	int carpet_width;
	int carpet_lengt;
	int field_length;
	int field_width;
	int field_boundary;
	int penalty_width;
	int penalty_length;
	int goal_width;
	int goal_length;
	int center_circlex;
	int center_circley;
	int centerCircleRadius;
	int fieldLinesWidth;
	float scale;
	float zoom;
	QPoint offset;


};

#endif //FieldDrawing
