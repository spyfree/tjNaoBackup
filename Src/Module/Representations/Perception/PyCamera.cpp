#include "PyCamera.h"

const static CameraConfig* camera_reference = 0;

class PyCameraConfig
{
public:
	PyCameraConfig(bool usingTopCam, bool CamChanged)
	{
		_usingTopCam = usingTopCam;
		_CamChanged = CamChanged;
	}
	bool getIsTopCam(){return _usingTopCam; }
	bool getCamChanged(){return _CamChanged; }
private:
	bool _usingTopCam;
	bool _CamChanged;
};

class PyCameraInterface
{
public:
	PyCameraInterface()
	{
		cameraConfig = camera_reference;
	}
	void setCameraConfig(PyCameraConfig* command)
	{
		cameraConfig->usingTopCam = command->getIsTopCam();
		cameraConfig->CamChanged = command->getCamChanged();
	}
	
private:
	const CameraConfig* cameraConfig;
};

BOOST_PYTHON_MODULE(PyCamera)
{

	class_<PyCameraConfig>("CameraCmd",init<bool,bool>());
	class_<PyCameraInterface>("camera")
		.def("setCameraConfig", &PyCameraInterface::setCameraConfig)
		;

}

void c_init_camera()
{
	if (!Py_IsInitialized())
		Py_Initialize();

	try 
	{
		initPyCamera();
	} 
	catch (error_already_set) 
	{
		PyErr_Print();
	}
}

void set_camera_reference (const CameraConfig* _cameraConfig)
{
	camera_reference = _cameraConfig;
}
