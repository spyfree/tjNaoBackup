/**
 * @file UniversalJoint.h
 * 
 * Definition of class UniversalJoint
 *
 * @author <A href="mailto:kspiess@informatik.uni-bremen.de">Kai Spiess</A>
 */ 

#ifndef UNIVERSALJOINT_H_
#define UNIVERSALJOINT_H_

#include "Joint.h"

/**
* @class UniversalJoint
*
* A class representing a universal joint with two axes.
*/
class UniversalJoint : public Joint
{
public:
  /** Constructor*/
  UniversalJoint();

  /** Constructor
  * @param attributes The attributes of the universal joint
  */
  UniversalJoint(const AttributeSet& attributes);

  /** Returns a string describing the kind of object
  * @return The kind
  */
  virtual std::string getKind() const {return "universaljoint";}

  /**
  * Parses the attributes of a simulated object which are
  * defined in seperate elements
  * @param name The name of the attribute element
  * @param attributes The attributes of the element
  * @param line The line in the scene description
  * @param column The column in the scene description
  * @param errorManager An object managing errors
  */
  virtual bool parseAttributeElements(const std::string& name, 
                                      const AttributeSet& attributes,
                                      int line, int column, 
                                      ErrorManager* errorManager);

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
   * Creats the physical joints used by the OpenDynamicsEngine (ODE).
   * This has to be done after the physical objects (geometry and body)
   * have been created.
   */
  virtual void createJointPhysics();

  /** 
   * Creats the physical motors for the joints used by the OpenDynamicsEngine (ODE).
   * This has to be done after the physical joints have been created.
   */
  virtual void createMotorPhysics();

  /** 
   * Apply friction for non-powered joint axes
   * @param initial Is this the initial step?
   */
  virtual void applyFriction(bool initial);

  /** Clones the object and its child nodes
   * @return A pointer to a copy of the object
   */
  virtual SimObject* clone() const;

  /** 
   * Draws the universal joint anchor point and the axes
   * @param visParams Parameters for Visualization
   */
  virtual void drawObject(const VisualizationParameterSet& visParams);

protected:
  /** The ports of a UniversalJoint*/
  enum UniversalPort{
    AXIS1_JOINT=0,
    AXIS2_JOINT=1,
    AXIS1_MOTOR=2,
    AXIS2_MOTOR=3
  };

  /** The anchor point of this joint*/
  Vector3d& anchorPoint;
  /** A pointer to the first axis of this joint*/
  AxisDescription* axis1;
  /** A pointer to the second axis of this joint*/
  AxisDescription* axis2;
};


#endif
