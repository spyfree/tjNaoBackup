#ifndef __FSRSensor_h_
#define __FSRSensor_h_
#include "SimSensor.h"
#include <ode/common.h>
class FSRSensor : public SimSensor
{
public:
enum{
	MAX_FEEDBACKS = 10
};
protected:
	/** The current force of the FSRSensor */
	double value;
	dJointFeedback jFeedBack[MAX_FEEDBACKS];
	int numFeedbacks;
	PhysicalObject* physObj;   /**< The body to which the measurements refer to*/
	dBodyID *bodyID;           /**< The ID of the body used for acceleration measurement */
public:
	FSRSensor();
	FSRSensor(const AttributeSet& attributes);
	virtual std::string getKind() const { return "FSRSensor"; }
	virtual void computeValue(double& value, int portID);
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
	/**   * Returns a pointer to the FSRSensor. This is used to prevent dynamic downcasts from sensor objects to bumper objects   * @ return A pointer to the FSRSensor   */
	FSRSensor* castToFSRSensor(){return this;};
	/** 
	*  Sets the value of the FSR
	*  @param newValue the new value
	*/
	void setValue(int newValue){ value = newValue; }
	double getValue(){return value;}
	void setNumFeedbacks(int n) { numFeedbacks = n;}
	void resetJointFeedback();
	dJointFeedback* getJointFeedBackBuf() { return jFeedBack;}
};
#endif