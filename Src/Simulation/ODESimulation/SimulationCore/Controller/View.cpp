/**
 * @file Controller/View.cpp
 * 
 * Implementation of class View
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 */ 

#include "View.h"
#include "../Simulation/Sensorport.h"

void View::addToLists(std::vector<Sensorport*>& sensorportList,
                std::vector<Actuatorport*>&,
                std::vector<Actuator*>&) 
{
  sensorportList.push_back(new Sensorport("", 0, viewSensor, this, 0, 0));
}

void View::addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
                       int depth) 
{
  //Add Sensorport:
  ObjectDescription sensorportDesc;
  int p = name.find_last_of('.');
  sensorportDesc.name = p == -1 ? name : name.substr(p + 1);
  sensorportDesc.fullName = fullName;
  sensorportDesc.depth = depth;
  sensorportDesc.type = OBJECT_TYPE_SENSORPORT;
  objectDescriptionTree.push_back(sensorportDesc);
}
