#ifndef AX12COMMMANAGER_H
#define AX12COMMMANAGER_H

#include <QTimer>
#include <QHash>
#include <QSerialPort>

#include "AbstractLogger.h"
#include "ProtocolAX12.h"

namespace Comm
{
	class AX12
	{
	public:
		AX12();
		AX12(float minAngle, float maxAngle);

		void setGoal(float angle, float torque);
		void setSpeed(float speed);

		float getPosition() const;
		float getLoad() const;
		float getSpeed() const;

		void lockServo();
		void releaseServo();

        bool isTimeout() const;
        void setTimeout();
		void setState(float position, float speed, float load);
		void getCommand(float &position, float &speed, float &torque) const;
		float getAngleDifference() const;

		bool goalHasChanged() const;
		bool isStopped() const;
		void notifyCommandSent();

	private:
		float _CWAngleLimit;		// clockwise angle limit (between 0 and 300°)
		float _CCWAngleLimit;	// anti-clockwise angle limit (between 0 and 300°)
		float _goalPosition;		// between 0 and 300°
		float _goalSpeed;		// 0 to 114RPM (0 = no velocity control)
		float _maxTorque;		// 0 to 100% (0 = free run mode, 100% = maximum torque
		float _currentPosition;	// in degrees
		float _currentSpeed;		// in RPM (negative = CW, positive = CCW)
		float _currentLoad;		// -100% to 100% (negative = CW load, positive = CCW load)
		float _angleDiff;
		bool _goalChanged;
	};

	class AX12CommManager : public QObject, public Tools::LoggerInterface
    {
		Q_OBJECT

	signals:
		void allMessagesSent();
        void servosStatusUpdated(const QList<quint8>& ids);
        void requestTimeoutReceived(const QList<quint8>& ids);

	public:
        enum ReadingLoopMode
        {
            AUTO_MODE,
            TIMER_MODE
        };

        enum ControllerMode
        {
            NO_CONTROLLER,
            USB2AX_CONTROLLER
        };

        AX12CommManager(ControllerMode mode = USB2AX_CONTROLLER);
		AX12CommManager(const QString& portname, qint32 baudrate, ControllerMode mode, Tools::AbstractLogger* logger);
		~AX12CommManager();

		bool open();
		bool isOpened() const;

		ProtocolAX12* getProtocol() const;

        void setControllerMode(ControllerMode mode);
        ControllerMode getControllerMode() const;

		void setReadingLoopMode(ReadingLoopMode mode);
		void setTimerReadingLoopInterval(int ms = 200);
		bool isReadingLoopStarted() const;

		void setRequestTimeout(int ms);

        void resetServo(quint8 id, float minAngle = 0, float maxAngle = 300);
		
        bool isServoTimeout(quint8 id) const;
		float getServoPosition(quint8 id) const;
		float getServoLoad(quint8 id) const;
		const AX12 getAX12(quint8 id) const;
		bool contains(quint8 id) const;
		QList<quint8> getAllServos() const;

		void setGoal(quint8 servoId, float angle, float speed, float torque, bool synchronous = true);
		
		void synchronize(quint8 id);
		void synchronize(const QList<quint8>& servoIds);
		
		double calculateSmoothServosSpeed(const QList<quint8>& servoIds, float maxSpeed); //return the estimated duration of the movement in ms

		void lockServo(quint8 id, bool synchronous = true);
		void releaseServo(quint8 id, bool synchronous = true);

        void requestServoStatus(quint8 id, bool loop = false);
        void requestServoStatus(const QList<quint8>& ids, bool loop = false);
        void cancelLoopStatusRequest(quint8 id);
        void cancelLoopStatusRequest(const QList<quint8>& ids);

	private:
        ControllerMode _controllerMode;
		Comm::ProtocolAX12* _protocol;
		QHash<quint8, AX12> _servos;
        QHash<quint8, int> _loopDemandCount;
		qint32 _baudrate;

		QTimer* _readTimer;
		bool _autoReadingLoop;
		QTimer* _requestTimeoutTimer;
		ReadingLoopMode _readingLoopMode;
		float _maxSpeed;
		
		struct CommMessage
		{
			enum Type
			{
				None,
				StatusRequest,
				Synchronization
			};
			
			CommMessage() : type(None) {}
			CommMessage(quint8 servoId, Type messageType) : type(messageType) {ids.append(servoId);}
			CommMessage(const QList<quint8>& servoIds, Type messageType) : ids(servoIds), type(messageType) {}
			bool isValid() const {return type != None;}
			
			QList<quint8> ids;
			Type type;
		};

		bool _isBusy;
		CommMessage _currentMessage;
		QList<CommMessage> _messagePendingList;
		
        void askStartReadingLoop();
        void askStopReadingLoop();

        void askSendNextRequestMessage();
        void askSendNextSynchroMessage();
		
		void addServoCommandData(AX12& ax12, Data& data);
		
		void sendServoSynchronizeMessage(quint8 id);
		void sendServoMultiSynchronizeMessage(const QList<quint8>& ids);
		void sendServoRequestStatusMessage(quint8 id);
        void sendServoMultiRequestStatusMessage(const QList<quint8>& ids);

		void readReceivedData(quint8 id, Data &data);

	private slots:
		void messageReceived(quint8 instruction, const Comm::Data&, quint8 id);   
		void sendNextMessage();
		void requestTimeout();
        void requestAllServoStatusForReadingLoop();
		void handleSerialError(QSerialPort::SerialPortError error);
	};
}

#endif // AX12COMMMANAGER_H
