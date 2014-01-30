#include "Tools/TJAssign.h"
#include <fstream>
using namespace std;

void paramAssign::paramReset()
{
	params.clear();
}

void paramAssign::readCfgFile(const std::string& name)
{
	ifstream inF(name.c_str());
	ASSERT(inF.is_open());
	//if (!inF.is_open())
	//{
	//	std::cout<<"inF open failed!"<<std::endl;
	//}
	std::string line,key,mapValue;
	int index=0;
	//char buf[20]; 
	while (getline(inF,line))
	{
		if (line.find(' ')!=std::string::npos)
			line=line.erase(line.find(' '),std::string::npos);
		if(line.find('/')!=std::string::npos)
			line=line.erase(line.find('/'),std::string::npos);
		if (line.at(0)=='#')
		{
			index++;
			key=line.substr(1);
		}
		else if (isdigit(line.at(0))||line.at(0)=='-'&&isdigit(line.at(1)))
		{
			mapValue=line;
			if ((++index)%2==0)
			{
				params.insert(make_pair(key,mapValue));
			}
		}
	}
}