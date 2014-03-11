#ifndef UM6_H
#define UM6_H

#include "AbstractLogger.h"
#include "ProtocolUM6.h"

#include <QSerialPort>

namespace Comm
{
	class UM6 : public QObject, public Tools::LoggerInterface
	{
		Q_OBJECT

	public:
		UM6(const QString& portname, qint32 baudrate, Tools::AbstractLogger* logger);
		~UM6();

		bool open();
		float getPhi();
		float getTheta();
		float getPsi();
		quint32 getRegister(quint8 address);
		qint16 get16BitRegister(quint8 address, quint8 sel);
		void calibrateGyros();
		bool gyrosCalibrated();

	private:
		Comm::ProtocolUM6* _protocol;
		qint32 _baudrate;
		quint32 _registers[133];
		bool _gyrosCalibrated;

	private slots:
		void messageReceived(quint8 registerAddress, const Comm::Data&);
		void handleSerialError(QSerialPort::SerialPortError error);
	};
}
#endif // UM6_H
