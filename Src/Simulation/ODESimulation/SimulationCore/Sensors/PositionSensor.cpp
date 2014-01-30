/**
 * @file PositionSensor.cpp
 * 
 * Implementation of of class PositionSensor
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 */

#include <Simulation/Simulation.h>
#include "PositionSensor.h"
#undef max
#undef min
#include <limits>

PositionSensor::PositionSensor(const AttributeSet& attributes)
{
  name = ParserUtilities::getValueFor(attributes, "name");
}

bool PositionSensor::parseAttributeElements(const std::string& name, 
                                            const AttributeSet& attributes,
                                            int line, int column, 
                                            ErrorManager* errorManager)
{
  return SimObject::parseAttributeElements(name,attributes,line,column, errorManager);
}

void PositionSensor::addToLists(std::vector<Sensorport*>& sensorportList,
                                std::vector<Actuatorport*>& actuatorportList,
                                std::vector<Actuator*>& actuatorList) 
{
  std::vector<int> dimensions;
  dimensions.push_back(3);
  Sensorport* positionPort = new Sensorport("position", 0, doubleArraySensor, this, 
                                            std::numeric_limits<double>::min(), std::numeric_limits<double>::max(),
                                            dimensions);
  sensorportList.push_back(positionPort);
}

void PositionSensor::addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
                                       int depth) 
{
  SimSensor::addToDescriptions(objectDescriptionTree, depth);  
  ObjectDescription sensorportDesc;
  sensorportDesc.name = "position";
  sensorportDesc.fullName = fullName + ".position";
  sensorportDesc.depth = depth + 1;
  sensorportDesc.type = OBJECT_TYPE_SENSORPORT;
  objectDescriptionTree.push_back(sensorportDesc);
}

void PositionSensor::computeValue(double*& value, int portId)
{
  int simulationStep(simulation->getSimulationStep());
  if(simulationStep>lastComputationStep)
  {
    sensorData = position;
    lastComputationStep = simulationStep;
  }
  value = sensorData.v;
}

SimObject* PositionSensor::clone() const 
{
  PositionSensor* newPositionSensor = new PositionSensor();
  newPositionSensor->copyStandardMembers(this);
  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newPositionSensor->addChildNode(childNode, false);
  }
  SimObject* newObject = newPositionSensor;
  return newObject;
}
