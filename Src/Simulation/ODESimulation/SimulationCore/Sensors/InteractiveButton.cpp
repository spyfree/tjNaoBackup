/**
 * @file InteractiveButton.cpp
 * 
 * Implementation of class InteractiveButton
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 */ 

#include <Simulation/Sensorport.h>
#include <Simulation/Simulation.h>
#include "InteractiveButton.h"


InteractiveButton::InteractiveButton()
{
  value = false;
  snapIn = false;
}

InteractiveButton::InteractiveButton(const AttributeSet& attributes) : value(false), snapIn(false)
{
  for(unsigned int i=0; i<attributes.size(); i++)
  {
    if(attributes[i].attribute == "name")
    {
      name = name = attributes[i].value;
    }
    else if(attributes[i].attribute == "snapIn")
    {
      snapIn = ParserUtilities::toBool(attributes[i].value);
    }
  }
}

void InteractiveButton::addToLists(std::vector<Sensorport*>& sensorportList,
                            std::vector<Actuatorport*>& actuatorportList,
                            std::vector<Actuator*>& actuatorList) 
{
  Sensorport* buttonPort = new Sensorport("click", 0,  boolSensor, this, 
                                          0.0, 1.0);
  sensorportList.push_back(buttonPort);
}

void InteractiveButton::addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
                               int depth) 
{
  SimSensor::addToDescriptions(objectDescriptionTree, depth);  
  ObjectDescription sensorportDesc;
  sensorportDesc.name = "click";
  sensorportDesc.fullName = fullName + ".click";
  sensorportDesc.depth = depth + 1;
  sensorportDesc.type = OBJECT_TYPE_SENSORPORT;
  objectDescriptionTree.push_back(sensorportDesc);
}

void InteractiveButton::computeValue(bool& value, int portId)
{
  value = this->value;
}

void InteractiveButton::hasBeenClicked()
{
  if(snapIn)
  {
    value = !value;
  }
  else
  {
    value = true;
  }
}

void InteractiveButton::hasBeenReleased()
{
  if(!snapIn)
  {
    value = false;
  }
}

SimObject* InteractiveButton::clone() const 
{
  InteractiveButton* newInteractiveButton = new InteractiveButton();
  newInteractiveButton->copyStandardMembers(this);
  bool createsNewMovID = newInteractiveButton->adjustMovableID();
  newInteractiveButton->snapIn = snapIn;
  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newInteractiveButton->addChildNode(childNode, false);
  }

  if(createsNewMovID)
    this->simulation->removeCurrentMovableID();

  SimObject* newObject = newInteractiveButton;
  return newObject;
}

bool InteractiveButton::isMovableOrClickable(SimObject*& object,
                                             bool testClickable)
{
  if(testClickable)
  {
    object = this;
    return true;
  }
  else
  {
    return SimObject::isMovableOrClickable(object, testClickable);
  }
}
