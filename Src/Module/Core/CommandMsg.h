#ifndef CommandMsg_H
#define CommandMsg_H
// TODO: It's better to use macros to define send and recieve messages
typedef enum
{
	idUndefine,
	idUncorrectedRaw,
	idClassifyImage,
	idVisionInterface,
	idVisionPercept,//////////////add
	idTJImage,
	idLut,
	idMotionJoint,
	idSimImage,
	idFrameInfo,
	idActuatorFrameInfo,
	idSimSensorData,
	idSensoryJointData,
	idMotionCmd,
	idJointCmd,
	idMotionBoardJointCmd,
	idVisionSimData,
	idGameControlData,
	idLocData,
	idTeamMateInfo,
	idRobotState,
	idInertiaMatrix,
	idOdometryData,
	idLocDrawing,
	numCMsgID,
	idGoalPercept,
	idBallresult,
	idCircle,
	idObjectDraw,
	idCmd,
	idVisionCmd,
	idLocCmd,
	idBallSample,
	idLocSample,
	idFreePart,
	idSended,
	idGlobalInfo,
	idBallModel

}CMsgID;
typedef enum
{
	idundefineThread,
	idImageDialog,
	idMotionEngineWindow,
	idMotionCmdBuffer,
	idVisionThread,
	idActuatorThread,
	idRobotThread,
	idNetworkThread,
	idBehaviorThread,
	idODESimulation,
	idLocatorThread,
	idMainWindow
	//idMainThread

}CMsgThID;

class MessageType
{
public:
	MessageType();
	MessageType(CMsgID dataID,CMsgThID sendThread,CMsgThID receiveThread);
	void init();
	void setMessageType(CMsgID dataID,CMsgThID sendThread,CMsgThID receiveThread);
	bool operator==(const MessageType&type);

private:
	CMsgID RS_COMMAND;
	CMsgThID SendThread;
	CMsgThID RecThread;
};

class CommandMsg
{
public:
	CommandMsg();
	~CommandMsg();
	CommandMsg(const MessageType &type,const long &size,char*data);
public:
	void  GetData(char *data,int size=-1);
	void setValue(const MessageType &type,const long &size,char*data );
	bool operator==(const CommandMsg& msg);
	bool IsMessageType(MessageType isMessageType);
	long CmSize;
	void releaseData();
private:
	void init();
	MessageType messageType;
	char *CmData;
};
#endif
