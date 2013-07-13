/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Neobot wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy us a beer in return.
 * ----------------------------------------------------------------------------
 */

#ifndef NEOBOTSTRATEGYLAUNCHER_H
#define NEOBOTSTRATEGYLAUNCHER_H

#include "NeobotMainOptions.h"
#include <QCoreApplication>

class StrategyInterface;
class NMicropather;
class Pather;
class Simulator;
class SimProtocol;

namespace Comm
{
	class AbstractProtocol;
    class RobotProtocol;
}

namespace Tools
{
	class AbstractLogger;
	class MultipleLogger;
	class StdoutLogger;
	class FileLogger;
}

class NeobotStrategyLauncher
{
public:
    NeobotStrategyLauncher();
    ~NeobotStrategyLauncher();

    int exec(const NeobotMainOptions& options, const QCoreApplication& a);

private:
    Tools::AbstractLogger*  _logger;
    Tools::MultipleLogger*  _multiLogger;
    Tools::StdoutLogger*    _stdoutLogger;
    Tools::FileLogger*	     _fileLogger;

    Comm::AbstractProtocol* _protocol;
    Comm::RobotProtocol* _standardProtocol;
    SimProtocol*    _simulationProtocol;

    Simulator* _simulator;

    Pather* _pather;
    NMicropather* _micropather;

    StrategyInterface* _str;

    bool testDebugLevel(int value, NeobotMainOptions::DebugLevel d);
};

#endif // NEOBOTSTRATEGYLAUNCHER_H
