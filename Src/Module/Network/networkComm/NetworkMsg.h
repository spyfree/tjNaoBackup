#ifndef COMMANDMSG_H
#define COMMANDMSG_H
typedef enum
{
	idUndefined,/*!< undefined message ID */
	idString, /*!< the message contains plain string*/
	//=======Sendings request=======/
	idSensorData,/*!< raw Sensors data*/
	idGpsSImple,/*!< Simple Gps information,e.g: Self pos,heading, var, Ball pos, speed, and variance*/
	idGpsAll, /*!< Information fo All Gps State*/
	//Images
	idRawImage,/*!< YUV raw image, 6byte every pixel, followed by some other data*/
	idClassifidImage, /*!< Classifyed image, size: 208*160 byte*/
	idDebugImageDrawings,/*!< Debug Drawings of vision prcosess*/
	//perception
	idHorizonLine,/*!< Horizantal line computed by the robot*/
	idSimplePerceptAll,/*!< Simple Vision result of robot,e.g: relative coordinate of all recognised object*/
	idBallPercept,/*!<  contains the ballpercept from the scanvision process*/
	idLandmarkPercept,/*!<  contains the landmarks percept from the scanvision process*/
	idLinesPercept, /*!<  contains the lines percept from the scanvision process*/
	idObstaclePercept,/*!<  contains the obstacles percept from the scanvision process*/
	idLocatorData,
	//==========commands========/
	idWalkType, /*!< Change the Walk type*/
	idWalkSpeed,/*!< Change the walk speed, forward, left, turnCCW*/
	idHeadMove, /*!< Head type, panx, tilty, cranez*/
	idCmeraSet,/*!< Set Cemera mode, Shutter, Gain, Mode*/
	idPIDSet,/*!< Set PID gain*/
	idActionReload,/*!< Let robot reload the special action files*/
	idLUTReload, /*!< Reload Corlor look-up-table*/
	//=========Data send to robot=========/
	idWalkParams, /*!< Walk parameters to robot*/
	numOfMessageIDs /*!< The number of over all message ids*/
} NetMsgID;

typedef enum
{
	INFO_REQUEST,
	INFO_PROVIDE,
	NO_REQUEST,
	SEND_COMMAND,
	DISCONNECT
}CommandType;

typedef enum {
	NO_COMMAND,
	MODE_CHANGE,
	UTHER_KICKING_OFF,
	PLAYER_CHANGE,
	WALKTYPE_CHANGE,
	WALKPARAM1_CHANGE,  // 5
	WALKPARAM2_CHANGE,
	WALKPARAM3_CHANGE,
	FORWARD_MOVE,
	HEAD_MOVE,
	LEFT_MOVE,          //10
	TURN_MOVE,
	SET_TILT,
	SET_TILT2,
	SET_PAN,
	//----newly added for TJDebuger to control mouth joint-----
	//SET_MOUTH,
	//--------------------------------------------------
	DO_BLOCK_STANCE,    //15
	DO_HOLD_STANCE,
	NORMAL_STANCE_ABSH,
	NORMAL_STANCE_RELH,
	SET_STANCE,
	DO_TURN_KICK,       //20
	PAN_TILT,
	TILT_PAN,
	BALL_TRK,
	MOVE_COMMAND,
	HUMAN_CONTROL,      //25
	SET_COLOUR,
	SET_INDICATOR,
	EDGE_DETECT_DEBUG,
	CPLANE_SEND,
	BURST_MODE,         //30
	WM_SEND,
	YUVPLANE_SEND,
	OPLANE_SEND,
	BEACON_DEBUG,
	SET_BEACON_DIST_CONST, //35
	TEST_TURN_KICK,
	SET_CURRENTLY_CARRYING,
	DO_WM_LAT_TEST,
	CAL_TURN_KICK,
	WRITE_TO_CONSOLE,       //40
	RELAX_DOG,
	WALK_LEARNING,
	WALK_CALIBRATE,
	SEND_JOINT_DEBUG,
	GENERAL_TESTER_COMMAND,   //45
	SET_JOINT_SPEED_LIMIT,
	SHUT_DOWN_ROBOT,
	DANIEL_INDICATOR_TEST1,
	DANIEL_INDICATOR_TEST2,
	PYTHON_COMMAND,            //50
	COMMAND_TO_PYTHON_MODULE,
	CHANGE_PIDGAIN,
	SET_CPLANE_SENDING_RATE,
	RELOAD_SPECIAL_ACTION,
	YUVPLANE_STREAM	,			//55
	MCL_Data_Debug,
	Loc_Send
} Instruction;


class NetworkMsg
{
public:
	NetworkMsg();
	NetworkMsg(NetMsgID commandmsg,double data1,double data2,double data3);
public:
	NetMsgID RS_COMMAND;
	double value1, value2, value3;
private:
	void init();


};
// structure for commands, will be passed from wireless to vision
class CommandData
{
public:
	Instruction RC_COMMAND;
	//default constructor
	CommandData()
	{
		RC_COMMAND = NO_COMMAND;
	};
	void clear()
	{
		//type = ROBOCOMMANDER_DATA;
		RC_COMMAND = NO_COMMAND;
	};
};
#endif
