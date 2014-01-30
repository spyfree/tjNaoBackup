/**
 * @file WheelSuspension.cpp
 * 
 * Implementation of class WheelSuspension
 *
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 */ 

#include "WheelSuspension.h"
#include "Motor.h"
#include "MotorController.h"
#include "../Simulation/Sensorport.h"
#include "../Simulation/Actuatorport.h"
#include "../Simulation/Simulation.h"
#include "../PhysicalObjects/PhysicalObject.h"
#include "../Simulation/PhysicsParameterSet.h"
#include "../Tools/SimGraphics.h"


WheelSuspension::WheelSuspension() : anchorPoint(position), axis1(0), axis2(0)
{
}

WheelSuspension::WheelSuspension(const AttributeSet& attributes) : anchorPoint(position), axis1(0), axis2(0)
{
  name = ParserUtilities::getValueFor(attributes, "name");
}

bool WheelSuspension::parseAttributeElements(const std::string& name, 
                                       const AttributeSet& attributes,
                                       int line, int column,
                                       ErrorManager* errorManager)
{
  if(name == "AnchorPoint")
  {
    anchorPoint = parentNode->getPosition();
    anchorPoint += parentNode->getRotation() * (ParserUtilities::toPoint(attributes));
  }
  else if(name == "Axis1" || name == "Axis2")
  {
    this->parseAxis(name, attributes);
    if(name == "Axis1")
      this->axis1 = &this->axes[0];
    else if(name == "Axis2")
    {
      this->axis1 = &this->axes[0];
      this->axis2 = &this->axes[1];
    }
  }
  else if(name == "AngularMotor")
  {
    this->parseMotor(name, attributes);
  }
  else if(name == "VelocityMotor")
  {
    this->parseMotor(name, attributes);
  }
  else if(name == "Deflection")
  {
    this->axes.back().minValue = (ParserUtilities::toDouble(ParserUtilities::getValueFor(attributes, "min")));
    this->axes.back().maxValue = (ParserUtilities::toDouble(ParserUtilities::getValueFor(attributes, "max")));
    this->axes.back().limited = true;
  }
  else if(name == "PController")
  {
    this->axes.back().motor->parseController(name, attributes);
  }
  else if(name == "PIDController")
  {
    this->axes.back().motor->parseController(name, attributes);
    ((PIDController*)this->axes.back().motor->getMotorController())->setTimeStep(this->simulation->getStepLength());
  }
  else if(name == "Friction")
  {
    if(!this->axes.empty())
      this->axes.back().friction = ParserUtilities::toDouble(ParserUtilities::getValueFor(attributes, "value"));
  }


  return true;
}

SimObject* WheelSuspension::clone() const
{
  WheelSuspension* newWheelSuspension = new WheelSuspension();
  //copy SimObject members
  newWheelSuspension->copyStandardMembers(this);
  bool createsNewMovID = newWheelSuspension->adjustMovableID();
  //copy joint members
    //axes
  std::vector<AxisDescription>::const_iterator axesIter;
  for(axesIter = axes.begin(); axesIter != axes.end(); ++axesIter)
  {
    newWheelSuspension->axes.push_back(*axesIter);
    if(axesIter->motor != NULL)
    {
      newWheelSuspension->motors.push_back(axesIter->motor->clone());
      newWheelSuspension->axes.back().motor = newWheelSuspension->motors.back();
    }
  }
    //currentMotor
  if(!newWheelSuspension->motors.empty())
    newWheelSuspension->currentMotor = newWheelSuspension->motors.back();

  //copy wheel suspension joint members
  newWheelSuspension->anchorPoint = this->anchorPoint;
  assert(this->axes.size() >= 2);
  newWheelSuspension->axis1 = &(newWheelSuspension->axes[0]);
  newWheelSuspension->axis2 = &(newWheelSuspension->axes[1]);

  //copy child nodes
  newWheelSuspension->childNodes.clear();
  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newWheelSuspension->addChildNode(childNode, false);
  }

  if(createsNewMovID)
    this->simulation->removeCurrentMovableID();

  SimObject* newObject = newWheelSuspension;
  return newObject;
}

void WheelSuspension::addToLists(std::vector<Sensorport*>& sensorportList,
                       std::vector<Actuatorport*>& actuatorportList,
                       std::vector<Actuator*>& actuatorList) 
{
  if(this->axis1 != 0 && this->axis2 != 0)
  {
    Sensorport* valueSensor = new Sensorport
      ("jointAxis1SensorValue", AXIS1_JOINT, doubleSensor, this, this->axis1->minValue, this->axis1->maxValue);
    sensorportList.push_back(valueSensor);
    valueSensor = new Sensorport
      ("jointAxis2SensorValue", AXIS2_JOINT, doubleSensor, this, this->axis2->minValue, this->axis2->maxValue);
    sensorportList.push_back(valueSensor);

    if(this->axis1->motor != 0)
    {
      std::string actuatorDescription;
      std::string sensorDescription;
      double minV=0.0, maxV=0.0;
      if(this->axis1->motor->getMotorType() == VELOCITY)
      {
        actuatorDescription = "motorAxis1Velocity";
        sensorDescription = "motorAxis1VelocitySensor";
        if(this->axis1->limited)
        {
          minV = this->axis1->minValue;
          maxV = this->axis1->maxValue;
        }
        else
        {
          minV = -(this->axis1->motor->getMaxVelocity());
          maxV = this->axis1->motor->getMaxVelocity();
        }
      }
      else if(this->axis1->motor->getMotorType() == ANGULAR)
      {
        actuatorDescription = "motorAxis1Angle";
        sensorDescription = "motorAxis1AngleSensor";
        if(this->axis1->limited)
        {
          minV = this->axis1->minValue;
          maxV = this->axis1->maxValue;
        }
        else
        {
          minV = -180.0;
          maxV = 180.0;
        }
      }

      Actuatorport* value = new Actuatorport
        (actuatorDescription, this, AXIS1_MOTOR, minV, maxV);
      actuatorportList.push_back(value);
      Sensorport* valueSensor = new Sensorport
        (sensorDescription, AXIS1_MOTOR, doubleSensor, this, minV, maxV);
      sensorportList.push_back(valueSensor);
    }
    if(this->axis2->motor != 0)
    {
      std::string actuatorDescription;
      std::string sensorDescription;
      double minV=0.0, maxV=0.0;
      if(this->axis2->motor->getMotorType() == VELOCITY)
      {
        actuatorDescription = "motorAxis2Velocity";
        sensorDescription = "motorAxis2VelocitySensor";
        if(this->axis2->limited)
        {
          minV = this->axis2->minValue;
          maxV = this->axis2->maxValue;
        }
        else
        {
          minV = -(this->axis2->motor->getMaxVelocity());
          maxV = this->axis2->motor->getMaxVelocity();
        }
      }
      // this case is not supported by ODE
      else if(this->axis1->motor->getMotorType() == ANGULAR)
      {
        actuatorDescription = "motorAxis2Angle";
        sensorDescription = "motorAxis2AngleSensor";
        if(this->axis2->limited)
        {
          minV = this->axis2->minValue;
          maxV = this->axis2->maxValue;
        }
        else
        {
          minV = -180.0;
          maxV = 180.0;
        }
      }
      Actuatorport* value = new Actuatorport
        (actuatorDescription, this, AXIS2_MOTOR, minV, maxV);
      actuatorportList.push_back(value);
      Sensorport* valueSensor = new Sensorport
        (sensorDescription, AXIS2_MOTOR, doubleSensor, this, minV, maxV);
      sensorportList.push_back(valueSensor);
    }

    if(this->axis1->motor != 0 || this->axis2->motor != 0)
      actuatorList.push_back(this);
  }
}


void WheelSuspension::addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree, int depth) 
{
  SimSensor::addToDescriptions(objectDescriptionTree, depth);  
  //Add Sensorports:
  ObjectDescription valueDesc;
  valueDesc.name = "jointAxis1SensorValue";
  valueDesc.fullName = fullName + ".jointAxis1SensorValue";
  valueDesc.depth = depth + 1;
  valueDesc.type = OBJECT_TYPE_SENSORPORT;
  objectDescriptionTree.push_back(valueDesc);
  valueDesc.name = "jointAxis2SensorValue";
  valueDesc.fullName = fullName + ".jointAxis2SensorValue";
  valueDesc.depth = depth + 1;
  valueDesc.type = OBJECT_TYPE_SENSORPORT;
  objectDescriptionTree.push_back(valueDesc);

  if(this->axis1->motor != 0)
  {
    std::string actuatorDescription;
    std::string sensorDescription;
    if(this->axis1->motor->getMotorType() == VELOCITY)
    {
      actuatorDescription = "motorAxis1Velocity";
      sensorDescription = "motorAxis1VelocitySensor";
    }
    else if(this->axis1->motor->getMotorType() == ANGULAR)
    {
      actuatorDescription = "motorAxis1Angle";
      sensorDescription = "motorAxis1AngleSensor";
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

  if(this->axis2->motor != 0)
  {
    std::string actuatorDescription;
    std::string sensorDescription;
    if(this->axis2->motor->getMotorType() == VELOCITY)
    {
      actuatorDescription = "motorAxis2Velocity";
      sensorDescription = "motorAxis2VelocitySensor";
    }
// this case is not supported by ODE
    else if(this->axis1->motor->getMotorType() == ANGULAR)
    {
      actuatorDescription = "motorAxis2Angle";
      sensorDescription = "motorAxis2AngleSensor";
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


void WheelSuspension::setValue(double value, int port)
{
  /* TEMPORARY FIX */
  if(port == AXIS1_MOTOR)
  {
    double newValue = value;
    if(this->axis1->motor->getMotorType() == ANGULAR)
    {
      if(value == this->axis1->maxValue || value == this->axis1->minValue)
      {
        if(value == 0.0 && this->axis1->minValue == 0.0)
        {
          newValue = -0.01;
        }
        else if(value == 0.0 && this->axis1->maxValue == 0.0)
        {
          newValue = 0.01;
        }
        else
          newValue -= (value *dReal(0.01));
      }
      newValue = Functions::toRad(newValue);
    }
    this->axis1->motor->setValue(newValue, port);
  }
  else if(port == AXIS2_MOTOR)
  {
    double newValue = value;
    if(this->axis2->motor->getMotorType() == ANGULAR)
    {
      if(value == this->axis2->maxValue || value == this->axis2->minValue)
      {
        if(value == 0.0 && this->axis2->minValue == 0.0)
        {
          newValue = -0.01;
        }
        else if(value == 0.0 && this->axis2->maxValue == 0.0)
        {
          newValue = 0.01;
        }
        else
          newValue -= (value *dReal(0.01));
      }
      newValue = Functions::toRad(newValue);
    }
    this->axis2->motor->setValue(newValue, port);
  }
}


void WheelSuspension::act(bool initial)
{
  if(!initial)
  {
    if(this->axis1->motor != 0)
    {
      if(this->axis1->motor->getMotorType() == VELOCITY)
      {
        double desiredVelocity(this->axis1->motor->getDesiredVelocity());
        dJointSetHinge2Param(this->physicalJoint, dParamFMax, dReal(this->axis1->motor->getMaxForce()));
        dJointSetHinge2Param(this->physicalJoint, dParamVel, dReal(desiredVelocity));
      }
      else if(this->axis1->motor->getMotorType() == ANGULAR)
      {
        double controllerOutput = this->axis1->motor->getControllerOutput(dJointGetHinge2Angle1(this->physicalJoint));
        dJointSetHinge2Param(this->physicalJoint, dParamFMax, dReal(this->axis1->motor->getMaxForce()));
        dJointSetHinge2Param(this->physicalJoint, dParamVel, dReal(controllerOutput));
      }
    }

    if(this->axis2->motor != 0)
    {
      if(this->axis2->motor->getMotorType() == VELOCITY)
      {
        double desiredVelocity = this->axis2->motor->getDesiredVelocity();
        dJointSetHinge2Param(this->physicalJoint, dParamFMax2, dReal(this->axis2->motor->getMaxForce()));
        dJointSetHinge2Param(this->physicalJoint, dParamVel2, dReal(desiredVelocity));
      }
      else if(this->axis2->motor->getMotorType() == ANGULAR)
      {
        //error: mode not supported by ODE
      }
    }
  }

}


void WheelSuspension::computeValue(double& value, int portId)
{
  if(portId == AXIS1_JOINT)
  {
    value = (dJointGetHinge2Angle1(this->physicalJoint) * 180.0 / M_PI);
  }
  else if(portId == AXIS1_MOTOR)
  {
    if(this->axis1->motor->getMotorType() == VELOCITY)
      value = (dJointGetHinge2Param(this->physicalJoint, dParamVel));
    else
      value = (dJointGetHinge2Angle1(this->physicalJoint) * 180.0 / M_PI);
  }
  else if(portId == AXIS2_JOINT)
  {
    // angle not supported for axis 2 by ODE
    // velocity is returned for joint sensor
    value = (dJointGetHinge2Param(this->physicalJoint, dParamVel2));
  }
  else if(portId == AXIS2_MOTOR)
  {
    // angle not supported for axis 2 by ODE
    // velocity is returned for motor sensor
    value = (dJointGetHinge2Param(this->physicalJoint, dParamVel2));
  }
}

void WheelSuspension::createJointPhysics()
{
  this->physicalJoint = dJointCreateHinge2(*this->simulation->getPhysicalWorld(), 0);
  PhysicalObject* o1 = this->getPhysicalParentObject();
  PhysicalObject* o2 = this->getPhysicalChildObject(this);
  assert (o1 != NULL && o2 != NULL);
  if(o1 && o2)
  {
    dJointAttach(this->physicalJoint, *(o1->getBody()), *(o2->getBody()));
    //set hinge joint parameter
    dJointSetHinge2Anchor(this->physicalJoint, dReal(anchorPoint.v[0]), dReal(anchorPoint.v[1]), dReal(anchorPoint.v[2]));
    dJointSetHinge2Axis1(this->physicalJoint, dReal(axis1->direction.v[0]), dReal(axis1->direction.v[1]), dReal(axis1->direction.v[2]));
    dJointSetHinge2Axis2(this->physicalJoint, dReal(axis2->direction.v[0]), dReal(axis2->direction.v[1]), dReal(axis2->direction.v[2]));

    if(this->axis1->cfm != -1.0)
      dJointSetHinge2Param(this->physicalJoint, dParamCFM, dReal(this->axis1->cfm));
    if(this->axis2->cfm != -1.0)
      dJointSetHinge2Param(this->physicalJoint, dParamCFM2, dReal(this->axis2->cfm));


    if(this->axis1->limited || (this->axis1->motor != 0 && this->axis1->motor->getMotorType() == ANGULAR))
    {  
      dJointSetHinge2Param(this->physicalJoint, dParamLoStop, dReal(Functions::toRad(this->axis1->minValue)));
      dJointSetHinge2Param(this->physicalJoint, dParamHiStop, dReal(Functions::toRad(this->axis1->maxValue)));
      // this has to be done due to the way ODE sets joint stops
      dJointSetHinge2Param(this->physicalJoint, dParamLoStop, dReal(Functions::toRad(this->axis1->minValue)));

      if(this->axis1->fullScaleDeflectionCFM != -1.0)
        dJointSetHinge2Param(this->physicalJoint, dParamStopCFM, dReal(this->axis1->fullScaleDeflectionCFM));
      if(this->axis1->fullScaleDeflectionERP != -1.0)
        dJointSetHinge2Param(this->physicalJoint, dParamStopERP, dReal(this->axis1->fullScaleDeflectionERP));
      // set suspension parameters
      if(this->axis1->suspensionCFM != -1.0)
        dJointSetHinge2Param(this->physicalJoint, dParamSuspensionCFM, dReal(this->axis1->suspensionCFM));
      if(this->axis1->suspensionERP != -1.0)
        dJointSetHinge2Param(this->physicalJoint, dParamSuspensionERP, dReal(this->axis1->suspensionERP));
    }
  }

}

void WheelSuspension::createMotorPhysics()
{
}

void WheelSuspension::applyFriction(bool initial)
{
  if(!initial)
  {
    double velocity = 0.0;
    double gravityFactor = fabs((this->simulation->getPhysicsParameters())->getGravity());
    double fmax;
    if(this->axis1->motor == 0)
    {
      fmax = this->axis1->friction * gravityFactor;
      dJointSetHinge2Param(this->physicalJoint, dParamFMax, dReal(fmax));
      dJointSetHinge2Param(this->physicalJoint, dParamVel, dReal(velocity));
    }
    if(this->axis2->motor == 0)
    {
      fmax = this->axis2->friction * gravityFactor;
      dJointSetHinge2Param(this->physicalJoint, dParamFMax2, dReal(fmax));
      dJointSetHinge2Param(this->physicalJoint, dParamVel2, dReal(velocity));
    }
  }
}

void WheelSuspension::drawObject(const VisualizationParameterSet& visParams)
{
  if(visParams.surfaceStyle == VisualizationParameterSet::PHYSICS_WIREFRAME ||
     visParams.surfaceStyle == VisualizationParameterSet::PHYSICS_FLAT ||
     visParams.surfaceStyle == VisualizationParameterSet::PHYSICS_SMOOTH ||
     visParams.surfaceStyle == VisualizationParameterSet::MIXED_APPEARANCE ||
     visParams.surfaceStyle == VisualizationParameterSet::MIXED_PHYSICS)
  {
    dVector3 tempA;
    dJointGetHinge2Anchor(this->physicalJoint, tempA);
    Vector3d tempAnchor(tempA[0],tempA[1], tempA[2]);

    dJointGetHinge2Axis1(this->physicalJoint, tempA);
    Vector3d tempAxis1(tempA[0], tempA[1], tempA[2]);
    tempAxis1 *= 0.1;
    dJointGetHinge2Axis2(this->physicalJoint, tempA);
    Vector3d tempAxis2(tempA[0], tempA[1], tempA[2]);
    tempAxis2 *= 0.1;

    if(visParams.surfaceStyle == VisualizationParameterSet::MIXED_APPEARANCE)
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glShadeModel(GL_FLAT);
      GLHelper::getGLH()->drawWheelSuspension(tempAnchor, tempAxis1, tempAxis2, true);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glShadeModel(GL_SMOOTH);
    }
    else
      GLHelper::getGLH()->drawWheelSuspension(tempAnchor, tempAxis1, tempAxis2, false);
  }
}
