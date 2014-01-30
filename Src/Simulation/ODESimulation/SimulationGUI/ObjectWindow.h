#ifndef __ObjectWindow_h_
#define __ObjectWindow_h_
#include <GL/glew.h>
#include <QGLWidget>
//#include <QSignaleMapper>
#include "Simulation/SubWindow.h"
#include "../SimulationCore/OpenGL/ObjectRenderer.h"

class ObjectGLWidget;
class ObjectWindow : public SubWindow
{
	Q_OBJECT
public:
	ObjectWindow(QMdiArea* mdiArea, const QString& object);
	QMenu* createPopupMenu();
    
public slots:
	void setDragPlaneSlot(int plane);
private slots:
	void setSurfaceStyleSlot(int style);
	void setCameraModeSlot(int mode);
	void setFovYSlot(int fovy);
	void setDragModeSlot(int mode);
	void resetCameraSlot();
	void toggleCameraModeSlot();
	void fitCameraSlot();
	void snapToRootSlot();
	void showDetailSensorsSlot();
	void showControllerDrawingsSlot();
	void fitWindowSizeSlot();
// 	void startSimulationSlot();
// 	void stepSimulationSlot();
// 	void resetSimulationSlot();
	void copy();
private:
// 	void connectMenuBar();
// 	void disconnectMenuBar();
// 	void restoreLayout();
// 	void writeLayout();
	void updateObject();

	void keyPressEvent(QKeyEvent* event);
	void wheelEvent(QWheelEvent* event);
	void focusInEvent(QFocusEvent* event);
	void focusOutEvent(QFocusEvent* event);
	void contextMenuEvent(QContextMenuEvent* event);

	ObjectRenderer objectRenderer;
	ObjectGLWidget* objectGlWidget;
	QAction* showSensorsAct;
	QAction* showDrawingsAct;
	QAction* fitWindowSizeAct;
// 	QSignalMapper surfaceMapper;
// 	QSignalMapper cameraMapper;
// 	QSignalMapper fovMapper;
// 	QSignalMapper dragPlaneMapper;
// 	QSignalMapper dragModeMapper;
};
class ObjectGLWidget : public QGLWidget
{
public:
	ObjectGLWidget(ObjectRenderer& render, ObjectWindow* parent);
	~ObjectGLWidget();
private:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void focusInEvent(QFocusEvent * event);
	void focusOutEvent(QFocusEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	/*void contextMenuEvent(QContextMenuEvent* event);*/

	static QList<ObjectGLWidget*> objectGLWidgetList;

	ObjectRenderer& objectRenderer;
	ObjectWindow* parent;
};
#endif