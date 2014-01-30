#ifndef __ColorSpaceDialog_h_
#define __ColorSpaceDialog_h_
#include <GL/glew.h>
#include<QtGui>
#include <QGLWidget>
#include "../SubWindow.h"
#include "Module/Common/VisionHeader.h"
class ColorSpaceDialog;
class ColorSpaceWindow:public SubWindow
{
	Q_OBJECT
public:
	ColorSpaceWindow(QMdiArea* mdiArea, const QString& object);
	~ColorSpaceWindow();
private:
	ColorSpaceDialog* myColorSpaceDialog;
public:
	void show();
public slots:
	void setLut(unsigned char* lut_);
};
class ColorSpaceDialog:public QGLWidget
{
	Q_OBJECT
public:
	ColorSpaceDialog(QWidget *parent=0);
	~ColorSpaceDialog();
	void setLut(unsigned char *in);
	void paintGL();
protected:
	void initializeGL();
	void resizeGL(int width,int height);

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent* event);
private:
	void draw();
	GLfloat rotationX;
	GLfloat rotationY;
	GLfloat rotationZ;
	QColor faceColor[4];
	QPoint lastPos;
	unsigned char* lut_;

};
#endif