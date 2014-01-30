/**
 * @file StandardMotor.cpp
 * 
 * Implementation of class StandardMotor
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 */ 

#include "StandardMotor.h"
#include "MotorController.h"
#include "../Simulation/Simulation.h"
#include "../Actuators/Joint.h"
#include "../Simulation/Actuatorport.h"


StandardMotor::StandardMotor(const AttributeSet& attributes): Motor(attributes)
{
  for(unsigned int i=0; i<attributes.size(); i++)
  {
    if(attributes[i].attribute == "maxVelocity")
    {
      maxVelocity = ParserUtilities::toDouble(attributes[i].value);
    }
    else if(attributes[i].attribute == "maxForce")
    {
      maxForce = ParserUtilities::toDouble(attributes[i].value);
    }
  }
}

Motor* StandardMotor::clone() const
{
  StandardMotor* newStandardMotor = new StandardMotor();
  newStandardMotor->copyStandardMembers(this);
  if(motorController)
    newStandardMotor->motorController = motorController->clone();
  return newStandardMotor;
}

void StandardMotor::setValue(double value, int port)
{
  if(motorType == VELOCITY)
  {
    if(value >= maxVelocity)
      desiredVelocity = maxVelocity;
    else
      desiredVelocity = value;
  }
  else if(this->motorType == ANGULAR)
  {
    desiredValue = value;
  }
};
