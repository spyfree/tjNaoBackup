/**
* @file Accelerometer.cpp
* 
* Implementation of class Accelerometer
*
* @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
* @author <A href="mailto:tobias.schade@tu-dortmund.de">Tobias Schade</A>
* @author <A href="mailto:markus.kuech@tu-dortmund.de">Markus K�ch</A>
* @author <A href="mailto:Christian.Buettner@tu-dortmund.de">Christian B�ttner</A>
*/

#include <Simulation/Simulation.h>
#include "Accelerometer.h"
#undef max
#undef min
#include <limits>
#include <PhysicalObjects/PhysicalObject.h>
#include <Tools/ODETools.h>


Accelerometer::Accelerometer(const AttributeSet& attributes)
{
  name = ParserUtilities::getValueFor(attributes, "name");
}

bool Accelerometer::parseAttributeElements(const std::string& name, 
                                           const AttributeSet& attributes,
                                           int line, int column, 
                                           ErrorManager* errorManager)
{
  return SimObject::parseAttributeElements(name,attributes,line,column, errorManager);
}

void Accelerometer::addToLists(std::vector<Sensorport*>& sensorportList,
                               std::vector<Actuatorport*>& actuatorportList,
                               std::vector<Actuator*>& actuatorList) 
{
  std::vector<int> dimensions;
  dimensions.push_back(3);
  Sensorport* accPort = new Sensorport("acceleration", 0, doubleArraySensor, this, 
    std::numeric_limits<double>::min(), std::numeric_limits<double>::max(),
    dimensions);
  sensorportList.push_back(accPort);
}

void Accelerometer::addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
                                      int depth) 
{
  SimSensor::addToDescriptions(objectDescriptionTree, depth);  
  ObjectDescription sensorportDesc;
  sensorportDesc.name = "acceleration";
  sensorportDesc.fullName = fullName + ".acceleration";
  sensorportDesc.depth = depth + 1;
  sensorportDesc.type = OBJECT_TYPE_SENSORPORT;
  objectDescriptionTree.push_back(sensorportDesc);
}

void Accelerometer::computeValue(double*& value, int portId)
{
  // Set step counter and check, if new data needs to be computed:
  int simulationStep(simulation->getSimulationStep());
  if(!(simulationStep>lastComputationStep))
  {
    value = accValues.v;
    return;
  }

  // calculate a rotation Offset which is necessary to compensate the initial rotation
  // of the robot (and sensor) in the scene	
  if(!isRotationOffsetSet)
  {
    //get the initial rotation of the sensor in worldcoordinates 
    rotationOffset = physObj->getRotation();
    //get the rotation angle of z axis and set it negativ for correct evalutation of rotationOffset
    rotationOffset.transpose();
    //now the function rotationOffset = zRotation(pi_2) - zRotation(angle) can be evalueated
    //zRotation(pi_2)
    Matrix3d coordinateOffset;
    coordinateOffset.col[0] = Vector3d(0,1,0);
    coordinateOffset.col[1] = Vector3d(-1,0,0);
    coordinateOffset.col[2] = Vector3d(0,0,1);
    //compute offset
    rotationOffset = rotationOffset * coordinateOffset;    
    isRotationOffsetSet = true;
  }  

  // get the orientation of the acceleration sensor
  // orientationMatrix contains a 4x3 Matrix
  // but only the information from the left 3x3 part is needed to get the orientation of the Obj.
  const dReal *orientationMatrix = dBodyGetRotation(*bodyID);
  Matrix3d  objOrientation;
  objOrientation.col[0] = Vector3d(orientationMatrix[0],orientationMatrix[1],orientationMatrix[2]);
  objOrientation.col[1] = Vector3d(orientationMatrix[4],orientationMatrix[5],orientationMatrix[6]);
  objOrientation.col[2] = Vector3d(orientationMatrix[8],orientationMatrix[9],orientationMatrix[10]);

  //velocity of body in world coordinates
  Vector3d velocityVec(ODETools::convertVector(dBodyGetLinearVel(*bodyID)));

  //derive velocity -> acceleration is velocitychange divided by timedifference
  //deltaT contains the elapsed simulation time between two method calls
  double deltaT = simulationStep - lastComputationStep;  
  deltaT *= simulation->getStepLength(); 

  // calculation of the velocitychange between two timesteps
  // the change in velocity is divided by the length of a timestep to get the acceleration
  accValues = (velocityVec - lastVelocity)/deltaT;

  // get the gravityvector from the ODE
  dVector3 odeGravity;
  dWorldID *worldID = simulation->getPhysicalWorld();
  dWorldGetGravity(*worldID,odeGravity);
  Vector3d gravityVector(odeGravity[0],odeGravity[1],odeGravity[2]);

  //because free falling acceleration Sensor does not measure anything, you have to substract the gravity part
  //of the accelerationvector
  accValues = accValues - gravityVector;

  // project the acceleration vector which is still in world coordinates into to robot coordinate system
  accValues = objOrientation * accValues;
  accValues = rotationOffset * accValues;

  // all values are negative for some reason
  accValues *= -1;

  //saved velocity of current computation step for velocity integration next step
  lastVelocity = velocityVec;
  lastComputationStep = simulationStep;
  value = accValues.v;
}

SimObject* Accelerometer::clone() const 
{
  Accelerometer* newAccelerometer = new Accelerometer();
  newAccelerometer->copyStandardMembers(this);
  std::list<SimObject*>::const_iterator pos;
  for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
  {
    SimObject* childNode = (*pos)->clone();
    newAccelerometer->addChildNode(childNode, false);
  }
  SimObject* newObject = newAccelerometer;
  return newObject;
}

void Accelerometer::postProcessAfterParsing()
{
  // Get pointer and body ID of physical body
  physObj = (*(childNodes.begin()))->castToPhysicalObject();
  bodyID = physObj->getBody();
  isRotationOffsetSet = false;
}
