/**
 * @file Led.h
 * 
 * Led Object.
 *
 * @author <A href="mailto:armin@informatik.uni-bremen.de">Armin Burchardt</A>
 */ 

#ifndef LED_H_
#define LED_H_

#include <string>
#include <vector>

#include "Actuator.h"


/**
* @class Led
*
* An class representing a Led
*/
class Led: public Actuator
{
public:
  /** Constructor*/
  Led(const AttributeSet& attributes);
  
  /** Constructor*/
  Led();

  /** Destructor*/
  virtual ~Led();

  /** Returns a string describing the kind of object
  * @return The kind
  */
  virtual std::string getKind() const {return "led";}

  /** Sets the value of one actuatorport
  * @param value The value
  */
  virtual void setValue(double value, int port);

  /** Clones the led
   *  @return A pointer to a copy of the motor
   */
  virtual SimObject* clone() const;

  /** Adds the object to some internal lists
   * @param sensorportList A list of all sensor ports in the scene
   * @param actuatorportList A list of all actuator ports in the scene
   * @param actuatorList A list of all actuators in the scene
   */
  virtual void addToLists(std::vector<Sensorport*>& sensorportList,
                          std::vector<Actuatorport*>& actuatorportList,
                          std::vector<Actuator*>& actuatorList);

  /** Let the actuator do what it is supposed to do
   * @param initial Is this the initial step?
   */
  virtual void act(bool initial = false);

protected:
  Surface* surfaceOn;
  Surface* surfaceOff;
  bool state;
};

#endif //LED_H_
