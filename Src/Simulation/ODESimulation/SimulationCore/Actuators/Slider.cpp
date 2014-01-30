/**
 * @file Slider.cpp
 * 
 * Implementation of class Slider
 *
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 */ 

#include "Slider.h"
#include "Motor.h"
#include "MotorController.h"
#include "../Simulation/Sensorport.h"
#include "../Simulation/Actuatorport.h"
#include "../Simulation/Simulation.h"
#include "../PhysicalObjects/PhysicalObject.h"
#include "../Simulation/PhysicsParameterSet.h"
#include "../Tools/SimGraphics.h"

Slider::Slider() 
{
}

Slider::Slider(const AttributeSet& attributes)
{
  name = ParserUtilities::getValueFor(attributes, "name");
}


bool Slider::parseAttributeElements(const std::string& name, 
                                    const AttributeSet& attributes,
                                    int line, int column,
                                    ErrorManager* errorManager)
{
  if(name == "Axis")
  {
    parseAxis(name, attributes);
    axis = &axes[0];
  }
  else if(name == "AngularMotor")
  {
    parseMotor(name, attributes);
  }
  else if(name == "VelocityMotor")
  {
    parseMotor(name, attributes);
  }
  else if(name == "Deflection")
  {
    axes.back().minValue = (ParserUtilities::toDouble(ParserUtilities::getValueFor(attributes, "min")));
    axes.back().maxValue = (ParserUtilities::toDouble(ParserUtilities::getValueFor(attributes, "max")));
    axes.back().limited = true;
  }
  else if(name == "PController")
  {
    axis->motor->parseController(name, attributes);
  }
  else if(name == "PIDController")
  {
    axis->motor->parseController(name, attributes);
    ((PIDController*)axis->motor->getMotorController())->setTimeStep(simulation->getStepLength());
  }
  else if(name == "Friction")
  {
    if(!axes.empty())
      axis->friction = ParserUtilities::toDouble(ParserUtilities::getValueFor(attributes, "value"));
  }
  return true;
}

SimObject* Slider::clone() const
{
  Slider* newSlider = new Slider();
  //copy SimObject members
  newSlider->copyStandardMembers(this);
  bool createsNewMovID = newSlider->adjustMovableID();
  //copy joint members
    //axes
  std::vector<AxisDescription>::const_iterator axesIter;
  for(axesIter = axes.begin(); axesIter != axes.end(); ++axesIter)
  {
    newSlider->axes.push_back(*axesIter);
    if(axesIter->motor != NULL)
    {
      newSlider->motors.push_back(axesIter->motor->clone());
      newSlider->axes.back().motor = newSlider->motors.back();
    }

  }
    //currentMotor
  if(!newSlider->motors.empty())
    newSlider->currentMotor = newSlider->motors.back();

  //copy slider members
  newSlider->axis = &(newSlider->axes[0]);

  //copy child nodes
  newSlider->childNodes.clear();
  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newSlider->addChildNode(childNode, false);
  }

  if(createsNewMovID)
    this->simulation->removeCurrentMovableID();

  SimObject* newObject = newSlider;
  return newObject;
}

void Slider::addToLists(std::vector<Sensorport*>& sensorportList,
                        std::vector<Actuatorport*>& actuatorportList,
                        std::vector<Actuator*>& actuatorList) 
{
  if(this->axis != 0)
  {
    Sensorport* valueSensor = new Sensorport
          ("jointSensorValue", AXIS_JOINT, doubleSensor, this, axis->minValue, axis->maxValue);
    sensorportList.push_back(valueSensor);

    if(this->axis->motor != 0)
    {
      std::string actuatorDescription;
      std::string sensorDescription;
      double minVal=0.0, maxVal=0.0;
      if(this->axis->motor->getMotorType() == VELOCITY)
      {
        actuatorDescription = "motorVelocity";
        sensorDescription = "motorVelocitySensor";
        minVal = -this->axis->motor->getMaxVelocity();
        maxVal = this->axis->motor->getMaxVelocity();
      }
      else if(this->axis->motor->getMotorType() == ANGULAR)
      {
        actuatorDescription = "motorPosition";
        sensorDescription = "motorPositionSensor";
        minVal = this->axis->minValue;
        maxVal = this->axis->maxValue;
      }
      Actuatorport* value = new Actuatorport(actuatorDescription, this, AXIS_MOTOR, minVal, maxVal);
      actuatorportList.push_back(value);
      Sensorport* valueSensor = new Sensorport(sensorDescription, AXIS_MOTOR, doubleSensor, this, minVal, maxVal);
      sensorportList.push_back(valueSensor);
      actuatorList.push_back(this);
    }
  }

}

void Slider::addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
                               int depth) 
{
  SimSensor::addToDescriptions(objectDescriptionTree, depth);  
  //Add Sensorports:
  ObjectDescription valueDesc;
  valueDesc.name = "jointSensorValue";
  valueDesc.fullName = fullName + ".jointSensorValue";
  valueDesc.depth = depth + 1;
  valueDesc.type = OBJECT_TYPE_SENSORPORT;
  objectDescriptionTree.push_back(valueDesc);

  if(this->axis->motor !=0)
  {
    std::string actuatorDescription;
    std::string sensorDescription;
    if(this->axis->motor->getMotorType() == VELOCITY)
    {
      actuatorDescription = "motorVelocity";
      sensorDescription = "motorVelocitySensor";
    }
    else if(this->axis->motor->getMotorType() == ANGULAR)
    {
      actuatorDescription = "motorPosition";
      sensorDescription = "motorPositionSensor";
    }

    //Add Actuatorport:
    valueDesc.name = actuatorDescription;
    valueDesc.fullName = fullName + "." + actuatorDescription;
    valueDesc.depth = depth + 1;
    valueDesc.type = OBJECT_TYPE_ACTUATORPORT;
    objectDescriptionTree.push_back(valueDesc);
    //Add Sensorport:
    valueDesc.name = sensorDescription;
    valueDesc.fullName = fullName + "." + sensorDescription;
    valueDesc.depth = depth + 1;
    valueDesc.type = OBJECT_TYPE_SENSORPORT;
    objectDescriptionTree.push_back(valueDesc);
  }
}


void Slider::setValue(double value, int port)
{
  /* TEMPORARY FIX */
  if(port == AXIS_MOTOR)
  {
    double newValue = value;
    if(this->axis->motor->getMotorType() == ANGULAR)
    {
      if(value == this->axis->maxValue || value == this->axis->minValue)
      {
        if(value == 0.0 && this->axis->minValue == 0.0)
        {
          newValue = -0.01;
        }
        else if(value == 0.0 && this->axis->maxValue == 0.0)
        {
          newValue = 0.01;
        }
        else
          newValue -= (value *dReal(0.01));
      }
    }
    this->axis->motor->setValue(newValue, port);
  }
}

void Slider::act(bool initial)
{
  if(!initial)
  {
    if(this->axis->motor->getMotorType() == VELOCITY)
    {
      dReal desiredVelocity = dReal(this->axis->motor->getDesiredVelocity());
      dJointSetSliderParam(this->physicalJoint, dParamFMax, dReal(this->axis->motor->getMaxForce()));
      dJointSetSliderParam(this->physicalJoint, dParamVel, desiredVelocity);
    }
    else if(this->axis->motor->getMotorType() == ANGULAR)
    {
      double controllerOutput = this->axis->motor->getControllerOutput(dJointGetSliderPosition(this->physicalJoint));
      dJointSetSliderParam(this->physicalJoint, dParamFMax, dReal(this->axis->motor->getMaxForce()));
      dJointSetSliderParam(this->physicalJoint, dParamVel, dReal(controllerOutput));
    }
  }
}

void Slider::computeValue(double& value, int portId)
{
  if(portId == AXIS_JOINT)
    value = (dJointGetSliderPosition(this->physicalJoint));
  else if(portId == AXIS_MOTOR)
  {
    if(this->axis->motor->getMotorType() == VELOCITY)
      value = (dJointGetSliderParam(this->physicalJoint, dParamVel));
    else
      value = (dJointGetSliderPosition(this->physicalJoint));
  }
}



void Slider::createJointPhysics()
{
  Vector3d physAxis = this->axis->direction;
  physAxis.rotate(this->rotation);

  this->physicalJoint = dJointCreateSlider(*this->simulation->getPhysicalWorld(), 0);
  PhysicalObject* o1 = this->getPhysicalParentObject();
  PhysicalObject* o2 = this->getPhysicalChildObject(this);
  assert (o1 != NULL && o2 != NULL);
  if(o1 && o2)
  {
    dJointAttach(this->physicalJoint, *(o2->getBody()), *(o1->getBody()));
    //set slider joint parameter
    dJointSetSliderAxis(this->physicalJoint, dReal(this->axis->direction.v[0]), dReal(this->axis->direction.v[1]), dReal(this->axis->direction.v[2]));

    if(this->axis->cfm != -1.0)
      dJointSetSliderParam(this->physicalJoint, dParamCFM, dReal(this->axis->cfm));

    if(this->axis->limited || (this->axis->motor != 0 && this->axis->motor->getMotorType() == ANGULAR))
    {
      dJointSetSliderParam(this->physicalJoint, dParamLoStop, dReal(this->axis->minValue));
      dJointSetSliderParam(this->physicalJoint, dParamHiStop, dReal(this->axis->maxValue));
      // this has to be done due to the way ODE sets joint stops
      dJointSetSliderParam(this->physicalJoint, dParamLoStop, dReal(this->axis->minValue));

      if(this->axis->fullScaleDeflectionCFM != -1.0)
        dJointSetSliderParam(this->physicalJoint, dParamStopCFM, dReal(this->axis->fullScaleDeflectionCFM));
      if(this->axis->fullScaleDeflectionERP != -1.0)
        dJointSetSliderParam(this->physicalJoint, dParamStopERP, dReal(this->axis->fullScaleDeflectionERP));
    }
  }
}


void Slider::createMotorPhysics()
{
}

void Slider::applyFriction(bool initial)
{
  if(!initial)
  {
    if(this->motors.size()== 0)
    {
      double fmax = axis->friction * fabs((simulation->getPhysicsParameters())->getGravity());
      double velocity = 0.0;
      dJointSetSliderParam(physicalJoint, dParamFMax, dReal(fmax));
      dJointSetSliderParam(physicalJoint, dParamVel, dReal(velocity));
    }
  }
}

void Slider::drawObject(const VisualizationParameterSet& visParams)
{
  if(visParams.surfaceStyle == VisualizationParameterSet::PHYSICS_WIREFRAME ||
     visParams.surfaceStyle == VisualizationParameterSet::PHYSICS_FLAT ||
     visParams.surfaceStyle == VisualizationParameterSet::PHYSICS_SMOOTH ||
     visParams.surfaceStyle == VisualizationParameterSet::MIXED_APPEARANCE ||
     visParams.surfaceStyle == VisualizationParameterSet::MIXED_PHYSICS)
  {
    dVector3 tempA;
    dJointGetSliderAxis(physicalJoint, tempA);
    Vector3d tempAxis(tempA[0], tempA[1], tempA[2]);
    tempAxis *= 0.1;
    dBodyID b2 = dJointGetBody(physicalJoint, 0);
    if(b2 == NULL)
      b2 = dJointGetBody(physicalJoint, 1);
    const dReal* b2Pos = dBodyGetPosition(b2);
    Vector3d pos(b2Pos[0], b2Pos[1], b2Pos[2]);

    if(visParams.surfaceStyle == VisualizationParameterSet::MIXED_APPEARANCE)
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glShadeModel(GL_FLAT);
      GLHelper::getGLH()->drawSlider(pos, tempAxis, true);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glShadeModel(GL_SMOOTH);
    }
    else
      GLHelper::getGLH()->drawSlider(pos, tempAxis, false);
  }
}
