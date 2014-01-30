#include "PyTeammate.h"

static const perceiveInfo* locData_reference = 0;
static const TeamMateInfo* teammateData = 0;

class PyLocData
{
public:
	PyLocData(boost::python::tuple locData)
	{
		_locData.team = extract<int>(locData[0]);
		_locData.player = extract<int>(locData[1]);
		_locData.color = extract<unsigned char>(locData[2]);
		_locData.playerX = extract<float>(locData[3]);
		_locData.playerY = extract<float>(locData[4]);
		_locData.playerH = extract<float>(locData[5]);
		_locData.ballX = extract<float>(locData[6]);
		_locData.ballY = extract<float>(locData[7]);
		_locData.ballDist = extract<float>(locData[8]);
		_locData.ballVelX = extract<float>(locData[9]);
		_locData.ballVelY = extract<float>(locData[10]);
	}
	perceiveInfo getLocData(){ return _locData; }
private:
	perceiveInfo _locData;
};

class PyLocDataInterface
{
public:
	PyLocDataInterface()
	{
		locData = locData_reference;
	}
	void setLocData(PyLocData* command)
	{
		locData = &(command->getLocData());
	}

private:
	const perceiveInfo* locData;
};

static PyObject* getMate()
{
	PyObject *t;
	t = PyTuple_New(2);
	for (int i=0;i<2;i++)
	{
		PyObject *row = PyTuple_New(11);

		PyTuple_SetItem(row,0,PyInt_FromLong(teammateData->MatesInfo[i].team));
		PyTuple_SetItem(row,1,PyInt_FromLong(teammateData->MatesInfo[i].player));
		PyTuple_SetItem(row,2,PyFloat_FromDouble(teammateData->MatesInfo[i].color));
		PyTuple_SetItem(row,3,PyFloat_FromDouble(teammateData->MatesInfo[i].playerX));
		PyTuple_SetItem(row,4,PyFloat_FromDouble(teammateData->MatesInfo[i].playerY));
		PyTuple_SetItem(row,5,PyFloat_FromDouble(teammateData->MatesInfo[i].playerH));
		PyTuple_SetItem(row,6,PyFloat_FromDouble(teammateData->MatesInfo[i].ballX));
		PyTuple_SetItem(row,7,PyFloat_FromDouble(teammateData->MatesInfo[i].ballY));
		PyTuple_SetItem(row,8,PyFloat_FromDouble(teammateData->MatesInfo[i].ballDist));
		PyTuple_SetItem(row,9,PyFloat_FromDouble(teammateData->MatesInfo[i].ballVelX));
		PyTuple_SetItem(row,10,PyFloat_FromDouble(teammateData->MatesInfo[i].ballVelY));

		PyTuple_SetItem(t,i,row);
	}
	return t;

}

BOOST_PYTHON_MODULE(PyTeammate)
{

	class_<PyLocData>("LocData",init<boost::python::tuple>());
	class_<PyLocDataInterface>("SendLocData")
		.def("setLocData", &PyLocDataInterface::setLocData);
	def("getMates",&getMate);
	
}

void c_init_teammate()
{
	if (!Py_IsInitialized())
		Py_Initialize();

	try 
	{
		initPyTeammate();
	} 
	catch (error_already_set) 
	{
		PyErr_Print();
	}
}

void set_teammate_reference(const perceiveInfo *_locData,const TeamMateInfo*_teammateData)
{
	locData_reference = _locData;
	teammateData = _teammateData;
}
