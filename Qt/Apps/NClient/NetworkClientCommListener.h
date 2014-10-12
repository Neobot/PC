#ifndef NETWORKCLIENTCOMMLISTENER_H
#define NETWORKCLIENTCOMMLISTENER_H

#include <QStringList>

class NetworkClientCommListener
{
public:
	virtual ~NetworkClientCommListener() {}

	virtual void serverAnnouncement(const QByteArray& message) {Q_UNUSED(message);}
	virtual void networkNoticeOfReceipt(quint8 instruction, bool result) {Q_UNUSED(instruction); Q_UNUSED(result);}

	virtual void setStrategies(const QStringList& strategies) {Q_UNUSED(strategies);}

	virtual void configurationFiles(int category, const QStringList& filenames) {Q_UNUSED(category); Q_UNUSED(filenames);}
	virtual void configurationFileData(int category, const QString& filename, const QByteArray& data) {Q_UNUSED(category); Q_UNUSED(filename); Q_UNUSED(data);}
	virtual void configurationFileEvent(int category, const QString& filename, int event) {Q_UNUSED(category); Q_UNUSED(filename); Q_UNUSED(event);}

	virtual void strategyStatusUpdated(int strategyNum, bool isRunning) {Q_UNUSED(strategyNum); Q_UNUSED(isRunning);}
	virtual void autoStrategyInfo(bool enabled, int strategyNum, const QString &robotPort, const QString &ax12Port, bool simulation, bool mirror, int delayInSeconds) {Q_UNUSED(strategyNum); Q_UNUSED(enabled);
						Q_UNUSED(robotPort); Q_UNUSED(ax12Port); Q_UNUSED(simulation); Q_UNUSED(mirror); Q_UNUSED(delayInSeconds);}

	virtual void setSerialPorts(const QStringList& ports) {Q_UNUSED(ports);}

	virtual void setAx12Positions(const QList<int> ids, const QList<float>& positions, const QList<float>& loads)  {Q_UNUSED(ids); Q_UNUSED(positions); Q_UNUSED(loads);}
	virtual void setAx12MovementsFile(const QByteArray& data) {Q_UNUSED(data);}
};

#endif // NETWORKCLIENTCOMMLISTENER_H
