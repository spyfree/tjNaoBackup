/**
 * @file Sensorport.h
 * 
 * Definition of class Sensorport
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 */ 

#ifndef SENSORPORT_H_
#define SENSORPORT_H_

#include <Sensors/SimSensor.h>
#include <string>
#include <vector>
#include <cassert>

/** An identifier type for the different kinds of sensors*/
enum SensorType
{
  boolSensor,
  doubleSensor,
  cameraSensor,
  doubleArraySensor,
  viewSensor,
  noSensor
};


/**
* @class Sensorport
*
* A class representing a sensor port
*/
class Sensorport
{
private:
  /** The name of the port*/
  std::string name;
  /** The dimensions of the sensor data*/
  std::vector<int> dimensions;
  /** The type of the sensor*/
  SensorType sensorType;
  /** A pointer to the sensor*/
  SimSensor* sensor;
  /** The id of the port*/
  int portId;
  /** The minimum sensor value*/
  double minValue;
  /** The maximum sensor value*/
  double maxValue;

public:
  /** Constructor
  * @param name The name of the port
  * @param portId The id of the port
  * @param sensorType The type of the sensor
  * @param sensor A pointer to the sensor
  * @param minValue The minimum sensor value
  * @param maxValue The maximum sensor value
  * @param dimensions The dimensions of the sensor data
  */
  Sensorport(const std::string& name, int portId, SensorType sensorType, 
             SimSensor* sensor, double minValue, double maxValue,
             std::vector<int> dimensions)
  {
    this->sensor = sensor; 
    this->portId = portId;
    this->sensorType = sensorType;
    this->name = sensor->getFullName() + (name == "" ? "" : "." + name);
    this->minValue = minValue;
    this->maxValue = maxValue;
    this->dimensions = dimensions;
  }

  /** Constructor
  * Sets the dimension of the sensor data to 1
  * @param name The name of the port
  * @param portId The id of the port
  * @param sensorType The type of the sensor
  * @param sensor A pointer to the sensor
  * @param minValue The minimum sensor value
  * @param maxValue The maximum sensor value
  */
  Sensorport(const std::string& name, int portId, SensorType sensorType, 
             SimSensor* sensor, double minValue, double maxValue)
  {
    this->sensor = sensor; 
    this->portId = portId;
    this->sensorType = sensorType;
    this->name = sensor->getFullName() + (name == "" ? "" : "." + name);
    this->minValue = minValue;
    this->maxValue = maxValue;
    dimensions.push_back(1);
  }

  /** Returns the name of the port
  * @return The name
  */
  const std::string& getName() const {return name;}

  /** Returns the type of the sensor
  * @return The type
  */
  SensorType getType() const {return sensorType;}

  /** Returns the dimensions of the sensor data
  * @return the dimensions
  */
  const std::vector<int>& getDimensions() const {return dimensions;}

  /** Returns a floating point sensor value
  * @return The value
  */
  void getValue(double& value) {assert(sensorType == doubleSensor); sensor->computeValue(value,portId);}

  /** Returns a boolean sensor value
  * @return The value
  */
  void getValue(bool& value) {assert(sensorType == boolSensor); sensor->computeValue(value,portId);}

  /** Returns a list of floating point sensor values
  * @return The value
  */
  void getValue(double*& value) {assert(sensorType == doubleArraySensor); sensor->computeValue(value,portId);}

  /** Returns a list of unsigned char* sensor values (e.g. an image)
  * @return The value
  */
  void getValue(unsigned char*& value) {assert(sensorType == cameraSensor); sensor->computeValue(value,portId);}

  /** Returns the minimum sensor value
  * @return The value
  */
  double getMinValue() const {return minValue;}

  /** Returns the maximum sensor value
  * @return The value
  */
  double getMaxValue() const {return maxValue;}

  /** Returns the associated sensor
  * @return The sensor
  */
  SimSensor* getSensor() {return sensor;}
};

#endif //SENSORPORT_H_
