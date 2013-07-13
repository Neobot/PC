#ifndef UM6_H
#define UM6_H

#include <qextserialport.h>
#include "AbstractLogger.h"
#include "ProtocolUM6.h"

namespace Comm
{
	class UM6 : public QObject, public Tools::LoggerInterface
	{
		Q_OBJECT

	public:
		UM6();
		UM6(const QString& portname, BaudRateType baudrate, Tools::AbstractLogger* logger);

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
		quint32 _registers[133];
		bool _gyrosCalibrated;

	private slots:
		void messageReceived(quint8 registerAddress, const Comm::Data&);
	};
}
#endif // UM6_H
