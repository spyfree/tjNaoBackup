#ifndef _KSManager_h_
#define _KSManager_h_

#include "KS.h"
#ifdef NAO_DEBUG
#include "Simulation/SignalControl.h"
#endif

class KSManager
{
private:
	
	class KSAgent
	{
	public:
		KSBase* ks;		/**< A pointer to the ks base that is able to create an instance of the ks. */
		Blackboard* ksInstance;/**< A pointer to the instance of the ks if it was created. Otherwise the pointer is 0. */
		bool required; /**< A flag that is required when determining whether a ks is currently required or not. */
		/**
		* Constructor.
		* @param ks A pointer to the ks base that is able to create an instance of the ks.
		*/
		KSAgent(KSBase* _ks): ks(_ks), ksInstance(NULL), required(false){}

		/**
		* Comparison operator
		*/
		bool operator==(const std::string other) {return ks->getName() == other;}
	};

	class Provider
	{
	public:
		std::string representation; /**< The representation that will be provided. */
		std::string ksname; /**< The name of the ks that provide the representation*/
		const KSAgent* ksAgent;  /**< The ks Agent that will give access to the ks that provides the information. */
		void (*update)(Blackboard*); /**< The update handler within the ks. */
		void (*create)();						/**< The method to create a new instance of the representation. */
		void (*free)();		/**< The method to delete an instance of the representation. */
		/**
		* Constructor.
		* @param representation The name of the representation provided.
		* @param moduleState The moduleState that will give access to the module that provides the information.
		* @param update The update handler within the module.
		* @param create The create handler for the representation.
		* @param free The free handler for the representation.
		*/
		Provider(const std::string& representation, const std::string ksname_, const KSAgent* ksAgent, 
			void (*update)(Blackboard*), void (*create)(), void (*free)()) :
			representation(representation),
			ksname(ksname_),
			ksAgent(ksAgent),
			update(update),
			create(create),
			free(free)
		{
		}
		/**
		* Comparison operator. Only uses the representation for comparison.
		* @param other The other provider this one is compared to.
		* @return Are the representation names the same?
		*/
		bool operator==(const Provider& other) const {return (representation == other.representation && ksname == other.ksname);}

		/**
		* Only uses the representation for comparison.
		* @param representation The representation this one is compared to.
		* @return Does this provider provide the given representation?
		*/
		//bool ifProvide(const std::string& representation) const {return this->representation == representation;}
	};

class Requirement
{
public:
	std::string representation;
	void (*create)();
	void(*free)();

	Requirement(const std::string& _represent):representation(_represent), create(NULL), free(NULL){}

	/**
	* Comparison operator. Only uses reprensentation for compare;
	* Use for std::find()
	*@param other  The representation name;
	*@return Are the requirement with the name of other
	*/
	bool operator==(const std::string& other) const {return representation == other;}
};

private:
	std::list<KSAgent> ksAgents;/**< a list of the current ks agents. */

	std::list<Provider> providers; /**< The list of providers that will be executed. */

	std::list<Requirement> requires; /**<The list of required representations*/

	unsigned timeStamp; /**< The timeStamp of the last module request. Communication is only possible if both sides use the same timestamp. */
	/**
	* The method brings the providers in the correct sequence.
	* @return Is the set of providers consistent?
	*/
	std::string managername;
	
	//bool sortProviders();

public:
	KSManager();
	/**
	* Feed the name of the thead module, e.g. Vision, Actuator, ...
	* Produce Blackboard System according to this thread.
	* @param nameThread name of the thread
	*/
	KSManager(const std::string& nameThread);
	/**
	* Destructor
	*/
	~KSManager();
	/**
	* Destroy  allocated things within this KSManager
	* This can be called before default destructor, e.g. Motion.terminate(){manager.destory();process.terminate();}
	*/
	void destroy();
	/**
	* Rebuild the Blackboard System and all the KS
	*/
	void update(unsigned _timeStamp);
	/**
	* Load config from a file and then build the KS system
	*/
	//void load();
	/**
	* Run all KS
	*/
	void execute();
};
#endif //_KSManager_h_