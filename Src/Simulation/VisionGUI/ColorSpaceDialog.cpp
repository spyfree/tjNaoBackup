#include "ColorSpaceDialog.h"
ColorSpaceWindow::ColorSpaceWindow(QMdiArea* mdiArea, const QString& object):SubWindow(mdiArea, object)
{
	setWindowTitle(tr("ColorSpaceWindow - ") + object);
	setMinimumSize(QSize(400,400));
	myColorSpaceDialog=NULL;
	myColorSpaceDialog=new ColorSpaceDialog(this);
	setWidget(myColorSpaceDialog);
}
ColorSpaceWindow::~ColorSpaceWindow()
{
	if (myColorSpaceDialog)
	{
		delete myColorSpaceDialog;
		myColorSpaceDialog=NULL;
	}
}
void ColorSpaceWindow::show()
{
	if (myColorSpaceDialog)
	{
		myColorSpaceDialog->paintGL();
	}
	if(!isVisible())
	{	
		setVisible(!isVisible());
		myColorSpaceDialog->show();
	}
}
void ColorSpaceWindow::setLut(unsigned char* lut_)
{
	myColorSpaceDialog->setLut(lut_);
// 	show();
}
ColorSpaceDialog::ColorSpaceDialog(QWidget *parent):QGLWidget(parent)
{
	setFormat(QGLFormat(QGL::DoubleBuffer| QGL::DepthBuffer));
	rotationX=-21.0;
	rotationY=-57.0;
	rotationZ=0.0;
	faceColor[0]=Qt::red;
	faceColor[1]=Qt::green;
	faceColor[2]=Qt::blue;
	faceColor[3]=Qt::yellow;
	lut_=NULL;
// 	lut_ = new unsigned char[64*64*64]; // Stores the look up table
// 	memset(lut_,0,64*64*64);

}
ColorSpaceDialog::~ColorSpaceDialog()
{
// 	if (lut_)
// 	{
// 		delete[] lut_;
// 		lut_=NULL;
// 	}
}
void ColorSpaceDialog::initializeGL()
{
	qglClearColor(Qt::black);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
// 	glEnable(GL_CULL_FACE);
}
void ColorSpaceDialog::resizeGL(int width,int height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat x=GLfloat(width)/height;
// 	glFrustum(-x,+x,-1.0,1.0,4.0,15.0);
  	glOrtho(-x,+x,-1.0,1.0,4.0,15.0);
	glMatrixMode(GL_MODELVIEW);
}
void ColorSpaceDialog::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	draw();
}
void ColorSpaceDialog::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-10);
	glRotatef(rotationX,1,0,0);
	glRotatef(rotationY,0,1,0);
	glRotatef(rotationZ,0,0,0);
	/////////////»­±ß¿ò//////////////
	glColor3ub(70, 130, 70);	
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glBegin(GL_QUAD_STRIP);
		glVertex3d(-0.5, -0.5,-0.5);
		glVertex3d(-0.5, 0.5,-0.5);
		glVertex3d(0.5, -0.5,-0.5);
		glVertex3d(0.5, 0.5, -0.5);
		glVertex3d(0.5, -0.5, 0.5);
		glVertex3d(0.5, 0.5, 0.5);
		glVertex3d(-0.5,-0.5, 0.5);
		glVertex3d(-0.5,0.5, 0.5);
		glVertex3d(-0.5,-0.5, -0.5);
		glVertex3d(-0.5,0.5, -0.5);
	glEnd();
	//glLineWidth(3);

	///////////»­É«²Ê¿Õ¼ä////////////
	glPointSize(3);
	glBegin(GL_POINTS);
		double tmp = 1.0f/255.0f;
		double x,y;
		double z00;
		for(int colorSpaceY = 0; colorSpaceY < 255; colorSpaceY += 4)
		{
			for(int colorSpaceU = 0; colorSpaceU < 255; colorSpaceU += 4)
			{
				for(int colorSpaceV = 0; colorSpaceV < 255; colorSpaceV += 4)
				{
					int offset = (colorSpaceY >> 2 << 12)+(colorSpaceU >> 2 << 6)+(colorSpaceV >> 2);
					unsigned char color = lut_[offset];
					x   = (double)(-0.5 + colorSpaceV*tmp);
					y   = (double)(-0.5 + colorSpaceU*tmp);
					z00 = (double)(-0.5 + colorSpaceY*tmp);
					switch (color)
					{
					case c_WHITE:
						glColor3ub(255, 255, 255);
						glVertex3d(x, y, z00);
						break;
					case c_FIELD_GREEN:
						glColor3ub(0, 255, 0);
						glVertex3d(x, y, z00);
						break;
					case c_BEACON_YELLOW:
						glColor3ub(255, 255, 0);
						glVertex3d(x, y, z00);
						break;
					case c_ROBOT_BLUE:
						glColor3ub(50, 0, 160);
						glVertex3d(x, y, z00);
						break;
					case c_BEACON_PINK:
						glColor3ub(220, 60, 220);
						glVertex3d(x, y, z00);
						break;
					case c_ROBOT_RED:
						glColor3ub(255, 0, 0);
						glVertex3d(x, y, z00);
						break;
					case c_BALL_ORANGE:
						glColor3ub(220, 150, 0);
						glVertex3d(x, y, z00);
						break;
					case c_BEACON_BLUE:
						glColor3ub(57,134,210);
						glVertex3d(x, y, z00);
						break;
					default:
						break;
					}
				}
			}
		}    
	glEnd();
	///////////x,y,z///////
	///////////v,u,y///////
	glPointSize(15);
	glBegin(GL_POINTS);
		//////Ô­µã////////
		qglColor(Qt::white);
		//glColor3ub(0, 78.5, 0);
		glVertex3d(-0.5, -0.5,-0.5);
		/////uÖá¶¥µã//////
		qglColor(Qt::red);
// 		glColor3ub(255, 0, 0);
		glVertex3d(-0.5, 0.5,-0.5);
		/////vÖá¶¥µã/////
		qglColor(Qt::blue);
// 		glColor3ub(0, 81.6, 255);
		glVertex3d(0.5, -0.5,-0.5);
		//////yÖá¶¥µã//////
		qglColor(Qt::green);
// 		glColor3ub(0, 76.5, 0);
		glVertex3d(-0.5,-0.5, 0.5);
	glEnd();
	glFlush();

}
void ColorSpaceDialog::mousePressEvent(QMouseEvent *event)
{
	lastPos=event->pos();
}
void ColorSpaceDialog::mouseMoveEvent(QMouseEvent* event)
{
	GLfloat dx=GLfloat(event->x()-lastPos.x())/width();
	GLfloat dy=GLfloat(event->y()-lastPos.y())/height();
	int a=event->buttons();
	if (event->buttons()==Qt::LeftButton)
	{
		rotationX+=180*dy;
		rotationY+=180*dx;
		updateGL();
	}
	else if (event->buttons()==Qt::RightButton)
	{
		rotationX+=180*dy;
		rotationZ+=180*dx;
		updateGL();
	}
	lastPos=event->pos();
}
void ColorSpaceDialog::setLut(unsigned char *in)
{
	lut_=in;
	updateGL();
}