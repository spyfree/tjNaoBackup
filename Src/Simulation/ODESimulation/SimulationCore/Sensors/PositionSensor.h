/**
 * @file PositionSensor.h
 * 
 * Definition of class PositionSensor
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 */ 

#ifndef POSITION_SENSOR_H_
#define POSITION_SENSOR_H_

#include "SimSensor.h"


/**
* @class PositionSensor
*
* A sensor returning its position in 3D space
*/
class PositionSensor : public virtual SimSensor
{  
protected:
  /** The data generated by this sensor*/
  Vector3d sensorData;

public:
  /** Constructor*/
  PositionSensor() {};

  /** Constructor
  * @param attributes The attributes of the sensor
  */
  PositionSensor(const AttributeSet& attributes);

  /** Destructor */
  virtual ~PositionSensor() {};

  /** Returns a string describing the kind of object
  * @return The kind
  */
  virtual std::string getKind() const {return "position-sensor";}

  /** Computes the current value of the sensor
  * @param value The computed value
  * @param portId The id of the sensorport
  */
  virtual void computeValue(double*& value, int portId);
  
  /** Adds the object to some internal lists
  * @param sensorportList A list of all sensor ports in the scene
  * @param actuatorportList A list of all actuator ports in the scene
  * @param actuatorList A list of all actuators in the scene
  */
  virtual void addToLists(std::vector<Sensorport*>& sensorportList,
    std::vector<Actuatorport*>& actuatorportList,
    std::vector<Actuator*>& actuatorList);
  
  /** Adds descriptions of the object and its children to a list
  * @param objectDescriptionTree The list of descriptions
  * @param The depth of the object in the object tree
  */
  virtual void addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
    int depth);

  /** Clones the object and its child nodes
  * @return A pointer to a copy of the object
  */
  virtual SimObject* clone() const;

  /**
  * Parses the attributes of a simulated object which are
  * defined in seperate elements
  * @param name The name of the attribute element
  * @param attributes The attributes of the element
  * @param line The line in the scene description
  * @param column The column in the scene description
  * @param errorManager An object managing errors
  * @return true, if any element has been parsed
  */
  virtual bool parseAttributeElements(const std::string& name, 
                                      const AttributeSet& attributes,
                                      int line, int column, 
                                      ErrorManager* errorManager);
};

#endif //POSITION_SENSOR_H_
