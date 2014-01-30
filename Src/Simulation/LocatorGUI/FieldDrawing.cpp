
#include "FieldDrawing.h"

FieldDrawing *FieldDrawing::fieldDrawing=0;
FieldDrawing::FieldDrawing(QWidget *parent /* =0 */):QWidget(parent)
{
	zoom = 1.0;
	theDrawBuffer = new DrawBuffer();
	theLocMessageData = new LocMessageData();
	drawManager = new DrawManager();
	fieldDimensions = new FieldDimensions();
	fieldDimensions->readDimensions();
	readDimensions();
	fieldDrawing = this;
}

FieldDrawing* FieldDrawing::getInstance()
{
	return fieldDrawing;
}

FieldDrawing::~FieldDrawing()
{
	if(fieldDimensions != NULL)
	{
		delete fieldDimensions;
		fieldDimensions = NULL;
	}
	if(drawManager != NULL)
	{
		delete drawManager;
		drawManager = NULL;
	}
	if(theLocMessageData != NULL)
	{
		delete theLocMessageData;
		theLocMessageData = NULL;
	}
	if(theDrawBuffer != NULL)
	{
		delete theDrawBuffer;
		theDrawBuffer = NULL;
	}
}

void FieldDrawing::readDimensions()
{
	carpet_lengt = 2*fieldDimensions->yPosLeftFieldBorder; 
	carpet_width = 2*fieldDimensions->xPosOpponentFieldBorder;
	field_length = 2*fieldDimensions->yPosLeftGroundline;
	field_width = 2*fieldDimensions->xPosOpponentGroundline;
	penalty_width = (fieldDimensions->xPosOpponentGroundline - fieldDimensions->xPosOpponentPenaltyArea);
	penalty_length = 2*fieldDimensions->yPosLeftPenaltyArea;
	center_circlex = (carpet_width/2);
	center_circley = (carpet_lengt/2);
	goal_length = 2*fieldDimensions->yPosLeftGoal;
	goal_width = (fieldDimensions->xPosOpponentGoal - fieldDimensions->xPosOpponentGroundline);
	centerCircleRadius = fieldDimensions->centerCircleRadius;
	field_boundary = (fieldDimensions->yPosLeftFieldBorder - fieldDimensions->yPosLeftSideline);
	fieldLinesWidth = fieldDimensions->fieldLinesWidth;
}

void FieldDrawing::updateLocWidget()
{
	//刷新视图
	update();
}

void FieldDrawing::paintEvent(QPaintEvent* event)
{
	painter.begin(this);
	//缩小视图
	painter.scale(0.1,0.1);
	painter.translate(0,7400);
	painter.rotate(-90);

	drawField(painter);
	drawGoals(painter);
	painter.rotate(90);
	painter.translate(0,-7400);

	//将坐标原点变化到机器人场地的全局坐标原点下
	//painter.translate(carpet_lengt/2,carpet_width/2); 
	//QString i=setNum ( double n, char format = 'g', int precision = 6 );
	QString i;
	//positionX=100.;
	//positionY=100.;
	QString LocResult="RobotX:"+i.setNum(positionX,'g',6)+"   "+"RobotY:"+i.setNum(positionY,'g',6);
	painter.setFont(QFont("Helvetica", 120));
	painter.setPen(QPen(Qt::black, 2));
	painter.drawText(100, 200, LocResult);
	const QSize& size(this->size());
	int viewWidth = int(carpet_lengt);
	int viewHeight = int(carpet_width);
	float xScale = float(size.width()) / viewWidth;
	float yScale = float(size.height()) / viewHeight;
	scale = xScale < yScale ? xScale:yScale;
	scale *= zoom;
	//painter.setTransform(QTransform(1,0,0,1,100,100));
	/*painter.drawLine(500,500,500,1000);
	painter.drawLine(1000,1000,1000,2000);*/
	//坐标变换，将视图坐标变换到机器人坐标下
	//QTransform translationTransform(1, 0, 0, 1, carpet_lengt/2 +size.width() / 2. - offset.x()*scale, size.height() / 2. - offset.y()*scale+carpet_width/2);
	//QTransform rotationTransform(0, 1,  1, 0, 0, 0);
	//QTransform scalingTransform(scale, 0, 0, scale, 0, 0);
	//QTransform transform;
	//transform = scalingTransform *rotationTransform*translationTransform;
	//painter.setTransform(transform);
	painter.setTransform(QTransform(0, scale, scale,0, size.width() / 2. - offset.x()*scale, size.height() / 2. - offset.y()*scale));
	drawLocDrawings(painter);
	painter.end();
}


void FieldDrawing::drawField(QPainter& painter)
{
	//draw carpet
	painter.setBrush(QColor::fromRgb(0,180,0));
	QPen pen;
	pen.setColor(QColor::fromRgb(0,180,0));
	painter.drawRect(0,0,carpet_width,carpet_lengt);

	//draw lines
	painter.setBrush(Qt::NoBrush);
	pen.setColor(Qt::white);
	pen.setWidth(fieldLinesWidth);
	painter.setPen(pen);
	painter.drawRect(field_boundary,field_boundary,field_width,field_length);
	painter.drawRect(field_boundary,field_boundary + 
		(field_length - penalty_length)/2,penalty_width,penalty_length);
	painter.drawRect(carpet_width-field_boundary-penalty_width,field_boundary + 
		(field_length - penalty_length)/2,penalty_width,penalty_length);
	painter.drawLine(carpet_width/2,field_boundary,
		carpet_width/2,carpet_lengt-field_boundary);
	painter.drawEllipse(carpet_width/2-centerCircleRadius,
		carpet_lengt/2-centerCircleRadius,2*centerCircleRadius,2*centerCircleRadius);
}


void FieldDrawing::drawGoals(QPainter& painter)
{
	/*painter.setBrush(QColor::fromRgb(77,155,179));
	QPen pen;
	pen.setColor(QColor::fromRgb(77,155,179));
	painter.setPen(pen);*/
	painter.setBrush(QColor::fromRgb(208,197,47));
	QPen pen;
	pen.setColor(QColor::fromRgb(208,197,47));
	painter.setPen(pen);
	painter.drawRect(field_boundary-goal_width,carpet_lengt/2-goal_length/2,goal_width,goal_length);
	painter.setBrush(QColor::fromRgb(56,64,199));
	painter.drawEllipse(field_boundary-5,carpet_lengt/2-goal_length/2-5,10,10);
	painter.drawEllipse(field_boundary-5,carpet_lengt/2+goal_length/2-5,10,10);

	painter.setBrush(QColor::fromRgb(208,197,47));
	pen.setColor(QColor::fromRgb(208,197,47));
	painter.setPen(pen);
	//painter.setBrush(QColor::fromRgb(77,155,179));
	//pen.setColor(QColor::fromRgb(77,155,179));
	//painter.setPen(pen);//对方门为蓝门时；
	painter.drawRect(carpet_width-field_boundary,carpet_lengt/2-goal_length/2,goal_width,goal_length);
	painter.setBrush(QColor::fromRgb(249,231,6));
	painter.drawEllipse(carpet_width-field_boundary-5,carpet_lengt/2-goal_length/2-5,10,10);
	painter.drawEllipse(carpet_width-field_boundary-5,carpet_lengt/2+goal_length/2-5,10,10);

	painter.setBrush(Qt::NoBrush);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
}

void FieldDrawing::drawLocDrawings(QPainter& painter)
{
	
	selfMessageQueue->SearchMyMessage(idLocDrawing,idBehaviorThread,idMainWindow,
		(char*)theLocMessageData);

	drawManager->paintDebugDrawing(painter,*theLocMessageData);
}

