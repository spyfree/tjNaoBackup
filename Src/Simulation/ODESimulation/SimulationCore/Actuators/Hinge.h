/**
 * @file Hinge.h
 * 
 * Definition of class Hinge
 *
 * @author <A href="mailto:timlaue@informatik.uni-bremen.de">Tim Laue</A>
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 */ 

#ifndef HINGE_H_
#define HINGE_H_

#include "Joint.h"


/**
* @class HingeParameterUpdate
*
* This class contains a set of parameters which may be changed
* at runtime. This is useful for parameter optimization.
*/
class HingeParameterUpdate
{
public:
  double maxMotorForce;       /**< Motor parameter, only update, if the hinge axis has a motor*/
  double maxMotorVelocity;    /**< Motor parameter, only update, if the hinge axis has a motor*/
  double motorPControlValue;  /**< Motor parameter, only update, if the hinge axis has a motor*/
  double motorIControlValue;  /**< Motor parameter, only update, if the hinge axis has a motor*/
  double motorDControlValue;  /**< Motor parameter, only update, if the hinge axis has a motor*/
};


/**
* @class Hinge
*
* A class representing a hinge joint.
*/
class Hinge : public Joint
{
public:
  /** Constructor*/
  Hinge(): anchorPoint(position), axis(0), destinationValue(0.0), lastValue(0.0)
  {}

  /** Constructor
  * @param attributes The attributes of the hinge joint
  */
  Hinge(const AttributeSet& attributes);

  /** Returns a string describing the kind of object
  * @return The kind
  */
  virtual std::string getKind() const {return "hinge";}

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
  virtual void addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree, int depth);

  /** Sets the value of one actuatorport
  * @param value The value
  * @param port The port
  */
  virtual void setValue(double value, int port);

  /** Let the actuator do what it is supposed to do
   * @param initial Is this the initial step?
   */
  virtual void act(bool initial);

  /** Computes the position of the joint
  * @param value The value
  * @param portId The id of the sensorport
  */
  virtual void computeValue(double& value, int portId);

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
  
  /** 
   * Creats the physical joints used by the OpenDynamicsEngine (ODE).
   * This has to be done after the physical objects (geometry and body)
   * have been created.
   */
  virtual void createJointPhysics();

  /** 
   * Apply friction for non-powered joint axis
   * @param initial Is this the initial step?
   */
  virtual void applyFriction(bool initial);

  /** Clones the object and its child nodes
   * @return A pointer to a copy of the object
   */
  virtual SimObject* clone() const;

  /** 
   * Draws the hinge anchor point and the axis
   * @param visParams Parameters for Visualization
   */
  virtual void drawObject(const VisualizationParameterSet& visParams);

  /**
  * Sets new parameters during runtime
  * @param params The parameter set
  */
  void updateParameters(const HingeParameterUpdate& params);

protected:

  enum HingePorts
  {
    AXIS_JOINT=0,
    AXIS_MOTOR
  };

  /** The anchor point of the joint as defined by ODE*/
  Vector3d& anchorPoint;
  /** A pointer to the axis of this joint*/
  AxisDescription* axis;
  /** The destination value for this joint*/
  double destinationValue;
  /** The last destination of this joint*/
  double lastValue;
};


#endif
