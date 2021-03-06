/**
 * @file Led.cpp
 * 
 * Implementation of class Led
 *
 * @author <A href="mailto:armin@informatik.uni-bremen.de">Armin Burchardt</A>
 */ 

#include <Simulation/Actuatorport.h>
#include <Simulation/Simulation.h>
#include "Led.h"

Led::Led()
{}


Led::Led(const AttributeSet& attributes)
{
  name = ParserUtilities::getValueFor(attributes, "name");

  std::string surfaceNameOn = ParserUtilities::getValueFor(attributes, "appearanceWhenOn");
  std::string surfaceNameOff = ParserUtilities::getValueFor(attributes, "appearanceWhenOff");
    
  surfaceOn = simulation->getSurface(surfaceNameOn);
  surfaceOff = simulation->getSurface(surfaceNameOff);
}

Led::~Led()
{}


SimObject* Led::clone() const
{
  Led* newLed = new Led();
  //copy SimObject members
  newLed->copyStandardMembers(this);
  bool createsNewMovID = newLed->adjustMovableID();

  //copy members
  newLed->surfaceOff = surfaceOff;
  newLed->surfaceOn = surfaceOn;
  newLed->state = state;

  //copy child nodes
  newLed->childNodes.clear();
  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newLed->addChildNode(childNode, false);
  }

  if(createsNewMovID)
    this->simulation->removeCurrentMovableID();

  SimObject* newObject = newLed;
  return newObject;
}


void Led::setValue(double value, int port)
{
  bool newState = (value != 0.);
  if (newState != state)
  {
    replaceSurface(newState ? surfaceOn : surfaceOff);
    state = newState;
  }
}


void Led::addToLists(std::vector<Sensorport*>& sensorportList,
                       std::vector<Actuatorport*>& actuatorportList,
                       std::vector<Actuator*>& actuatorList) 
{
  std::string actuatorDescription(name + ".state");
  Actuatorport* value = new Actuatorport(actuatorDescription, this, 0, 0., 1.);
  actuatorportList.push_back(value);

  actuatorList.push_back(this);
}

void Led::act(bool initial)
{
  if(initial)
  {
    replaceSurface(surfaceOff);
    state = false;
  }
}
