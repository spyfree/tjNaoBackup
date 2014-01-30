/**
 * @file Hinge.cpp
 *
 * Implementation of class Hinge
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 */

#include "Hinge.h"
#include "Motor.h"
#include "MotorController.h"
#include "../Simulation/Sensorport.h"
#include "../Simulation/Actuatorport.h"
#include "../Simulation/Simulation.h"
#include "../Simulation/PhysicsParameterSet.h"
#include "../PhysicalObjects/PhysicalObject.h"
#include "../Tools/ODETools.h"
#include "../Tools/SimGraphics.h"


Hinge::Hinge(const AttributeSet& attributes) : anchorPoint(position), axis(0)
{
  name = ParserUtilities::getValueFor(attributes, "name");
}

bool Hinge::parseAttributeElements(const std::string& name,
                                   const AttributeSet& attributes,
                                   int line, int column,
                                   ErrorManager* errorManager)
{
  if(name == "AnchorPoint")
  {
    anchorPoint = parentNode->getPosition();
    anchorPoint += parentNode->getRotation() * (ParserUtilities::toPoint(attributes));
  }
  else if(name == "Axis")
  {
    parseAxis(name, attributes);
    axis = &axes[0];
  }
  else if(name == "AngularMotor" || name == "VelocityMotor")
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

SimObject* Hinge::clone() const
{
  Hinge* newHinge = new Hinge();
  //copy SimObject members
  newHinge->copyStandardMembers(this);
  bool createsNewMovID = newHinge->adjustMovableID();
  //copy joint members
    //axes
  std::vector<AxisDescription>::const_iterator axesIter;
  for(axesIter = axes.begin(); axesIter != axes.end(); ++axesIter)
  {
    newHinge->axes.push_back(*axesIter);
    if(axesIter->motor != NULL)
    {
      newHinge->motors.push_back(axesIter->motor->clone());
      newHinge->axes.back().motor = newHinge->motors.back();
    }
  }
    //currentMotor
  if(!newHinge->motors.empty())
    newHinge->currentMotor = newHinge->motors.back();

  //copy hinge members
  newHinge->anchorPoint = this->anchorPoint;
  newHinge->axis = &(newHinge->axes[0]);

  //copy child nodes
  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newHinge->addChildNode(childNode, false);
  }

  if(createsNewMovID)
    simulation->removeCurrentMovableID();

  SimObject* newObject = newHinge;
  return newObject;
}

void Hinge::addToLists(std::vector<Sensorport*>& sensorportList,
                       std::vector<Actuatorport*>& actuatorportList,
                       std::vector<Actuator*>& actuatorList)
{
  if(axis)
  {
    Sensorport* valueSensor = new Sensorport
          ("jointSensorValue", AXIS_JOINT, doubleSensor, this, axis->minValue, axis->maxValue);
    sensorportList.push_back(valueSensor);
    if(axis->motor)
    {
      std::string actuatorDescription;
      std::string sensorDescription;
      double minVal=0.0, maxVal=0.0;
      if(axis->motor->getMotorType() == VELOCITY)
      {
        actuatorDescription = "motorVelocity";
        sensorDescription = "motorVelocitySensor";
        minVal = -axis->motor->getMaxVelocity();
        maxVal = axis->motor->getMaxVelocity();
      }
      else if(axis->motor->getMotorType() == ANGULAR)
      {
        actuatorDescription = "motorAngle";
        sensorDescription = "motorAngleSensor";
        minVal = axis->minValue;
        maxVal = axis->maxValue;
      }
      Actuatorport* value = new Actuatorport(actuatorDescription, this, AXIS_MOTOR, minVal, maxVal);
      actuatorportList.push_back(value);
      Sensorport* valueSensor = new Sensorport(sensorDescription, AXIS_MOTOR, doubleSensor, this, minVal, maxVal);
      sensorportList.push_back(valueSensor);
      actuatorList.push_back(this);
    }
  }
}

void Hinge::addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
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

  if(axis->motor != 0)
  {
    std::string actuatorDescription;
    std::string sensorDescription;
    if(axis->motor->getMotorType() == VELOCITY)
    {
      actuatorDescription = "motorVelocity";
      sensorDescription = "motorVelocitySensor";
    }
    else if(axis->motor->getMotorType() == ANGULAR)
    {
      actuatorDescription = "motorAngle";
      sensorDescription = "motorAngleSensor";
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

void Hinge::setValue(double value, int port)
{
  /* TEMPORARY FIX <- WHAT_IS_THIS?*/
  if(port == AXIS_MOTOR)
  {
    double newValue = value;
    if(axis->motor->getMotorType() == ANGULAR)
    {
      /* WHAT IS THIS? - Maybe there is a better solution...*/
      if(value == axis->maxValue || value == axis->minValue)
      {
        if(value == 0.0 && axis->minValue == 0.0)
          newValue = -0.01;
        else if(value == 0.0 && axis->maxValue == 0.0)
          newValue = 0.01;
        else
          newValue -= (value *dReal(0.01));
      }
      newValue = Functions::toRad(newValue);
    }
    destinationValue = newValue;
    axis->motor->setValue(newValue, port);
  }
}

void Hinge::act(bool initial)
{
  if(!initial)
  {
    if(axis->motor->getMotorType() == VELOCITY)
    {
      dReal desiredVelocity = dReal(axis->motor->getDesiredVelocity());
      dJointSetHingeParam(physicalJoint, dParamFMax, dReal(axis->motor->getMaxForce()));
      dJointSetHingeParam(physicalJoint, dParamVel, dReal(desiredVelocity));
    }
    else if(axis->motor->getMotorType() == ANGULAR)
    {
      //dJointGetHingeAngle scales values to [-pi,..,pi]
      //This may cause problems for the motor controller, if the current angle
      //is near (-)pi, therefore the angles have to be modified in some cases
      double currentJointAngle(dJointGetHingeAngle(physicalJoint));
      if(destinationValue > 0.0 && lastValue > 0.0 && 
         currentJointAngle < 0.0 && fabs(currentJointAngle) > 2.0)
      {
        currentJointAngle += 2*M_PI;
      }
      else if(destinationValue < 0.0 && lastValue < 0.0 && 
              currentJointAngle > 0.0 && fabs(currentJointAngle) > 2.0)
      {
        currentJointAngle -= 2*M_PI;
      }
      double controllerOutput = axis->motor->getControllerOutput(currentJointAngle);
      dJointSetHingeParam(physicalJoint, dParamFMax, dReal(axis->motor->getMaxForce()));
      dJointSetHingeParam(physicalJoint, dParamVel, dReal(controllerOutput));
      lastValue = currentJointAngle;
    }
  }
}

void Hinge::computeValue(double& value, int portId)
{
  if(portId == AXIS_JOINT)
    value = (dJointGetHingeAngle(physicalJoint) * 180.0 / M_PI);
  else if(portId == AXIS_MOTOR)
  {
    if(axis->motor->getMotorType() == VELOCITY)
      value = (dJointGetHingeParam(physicalJoint, dParamVel));
    else if(axis->motor->getMotorType() == ANGULAR)
      value = (dJointGetHingeAngle(physicalJoint) * 180.0 / M_PI);
  }
}

void Hinge::createJointPhysics()
{
  Vector3d physAxis = axis->direction;
  physAxis.rotate(rotation);

  physicalJoint = dJointCreateHinge(*simulation->getPhysicalWorld(), 0);  
  PhysicalObject* o1 = getPhysicalParentObject();
  PhysicalObject* o2 = getPhysicalChildObject(this);
  assert (o1 != 0 && o2 != 0);
  if(o1 && o2)
  {
    dJointAttach(physicalJoint, *(o2->getBody()), *(o1->getBody()));
    //set hinge joint parameter
    dJointSetHingeAnchor(physicalJoint, dReal(anchorPoint.v[0]), dReal(anchorPoint.v[1]), dReal(anchorPoint.v[2]));
    dJointSetHingeAxis(physicalJoint, dReal(physAxis.v[0]), dReal(physAxis.v[1]), dReal(physAxis.v[2]));

    if(axis->cfm != -1.0)
      dJointSetHingeParam(physicalJoint, dParamCFM, dReal(axis->cfm));

    if(axis->limited || (axis->motor != 0 && axis->motor->getMotorType() == ANGULAR))
    {
      double minHingeLimit(Functions::toRad(axis->minValue));
      double maxHingeLimit(Functions::toRad(axis->maxValue));
      //Set physical limits to higher values (+10%) to avoid strange hinge effects.
      //Otherwise, sometimes the motor exceeds the limits.
      assert(maxHingeLimit >= minHingeLimit);
      double totalHingeRange(maxHingeLimit - minHingeLimit);
      double internalTolerance(totalHingeRange / 10);
      if(axis->motor != 0 && axis->motor->getMotorType() == ANGULAR)
      {
        minHingeLimit -= internalTolerance;
        maxHingeLimit += internalTolerance;
      }
      dJointSetHingeParam(physicalJoint, dParamLoStop, dReal(minHingeLimit));
      dJointSetHingeParam(physicalJoint, dParamHiStop, dReal(maxHingeLimit));
      // this has to be done due to the way ODE sets joint stops
      dJointSetHingeParam(physicalJoint, dParamLoStop, dReal(minHingeLimit));
      if(axis->fullScaleDeflectionCFM != -1.0)
        dJointSetHingeParam(physicalJoint, dParamStopCFM, dReal(axis->fullScaleDeflectionCFM));
      if(axis->fullScaleDeflectionERP != -1.0)
        dJointSetHingeParam(physicalJoint, dParamStopERP, dReal(axis->fullScaleDeflectionERP));
    }
  }
}

void Hinge::applyFriction(bool initial)
{
  if(!initial)
  {
    if(motors.size() == 0)
    {
      double fmax = axis->friction * fabs((simulation->getPhysicsParameters())->getGravity());
      double velocity = 0.0;
      dJointSetHingeParam(physicalJoint, dParamFMax, dReal(fmax));
      dJointSetHingeParam(physicalJoint, dParamVel, dReal(velocity));
    }
  }
}

void Hinge::drawObject(const VisualizationParameterSet& visParams)
{
  if(visParams.surfaceStyle == VisualizationParameterSet::PHYSICS_WIREFRAME ||
     visParams.surfaceStyle == VisualizationParameterSet::PHYSICS_FLAT ||
     visParams.surfaceStyle == VisualizationParameterSet::PHYSICS_SMOOTH ||
     visParams.surfaceStyle == VisualizationParameterSet::MIXED_APPEARANCE ||
     visParams.surfaceStyle == VisualizationParameterSet::MIXED_PHYSICS)
  {
    dVector3 tempA;
    dJointGetHingeAxis(physicalJoint, tempA);
    Vector3d tempAxis(tempA[0], tempA[1], tempA[2]);
    tempAxis *= 0.1;

    dJointGetHingeAnchor(physicalJoint, tempA);
    Vector3d tempAnchor(tempA[0],tempA[1], tempA[2]);

    if(visParams.surfaceStyle == VisualizationParameterSet::MIXED_APPEARANCE)
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glShadeModel(GL_FLAT);
      GLHelper::getGLH()->drawHinge(tempAnchor, tempAxis, true);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glShadeModel(GL_SMOOTH);
    }
    else
    {
      GLHelper::getGLH()->getGLH()->drawHinge(tempAnchor, tempAxis, false);
    }
  }
}

void Hinge::updateParameters(const HingeParameterUpdate& params)
{
  if(axis->motor == 0)                        // No motor, no parameter update
    return;
  if(axis->motor->getMotorType() == VELOCITY) // Currently, only angular motors are supported
    return;
  axis->motor->setNewParameters(params.maxMotorForce, params.maxMotorVelocity, params.motorPControlValue, 
    params.motorIControlValue, params.motorDControlValue);
}
