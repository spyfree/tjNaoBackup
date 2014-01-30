#include "KS.h"

void Requires::add(const std::string name, void (*create)(),void (*free)())
{
	if(record)
		entries.push_back(Entry(name, create, free));
}

void Provides::add(const std::string name, const std::string ksname, void (*update)(Blackboard*), void (*create)(),void (*free)())
{
	if(record)
		entries.push_back(Entry(name, ksname, update, create, free));
}

// init before all other global variables
 #ifdef _MSC_VER
 #pragma warning(disable:4073)
 #pragma init_seg(lib)
#define FIRST
 #else
 #ifdef __GNUC__
 #define FIRST __attribute__ ((init_priority (101)))
 #else
 #define FIRST
 #endif
 #endif

std::list<Requires::Entry> Requires::entries FIRST;
bool Requires::record = false;
std::list<Provides::Entry> Provides::entries FIRST;
bool Provides::record = false;
std::list<KSBase*> KSBase::kses FIRST;// init before all other global variables

