/**
 * @file Controller/View.h
 * 
 * Definition of class View
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 */ 

#ifndef SIMROBOTVIEW_H_
#define SIMROBOTVIEW_H_

#include <string>

#include "../Sensors/SimSensor.h"

/**
 * @class View
 * 
 * A class for visualizing extra information
 * (Not implemented yet)
 */ 
class View : public SimSensor
{
public:
  /**
  * All possible mouse events.
  */
  enum MouseEvent
  {
    leftButtonDown,
    leftButtonDrag,
    leftButtonUp,
    mouseMove,
  };

  /**
  * The keys that can be pressed during a mouse event.
  * @attention Each key requires a separate bit.
  */
  enum Key
  {
    none,
    control = 1,
    shift = 2
  };

  /** Updates the current view*/
  virtual void update() {};

  /** Adds the object to some internal lists
  * @param sensorportList A list of all sensor ports in the scene
  * @param actuatorportList A list of all actuator ports in the scene
  * @param actuatorList A list of all actuators in the scene
  */
  void addToLists(std::vector<Sensorport*>& sensorportList,
                  std::vector<Actuatorport*>&,
                  std::vector<Actuator*>&);

  /** Adds descriptions of the object and its children to a list
  * @param objectDescriptionTree The list of descriptions
  * @param The depth of the object in the object tree
  */
  void addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
                         int depth);

  /** Returns a string describing the kind of object
  * @return The kind
  */
  virtual std::string getKind() const {return "view";}

  /**
  * The function handles a mouse event.
  * @param event The event.
  * @param x The x coordinate of the cursor when the event occured.
  * @param y The y coordinate of the cursor when the event occured.
  * @param keys The set of keys pressed when the event occured.
  * @return Does the view need to be repainted?
  */
  virtual bool handleMouseEvent(MouseEvent event, int x, int y, int keys) {return false;}

  /**
  * The function is called to return a string that contains the configuration of the view.
  * @return A text string containing all information to restore the internal state of the view.
  */
  virtual std::string getConfiguration() const {return "";}

  /**
  * The function is called to restore the configuration of the view.
  * @param A text string containing all information to restore the internal state of the view.
  */
  virtual void setConfiguration(const std::string& configuration) {}
};

#endif //SIMROBOTVIEW_H_
