#include <QKeyEvent>
#include <QApplication>
#include <QSignalMapper>
#include <QSettings>
#include <QMenu>
#include <QClipboard>
#include <QLayout>
#include <QMainWindow>
#include <QAction>
#include "ObjectWindow.h"
#include "Document.h"
#include "..\..\MenuBar.h"
ObjectWindow::ObjectWindow(QMdiArea* mdiArea, const QString& object)
:SubWindow(mdiArea, object), objectRenderer(Document::document->getSimulation(), object.toAscii().constData()), 
objectGlWidget(0),showSensorsAct(0), showDrawingsAct(0)
{
	setWindowTitle(tr("Object - ") + object);
	setMinimumSize(QSize(600,450));


	updateObject();
	setSurfaceStyleSlot(VisualizationParameterSet::TEXTURE_SHADING);
}
void ObjectWindow::updateObject()
{
	Simulation* sim = Document::document->getSimulation();
	if(objectRenderer.getSimulation() != sim || (!objectGlWidget && sim))
	{
		objectRenderer.setSimulation(sim);
		QWidget* oldWidget = objectGlWidget;
		objectGlWidget = sim ? new ObjectGLWidget(objectRenderer, this) : 0;
		setWidget(objectGlWidget);
		if(oldWidget)
			delete oldWidget;
	}
	else
		objectRenderer.setObject(object.toAscii().constData());
}
void ObjectWindow::setSurfaceStyleSlot(int style)
{
 	objectRenderer.setSurfaceStyle(VisualizationParameterSet::SurfaceStyle(style));
 	if(objectGlWidget)
 		objectGlWidget->updateGL();
// 	qobject_cast<QAction*>(surfaceMapper.mapping(style))->setChecked(true);
}

void ObjectWindow::setCameraModeSlot(int mode)
{
	objectRenderer.setCameraMode(VisualizationParameterSet::CameraMode(mode));
	if(objectGlWidget)
		objectGlWidget->updateGL();
	/*qobject_cast<QAction*>(cameraMapper.mapping(mode))->setChecked(true);*/
}

void ObjectWindow::setFovYSlot(int fovy)
{
	objectRenderer.setFovY(fovy);
	if(objectGlWidget)
		objectGlWidget->updateGL();
	/*qobject_cast<QAction*>(fovMapper.mapping(fovy))->setChecked(true);*/
}

void ObjectWindow::setDragPlaneSlot(int plane)
{
	objectRenderer.setDragPlane(DragAndDropPlane(plane));
	if(objectGlWidget)
		objectGlWidget->updateGL();
	/*qobject_cast<QAction*>(dragPlaneMapper.mapping(plane))->setChecked(true);*/
}

void ObjectWindow::setDragModeSlot(int mode)
{
	objectRenderer.setDragMode(DragAndDropMode(mode));
	if(objectGlWidget)
		objectGlWidget->updateGL();
	/*qobject_cast<QAction*>(dragModeMapper.mapping(mode))->setChecked(true);*/
}

void ObjectWindow::resetCameraSlot()
{
	objectRenderer.resetCamera();
	if(objectGlWidget)
		objectGlWidget->updateGL();
}

void ObjectWindow::toggleCameraModeSlot()
{
	objectRenderer.toggleCameraMode();
	if(objectGlWidget)
		objectGlWidget->updateGL();
	/*qobject_cast<QAction*>(cameraMapper.mapping(objectRenderer.getCameraMode()))->setChecked(true);*/
}

void ObjectWindow::fitCameraSlot()
{
	objectRenderer.fitCamera();
	if(objectGlWidget)
		objectGlWidget->updateGL();
}

void ObjectWindow::snapToRootSlot()
{
	objectRenderer.setSnapToRoot(!objectRenderer.hasSnapToRoot());
	if(objectGlWidget)
		objectGlWidget->updateGL();
	/*MenuBar::menuBar->camSnapToRoot->setChecked(objectRenderer.hasSnapToRoot());*/
}

void ObjectWindow::showDetailSensorsSlot()
{
	objectRenderer.setShowDetailSensors(!objectRenderer.hasShowDetailSensors());
	if(objectGlWidget)
		objectGlWidget->updateGL();
	/*MenuBar::menuBar->viewDetailSensorsAction->setChecked(objectRenderer.hasShowDetailSensors());*/
}

void ObjectWindow::showControllerDrawingsSlot()
{
	objectRenderer.setShowControllerDrawings(!objectRenderer.hasShowControllerDrawings());
	if(objectGlWidget)
		objectGlWidget->updateGL();
	/*MenuBar::menuBar->viewControllerDrawingsAction->setChecked(objectRenderer.hasShowControllerDrawings());*/
}

void ObjectWindow::keyPressEvent(QKeyEvent* event)
{
	SubWindow::keyPressEvent(event);

	switch(event->key())
	{
	case Qt::Key_PageUp:
		event->accept();
		objectRenderer.zoom(0.05);
		if(objectGlWidget)
			objectGlWidget->updateGL();
		break;

	case Qt::Key_PageDown:
		event->accept();
		objectRenderer.zoom(-0.05);
		if(objectGlWidget)
			objectGlWidget->updateGL();
		break;
	}
}

void ObjectWindow::wheelEvent(QWheelEvent* event)
{
	SubWindow::wheelEvent(event);

	if(event->orientation() == Qt::Vertical)
	{
		objectRenderer.zoom(-0.05 * event->delta() / 160);
		if(objectGlWidget)
			objectGlWidget->updateGL();
		event->accept();
	}
}

void ObjectWindow::focusInEvent(QFocusEvent* event)
{
	SubWindow::focusInEvent(event);

	objectRenderer.setFocus(true);
}

void ObjectWindow::focusOutEvent(QFocusEvent* event)
{
	SubWindow::focusOutEvent(event);

	objectRenderer.setFocus(objectGlWidget && objectGlWidget->hasFocus());
}
QMenu* ObjectWindow::createPopupMenu()
{

	QMenu* menu = new QMenu(tr("&Object"));
	QAction* action;
// 	action = menu->addAction(QIcon(":icons/page_copy.png"), tr("&Copy"));
// 	action->setShortcut(QKeySequence(QKeySequence::Copy));
// 	action->setStatusTip(tr("Copy the current selection's contents or view to the clipboard"));
// 	connect(action, SIGNAL(triggered()), this, SLOT(copy()));

/*	menu->addSeparator();*/

	QMenu* subMenu = menu->addMenu(tr("&Drag Plane"));
	action = subMenu->menuAction();
	action->setIcon(QIcon(":icons/DragPlane.png"));
	action->setStatusTip(tr("Select the drag and drop dynamics mode and plane along which operations are performed"));
	QActionGroup* actionGroup = new QActionGroup(subMenu);
	QSignalMapper* signalMapper = new QSignalMapper(subMenu);
	connect(signalMapper, SIGNAL(mapped(int)), SLOT(setDragPlaneSlot(int)));
	action = subMenu->addAction(tr("X/Y Plane"));
	actionGroup->addAction(action);
	signalMapper->setMapping(action, XY_PLANE);
	/*action->setShortcut(QKeySequence(Qt::Key_Z));*/
	action->setCheckable(true);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("X/Z Plane"));
	actionGroup->addAction(action);
	signalMapper->setMapping(action, XZ_PLANE);
	/*action->setShortcut(QKeySequence(Qt::Key_Y));*/
	action->setCheckable(true);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("Y/Z Plane"));
	actionGroup->addAction(action);
	signalMapper->setMapping(action, YZ_PLANE);
	/*action->setShortcut(QKeySequence(Qt::Key_X));*/
	action->setCheckable(true);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	qobject_cast<QAction*>(signalMapper->mapping(objectRenderer.getDragPlane()))->setChecked(true);
// 	subMenu->addSeparator();
// 	actionGroup = new QActionGroup(subMenu);
// 	signalMapper = new QSignalMapper(subMenu);
// 	connect(signalMapper, SIGNAL(mapped(int)), SLOT(setDragModeSlot(int)));
// 	action = subMenu->addAction(tr("&Keep Dynamics"));
// 	actionGroup->addAction(action);
// 	signalMapper->setMapping(action, KEEP_DYNAMICS);
// 	action->setShortcut(QKeySequence(Qt::Key_8));
// 	action->setCheckable(true);
// 	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
// 	action = subMenu->addAction(tr("&Reset Dynamics"));
// 	actionGroup->addAction(action);
// 	signalMapper->setMapping(action, RESET_DYNAMICS);
// 	action->setShortcut(QKeySequence(Qt::Key_9));
// 	action->setCheckable(true);
// 	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
// 	action = subMenu->addAction(tr("&Apply Dynamics"));
// 	actionGroup->addAction(action);
// 	signalMapper->setMapping(action, APPLY_DYNAMICS);
// 	action->setShortcut(QKeySequence(Qt::Key_0));
// 	action->setCheckable(true);
// 	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
// 	qobject_cast<QAction*>(signalMapper->mapping(objectRenderer.getDragMode()))->setChecked(true);

	menu->addSeparator();

	subMenu = menu->addMenu(tr("&Object Surface"));
	actionGroup = new QActionGroup(subMenu);
	signalMapper = new QSignalMapper(subMenu);
	connect(signalMapper, SIGNAL(mapped(int)), SLOT(setSurfaceStyleSlot(int)));
	action = subMenu->menuAction();
	action->setIcon(QIcon(":icons/layers.png"));
	action->setStatusTip(tr("Select different rendering techniques for displaying the scene"));
	action = subMenu->addAction(tr("Appearance &Wire Frame"));
	actionGroup->addAction(action);
	/*action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));*/
	action->setCheckable(true);
	signalMapper->setMapping(action, VisualizationParameterSet::WIREFRAME);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("Appearance &Flat Shading"));
	actionGroup->addAction(action);
	/*action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));*/
	action->setCheckable(true);
	signalMapper->setMapping(action, VisualizationParameterSet::FLAT_SHADING);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("Appearance &Smooth Shading"));
	actionGroup->addAction(action);
	/*action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));*/
	action->setCheckable(true);
	signalMapper->setMapping(action, VisualizationParameterSet::SMOOTH_SHADING);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("Appearance &Texture Shading"));
	actionGroup->addAction(action);
	/*action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));*/
	action->setCheckable(true);
	signalMapper->setMapping(action, VisualizationParameterSet::TEXTURE_SHADING);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("Appearance &Camera Simulation"));
	actionGroup->addAction(action);
	action->setCheckable(true);
	signalMapper->setMapping(action, VisualizationParameterSet::CAMERA_SIMULATION);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	subMenu->addSeparator();
	action = subMenu->addAction(tr("Physics &Wire Frame"));
	actionGroup->addAction(action);
	action->setCheckable(true);
	signalMapper->setMapping(action, VisualizationParameterSet::PHYSICS_WIREFRAME);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("Physics &Flat Shading"));
	actionGroup->addAction(action);
	action->setCheckable(true);
	signalMapper->setMapping(action, VisualizationParameterSet::PHYSICS_FLAT);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("Physics &Smooth Shading"));
	actionGroup->addAction(action);
	action->setCheckable(true);  
	signalMapper->setMapping(action, VisualizationParameterSet::PHYSICS_SMOOTH);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	subMenu->addSeparator();
	action = subMenu->addAction(tr("Mixed Mode &1 (Shaded Physics)"));
	actionGroup->addAction(action);
	action->setCheckable(true);
	signalMapper->setMapping(action, VisualizationParameterSet::MIXED_PHYSICS);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("Mixed Mode &2 (Shaded Appearance)"));
	actionGroup->addAction(action);
	action->setCheckable(true);
	signalMapper->setMapping(action, VisualizationParameterSet::MIXED_APPEARANCE);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	qobject_cast<QAction*>(signalMapper->mapping(objectRenderer.getSurfaceStyle()))->setChecked(true);

	menu->addSeparator();

	subMenu = menu->addMenu(tr("&Camera"));
	action = subMenu->menuAction();
	action->setIcon(QIcon(":icons/camera.png"));
	action->setStatusTip(tr("Select different camera modes for displaying the scene"));
	actionGroup = new QActionGroup(subMenu);
	signalMapper = new QSignalMapper(subMenu);
	connect(signalMapper, SIGNAL(mapped(int)), SLOT(setCameraModeSlot(int)));
	action = subMenu->addAction(tr("&Target Mode"));
	action->setCheckable(true);
	actionGroup->addAction(action);
	signalMapper->setMapping(action, VisualizationParameterSet::TARGETCAM);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("&Free Mode"));
	action->setCheckable(true);
	actionGroup->addAction(action);
	signalMapper->setMapping(action, VisualizationParameterSet::FREECAM);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("&Presentation Mode"));
	action->setCheckable(true);
	actionGroup->addAction(action);
	signalMapper->setMapping(action, VisualizationParameterSet::PRESENTATION);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	qobject_cast<QAction*>(signalMapper->mapping(objectRenderer.getCameraMode()))->setChecked(true);
	subMenu->addSeparator();
	action = subMenu->addAction(tr("&Reset"));
	/*action->setShortcut(QKeySequence(Qt::Key_R));*/
	connect(action, SIGNAL(triggered()), this, SLOT(resetCameraSlot()));
	action = subMenu->addAction(tr("&Toggle"));
	/*action->setShortcut(QKeySequence(Qt::Key_T));*/
	connect(action, SIGNAL(triggered()), this, SLOT(toggleCameraModeSlot()));
	action = subMenu->addAction(tr("&Fit"));
	/*action->setShortcut(QKeySequence(Qt::Key_F));*/
	connect(action, SIGNAL(triggered()), this, SLOT(fitCameraSlot()));
	subMenu->addSeparator();
	action = subMenu->addAction(tr("&Snap To Root"));
	action->setCheckable(true);
	action->setChecked(objectRenderer.hasSnapToRoot());
	connect(action, SIGNAL(triggered()), this, SLOT(snapToRoot()));

	subMenu = menu->addMenu(tr("&Vertical Opening Angle"));
	action = subMenu->menuAction();
	action->setIcon(QIcon(":icons/opening_angle.png"));
	actionGroup = new QActionGroup(subMenu);
	signalMapper = new QSignalMapper(subMenu);
	connect(signalMapper, SIGNAL(mapped(int)), SLOT(setFovYSlot(int)));
	action = subMenu->addAction(tr("&20?"));
	actionGroup->addAction(action);
	signalMapper->setMapping(action, 20);
	/*action->setShortcut(QKeySequence(Qt::Key_1));*/
	action->setCheckable(true);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("&40?"));
	actionGroup->addAction(action);
	signalMapper->setMapping(action, 40);
	/*action->setShortcut(QKeySequence(Qt::Key_2));*/
	action->setCheckable(true);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("&60?"));
	actionGroup->addAction(action);
	signalMapper->setMapping(action, 60);
	/*action->setShortcut(QKeySequence(Qt::Key_3));*/
	action->setCheckable(true);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("&80?"));
	actionGroup->addAction(action);
	signalMapper->setMapping(action, 80);
	/*action->setShortcut(QKeySequence(Qt::Key_4));*/
	action->setCheckable(true);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("100?"));
	actionGroup->addAction(action);
	signalMapper->setMapping(action, 100);
	/*action->setShortcut(QKeySequence(Qt::Key_5));*/
	action->setCheckable(true);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	action = subMenu->addAction(tr("120?"));
	actionGroup->addAction(action);
	signalMapper->setMapping(action, 120);
	/*action->setShortcut(QKeySequence(Qt::Key_6));*/
	action->setCheckable(true);
	connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
	qobject_cast<QAction*>(signalMapper->mapping(objectRenderer.getFovY()))->setChecked(true);

    menu->addSeparator();

	if(!showSensorsAct)
	{
		showSensorsAct = new QAction(QIcon(":icons/transmit_go.png"), tr("Show &Sensors"), this);
		showSensorsAct->setStatusTip(tr("Show the values of the sensors in the scene view"));
		showSensorsAct->setCheckable(true);
		showSensorsAct->setChecked(objectRenderer.hasShowDetailSensors());
		connect(showSensorsAct, SIGNAL(triggered()), this, SLOT(showDetailSensorsSlot()));
	}
	menu->addAction(showSensorsAct);

// 	if(!showDrawingsAct)
// 	{
// 		showDrawingsAct = new QAction(QIcon(":icons/chart_line.png"), tr("Show Controller &Drawings"), this);
// 		showDrawingsAct->setStatusTip(tr("Show the controller 3D drawings in the scene view"));
// 		showDrawingsAct->setCheckable(true);
// 		showDrawingsAct->setChecked(objectRenderer.hasShowControllerDrawings());
// 		connect(showDrawingsAct, SIGNAL(triggered()), this, SLOT(showControllerDrawingsSlot()));
// 	}
// 	menu->addAction(showDrawingsAct);

	return menu;
}

QList<ObjectGLWidget*> ObjectGLWidget::objectGLWidgetList;

ObjectGLWidget::ObjectGLWidget(ObjectRenderer& render, ObjectWindow* parent)
:QGLWidget(parent, objectGLWidgetList.isEmpty()? 0 : objectGLWidgetList.first()),
objectRenderer(render), parent(parent)
{
	objectGLWidgetList.append(this);
	objectRenderer.setFocus(hasFocus() || parent->hasFocus());
	connect(Document::document, SIGNAL(updateViewsSignal()), SLOT(updateGL()));
}
ObjectGLWidget::~ObjectGLWidget()
{
	objectGLWidgetList.removeAll(this);
}
void ObjectGLWidget::initializeGL()
{
	objectRenderer.init(isSharing());
}
void ObjectGLWidget::paintGL()
{
	objectRenderer.draw();
}
void ObjectGLWidget::resizeGL(int width, int height)
{
	objectRenderer.setSize(width, height);
}

void ObjectGLWidget::focusInEvent(QFocusEvent * event)
{
	QGLWidget::focusInEvent(event);

	objectRenderer.setFocus(true);
}

void ObjectGLWidget::focusOutEvent(QFocusEvent * event)
{
	QGLWidget::focusOutEvent(event);

	objectRenderer.setFocus(parent->hasFocus());
}

void ObjectGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	QGLWidget::mouseMoveEvent(event);

	if(objectRenderer.moveDrag(event->x(), event->y()))
	{
		event->accept();
		updateGL();
	}
}

void ObjectGLWidget::mousePressEvent(QMouseEvent* event)
{
	QGLWidget::mousePressEvent(event);

	if(!objectRenderer.hasFocus())
		return;
	updateGL();

	if(event->button() == Qt::LeftButton || event->button() == Qt::MidButton)
		if(objectRenderer.startDrag(event->x(), event->y(), QApplication::keyboardModifiers() & Qt::ShiftModifier ? ObjectRenderer::DRAG_ROTATE : ObjectRenderer::DRAG_NORMAL))
		{
			event->accept();
			updateGL();
		}
}
void ObjectGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	QGLWidget::mouseReleaseEvent(event);

	if(objectRenderer.releaseDrag(event->x(), event->y()))
	{
		event->accept();
		updateGL();
	}
	/*
	else if(event->button() == Qt::RightButton)
	{

	} */
}

void ObjectWindow::contextMenuEvent(QContextMenuEvent* event)
{
	if(!this->widget()) return;
	QWidget* widget(this->widget());
	if(!widget)
	{
		ObjectWindow::contextMenuEvent(event);
		return;
	}

	QRect content(widget->geometry());
	if(!content.contains(event->x(), event->y()))
	{ // click on window frame
		//QDockWidget::contextMenuEvent(event);
		QMainWindow* parent((QMainWindow*)this->parent());
		QMenu* menu = parent->createPopupMenu();
		if(menu)
		{
			menu->exec(mapToGlobal(QPoint(event->x(), event->y())));
			delete menu;
		}
		return;
	};

	QMenu* menu = createPopupMenu();
// 	fitWindowSizeAct = new QAction(QIcon(":icons/application_fit.png"), tr("&Fit Size")/*tr("&Fit Window Size")*/, this);
// 	connect(fitWindowSizeAct, SIGNAL(triggered()), this, SLOT(fitWindowSizeSlot()));
	if(menu)
	{
		const QList<QAction*> actions(menu->actions());
		if(actions.size() == 0)
		{
			delete menu;
			menu = 0;
		}
		else
		{
			event->accept();
			MenuBar selfmenubar = (MenuBar*)parent();
			QAction* firstAction = actions.at(0);      
// 			menu->insertAction(firstAction, selfmenubar.simStart);
// 			menu->insertAction(firstAction, selfmenubar.simReset);
// 			menu->insertAction(firstAction, selfmenubar.simStep);
// 			menu->insertSeparator(firstAction);
// 			menu->insertAction(firstAction, fitWindowSizeAct);
/*			menu->insertSeparator(firstAction);*/
			menu->exec(mapToGlobal(QPoint(event->x(), event->y())));
// 			connect(selfmenubar->simStart, SIGNAL(triggered()), this, SLOT(MainWindow::startSimulation()));
// 			connect(selfmenubar->simReset, SIGNAL(triggered()), this, SLOT(MainWindow::resetSimulation()));
// 			connect(selfmenubar->simStep, SIGNAL(triggered()), this, SLOT(MainWindow::stepSimulation()));
			

			delete menu;
		}
	}
	if(!menu)
	{
		//QDockWidget::contextMenuEvent(event);
		event->accept();
		QMenu menu;
		MenuBar selfmenubar = (MenuBar*)parent();
// 		menu.addAction(selfmenubar.simStart);
// 		menu.addAction(selfmenubar.simReset);
// 		menu.addAction(selfmenubar.simStep);
// 		menu.addSeparator();
		/*menu.addAction(fitWindowSizeAct);*/
		menu.exec(mapToGlobal(QPoint(event->x(), event->y())));
	}
}
void ObjectWindow::fitWindowSizeSlot()
{
// 	QWidget* widget = this->widget();
// 	if(widget)
// 	{
// 		QSize size(widget->sizeHint());
// 		if(size.isEmpty() || size == QSize(1, 1))
// 			size = QSize(640, 480);
// 		QWidget* parent = (QWidget*)this->parent();
// 		parent->setUpdatesEnabled(false);
// 		widget->setFixedSize(size);
// 		parent->layout()->update();
// 		QApplication::processEvents();
// 		widget->setMinimumSize(QSize(0, 0));
// 		widget->setMaximumSize(QSize(16777215, 16777215));
// 		parent->layout()->update();
// 		QApplication::processEvents();
// 		parent->setUpdatesEnabled(true);
// 		parent->repaint();
// 	}
}


void ObjectWindow::copy()
{
// 	QApplication::clipboard()->clear();
// 	if(objectGlWidget)
// 		QApplication::clipboard()->setPixmap(QPixmap::fromImage(objectGlWidget->grabFrameBuffer()));
}