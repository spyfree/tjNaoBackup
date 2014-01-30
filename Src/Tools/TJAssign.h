/************************************************************************/
/* file TJAssign.h
Assign class parameters by values reading from config files*/
/************************************************************************/
#ifndef PARAMASSIGN_H_
#define PARAMASSIGN_H_
#include <fstream>
#include <iostream>
#include <string>
#include <typeinfo>
#include <map>
#include <ctype.h>
#include "Platform/GTAssert.h"
//#define ASSERT(c) if(!(c)) {__asm{int 3};}
typedef std::map<std::string,std::string> param;
class paramAssign
{
public:
	paramAssign(){}

	~paramAssign(){/*params.clear();*/}

	void paramReset();

	//A block data was assigned once a time ,abstract function override by child class
	virtual void massAssign(){} 

	//assign parameters of int or double 
	template <typename T>
	void paramAssigning(std::string x,T& y)
	{
		std::string name=x.substr(x.find('.')+1);
		param::iterator iter=params.find(name);
		//assert(iter!=y.end());
		ASSERT(iter!=params.end());
		//return iter->second;
		if (typeid(y)==typeid(int))
		{
			std::string temp=iter->second;
			y=atoi(temp.c_str());
		}
		if (typeid(y)==typeid(double))
		{
			std::string temp=iter->second;
			y= atof(temp.c_str());
		}
	}


	//read parameter's value from config file
	void readCfgFile(const std::string& name);


private:
	param params;
};




#define ASSIGN(x) paramAssigning(#x,x)
#endif