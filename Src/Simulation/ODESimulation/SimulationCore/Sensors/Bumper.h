/**
 * @file Bumper.h
 * 
 * Definition of class Bumper
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 */ 

#ifndef BUMPER_H_
#define BUMPER_H_

#include "SimSensor.h"


/**
* @class Bumper
*
* A class representing a bumper.
*/
class Bumper : public SimSensor
{
protected:
  /** The current value of the bumper (true/false)*/
  bool value;
  
public:
  /** Constructor */
  Bumper();

  /** Constructor
  * @param attributes The attributes of the bumper
  */
  Bumper(const AttributeSet& attributes);


  /** Returns a string describing the kind of object
  * @return The kind
  */
  virtual std::string getKind() const {return "bumper";}
  
  /** Computes the current value of the bumper
  * @param value The computed value
  * @param portId The id of the sensorport
  */
  virtual void computeValue(bool& value, int portId);
  
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
  /**   * Returns a pointer to the bumper. This is used to prevent dynamic downcasts from sensor objects to bumper objects   * @ return A pointer to the bumper   */
  Bumper* castToBumper(){return this;};
  /** 
   *  Sets the value of the bumper
   *  @param newValue the new value
   */
  void setValue(bool newValue){value = newValue;};

};

#endif //BUMPER_H_
