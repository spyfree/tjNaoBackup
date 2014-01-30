#include "PyTeamUDP.h"

static TeamUDPData* teamUDP_reference = 0;
static TeamMateInfo* teamMate_reference = 0;


MateInfo::MateInfo()
{
	matesInfo = teamMate_reference;
}
MateInfo::~MateInfo()
{

}
const int MateInfo::getPlayerNumber()
{
	return matesInfo->teamMateInfo0.header.player;
}
const float MateInfo::getBallLocationX()
{
	return matesInfo->teamMateInfo0.ballLocation.x;
}
const float MateInfo::getBallLocationY()
{
	return matesInfo->teamMateInfo0.ballLocation.y;
}
const float MateInfo::getBallCellX()
{
	return matesInfo->teamMateInfo0.ballLocation.cellX;
}
const float MateInfo::getBallCellY()
{
	return matesInfo->teamMateInfo0.ballLocation.cellY;
}
const int MateInfo::getState()
{
	return matesInfo->teamMateInfo0.state;
}
boost::python::dict MateInfo::GetMateDict()
{
	boost::python::dict mateDict;

	if(matesInfo->teamMateInfo1.received == true)
	{
		boost::python::list list1;

		list1.append(0);

		boost::python::list list11;
		list11.append(matesInfo->teamMateInfo1.robotLocation.x);
		list11.append(matesInfo->teamMateInfo1.robotLocation.y);
		list11.append(matesInfo->teamMateInfo1.robotLocation.angle);
		list11.append(matesInfo->teamMateInfo1.robotLocation.validity);

		list1.append(list11);
		list1.append(matesInfo->teamMateInfo1.ballSeen);

		boost::python::list list12;
		list12.append(matesInfo->teamMateInfo1.ballLocation.x);
		list12.append(matesInfo->teamMateInfo1.ballLocation.y);
		list12.append(matesInfo->teamMateInfo1.ballLocation.cellX);
		list12.append(matesInfo->teamMateInfo1.ballLocation.cellY);
		list12.append(matesInfo->teamMateInfo1.ballLocation.distance);

		list1.append(list12);
		list1.append(matesInfo->teamMateInfo1.kickPlayer);

		boost::python::list list13;
		list13.append(matesInfo->teamMateInfo1.state);
		list13.append(matesInfo->teamMateInfo1.mateNum);
		list13.append(matesInfo->teamMateInfo1.firstMateX);
		list13.append(matesInfo->teamMateInfo1.firstMateY);
		list13.append(matesInfo->teamMateInfo1.secondMateX);
		list13.append(matesInfo->teamMateInfo1.secondMateY);
		list13.append(matesInfo->teamMateInfo1.mateNear);

		list1.append(list13);

		mateDict[1] = list1;
	}

	if(matesInfo->teamMateInfo2.received == true)
	{
		boost::python::list list1;

		list1.append(0);

		boost::python::list list11;
		list11.append(matesInfo->teamMateInfo2.robotLocation.x);
		list11.append(matesInfo->teamMateInfo2.robotLocation.y);
		list11.append(matesInfo->teamMateInfo2.robotLocation.angle);
		list11.append(matesInfo->teamMateInfo2.robotLocation.validity);

		list1.append(list11);
		list1.append(matesInfo->teamMateInfo2.ballSeen);

		boost::python::list list12;
		list12.append(matesInfo->teamMateInfo2.ballLocation.x);
		list12.append(matesInfo->teamMateInfo2.ballLocation.y);
		list12.append(matesInfo->teamMateInfo2.ballLocation.cellX);
		list12.append(matesInfo->teamMateInfo2.ballLocation.cellY);
		list12.append(matesInfo->teamMateInfo2.ballLocation.distance);

		list1.append(list12);
		list1.append(matesInfo->teamMateInfo2.kickPlayer);

		boost::python::list list13;
		list13.append(matesInfo->teamMateInfo2.state);
		list13.append(matesInfo->teamMateInfo2.mateNum);
		list13.append(matesInfo->teamMateInfo2.firstMateX);
		list13.append(matesInfo->teamMateInfo2.firstMateY);
		list13.append(matesInfo->teamMateInfo2.secondMateX);
		list13.append(matesInfo->teamMateInfo2.secondMateY);
		list13.append(matesInfo->teamMateInfo2.mateNear);

		list1.append(list13);

		mateDict[2] = list1;
	}


	if(matesInfo->teamMateInfo3.received == true)
	{
		boost::python::list list1;

		list1.append(0);

		boost::python::list list11;
		list11.append(matesInfo->teamMateInfo3.robotLocation.x);
		list11.append(matesInfo->teamMateInfo3.robotLocation.y);
		list11.append(matesInfo->teamMateInfo3.robotLocation.angle);
		list11.append(matesInfo->teamMateInfo3.robotLocation.validity);

		list1.append(list11);
		list1.append(matesInfo->teamMateInfo3.ballSeen);

		boost::python::list list12;
		list12.append(matesInfo->teamMateInfo3.ballLocation.x);
		list12.append(matesInfo->teamMateInfo3.ballLocation.y);
		list12.append(matesInfo->teamMateInfo3.ballLocation.cellX);
		list12.append(matesInfo->teamMateInfo3.ballLocation.cellY);
		list12.append(matesInfo->teamMateInfo3.ballLocation.distance);

		list1.append(list12);
		list1.append(matesInfo->teamMateInfo3.kickPlayer);

		boost::python::list list13;
		list13.append(matesInfo->teamMateInfo3.state);
		list13.append(matesInfo->teamMateInfo3.mateNum);
		list13.append(matesInfo->teamMateInfo3.firstMateX);
		list13.append(matesInfo->teamMateInfo3.firstMateY);
		list13.append(matesInfo->teamMateInfo3.secondMateX);
		list13.append(matesInfo->teamMateInfo3.secondMateY);
		list13.append(matesInfo->teamMateInfo3.mateNear);

		list1.append(list13);

		mateDict[3] = list1;
	}


	if(matesInfo->teamMateInfo4.received == true)
	{
		boost::python::list list1;

		list1.append(0);

		boost::python::list list11;
		list11.append(matesInfo->teamMateInfo4.robotLocation.x);
		list11.append(matesInfo->teamMateInfo4.robotLocation.y);
		list11.append(matesInfo->teamMateInfo4.robotLocation.angle);
		list11.append(matesInfo->teamMateInfo4.robotLocation.validity);

		list1.append(list11);
		list1.append(matesInfo->teamMateInfo4.ballSeen);

		boost::python::list list12;
		list12.append(matesInfo->teamMateInfo4.ballLocation.x);
		list12.append(matesInfo->teamMateInfo4.ballLocation.y);
		list12.append(matesInfo->teamMateInfo4.ballLocation.cellX);
		list12.append(matesInfo->teamMateInfo4.ballLocation.cellY);
		list12.append(matesInfo->teamMateInfo4.ballLocation.distance);

		list1.append(list12);
		list1.append(matesInfo->teamMateInfo4.kickPlayer);

		boost::python::list list13;
		list13.append(matesInfo->teamMateInfo4.state);
		list13.append(matesInfo->teamMateInfo4.mateNum);
		list13.append(matesInfo->teamMateInfo4.firstMateX);
		list13.append(matesInfo->teamMateInfo4.firstMateY);
		list13.append(matesInfo->teamMateInfo4.secondMateX);
		list13.append(matesInfo->teamMateInfo4.secondMateY);
		list13.append(matesInfo->teamMateInfo4.mateNear);

		list1.append(list13);

		mateDict[4] = list1;
	}

	return mateDict;



}

class PyUDPPacket
{
public:
	PyUDPPacket(boost::python::dict packet)
	{
		_roleNumber = extract<int>(packet.keys()[0]);
		_frames = extract<int>(packet.values()[0][0]);
		_robotX = extract<float>(packet.values()[0][1][0]);
		_robotY = extract<float>(packet.values()[0][1][1]);
		_robotAngle = extract<float>(packet.values()[0][1][2]);
		_validity = extract<float>(packet.values()[0][1][3]);
		_ballSeen = extract<bool>(packet.values()[0][2]);
		_ballPeakX = extract<float>(packet.values()[0][3][0]);
		_ballPeakY = extract<float>(packet.values()[0][3][1]);
		_ballCellX = extract<float>(packet.values()[0][3][2]);
		_ballCellY = extract<float>(packet.values()[0][3][3]);
		_distanceToBall = extract<float>(packet.values()[0][3][4]);
		_kickPlayer = extract<int>(packet.values()[0][4]);

		_state = extract<int>(packet.values()[0][5][0]);
		_seenMateNum = extract<int>(packet.values()[0][5][1]);
		_firstMateX = extract<int>(packet.values()[0][5][2]);
		_firstMateY = extract<int>(packet.values()[0][5][3]);
		_secondMateX = extract<int>(packet.values()[0][5][4]);
		_secondMateY = extract<int>(packet.values()[0][5][5]);
		_mateNear = extract<int>(packet.values()[0][5][6]);

	}

	int    getRoleNumber(){return _roleNumber;}
	int	   getFrames(){return _frames;}
	float getRobotX(){return _robotX;}
	float getRobotY(){return _robotY;}
	float getRobotAngle(){return _robotAngle;}
	float getValidity(){return _validity;}
	bool getBallSeen(){return _ballSeen;}
	float getBallPeakX(){return _ballPeakX;}
	float getBallPeakY(){return _ballPeakY;}
	float getBallCellX(){return _ballCellX;}
	float getBallCellY(){return _ballCellY;}
	float getDistanceToBall(){return _distanceToBall;}
	int getKickPlayer(){return _kickPlayer;}

	int getState(){return _state;}
	int getSeenMateNum(){return _seenMateNum;}
	int getFirstMateX(){return _firstMateX;}
	int getFirstMateY(){return _firstMateY;}
	int getSecondMateX(){return _secondMateX;}
	int getSeconMateY(){return _secondMateY;}
	int getMateNear(){return _mateNear;}


	

private:
	int _roleNumber;
	int _frames;
	float _robotX;
	float _robotY;
	float _robotAngle;
	float _validity;
	bool _ballSeen;
	float _ballPeakX;
	float _ballPeakY;
	float _ballCellX;
	float _ballCellY;
	float _distanceToBall;
	int _kickPlayer;

	int _state;
	int _seenMateNum;
	int _firstMateX;
	int _firstMateY;
	int _secondMateX;
	int _secondMateY;
	int _mateNear;


	

};
class PyBallLocation
{
public:
	PyBallLocation(int playNumber,float x,float y,float cellX,float cellY)
	{
		_playerNumber = playNumber;
		_x = x;
		_y = y;
		_cellX = cellX;
		_cellY = cellY;
	}
	int getPlayerNumber(){ return _playerNumber; }
	float getBallX(){return _x;}
	float getBallY(){return _y;}
	float getCellX(){return _cellX;}
	float getCellY(){return _cellY;}
private:
	int _playerNumber;
	float _x;
	float _y;
	float _cellX;
	float _cellY;
};


class PyUDPInterface
{
public:
	PyUDPInterface()
	{
		teamUDPdata = teamUDP_reference;
	}
	void setBallLocation(PyBallLocation* ballLocation)
	{
		strcpy(teamUDPdata->header.header,PACKET_HEADER);
		teamUDPdata->header.player = ballLocation->getPlayerNumber();
		teamUDPdata->ballLocation.x = ballLocation->getBallX();
		teamUDPdata->ballLocation.y = ballLocation->getBallY();
		teamUDPdata->ballLocation.cellX = ballLocation->getCellX();
		teamUDPdata->ballLocation.cellY  = ballLocation->getCellY();

		teamUDPdata->send = true;
		teamUDPdata->role = 0;

	}
	void setDebugState(int state)
	{
		strcpy(teamUDPdata->header.header,DEBUG_HEADER);
		teamUDPdata->state = state;
		teamUDPdata->send = true;
		teamUDPdata->role = 0;
	}
	void sendUDPPacket(PyUDPPacket* packet)
	{
		strcpy(teamUDPdata->header.header,PACKET_HEADER);

		teamUDPdata->role = packet->getRoleNumber();
		teamUDPdata->frames = packet->getFrames();
		teamUDPdata->robotLocation.x = packet->getRobotX();
		teamUDPdata->robotLocation.y = packet->getRobotY();
		teamUDPdata->robotLocation.angle = packet->getRobotAngle();
		teamUDPdata->robotLocation.validity = packet->getValidity();
		teamUDPdata->ballSeen = packet->getBallSeen();
		teamUDPdata->ballLocation.x = packet->getBallPeakX();
		teamUDPdata->ballLocation.y = packet->getBallPeakY();
		teamUDPdata->ballLocation.cellX = packet->getBallCellX();
		teamUDPdata->ballLocation.cellY = packet->getBallCellY();
		teamUDPdata->ballLocation.distance = packet->getDistanceToBall();
		teamUDPdata->kickPlayer = packet->getKickPlayer();

		teamUDPdata->state = packet->getState();
		teamUDPdata->mateNum = packet->getSeenMateNum();
		teamUDPdata->firstMateX = packet->getFirstMateX();
		teamUDPdata->firstMateY = packet->getFirstMateY();
		teamUDPdata->secondMateX = packet->getSecondMateX();
		teamUDPdata->secondMateY = packet->getSeconMateY();
		teamUDPdata->mateNear = packet->getMateNear();

		teamUDPdata->send = true;


	}
	void setZero()
	{
		teamUDPdata->send = false;
	}

private:
	TeamUDPData* teamUDPdata;
};

BOOST_PYTHON_MODULE(PyTeamUDP)
{

	class_<MateInfo>("MateInfo")
		.def("playerNumber",&MateInfo::getPlayerNumber)
		.def("ballLocationX",&MateInfo::getBallLocationX)
		.def("ballLocationY",&MateInfo::getBallLocationY)
		.def("ballCellX",&MateInfo::getBallCellX)
		.def("ballCellY",&MateInfo::getBallCellY)
        .def("state",&MateInfo::getState)
		.def("getDict",&MateInfo::GetMateDict)
		;
	class_<PyBallLocation>("BallLocation",init<int,float,float,float,float>());
	class_<PyUDPPacket>("UDPPacket",init<boost::python::dict>());
	class_<PyUDPInterface>("TeamUDP")
		.def("setBallLocation", &PyUDPInterface::setBallLocation)
		.def("setDebugState",&PyUDPInterface::setDebugState)
		.def("setZero",&PyUDPInterface::setZero)
		.def("sendUDPPacket",&PyUDPInterface::sendUDPPacket);


}

void c_init_teamUDP()
{
	if (!Py_IsInitialized())
		Py_Initialize();

	try
	{
		initPyTeamUDP();
	}
	catch (error_already_set)
	{
		PyErr_Print();
	}
}

void set_teamUDP_reference (TeamUDPData* _teamUDPdata,TeamMateInfo*_teammateData)
{
	teamUDP_reference = _teamUDPdata;
	teamMate_reference= _teammateData;
}
