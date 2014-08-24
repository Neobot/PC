#include "Instructions.h"

using namespace Comm;

QList<quint8> Instruction::robotToPcInstructions()
{
	QList<quint8> result;
	result << COORD << OPPONENT << OBJECTIVE << ACTIVATED_SENSORS
		   << GO << LOG
		   << PARAMETERS << PARAMETER_NAMES << EVENT << SENSOR_EVENT;
	return result;
}

QList<quint8> Instruction::pcToRobotInstructions()
{
	QList<quint8> result;
	result << DEST_ADD << DEST_REPLACE << FLUSH << SET_POS << ENABLE_SENSOR << DISABLE_SENSOR 
		   << SET_PARAMETERS << ASK_PARAMETERS << ASK_GRAPHS << ACTIONS;
	return result;
}

QList<quint8> Instruction::serverToClientInstructions()
{
	QList<quint8> result;
	result << ANNOUNCEMENT << SEND_STRATEGIES << SEND_STRATEGY_FILES << SEND_STRATEGY_FILE_DATA
		   << SERIAL_PORTS << AX12_POSITIONS << STRATEGY_STATUS << AUTO_STRATEGY_INFO << AX12_MVT_FILE;
	return result;
}

QList<quint8> Instruction::clientToServerInstructions()
{
	QList<quint8> result;
	result << CONNECT << DISCONNECT << UPDATE_SERVER << PING_SERVER
		   << ASK_STRATEGIES << ASK_STRATEGY_FILES << ASK_STRATEGY_FILE_DATA << SET_STRATEGY_FILE_DATA
		   << MOVE_AX12 << ASK_AX12_POSITIONS << START_STRATEGY << STOP_STRATEGY << ASK_STRATEGY_STATUS
		   << SET_AUTO_STRATEGY << ASK_AUTO_STRATEGY_INFO << ASK_AX12_MVT_FILE << SET_AX12_MVT_FILE << RUN_AX12_MVT
		   << LOCK_AX12 << RESET_PARAMETERS << RUN_SCRIPT;
	return result;
}

QList<quint8> Instruction::globalInstructions()
{
	QList<quint8> result;
	result << PING << AR;
	return result;
}
