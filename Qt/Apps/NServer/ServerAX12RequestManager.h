#ifndef SERVERAX12REQUESTMANAGER_H
#define SERVERAX12REQUESTMANAGER_H

#include <QObject>
#include <QTimer>
#include <QHash>

namespace Comm
{
    class AX12StatusListener;
    class AX12CommManager;
}

class NetworkCommInterface;

class ServerAX12RequestManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerAX12RequestManager(Comm::AX12CommManager* ax12Manager, NetworkCommInterface* commInterface, int updateIntervalInMs, QObject *parent = 0);
    ~ServerAX12RequestManager();

    void setAX12CommManager(Comm::AX12CommManager* ax12Manager);
    void setRequest(const QList<quint8>& ids, bool recursive);

private:
    Comm::AX12CommManager* _ax12Manager;
    NetworkCommInterface* _interface;

    QList<quint8> _requestedAX12Ids;
    QHash<quint8, bool> _timeoutServo;
    bool _recursive;
    Comm::AX12StatusListener* _ax12StatusListener;

    QTimer* _recursiveTimer;

private slots:
    void sendPositions();
    void allServoUpdated();    
};

#endif // SERVERAX12REQUESTMANAGER_H
