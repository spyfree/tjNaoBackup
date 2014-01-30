#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <QFileDialog>
#include <QApplication>
#include <QMainWindow>
#include <QTimer>

#include "Document.h"
#include "Platform/SystemCall.h"
#include "../SimulationCore/Simulation/Simulation.h"
#include "../SimulationCore/Controller/Controller.h"

#include "Platform/SimuQt/RoboCupCtrl.h"
#include "../SimulationCore/Tools/SimGraphics.h"
#include "Module/Actuator/Actuator.h"

Connection* Connection::start = 0;
Simulation* Connection::simulation;
SimulatorGui* Connection::simulatorGui;

Connection::Connection(const char* sceneName)
: sceneName(sceneName)
{
	next = start;
	start = this;
}

void SimulatorGui::print(const std::string& text)
{
	//Document::document->print(text.c_str());
}

void SimulatorGui::clear()
{
	//Document::document->clearConsole();
}

void SimulatorGui::setUpdateDelay(unsigned ms)
{
	Document::document->setUpdateViewsDelay(ms);
}

void SimulatorGui::setStatusText(const std::string& text)
{
	//Document::document->setStatusText(text.c_str());
}


Document* Document::document = 0;
Document::Document(QMainWindow* parent, Actuator* actuatorThread, SmartPtr<Synchronizer> _synchro)
:QObject(parent), opened(false), simulation(0),controller(0), running(false),
performStep(false), updateViews(false), lastUpdateViewsTime(0),
updateViewsDelay(100), settings("TJArk", "NaoSimulation"), actuatorThread(actuatorThread), theSynchronizer(_synchro)
{
	Q_ASSERT(document == 0);
	document = this;
	recording = false;
}
Document::~Document()
{
	Q_ASSERT(!opened);
	Q_ASSERT(!simulation);
	Q_ASSERT(!controller);
	Q_ASSERT(!running);
	Q_ASSERT(document == this);
	document = 0;
}
Simulation* Document::getSimulation() const
{
	return simulation;
}
bool Document::isOpened() const
{
	return opened;
}
void Document::newSlot()
{
	closeSlot();

	file.clear();
	initDocument();
}
void Document::closeSlot()
{
	if(opened)
	{
		file.clear();
		opened = false;
		running = false;
		performStep = false;
		updateViews = false;
		if(controller)
		{
			controller->destroy();
			delete controller;
			controller = 0;
		}
		if(simulation)
		{
			delete simulation;
			simulation = 0;
		}
	}
}

void Document::openFileSlot(const QString& fileName)
{
	qobject_cast<QMainWindow *>(parent())->setUpdatesEnabled(false);
	closeSlot();
	QFile xmlFile(fileName);
	if(!xmlFile.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(qobject_cast<QMainWindow*>(parent()), tr("SimRobot"),
			tr("Cannot read file %1:\n%2.").arg(fileName).arg(xmlFile.errorString()));
		qobject_cast<QMainWindow *>(parent())->setUpdatesEnabled(true);
		return;
	}
	QFileInfo fileInfo(fileName);
	file = fileInfo.absoluteDir().canonicalPath() + '/' + fileInfo.fileName();
	initDocument();
	qobject_cast<QMainWindow *>(parent())->setUpdatesEnabled(true);
}
void Document::openSlot()
{
	QString fileName = QFileDialog::getOpenFileName(qobject_cast<QMainWindow*>(parent()), 
		tr("Open File"), settings.value("OpenDir", "").toString(), tr("Robot Simulatoin FIles (*.ros)"));
	if(fileName.isEmpty())
		return;
	settings.setValue("OpenDir", QFileInfo(fileName).dir().path());
	openFileSlot(fileName);
}
void Document::simResetSlot()
{
	bool wasRunning = running;
	running = false;
	if(controller)
	{
		controller->destroy();
		delete controller;
		controller = 0;
	}
	if(simulation)
	{
		delete simulation;
		simulation = 0;
		//objectDescriptionTree.clear();
		//emit updateObjectTreeSignal();
		updateObjects();
	}
	compile();
	if(simulation)
	{
		running = wasRunning;
		if(running)
			startTimer(0);
	}
}
void Document::simStartSlot()
{
	if(running)
	{
		running = false;
	}else{
		if(!simulation)
			simResetSlot();
		if(simulation)
		{
			running = true;
			startTimer(0);
		}
	}
}
void Document::simStepSlot()
{
	if(running)
	{
		running = false;
		performStep = true;
		return;
	}
	if(!simulation)
		simResetSlot();
	if(simulation)
	{
		performStep = true;
		startTimer(0);
	}
}

void Document::shootObjectSlot()
{
	SimObject* objectBall = simulation->getObjectReference("RoboCup.balls.ball");
	SimObject* objectRobot = simulation->getObjectReference(RoboCupCtrl::getRobotFullName());
	if(objectBall){
		//simulation->resetDynamicsForObject(objectBall);
		double offset = objectRobot->getNumberOfChildNodes() && objectRobot->getChildNode(0)->getName() != "CM5Box" ? -pi_2 : 0;
		Vector3d rPos = objectRobot->getPosition();
		Matrix3d rRot = objectRobot->getRotation();
		rRot.rotateZ(offset);
		Vector3d startPos = rPos +rRot * Vector3d(0,-1,0.1);
		Vector3d vBall = rRot * Vector3d(0,1,0) * 9;
		simulation->setObjectLinearVelocity(objectBall, 0, 0, 0);
		simulation->setObjectAngularVelocity(objectBall, 0, 0, 0);
		simulation->moveObject(objectBall, startPos);
		simulation->setObjectLinearVelocity(objectBall, vBall.v[0], vBall.v[1], 0);
	}
}
void Document::startRecord()
{
	GraphicsManager* graphic= simulation->getGraphicsManager();
	QString fileName = QFileDialog::getSaveFileName(qobject_cast<QMainWindow*>(parent()),
		tr("Saving AVI File..."),settings.value("Save File", "").toString(),tr("AVI Files (*.avi)"));
	if(fileName.isEmpty())
		return;
	std::string filename(fileName.toAscii().constData());
	if(graphic->beginAVIRecording(filename))
	{
		recording = true;
	}else
	{
		recording = false;
	}
}
void Document::endRecord()
{
	if(recording){
		GraphicsManager* graphic= simulation->getGraphicsManager();
		graphic->endAVIRecording();
		recording = false;
	}
}

void Document::saveScreenShot()
{
	if(simulation){
		QString fileName = QFileDialog::getSaveFileName(qobject_cast<QMainWindow*>(parent()),
			tr("Saving TGA File..."),settings.value("Save File", "").toString(),tr("TGA Files (*.tga)"));
		if(fileName.isEmpty())
			return;
		std::string filename(fileName.toAscii().constData());
		GraphicsManager* graphic= simulation->getGraphicsManager();
		// // 	graphic->toggleLightShowCones();
		// // 	simulation->toggleShowFPS();
		int width, height;
		graphic->getCurrentImageDimensions(height, width);
		graphic->saveScreenshot(width,height, filename, false);
	}
}
//void Document::shootObjectSlot()
//{
//	SimObject* objectBall = simulation->getObjectReference("RoboCup.balls.ball");
//	SimObject* objectRobot = simulation->getObjectReference(RoboCupCtrl::getRobotFullName());
//	if(objectBall){
//		//simulation->resetDynamicsForObject(objectBall);
//		double offset = objectRobot->getNumberOfChildNodes() && objectRobot->getChildNode(0)->getName() != "CM5Box" ? -pi_2 : 0;
//		Vector3d rPos = objectRobot->getPosition();
//		Matrix3d rRot = objectRobot->getRotation();
//		rRot.rotateZ(offset);
//		Vector3d startPos = rPos +rRot * Vector3d(-0.05,-0.6,0.18);
//		Vector3d vBall = rRot * Vector3d(0,1,0) * 4;
//		simulation->setObjectLinearVelocity(objectBall, 0, 0, 0);
//		simulation->setObjectAngularVelocity(objectBall, 0, 0, 0);
//		simulation->moveObject(objectBall, startPos);
//		simulation->setObjectLinearVelocity(objectBall, vBall.v[0], vBall.v[1], 0);
//	}
//
//}

void Document::initDocument()
{
	opened = true;
	running = false;
	performStep = false;
	updateViews = false;
	recording = false;
}
void Document::compile()
{
	if(file.isEmpty())
	{
		return;
	}
	QDir::setCurrent(QFileInfo(file).dir().path());
	QString roSiFilePath = QFileInfo(tr(".")).dir().path() + "/RoSi.xsd";
	Q_ASSERT(simulation == 0);
	Q_ASSERT(controller == 0);
	simulation = new Simulation();
	//
	std::string stdFIle(file.toAscii().constData());
	std::string stdRoSiFile(roSiFilePath.toAscii().constData());
	if(!simulation->loadFile(stdFIle, stdRoSiFile))
	{
		std::vector<ErrorDescription> errors;
		simulation->getFirstError(errorDescription);
		simulation->getAllErrors(errors);
		QString messages = tr(errorDescription.text.c_str());
		delete simulation;
		simulation  = 0;
		QApplication::beep();
		std::vector<ErrorDescription>::const_iterator it;
		printf("-------Simulation Loading Errors---------\n");
		printf("Scene Compiler Errors:\n");
		for(it = errors.begin(); it < errors.end(); it++)
			printf(((*it).text+"\n").c_str());
		printf("----------------------\n");
	}else
	{
		SimObject* objectTree = simulation->getObjectReference("");
		const std::string& sceneName(objectTree->getName());
		Connection* pThis = Connection::start;
		while (pThis && !(sceneName == pThis->sceneName))
			pThis = pThis->next;
		if(pThis)
		{
			controller = pThis->createController(*simulation, simulatorGui);
			controller->init();
			//simulation->getObjectDescriptionTree(objectDescriptionTree);
			//simulation->resetSceneGraphChanged();
			//emit updateObjectTreeSignal();
			updateObjects();
			//statusMessage = tr("Compilation successful (%1ms)").arg(Main::getSystemTime() - startTime);
			//emit updateStatusBarSignal();
		}
		else
		{
			//simulation->getObjectDescriptionTree(objectDescriptionTree);
			//simulation->resetSceneGraphChanged();
			///emit updateObjectTreeSignal();
			updateObjects();
			//statusMessage = tr("There's no controller for this scene");
			//emit updateStatusBarSignal();
		}    
	}
}
void Document::timerEvent(QTimerEvent* event)
{
	if(performStep || running)
	{
		performStep = false;
		if(controller)
		{
			#ifdef ODESimulationActuator
			if(actuatorThread && actuatorThread->isRunning())
				theSynchronizer->getEventbyName("Actuator->ODESimulation")->wait();
			#endif
			controller->execute();
			if(simulation->getResetFlag())
				simulation->resetSimulation();
			if(simulation->hasSceneGraphChanged())
			{
				//simulation->getObjectDescriptionTree(objectDescriptionTree);
				//simulation->resetSceneGraphChanged();
				updateObjects();
				//emit updateObjectTreeSignal();
			}
		}
// 		emit updateActuatorsSignal();
		simulation->doSimulationStep();
		emit updateActuatorThreadSignal();
		updateViews = true;
	}
	if(updateViews)
	{
		unsigned int t = SystemCall::getCurrentSystemTime();
		if(t - lastUpdateViewsTime >= updateViewsDelay)
		{
			lastUpdateViewsTime = t;
			updateViews = false;
			emit updateViewsSignal();
		}
		//Recording Clips
		if(recording)
		{
			GraphicsManager* graphic= simulation->getGraphicsManager();
			int width, height;
			graphic->getCurrentImageDimensions(height, width);
			if(!graphic->addAVIFrame(width, height))
			{
				recording = false;
			}
		}
	}else
	{
		killTimer(event->timerId());
	}
}
void Document::setUpdateViewsDelay(unsigned int ms)
{
	updateViewsDelay = ms;
}

void Document::updateObjects()
{
	// TODO: update objectwindow
	emit updateObjectWindowSignal();
}