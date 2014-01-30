/**
 * @file Bumper.cpp
 * 
 * Implementation of class Bumper
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 */ 

#include <Simulation/Sensorport.h>
#include <Simulation/Simulation.h>
#include "Bumper.h"



Bumper::Bumper() : value(false)
{
}

Bumper::Bumper(const AttributeSet& attributes) : value(false)
{
  for(unsigned int i=0; i<attributes.size(); i++)
  {
    if(attributes[i].attribute == "name")
    {
      name = attributes[i].value;
    }
  }
}


void Bumper::addToLists(std::vector<Sensorport*>& sensorportList,
                        std::vector<Actuatorport*>& actuatorportList,
                        std::vector<Actuator*>& actuatorList) 
{
  Sensorport* bumperPort = new Sensorport("collision", 0, boolSensor, 
                                          this, 0.0, 1.0);
  sensorportList.push_back(bumperPort);
}

void Bumper::addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
                               int depth) 
{
  SimSensor::addToDescriptions(objectDescriptionTree, depth);  
  ObjectDescription sensorportDesc;
  sensorportDesc.name = "collision";
  sensorportDesc.fullName = fullName + ".collision";
  sensorportDesc.depth = depth + 1;
  sensorportDesc.type = OBJECT_TYPE_SENSORPORT;
  objectDescriptionTree.push_back(sensorportDesc);
}

void Bumper::computeValue(bool& value, int portId)
{
  value = this->value;
}

SimObject* Bumper::clone() const 
{
  Bumper* newBumper = new Bumper();
  newBumper->copyStandardMembers(this);
  bool createsNewMovID = newBumper->adjustMovableID();
  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newBumper->addChildNode(childNode, false);
  }

  if(createsNewMovID)
    this->simulation->removeCurrentMovableID();

  SimObject* newObject = newBumper;
  return newObject;
}
