#include <Simulation/Sensorport.h>
#include <Simulation/Simulation.h>
#include <PhysicalObjects/PhysicalObject.h>
#include "FSRSensor.h"

FSRSensor::FSRSensor() : value(0), numFeedbacks(0)
{
	resetJointFeedback();
}
FSRSensor::FSRSensor(const AttributeSet &attributes):value(0), numFeedbacks(0)
{
	resetJointFeedback();
	for(unsigned int i =0; i < attributes.size(); ++i)
	{
		if(attributes[i].attribute == "name"){
			name = attributes[i].value;
		}
	}
}
bool FSRSensor::parseAttributeElements(const std::string& name, 
										   const AttributeSet& attributes,
										   int line, int column, 
										   ErrorManager* errorManager)
{
	return SimObject::parseAttributeElements(name,attributes,line,column, errorManager);
}

void FSRSensor::addToLists(std::vector<Sensorport*>& sensorportList,
						   std::vector<Actuatorport*>& actuatorportList,
						   std::vector<Actuator*>& actuatorList) 
{
	Sensorport* FSRPort = new Sensorport("fsr", 0, doubleSensor, 
		this, -10, 100);
	sensorportList.push_back(FSRPort);
}

void FSRSensor::addToDescriptions(std::vector<ObjectDescription>& objectDescriptionTree,
								  int depth) 
{
	SimSensor::addToDescriptions(objectDescriptionTree, depth);  
	ObjectDescription sensorportDesc;
	sensorportDesc.name = "fsr";
	sensorportDesc.fullName = fullName + ".fsr";
	sensorportDesc.depth = depth + 1;
	sensorportDesc.type = OBJECT_TYPE_SENSORPORT;
	objectDescriptionTree.push_back(sensorportDesc);
}

void FSRSensor::computeValue(double &value, int portID)
{
	double sumx = 0, sumy = 0, sumz = 0;
	for(int i = 0; i < numFeedbacks; ++i)
	{
		sumx += jFeedBack[i].f1[0];
		sumy += jFeedBack[i].f1[1];
		sumz += jFeedBack[i].f1[2];
	}
	//sumx /= numFeedbacks;
	//sumy /= numFeedbacks;
	//sumz /= numFeedbacks;
	this->value = sumz;
	value = this->value;
}

SimObject* FSRSensor::clone() const
{
	FSRSensor* newFSR = new FSRSensor();
	newFSR->copyStandardMembers(this);
	bool createsNewMovID = newFSR->adjustMovableID();
	std::list<SimObject*>::const_iterator pos;
	for(pos = childNodes.begin(); pos != childNodes.end(); ++pos)
	{
		SimObject* childNode = (*pos)->clone();
		newFSR->addChildNode(childNode, false);
	}

	if(createsNewMovID)
		this->simulation->removeCurrentMovableID();

	SimObject* newObject = newFSR;
	return newObject;
}
void FSRSensor::postProcessAfterParsing()
{
	// Get pointer and body ID of physical body
	physObj = (*(childNodes.begin()))->castToPhysicalObject();
	bodyID = physObj->getBody();
}

void FSRSensor::resetJointFeedback()
{
	for(int j = 0; j < numFeedbacks; ++j){
		for(int i = 0; i < 4; ++i)
		{
			jFeedBack[j].f1[i] = 0;
			jFeedBack[j].f2[i] = 0;
			jFeedBack[j].t1[i] = 0;
			jFeedBack[j].t2[i] = 0;
		}
	}
	numFeedbacks = 0;
}