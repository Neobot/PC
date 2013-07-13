/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Neobot wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy us a beer in return.
 * ----------------------------------------------------------------------------
 */

#ifndef NEOBOTMAINOPTIONS_H
#define NEOBOTMAINOPTIONS_H

#include <string>
#include <QString>

class NeobotMainOptions
{
public:
    NeobotMainOptions(); 

    enum DebugLevel
    {
	NoDebug = 0,
	StrategyIntermediatePointsDebug = 1,
	StrategyPositionsDebug = 2,
	StrategyTrajectoriesDebug = 4,
	StrategyCaptorsDebug = 8,
	ProtocolErrorsDebug = 16,
	ProtocolMessagesDebug = 32,
	ProtocolFramesDebug = 64
	//RobotBasicDebug = 128
    };

    std::string portname;
    std::string ax12Portname;

    bool simulation;
    bool dstar;

    int debugLevel;

    bool logFile;
    std::string filename;

    bool record;
    QString recordFilename;

    bool autoQuit;

    bool reversed;

    int strategy;

    void usage() const;
    bool parse(int argc, char* argv[]);
};

#endif // NEOBOTMAINOPTIONS_H
