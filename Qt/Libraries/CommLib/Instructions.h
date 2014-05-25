#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <QtGlobal>
#include <QList>

namespace Comm
{
	/**
	 * \brief Maximum lentgh of the data.
	 */
	const int MAX_LENGTH = 20;

	/**
	 * \brief Factor to send an angle in radian through the comm
	 */
    const double ANGLE_FACTOR = 1000.0;

    //PC -> microC
    const quint8 DEST_ADD = 1;
    const quint8 DEST_REPLACE = 2;
    const quint8 FLUSH = 3;
    const quint8 SET_POS = 10;
	const quint8 ENABLE_SENSOR = 20;
	const quint8 DISABLE_SENSOR = 21;
	const quint8 SET_PARAMETERS = 50;
	const quint8 ASK_PARAMETERS = 51;
    const quint8 ACTIONS = 60;

    //microC -> PC
    const quint8 COORD = 100;
    const quint8 OPPONENT = 101;
	const quint8 OBJECTIVE = 104;
    const quint8 AVOIDING_SENSORS = 110;
    const quint8 GO = 121;
	const quint8 LOG = 124;
	const quint8 PARAMETERS = 125;
	const quint8 PARAMETER_NAMES = 126;
	const quint8 EVENT = 130;
	const quint8 SENSOR_EVENT = 131;

    //global
    const quint8 PING = 254;
    const quint8 AR = 255;

	enum RobotAction
	{
		NO_ACTION = -1,
		ACTION_START_PUMP = 1,
		ACTION_STOP_PUMP = 2
	};

	enum RobotEvent
	{
		NO_EVENT = -1,
		EVENT_IS_ARRIVED = 1,
		EVENT_IS_BLOCKED = 2,
		EVENT_QUIT = 3,
		EVENT_RESTART = 4
	};
	
	enum SensorType
	{
		SharpSensor = 0,
		MicroswitchSensor = 1,
		ColorSensor = 2
	};

	enum SharpState
	{
		SharpNothingDetected,
		SharpObjectDetected,
		SharpObjectVeryClose,
	};

	enum MicroswicthState
	{
		MicroswicthOff,
		MicroswicthOn
	};

	enum ColorState
	{
		ColorUnknown,
		ColorRed,
		ColorGreen,
		ColorBlue,
		ColorYellow,
		ColorWhite,
		ColorBlack
	};

	//Network commands
	//Server -> Client
	const quint8 ANNOUNCEMENT = 180;
	const quint8 SEND_STRATEGIES = 181;
	const quint8 SEND_STRATEGY_FILES = 182;
	const quint8 SEND_STRATEGY_FILE_DATA = 183;
	const quint8 SERIAL_PORTS = 184;
	const quint8 AX12_POSITIONS = 185;
	const quint8 STRATEGY_STATUS = 186;
	const quint8 AUTO_STRATEGY_INFO = 187;
	const quint8 AX12_MVT_FILE = 188;

	//Client -> Server
	const quint8 CONNECT = 200;
	const quint8 DISCONNECT = 201;
	const quint8 UPDATE_SERVER = 202;
	const quint8 PING_SERVER = 203;
	const quint8 ASK_STRATEGIES = 204;
	const quint8 ASK_STRATEGY_FILES = 205;
	const quint8 ASK_STRATEGY_FILE_DATA = 206;
	const quint8 SET_STRATEGY_FILE_DATA = 207;
	const quint8 RESET_STRATEGY_FILE = 208;
	const quint8 ASK_SERIAL_PORTS = 209;
	const quint8 MOVE_AX12 = 210;
	const quint8 ASK_AX12_POSITIONS = 211;
	const quint8 START_STRATEGY = 212;
	const quint8 STOP_STRATEGY = 213;
	const quint8 ASK_STRATEGY_STATUS = 214;
	const quint8 SET_AUTO_STRATEGY = 215;
	const quint8 ASK_AUTO_STRATEGY_INFO = 216;
	const quint8 ASK_AX12_MVT_FILE = 217;
	const quint8 SET_AX12_MVT_FILE = 218;
	const quint8 RUN_AX12_MVT = 219;
	const quint8 LOCK_AX12 = 220;
	const quint8 RESET_PARAMETERS = 221;

	class Instruction
	{
	public:
		static QList<quint8> robotToPcInstructions();
		static QList<quint8> pcToRobotInstructions();
		static QList<quint8> serverToClientInstructions();
		static QList<quint8> clientToServerInstructions();
		static QList<quint8> globalInstructions();
	};

	struct Ax12Info
	{
		quint8 id;
		float angle;
		float speed;
		float torque;
	};
}

#endif // INSTRUCTIONS_H
