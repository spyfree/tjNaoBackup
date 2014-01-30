/**
 * @file SingleDistanceSensor.cpp
 * 
 * Implementation of of class SingleDistanceSensor
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 */

#include "Simulation/Simulation.h"
#include "SingleDistanceSensor.h"


SingleDistanceSensor::SingleDistanceSensor(): minRange(0.01), maxRange(100),
                                              sensorData(1.0),
                                              visualizationColor(0.3,1.0,0.3)
{
}

SingleDistanceSensor::SingleDistanceSensor(const AttributeSet& attributes):
                               minRange(0.01), maxRange(100),
                               sensorData(1.0),
                               visualizationColor(0.3,1.0,0.3)
{
  name = ParserUtilities::getValueFor(attributes, "name");
}

void SingleDistanceSensor::drawObject(const VisualizationParameterSet& visParams)
{
  if(visParams.visualizeSensors && !visParams.drawForSensor)
  {
    glColor3d(visualizationColor.v[0],
              visualizationColor.v[1],
              visualizationColor.v[2]);
    Vector3d distanceVec(sensorData,0,0);
    distanceVec.rotate(rotation);
    distanceVec+=position;
    glBegin(GL_LINES);
      glVertex3d (position.v[0], position.v[1], position.v[2]);
      glVertex3d (distanceVec.v[0], distanceVec.v[1], distanceVec.v[2]);
    glEnd();
  }
}

bool SingleDistanceSensor::parseAttributeElements(const std::string& name, 
                                                  const AttributeSet& attributes,
                                                  int line, int column, 
                                                  ErrorManager* errorManager)
{
  if(SimObject::parseAttributeElements(name,attributes,line,column, errorManager))
  {
    return true;
  }
  else if(name == "Range")
  {
    ParserUtilities::parseXYDoublePair(attributes, minRange, maxRange);
  }
  else
  {
    return false;
  }
  return true;
}

void SingleDistanceSensor::copyStandardMembers(const SingleDistanceSensor* other)
{
  SimObject::copyStandardMembers(other);
  minRange = other->minRange;
  maxRange = other->maxRange;
}

void SingleDistanceSensor::addToLists(std::vector<Sensorport*>& sensorportList,
                                      std::vector<Actuatorport*>& actuatorportList,
                                      std::vector<Actuator*>& actuatorList) 
{
  Sensorport* singleDistancePort = new Sensorport("distance", 0, doubleSensor, 
                                                  this, 0.0, 1.0);
  sensorportList.push_back(singleDistancePort);
}

void SingleDistanceSensor::addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
                                             int depth) 
{
  SimSensor::addToDescriptions(objectDescriptionTree, depth);  
  ObjectDescription sensorportDesc;
  sensorportDesc.name = "distance";
  sensorportDesc.fullName = fullName + ".distance";
  sensorportDesc.depth = depth + 1;
  sensorportDesc.type = OBJECT_TYPE_SENSORPORT;
  objectDescriptionTree.push_back(sensorportDesc);
}

void SingleDistanceSensor::computeValue(double& value, int portId)
{
  int simulationStep(simulation->getSimulationStep());
  if(simulationStep>lastComputationStep)
  {
    Vector3d ray(1.0,0.0,0.0);
    ray.rotate(rotation);
    double distance;
    if(simulation->intersectWorldWithRay(position, ray, maxRange,distance))
    {
      if(distance < minRange)
        sensorData = minRange;
      else
        sensorData = distance;
    }
    else
    {
      sensorData = maxRange;
    }
    lastComputationStep = simulationStep;
  }
  value = sensorData;
}

SimObject* SingleDistanceSensor::clone() const 
{
  SingleDistanceSensor* newSingleDistanceSensor = new SingleDistanceSensor();
  newSingleDistanceSensor->copyStandardMembers(this);
  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newSingleDistanceSensor->addChildNode(childNode, false);
  }
  SimObject* newObject = newSingleDistanceSensor;
  return newObject;
}
