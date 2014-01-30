#ifndef __MatlabHelp_h_
#define __MatlabHelp_h_

#ifndef WIN32
#define MATLABHELP_INIT()
#define MATLABHELP_DESTORY()
#define MATLABHELP_STORE_MATRIXMN(m,name)
#define MATLABHELP_PLOT(v, name)
#else
#include <string>
//#include "../JacobianMath/MatrixMN.h"
#include "engine.h"
#define MATLABHELP_INIT()  MatlabHelp::Init()
#define MATLABHELP_DESTORY() MatlabHelp::Destroy();
//#define MATLABHELP_STORE_MATRIXMN(m,name) MatlabHelp::getInstance()->storeMatrixMN(m,name)
#define MATLABHELP_PLOT(v, name) MatlabHelp::getInstance()->addPlot(v, name)
#define MATLABHELP_SAVE_VECTOR(v, n, name) MatlabHelp::getInstance()->addSaveVector(v, n, name)
class MatlabHelp
{
public:
	static void Init()
	{
		if(!theInstance)
			theInstance = new MatlabHelp();
	}
	static void Destroy()
	{
		if(theInstance){
			delete theInstance;
			theInstance = 0;
		}
	}
	static MatlabHelp* getInstance()
	{
		if(!theInstance)
			Init();
		return theInstance;
	}
	//void storeMatrixMN(const MatrixMN<double>& matrix, const std::string& name);
	void addPlot(const double& value, const std::string& name);
	void addSaveVector(const double* value, int n, const std::string& name);
private:
	static MatlabHelp* theInstance;
	MatlabHelp();
	~MatlabHelp();
	Engine *pMatlabEngine;
};
#endif //Win32
#endif