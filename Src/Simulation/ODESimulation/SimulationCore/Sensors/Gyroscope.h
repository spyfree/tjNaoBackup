/**
* @file Gyroscope.h
* 
* Definition of class Gyroscope
*
* @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
*/ 

#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include "SimSensor.h"
#include <Tools/SimGraphics.h> //is needed! Why?
#include <PhysicalObjects/Box.h>

/**
* @class Gyroscope
*
* A sensor returning angular velocity around three axes
*/
class Gyroscope : public virtual SimSensor
{  
protected:
  PhysicalObject* physObj;   /**< Pointer to the object to which the measurements refer to */
  dBodyID *bodyID;           /**< The ID of the body used for velocity measurement */
  Vector3d valueAngleVel;    /**< The values calculated. */

public:
  /** Empty default Constructor*/
  Gyroscope() {};

  /** Constructor
  * @param attributes The attributes of the sensor
  */
  Gyroscope(const AttributeSet& attributes);

  /** Returns a string describing the kind of object
  * @return The kind
  */
  virtual std::string getKind() const {return "gyroscope";}

  /** Computes the current value of the sensor
  * @param value The computed value in rad
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

  /** Do internal computations after parsing.*/
  virtual void postProcessAfterParsing();
};

#endif //GYROSCOPE_H_
