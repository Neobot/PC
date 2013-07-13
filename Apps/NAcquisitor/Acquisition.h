#ifndef ACQUISITION_H
#define ACQUISITION_H

#include <QMap>
#include <QHash>
#include <QTime>

#include "Samples.h"

class ViewInterface;

typedef QMap<int, PositionSample> PositionSamples;
typedef QMap<int, CommandSample> CommandSamples;
typedef QMap<int, SharpSample> SharpSamples;

class Acquisition
{
public:
	Acquisition();

	void start();
	void stop();
	void pause();

	int addPositionAcquisition(const PositionSample& sample);
	const PositionSamples& getPositionSamples() const;
	const PositionSample& getPositionSampleAt(int t) const;

	int addCommandAcquisition(const CommandSample& sample);
	const CommandSamples& getCammandSamples() const;
	const CommandSample& getCammandSampleAt(int t) const;

	int addAvoidingSharpAcquisition(const SharpSample& sample);
	const SharpSamples& getAvoidingSharpSamples() const;
	const SharpSample& getAvoidingSharpSampleAt(int t) const;

	void clear();
	bool inProgress() const;

	bool write(const QString& filename) const;
	bool read(const QString& filename);

	void setRealTimeTrackingEnabled(bool value);

	void connectView(ViewInterface* view);
	void disconnectView(ViewInterface* view);

	void refreshViews() const;


private:
	enum SampleType
	{
		Position,
		Command,
		AvoidingSharps
	};

	PositionSamples	_positions;
	CommandSamples	_commands;
	SharpSamples	_avoidingSharps;

	QList<ViewInterface*> _views;
	QHash<SampleType, bool> _receivedData;

	PositionSample _firstPositionSample;
	CommandSample _firstCommandSample;
	bool _realAcquisitionStarted;

	//real time tracking
	QTime _startTime;
	QTime _pauseTime;
	bool _isStarted;
	bool _isPaused;
	int _pausedSeconds;

	//Non real time tracking
	int _lastPositionCommandTime;
	bool _positionReceived;
	bool _commandReceived;
	int _lastAvoidingSharpTime;

	bool _realTimeTracking;

	int getCurrentAcquisitionTime();
	int getPositionTime();
	int getCommandTime();
	int getAvoidingSharpTime();

	bool receive(Acquisition::SampleType type);
	void setNewRecord();
};

#endif // ACQUISITION_H
