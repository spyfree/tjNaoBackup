/**
 * @file SimSensor.h
 * 
 * Definition of class SimSensor
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 */ 

#ifndef SENSOR_H_
#define SENSOR_H_

#include "../Simulation/SimObject.h"

class Bumper;
class FSRSensor;
/**
* @class SimSensor
*
* An abstract class simulating a sensor
* All sensor classes are derivated from SimSensor and 
* reimplement the functions needed
*/
class SimSensor : public virtual SimObject
{
protected:
  /** The number of step, the last value has been computed*/
  int lastComputationStep;
  
public:
  /** Constructor*/
  SimSensor() {lastComputationStep = -1;}

  /** Returns a string describing the kind of object
  * @return The kind
  */
  virtual std::string getKind() const {return "sensor";}

  /** Computes a floating point value
  * @param value The value
  * @param portId The id of the sensorport
  */
  virtual void computeValue(double& value, int portId) {}

  /** Computes a boolean value
  * @param value The value
  * @param portId The id of the sensorport
  */
  virtual void computeValue(bool& value, int portId) {}

  /** Computes an integer value
  * @param value The value
  * @param portId The id of the sensorport
  */
  virtual void computeValue(int& value, int portId) {}

  /** Computes an array of floating point values
  * (memory allocation is done internally, only a pointer will be returned)
  * @param value The value
  * @param portId The id of the sensorport
  */
  virtual void computeValue(double*& value, int portId) {}

  /** Computes a position in 3D space
  * @param value The value
  * @param portId The id of the sensorport
  */
  virtual void computeValue(Vector3d& value, int portId) {}

  /** Computes an array of bytes (e.g. an image)
  * (memory allocation is done internally, only a pointer will be returned)
  * @param value The value
  * @param portId The id of the sensorport
  */
  virtual void computeValue(unsigned char*& value, int portId) {}

  /**
   * Returns a pointer to the bumper if the sensor is one. This is used to prevent dynamic downcasts
   * from sensor objects to bumper objects. Has to be overloaded by the appropriate subclass
   * @ return A pointer to the bumper
   */
  virtual Bumper* castToBumper(){return NULL;};

  virtual FSRSensor* castToFSRSensor(){return NULL;};
};

#endif
