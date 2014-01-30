/**
* @file KS.h
* This file provide macroes and classes for the build of our Blackboard system,
* this file is a modification version of BHuman's Blackboard system.
* @author XU Tao
*/
#ifndef __KS_h_
#define __KS_h_
#include "Blackboard.h"
#include "Platform/GTAssert.h"
#include <list>
#include <string>
/**
* @class Requires
* The class collects all requirements of our project
* that have to be updated before the module is executed.
*/
class Requires
{
public:
	/**
	* A class for representing information about a representation.
	*/
	class Entry
	{
	public:
		const std::string name; /**< The name of the representation. */
		void (*create)(); /**< The handler that is called to create a new instance of the representation. */
		void (*free)(); /**< The handler that is called to delete the instance of the representation. */
		//void (*in)(In&); /**< The handler that is called to read the instance of the representation from a stream. */

		/**
		* Constructor.
		* @param name The name of the representation.
		* @param create The handler that is called to create a new instance of the representation.
		* @param free The handler that is called to delete the instance of the representation.
		*/
		Entry(const std::string name, void (*create)(), void (*free)()) :
			name(name),
			create(create),
			free(free)
			{}

		/**
		* Comparison operator. Only uses the name for comparison.
		* @param other The representaion name this one is compared to.
		* @return Are the representation names the same?
		*/
		bool operator==(const std::string& other) const {return other == name;}
	};
	typedef std::list<Entry> List; /**< Type of the list of all requirements. */
	static List entries; /**< The list of all requirements. */
	static bool record; /**< A flag that determines whether requirements are currently recorded or not. */
public: //2 change to protected: later!!
	/**
	* The method adds a new requirement to the list but only if the class
	* is currently in recording mode.
	* @param name The name of the requirement.
	* @param create The handler that is called to create a new instance of the representation.
	* @param free The handler that is called to delete the instance of the representation.
	*/
	static void add(const std::string name, void (*create)(), void (*free)());
};

/**
* @class Provides
* The class collects all representations that our project provides.
*/
class Provides
{
public:
	class ProviderPair
	{
	public:
		ProviderPair(const std::string& repname_, const std::string& ksname_)
			:repname(repname_), ksname(ksname_){}
		std::string repname;
		std::string ksname;
	};
	/**
	* A class for representing information about a Provide.
	*/
	class Entry
	{
	public:
		const std::string name; /**< The name of the representation. */
		const std::string ksname; /**< The name of the ks*/
		void (*update)(Blackboard*); /**< The handler that is called to update the representation. */
		void (*create)(); /**< The handler that is called to create a new instance of the representation. */
		void (*free)(); /**< The handler that is called to delete the instance of the representation. */
		//void (*out)(Out&); /**< The handler that is called to write the instance of the representation to a stream. */

		/**
		* Constructor.
		* @param name The name of the Provides.
		* @param update The handler that is called to update the representation.
		* @param create The handler that is called to create a new instance of the representation.
		* @param free The handler that is called to delete the instance of the representation.
		*/
		Entry(const std::string name, const std::string ksname, void (*update)(Blackboard*), void (*create)(), void (*free)()) :
			name(name),
			ksname(ksname),
			update(update),
			create(create),
			free(free)
		{}

		/**
		* Comparison operator. Only uses the name for comparison.
		* @param other The representaion name this one is compared to.
		* @return Are the representation names the same?
		*/
			bool operator==(const Provides::ProviderPair& other) const {return other.repname == name && other.ksname == ksname;}
	};

	typedef std::list<Entry> List; /**< Type of the list of all representations. */
	static List entries; /**< The list of a representations provided. */
	static bool record; /**< A flag that determines whether requirements are currently recorded or not. */

public: //2 change to protected: later!!
	/**
	* The method adds a new representation to the list but only if the class
	* is currently in recording mode.
	* @param name The name of the representation.
	* @param update The handler that is called to update the representation.
	* @param create The handler that is called to create a new instance of the representation.
	* @param free The handler that is called to delete the instance of the representation.
	*/
	static void add(const std::string name, const std::string ksname, void (*update)(Blackboard*), void (*create)(),void (*free)());
};

/**
* Macros to add requires into require list
*/
#define ADD_REQUIRES(cKS, cREP) \
	Requires::record = true; \
	Requires::add(#cREP, &(cKS::create2##cREP),&(cKS::free2##cREP)); \
	Requires::record = false;

/**
* Macros to add provides into provides list
*/
#define ADD_PROVIDES(cKS, cREP) \
	Provides::record = true; \
	Provides::add(#cREP, #cKS, &(cKS::update##cREP), &(cKS::create##cREP), &(cKS::free##cREP)); \
	Provides::record = false;

class KSManager;
/**
* @class KSBase
* The class is the abstract base of all template classes that create KSes.
*/
class KSBase
{
private:
	static std::list<KSBase*> kses;
	const std::string name;
protected:
	virtual Blackboard* createNew() = 0;
public:
	KSBase(const std::string _name):
	  name(_name)
	  {
		  kses.push_back(this);
	  }
	  const std::string getName() const {return name;}
friend class KSManager;
};
/**
* Knowledge Source Class
*/
template <class M> class KS : public KSBase
{
private:
	Blackboard* createNew()
	{
		return (Blackboard*) new M;
	}
public:
	KS(const std::string name)
		:KSBase(name)
	{
	}
};


#define DEFINE_KS(ks) \
class KSManager; \
class TJArkPro; \
class ks##Base : private Blackboard \
{ \
friend class KSManager; \
friend class TJArkPro; \
friend class MainModule;\
friend class MainWindow;\
private: typedef ks##Base _Me; \
private: bool _initialized; \
public: ks##Base(): Blackboard(*Blackboard::theInstance), _initialized(false) {} \
public: virtual void init(){} \
using Blackboard::operator new; \
using Blackboard::operator delete;

/**
* The macro defines a requirement.
* @param representation The representation that has to be updated before executing this module.
*/
#define REQUIRES(representation) \
protected: using Blackboard::the##representation; \
\
/** \
* The method creates the representation in the blackboard. \
*/ \
static void create2##representation() \
{ \
	if(!((_Me*) Blackboard::theInstance)->the##representation) \
		replace2##representation((representation**) Blackboard::theInstance, new representation); \
} \
	\
	/** \
* The method deletes the representation from the blackboard. \
*/ \
	static void free2##representation() \
{ \
	if(((_Me*) Blackboard::theInstance)->the##representation) \
{ \
	delete ((_Me*) Blackboard::theInstance)->the##representation; \
		replace2##representation((representation**) Blackboard::theInstance, NULL); \
} \
} \
\
static void replace2##representation(representation** r, representation* rNew) \
{ \
	for(unsigned i = 0; i < sizeof(Blackboard) / sizeof(representation*); ++i) \
	{ \
		representation* temp = r[i]; \
		r[i] = rNew; \
		distract(); /* This call forces the compiler to consider possible changes to global data. */ \
		if(((_Me*) Blackboard::theInstance)->the##representation == rNew) \
			return; \
		r[i] = temp; \
	} \
	ASSERT(false); \
} \
\
/** \
* The method get the representation. \
*/ \
private: virtual  void in##representation() = 0; \
private: \
	/** \
* The method returns the name of the representation. \
*/ \
static const char* getName2##representation() {return #representation;}
/**
* The macro defines a usage, i.e. a representation that is accessed but does not need to be up to date.
* See beginning of this file.
* @param representation The representation that is used.
*/
#define USES(representation) \
protected: using Blackboard::the##representation;

/**
* The macro defines a representation that is updated by this module.
* @param representation The representation that can be updated by this module.
*/
#define PROVIDES(representation) \
	_PROVIDES(representation, )

/**
* The macro defines a representation that is updated by this module.
* @attention Don't use this macro directly.
* This macro also define methods to create and delete this representation in the
* blackboard. Please note that replacing the references in the blackboard is
* really dirty, but it works and abstracts from the fact that not all representations
* exist all the time. In fact, only the representations currently in use are actually
* constructed.
* @param representation The representation that can be updated by this module.
* @param mod Either a MODIFY expression or nothing.
*/
#define _PROVIDES(representation, mod) \
/** \
* The derived class must implement an update method for this representation. \
* @param repName The representation that is updated. \
*/ \
protected: virtual void update(representation* the##representation) = 0; \
	\
/** \
* The method is called to update the representation by this module. \
* @param b The module. \
*/ \
private: static void update##representation(Blackboard* b) \
{ \
	const representation* r = ((_Me*) Blackboard::theInstance)->the##representation; \
	ASSERT(r); \
	if(!((_Me*) b)->_initialized) \
{ \
	((_Me*) b)->init(); \
	((_Me*) b)->_initialized = true; \
} \
	((_Me*) b)->update(const_cast<representation*>(r)); \
	mod \
} \
	\
/** \
* The method creates the representation in the blackboard. \
*/ \
static void create##representation() \
{ \
	if(!((_Me*) Blackboard::theInstance)->the##representation) \
		replace##representation((representation**) Blackboard::theInstance, new representation); \
} \
	\
/** \
* The method deletes the representation from the blackboard. \
*/ \
static void free##representation() \
{ \
	if(((_Me*) Blackboard::theInstance)->the##representation) \
	{ \
		delete ((_Me*) Blackboard::theInstance)->the##representation; \
			replace##representation((representation**) Blackboard::theInstance, 0); \
	} \
} \
\
/** \
* The method replaces the reference (!) to the representation in the blackboard by a new one. \
* Since references cannot be overwritten, this is done by a search and replace operation. \
* @param r A pointer to the blackboard as array of pointers. \
* @param rNew The new entry. \
*/ \
static void replace##representation(representation** r, representation* rNew) \
{ \
	for(unsigned i = 0; i < sizeof(Blackboard) / sizeof(representation*); ++i) \
	{ \
		representation* temp = r[i]; \
		r[i] = rNew; \
		distract(); /* This call forces the compiler to consider possible changes to global data. */ \
		if(((_Me*) Blackboard::theInstance)->the##representation == rNew) \
			return; \
		r[i] = temp; \
	} \
	ASSERT(false); \
} \
	\
/** \
* The method output the representation. \
*/ \
protected:virtual void out##representation(/*Blackboard* b*/) = 0; \
\
\
/** \
* The method returns the name of the representation. \
*/ \
	static const char* getName##representation() {return #representation;}

#define END_KS };


/**
* The macro creates a creator for the KS
* @param module The name of the module that can be created.
* @param base The name of the base class of the module.
*/
#define MAKE_SOLUTION(ks, base) KS<ks> the##ks##KS(#ks);

/**
* The macro creates a creator for the KS.
* @param module The name of the module that can be created.
*/
#define MAKE_KS(ks) MAKE_SOLUTION(ks, ks)

#endif //__KS_h_