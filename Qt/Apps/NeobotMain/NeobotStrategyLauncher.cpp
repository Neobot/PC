#include "NeobotStrategyLauncher.h"

#include <Simulator.h>
#include "Strategies/StrategyEnumerator.h"
#include "RobotProtocol.h"
#include "RobotCommInterface.h"
#include "vmath.h"
#include "AbstractLogger.h"

#include <QSerialPort>
#include <QBuffer>
#include <QScopedPointer>
#include <QDateTime>
#include <QDir>

#include <StrategyManager.h>
#include <NMicropather.h>
#include "NeobotMainOptions.h"
#include <CommException.h>
#include <SimProtocol.h>

#include <iostream>

NeobotStrategyLauncher::NeobotStrategyLauncher()
{
	_logger = 0;
	_multiLogger = 0;
	_stdoutLogger = 0;
	_fileLogger = 0;

	_protocol = 0;
	_standardProtocol = 0;
	_simulationProtocol = 0;

	_simulator = 0;

	_pather = 0;
	_micropather = 0;

	_str = 0;
}


NeobotStrategyLauncher::~NeobotStrategyLauncher()
{
	delete _str;

	delete _micropather;

	delete _simulator;

	delete _standardProtocol;
	delete _simulationProtocol;

	delete _stdoutLogger;
	delete _fileLogger;
	delete _multiLogger;
}

int NeobotStrategyLauncher::exec(const NeobotMainOptions& options, const QCoreApplication &a)
{
	qDebug() << "Debug Level =" << options.debugLevel;

	if (options.debugLevel > NeobotMainOptions::NoDebug)
	{

		_stdoutLogger = new Tools::StdoutLogger;
		if (options.logFile)
		{
			_multiLogger = new Tools::MultipleLogger;
			_fileLogger = new Tools::FileLogger(options.filename.c_str());
			_fileLogger->flush();

			_multiLogger->addLogger(_stdoutLogger);
			_multiLogger->addLogger(_fileLogger);

			_logger = _multiLogger;
		}
		else
			_logger = _stdoutLogger;
	}

	try
	{
		if (!options.simulation)
		{
			QSerialPort* port1 = new QSerialPort(options.portname.c_str());

			_standardProtocol = new Comm::RobotProtocol(port1, _logger, true);
			_standardProtocol->setDebugMode(testDebugLevel(options.debugLevel, NeobotMainOptions::ProtocolErrorsDebug),
											testDebugLevel(options.debugLevel, NeobotMainOptions::ProtocolMessagesDebug),
											testDebugLevel(options.debugLevel, NeobotMainOptions::ProtocolFramesDebug));

			_protocol = _standardProtocol;
			if (!_protocol->open())
				return 2;

			port1->setBaudRate(QSerialPort::Baud115200);
			port1->setFlowControl(QSerialPort::NoFlowControl);
			port1->setParity(QSerialPort::NoParity);
			port1->setDataBits(QSerialPort::Data8);
			port1->setStopBits(QSerialPort::OneStop);
		}
		else
		{

			_simulationProtocol = new SimProtocol;
			_simulator = new Simulator(_simulationProtocol, 100, 0.4, _logger);
			_simulator->setConfiguration(ChessUpConfiguration(true));
			_protocol = _simulationProtocol;
		}

		Comm::AX12CommManager ax12Manager(options.ax12Portname.c_str(), 1000000, Comm::AX12CommManager::USB2AX_CONTROLLER, _logger);
		ax12Manager.open();

		Comm::RobotCommInterface robot(_protocol, &ax12Manager, _logger);
		robot.disableNoticeOfReceiptChecking();

		_micropather = new NMicropather(_logger, NMicropather::Euclidean, 1000.0);
		_pather = _micropather;

		StrategyEnumerator strategies;
		_str = strategies.start((StrategyEnumerator::Strategy)options.strategy, _logger);
		if (!_str)
		{
			std::cout << "Error: Invalid strategy.";
			options.usage();
			return 3;
		}

		StrategyManager manager(&robot, _pather, _logger);
		manager.connectToRobot();
		manager.setStrategy(_str);
		manager.setDebugMode(testDebugLevel(options.debugLevel, NeobotMainOptions::StrategyIntermediatePointsDebug),
							 testDebugLevel(options.debugLevel, NeobotMainOptions::StrategyPositionsDebug),
							 testDebugLevel(options.debugLevel, NeobotMainOptions::StrategyTrajectoriesDebug),
							 testDebugLevel(options.debugLevel, NeobotMainOptions::StrategyCaptorsDebug));
		manager.setAutoQuit(options.autoQuit);
		manager.setReversedStrategy(options.reversed);

		if (options.simulation)
			_simulator->start();

		if (options.record)
		{
			QDir d;
			d.mkpath("recordings");
			manager.startRecording(QString("recordings/").append(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss")).append(".nrec"));
		}
		return a.exec();
	}
	catch(const Comm::CommException& e)
	{
		*_logger << e.what() << Tools::endl << Tools::endl;
		*_logger << "FAILED..." << Tools::endl;
		return 1;
	}
}

bool NeobotStrategyLauncher::testDebugLevel(int value, NeobotMainOptions::DebugLevel d)
{
	return (value & d) == d;
}
