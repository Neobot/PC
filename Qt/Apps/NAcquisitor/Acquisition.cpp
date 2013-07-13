#include "Acquisition.h"
#include "ViewInterface.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

const int ACQ_CURRENT_FILE_VERSION = 1;
const int ACQ_FIRST_SUPPORTED_VERSION = 1;
const int ACQ_LAST_SUPPORTED_VERSION = 1;
const char ACQ_FILE_SEPARATOR = ';';

Acquisition::Acquisition() : _realAcquisitionStarted(false), _isStarted(false), _isPaused(false), _pausedSeconds(0), _lastPositionCommandTime(1),
	_positionReceived(false), _commandReceived(false), _lastAvoidingSharpTime(1), _realTimeTracking(false)
{
}

const PositionSamples &Acquisition::getPositionSamples() const
{
	return _positions;
}

const PositionSample &Acquisition::getPositionSampleAt(int t) const
{
	return *_positions.lowerBound(t);
}

int Acquisition::addPositionAcquisition(const PositionSample &sample)
{
	int t = -1;

	if (inProgress())
	{
		t = getPositionTime();

		if (_realAcquisitionStarted || _positions.isEmpty())
		{
			_positions.insert(t, sample);

			bool sampleComplete = receive(Position);
			if (sampleComplete)
			{
				foreach(ViewInterface* view, _views)
					view->addPositionAcquisition(t, sample, (_commands.constEnd() - 1).key(), (_commands.constEnd() - 1).value());
			}
		}
	}

	return t;
}

const CommandSamples &Acquisition::getCammandSamples() const
{
	return _commands;
}

const CommandSample &Acquisition::getCammandSampleAt(int t) const
{
	return *_commands.lowerBound(t);
}

int Acquisition::addCommandAcquisition(const CommandSample &sample)
{
	int t = -1;

	if (inProgress())
	{

		t = getCommandTime();
		if (_commands.isEmpty())
			_firstCommandSample = sample;

		if (!_realAcquisitionStarted && _commands.count() == 1 && !(_firstCommandSample == sample))
			_realAcquisitionStarted = true;

		if (_realAcquisitionStarted || _commands.isEmpty())
		{
			_commands.insert(t, sample);

			bool sampleComplete = receive(Command);
			if (sampleComplete)
			{
				foreach(ViewInterface* view, _views)
					view->addPositionAcquisition((_positions.constEnd() - 1).key(), (_positions.constEnd() - 1).value(), t, sample);
			}
		}
	}

	return t;
}

const SharpSamples &Acquisition::getAvoidingSharpSamples() const
{
	return _avoidingSharps;
}

const SharpSample &Acquisition::getAvoidingSharpSampleAt(int t) const
{
	return *_avoidingSharps.lowerBound(t);
}

int Acquisition::addAvoidingSharpAcquisition(const SharpSample &sample)
{
	int t = -1;

	if (inProgress())
	{
		bool sampleComplete = receive(AvoidingSharps);

		t = getAvoidingSharpTime();
		_avoidingSharps.insert(t, sample);

		if (sampleComplete)
		{
			foreach(ViewInterface* view, _views)
				view->addAvoidingSharpAcquisition(t, sample);
		}
	}

	return t;
}

void Acquisition::start()
{
	if (!_isPaused)
	{
		if (!_isStarted)
		{
			_isStarted = true;
			_startTime.start();
		}
	}
	else
	{
		_isPaused = false;
		_pausedSeconds += _pauseTime.elapsed();
	}
}

void Acquisition::stop()
{
	if (_isStarted)
	{
		_isStarted = false;
	}
}

void Acquisition::pause()
{
	if (!_isPaused && _isStarted)
	{
		_isPaused = true;
		_pauseTime.start();
	}
}


int Acquisition::getCurrentAcquisitionTime()
{
	return _startTime.elapsed() - _pausedSeconds;
}

int Acquisition::getPositionTime()
{
	if (_realTimeTracking)
		return getCurrentAcquisitionTime();
	else
		return _lastPositionCommandTime;
}

int Acquisition::getCommandTime()
{
	if (_realTimeTracking)
		return getCurrentAcquisitionTime();
	else
		return _lastPositionCommandTime;
}

int Acquisition::getAvoidingSharpTime()
{
	if (_realTimeTracking)
		return getCurrentAcquisitionTime();
	else
	{
		return _lastAvoidingSharpTime++;
	}
}

bool Acquisition::receive(Acquisition::SampleType type)
{
	bool isNewRecord = true;
	if (type == Position || type == Command)
	{
		if (_receivedData[Position] == false && _receivedData[Command] == false)
		{
			isNewRecord = false;
			_receivedData[type] = true;

		}
		else if (_receivedData[type] == true)
		{
			setNewRecord();
			_receivedData[type] = true;
		}
		else if (_receivedData[type] == false)
		{
			setNewRecord();
		}
	}

	return isNewRecord;
}

void Acquisition::setNewRecord()
{
	++_lastPositionCommandTime;
	_receivedData[Position] = false;
	_receivedData[Command] = false;
}

void Acquisition::clear()
{
	_positions.clear();
	_commands.clear();
	_avoidingSharps.clear();
	_realAcquisitionStarted = false;

	foreach(ViewInterface* view, _views)
		view->clear();
}

bool Acquisition::inProgress() const
{
	return _isStarted && !_isPaused;
}

bool Acquisition::write(const QString &filename) const
{
	QFile f(filename);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream out(&f);

	//header
	out << "Acquisitions" << ACQ_FILE_SEPARATOR
		<< ACQ_CURRENT_FILE_VERSION << ACQ_FILE_SEPARATOR
		<< QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")
		<< '\n';

	//Positions
	for(PositionSamples::const_iterator it = _positions.constBegin(); it != _positions.constEnd(); ++it)
	{
		const PositionSample& sample = *it;
		out << "P" << ACQ_FILE_SEPARATOR
			<< it.key() << ACQ_FILE_SEPARATOR
			<< sample._x << ACQ_FILE_SEPARATOR
			<< sample._y << ACQ_FILE_SEPARATOR
			<< sample._theta
			<< '\n';
	}

	//Commands
	for(CommandSamples::const_iterator it = _commands.constBegin(); it != _commands.constEnd(); ++it)
	{
		const CommandSample& sample = *it;
		out << "C" << ACQ_FILE_SEPARATOR
			<< it.key() << ACQ_FILE_SEPARATOR
			<< sample._cx << ACQ_FILE_SEPARATOR
			<< sample._cy << ACQ_FILE_SEPARATOR
			<< sample._ctheta
			<< '\n';
	}

	//Avoiding sharps
	for(SharpSamples::const_iterator it = _avoidingSharps.constBegin(); it != _avoidingSharps.constEnd(); ++it)
	{
		const SharpSample& sample = *it;
		out << "AS" << ACQ_FILE_SEPARATOR
			<< it.key() << ACQ_FILE_SEPARATOR
			<< sample._values.count();

		foreach(int v, sample._values)
		{
			out << ACQ_FILE_SEPARATOR << v;
		}

		out << '\n';
	}

	return true;
}

bool Acquisition::read(const QString &filename)
{
	QFile f(filename);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QTextStream in(&f);

	//header
	QStringList headerLineToken = in.readLine().split(ACQ_FILE_SEPARATOR);
	if (headerLineToken.count() < 2)
		return false;

	int version = headerLineToken.value(1).toInt();
	if (version < ACQ_FIRST_SUPPORTED_VERSION || version > ACQ_LAST_SUPPORTED_VERSION)
		return false;

	clear();

	while(!in.atEnd())
	{
		QStringList tokens = in.readLine().split(ACQ_FILE_SEPARATOR);
		if (tokens.isEmpty())
			continue;

		QString marker = tokens.value(0);

		//Positions
		if (marker == "P")
		{
			if (tokens.count() < 5)
				continue;

			int t = tokens.value(1).toInt();
			PositionSample sample(tokens.value(2).toInt(), tokens.value(3).toInt(), tokens.value(4).toDouble());
			_positions.insert(t, sample);
		}

		//Commands
		else if (marker == "C")
		{
			if (tokens.count() < 5)
				continue;

			int t = tokens.value(1).toInt();
			CommandSample sample(tokens.value(2).toInt(), tokens.value(3).toInt(), tokens.value(4).toDouble());
			_commands.insert(t, sample);
		}

		//avoiding sharps
		else if (marker == "AS")
		{
			if (tokens.count() < 3)
				continue;

			int t = tokens.value(1).toInt();
			int nb = tokens.value(2).toInt();
			if (tokens.count() < 3 + nb)
				continue;

			QList<quint8> values;
			for(int i = 0; i < nb; ++i)
			{
				values << tokens.value(i + 3).toInt();
			}

			SharpSample sample(values);
			_avoidingSharps.insert(t, sample);
		}
	}

	return true;
}

void Acquisition::setRealTimeTrackingEnabled(bool value)
{
	_realTimeTracking = value;
}

void Acquisition::connectView(ViewInterface *view)
{
	if (!_views.contains(view))
		_views << view;
}

void Acquisition::disconnectView(ViewInterface *view)
{
	_views.removeAll(view);
}

void Acquisition::refreshViews() const
{
	foreach(ViewInterface* view, _views)
		view->setAcquisition(this);
}

