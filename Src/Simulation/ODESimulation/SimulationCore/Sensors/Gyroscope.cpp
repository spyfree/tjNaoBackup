/**
* @file Gyroscope.cpp
* 
* Implementation of class Gyroscope
*
* @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
* @author <A href="mailto:tobias.schade@tu-dortmund.de">Tobias Schade</A>
* @author <A href="mailto:markus.kuech@tu-dortmund.de">Markus Küch</A>
* @author <A href="mailto:Christian.Buettner@tu-dortmund.de">Christian Büttner</A>
*/

#include <Simulation/Simulation.h>
#include "Gyroscope.h"
#undef max
#undef min
#include <limits>
#include <PhysicalObjects/PhysicalObject.h>
#include <Tools/ODETools.h>


Gyroscope::Gyroscope(const AttributeSet& attributes)
{
  name = ParserUtilities::getValueFor(attributes, "name");
}

bool Gyroscope::parseAttributeElements(const std::string& name, 
                                       const AttributeSet& attributes,
                                       int line, int column, 
                                       ErrorManager* errorManager)
{
  return SimObject::parseAttributeElements(name,attributes,line,column, errorManager);
}

void Gyroscope::addToLists(std::vector<Sensorport*>& sensorportList,
                           std::vector<Actuatorport*>& actuatorportList,
                           std::vector<Actuator*>& actuatorList) 
{
  std::vector<int> dimensions;
  dimensions.push_back(3);
  Sensorport* gyroPort = new Sensorport("angularVelocity", 0, doubleArraySensor, this, 
                                         std::numeric_limits<double>::min(), std::numeric_limits<double>::max(),
                                         dimensions);
  sensorportList.push_back(gyroPort);
}

void Gyroscope::addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
                                  int depth) 
{
  SimSensor::addToDescriptions(objectDescriptionTree, depth);  
  ObjectDescription sensorportDesc;
  sensorportDesc.name = "angularVelocity";
  sensorportDesc.fullName = fullName + ".angularVelocity";
  sensorportDesc.depth = depth + 1;
  sensorportDesc.type = OBJECT_TYPE_SENSORPORT;
  objectDescriptionTree.push_back(sensorportDesc);
}

void Gyroscope::computeValue(double*& value, int portId)
{
  // Set step counter and check, if new data needs to be computed:
  int simulationStep(simulation->getSimulationStep());
  if(!(simulationStep>lastComputationStep))
  {
    value = valueAngleVel.v;
    return;
  }
  
  // get the angle velocity of the sensor in world coordinates
  ODETools::convertVector(dBodyGetAngularVel(*bodyID), valueAngleVel);  
  // get the orientation of the gyro sensor
  // orientationMatrix contains a 4x3 Matrix
  // but only the information from the left 3x3 part is needed to get the orientation of the Obj.
  const dReal *orientationMatrix = dBodyGetRotation(*bodyID);
  Matrix3d  objOrientation;
  objOrientation.col[0] = Vector3d(orientationMatrix[0],orientationMatrix[1],orientationMatrix[2]);
  objOrientation.col[1] = Vector3d(orientationMatrix[4],orientationMatrix[5],orientationMatrix[6]);
  objOrientation.col[2] = Vector3d(orientationMatrix[8],orientationMatrix[9],orientationMatrix[10]);  
  // project the angle velocity vector which is still in world coordinates to the robot coordinate system
  valueAngleVel = objOrientation * valueAngleVel;
  lastComputationStep = simulationStep;  
  value = valueAngleVel.v;
}

SimObject* Gyroscope::clone() const 
{
  Gyroscope* newGyroscope = new Gyroscope();
  newGyroscope->copyStandardMembers(this);
  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newGyroscope->addChildNode(childNode, false);
  }
  SimObject* newObject = newGyroscope;
  return newObject;
}

void Gyroscope::postProcessAfterParsing()
{
  // Get body ID of physical body
  physObj = (*(childNodes.begin()))->castToPhysicalObject();
  bodyID = physObj->getBody();
}
