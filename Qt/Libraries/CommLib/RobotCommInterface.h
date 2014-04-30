#ifndef ROBOT_H
#define ROBOT_H

#include "AbstractCommInterface.h"
#include "AbstractLogger.h"
#include "AbstractProtocol.h"
#include "CommListener.h"
#include "AX12CommManager.h"
#include <QQueue>
#include <QPair>
#include <QSet>
#include <RMovement.h>


namespace Comm
{
	/**
	 * \brief The main class of the CommLib API.
	 *
	 * It is an easy acess to the robot available instructions.
	 * It can works through #AbstractProtocol.
	 * Inherits your class from the interface #CommListener then register it into this class with #setListener
	 * in order to receive messages from the robot.
	 * Before to send or read any messages you must open the conection with #AbstractRobot::openConnections.
	 *
	 * \see CommListener
	 * \see AbstractProtocol
	 * \see AbstractLogger
	 */
	class RobotCommInterface : public AbstractCommInterface
	{
		Q_OBJECT

	public:
		RobotCommInterface(AbstractProtocol* protocolCard, AX12CommManager* ax12Manager, Tools::AbstractLogger* logger, CommListener* listener = 0, QObject* parent = 0);


		/**
		 * \brief Registers the listener which must receive the messages sent by the robot.
		 */
		void setListener(CommListener* listener);
		CommListener* getListener() const;

		AX12CommManager* getAx12Manager() const;

		//Instructions
		void ping();
		void setPosition(qint16 x, qint16 y, double theta);
		void setDestination(Tools::RMovement pos);
		void flush();
		void replaceDestination(QList<Tools::RMovement> positions);
		
		void moveAX12(quint8 id, float angle, float speed);
		
		void setParameters(const QList<float> &values);
		void askParameters();
		
		void executeAction(quint8 actionId, quint8 parameter);
		
		void enableSensor(SensorType type, quint8 sensorId); //sensorId starts at 0
		void disableSensor(SensorType type, quint8 sensorId); //sensorId starts at 0
		
		void sendNoticeOfReceipt(quint8 instruction, bool result);

	protected slots:
		void read(quint8 instruction, const Comm::Data& data);

	private:
		CommListener*   _listener;
		AX12CommManager* _ax12;

		void addPositionToData(const Tools::RMovement& pos, Data& data);
	};
}

#endif // ROBOT_H
