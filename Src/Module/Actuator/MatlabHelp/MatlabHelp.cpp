#include "MatlabHelp.h"
#include "Platform/GTAssert.h"
//#include "engine.h"
MatlabHelp* MatlabHelp::theInstance = 0;
//Engine *pMatlabEngine = NULL;
MatlabHelp::MatlabHelp()
{
	pMatlabEngine = 0;
	if((pMatlabEngine = engOpen("\0")))
	{
		engEvalString(pMatlabEngine, "clear");
		engEvalString(pMatlabEngine, "echo on");
	}
}
MatlabHelp::~MatlabHelp()
{
	if(pMatlabEngine)
		engClose(pMatlabEngine);
}
//void MatlabHelp::storeMatrixMN(const MatrixMN<double>& matrix, const std::string& name)
//{
//	ASSERT(pMatlabEngine);
//	mxArray *recMatrix = mxCreateDoubleMatrix(matrix.m, matrix.n,mxREAL);
//	memcpy((void *)mxGetPr(recMatrix), (void *)matrix.data, matrix.m * matrix.n *sizeof(double));
//	engPutVariable(pMatlabEngine, name.c_str(), recMatrix);
//	if(recMatrix)
//		mxDestroyArray(recMatrix);
//}
void MatlabHelp::addPlot(const double& value, const std::string& name)
{
	ASSERT(pMatlabEngine);
	mxArray *recData = mxCreateDoubleMatrix(1, 1,mxREAL);
	mxArray *vname = mxCreateString(name.c_str());
	memcpy((void *)mxGetPr(recData), (void *)(&value), sizeof(double));
	engPutVariable(pMatlabEngine, "tmp", recData);
	engPutVariable(pMatlabEngine, "vname", vname);
	std::string cmd = "";
	cmd =cmd + "if(exist(vname,'var')) " + name + "(length(" + name + ") +1) = tmp; else " + name +" = tmp; end";
	engEvalString(pMatlabEngine, cmd.c_str());
	if(recData)
		mxDestroyArray(recData);
}
void MatlabHelp::addSaveVector(const double* value, int n, const std::string& name)
{
	ASSERT(pMatlabEngine);
	mxArray *recData = mxCreateDoubleMatrix(1, n,mxREAL);
	mxArray *vname = mxCreateString(name.c_str());
	memcpy((void *)mxGetPr(recData), (void *)value, n *sizeof(double));
	engPutVariable(pMatlabEngine, "tmp", recData);
	engPutVariable(pMatlabEngine, "vname", vname);
	std::string cmd = "";
	cmd =cmd + "if(exist(vname,'var')) " + "vSize = size( "+ name + ");" + name + "( vSize(1)+1,:) = tmp; else " + name +" = tmp; end";
	engEvalString(pMatlabEngine, cmd.c_str());
	if(recData)
		mxDestroyArray(recData);
}