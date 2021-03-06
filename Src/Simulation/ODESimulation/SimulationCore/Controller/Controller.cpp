
#include "Controller.h"
#include "../Simulation/Simulation.h"

Controller::Controller() : simulation(*Connection::simulation), simulatorGui(*Connection::simulatorGui) {}

void Controller::resetSimulation()
{
  simulation.setResetFlag();
}

bool Controller::isSimulationReady() const
{
  return simulation.isSimulationReady();
}

std::string Controller::getSimulationFileName() const
{
  return simulation.getSimulationFileName();
}

unsigned int Controller::getSimulationStep() const
{
  return simulation.getSimulationStep();
}

Surface* Controller::getSurface(const std::string& name) const
{
  return simulation.getSurface(name);
}

SimObject* Controller::getObjectReference(const std::string& objectName)
{
  return simulation.getObjectReference(objectName);
}

void Controller::setBackgroundColor(const Vector3d& color)
{
  simulation.setBackgroundColor(color);
}

int Controller::getActuatorportId(const std::string& portName) const
{
  return simulation.getActuatorportId(portName);
}

int Controller::getActuatorportIdByPartOfName(const std::vector<std::string>& parts) const
{
  return simulation.getActuatorportIdByPartOfName(parts);
}

double Controller::getActuatorportMinValue(int id) const
{
  return simulation.getActuatorportMinValue(id);
}

double Controller::getActuatorportMaxValue(int id) const
{
  return simulation.getActuatorportMaxValue(id);
}

void Controller::setActuatorport(int id, double value)
{
  simulation.setActuatorport(id, value);
}

int Controller::getSensorportId(const std::string& portName) const
{
  return simulation.getSensorportId(portName);
}

int Controller::getSensorportIdByPartOfName(const std::vector<std::string>& parts) const
{
  return simulation.getSensorportIdByPartOfName(parts);
}

const std::vector<int>& Controller::getSensorDimensions(int id) const
{
  return simulation.getSensorDimensions(id);
}

void Controller::getSensorValue(int id, bool& value)
{
  simulation.getSensorportValue(id, value);
}

void Controller::getSensorValue(int id, double& value)
{
  simulation.getSensorportValue(id, value);
}

void Controller::getSensorValue(int id, double*& value)
{
  simulation.getSensorportValue(id, value);
}

void Controller::getSensorValue(int id, unsigned char*& value)
{
  simulation.getSensorportValue(id, value);
}

void Controller::addView(View* view, const std::string& name)
{
  simulation.addView(view, name);
}

void Controller::print(const std::string& text)
{
  simulatorGui.print(text);
}

void Controller::printLn(const std::string& text)
{
  print(text + "\r\n");
}

void Controller::clear()
{
  simulatorGui.clear();
}

void Controller::setStatusText(const std::string& text)
{
  simulatorGui.setStatusText(text);
}
/*
const std::string& Controller::getStatusText() const
{
  return statusText;  
}
*/
void Controller::setGUIUpdateDelay(unsigned ms)
{
  simulatorGui.setUpdateDelay(ms);
}

void Controller::registerDrawing(const std::string& fullObjectName, Controller3DDrawing* drawing)
{
  simulation.registerControllerDrawing(fullObjectName, drawing);
}

void Controller::unregisterDrawing(const std::string& fullObjectName, Controller3DDrawing* drawing)
{
  simulation.unregisterControllerDrawing(fullObjectName, drawing);
}

void Controller::registerDrawingByPartOfName(const std::vector<std::string>& parts,
                                             Controller3DDrawing* drawing)
{
  simulation.registerControllerDrawingByPartOfName(parts, drawing);
}

void Controller::unregisterDrawingByPartOfName(const std::vector<std::string>& parts,
                                               Controller3DDrawing* drawing)
{
  simulation.unregisterControllerDrawingByPartOfName(parts, drawing);
}

void Controller::moveObject(SimObject* object, const Vector3d& pos)
{
  simulation.moveObject(object, pos);
}
  
void Controller::moveObject(SimObject* object, const Vector3d& pos, const Vector3d& rot)
{
  simulation.moveObject(object, pos, rot);
}

std::vector<Actuator*>* Controller::getActuatorList()
{
  return simulation.getActuatorList();
}

void Controller::changePhysicsParametersAtRunTime(const PhysicsParameterSet& newParams)
{
  simulation.changePhysicsParametersAtRunTime(newParams);
}

PhysicsParameterSet* Controller::getPhysicsParameters()
{
  return simulation.getPhysicsParameters();
}
