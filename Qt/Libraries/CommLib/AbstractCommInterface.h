#ifndef ABSTRACTROBOT_H
#define ABSTRACTROBOT_H

#include <QObject>
#include "AbstractLogger.h"
#include "AbstractProtocol.h"
#include "CommListener.h"
#include <QQueue>
#include <QPair>
#include <QSet>

namespace Comm
{
	/**
	 * \brief The base comm class.
	 *
	 * It uses #AbstractProtocol to provides the connection with multiple microC.
	 * It also manages the notices of receipt by sending again the instructions after a timeout.
	 * See #checkNoticeOfReceipt for more information.
	 *
	 * \see AbstractProtocol
	 * \see AbstractLogger
	 */
	class AbstractCommInterface : public QObject, public Tools::LoggerInterface
	{
		Q_OBJECT

	public:
		AbstractCommInterface(Tools::AbstractLogger* logger, QObject* parent = 0);
		AbstractCommInterface(AbstractProtocol* protocol, Tools::AbstractLogger* logger, QObject* parent = 0);
		AbstractCommInterface(QList<AbstractProtocol*> protocolList, Tools::AbstractLogger* logger, QObject* parent = 0);
		virtual ~AbstractCommInterface() {}

		/**
		 * \brief Returns the protocol associated with the microC \p i.
		 */
		AbstractProtocol* getProtocol(int i = 0) const;

		/**
		 * \brief Sets the time out of AR messages in ms.
		 * \note Default value is 300.
		 */
		void setNoticeOfReceiptTimeOut(int time);

		/**
		 * \brief Disable the checking of notice of receipt.
		 */
		void disableNoticeOfReceiptChecking();

	protected:
		/**
		 * \brief Wait for a notice of receipt.
		 * \details This function post an AR event and return immediately.
		 * If a notice of receipt for \p instruction is not received before the timeout, the message \p data is sent again on \p protocol.
		 * \p n is the maximum number of attempt allowed.
		 * \see noticeOfReceiptReceived
		 * \see setNoticeOfReceiptTimeOut
		 * \see disableNoticeOfReceiptChecking
		 */
		void checkNoticeOfReceipt(quint8 instruction, const Data& data, AbstractProtocol* protocol, int n = 1);

		/**
		 * \brief Notify that a notice of receipt for \p instruction has been received.
		 */
		void noticeOfReceiptReceived(quint8 instruction);

		/**
	 * \brief Add \p protocol to the list of AbstractProtocol, and connect it.
	 */
		void addProtocol(AbstractProtocol* protocol);

		//Protocols management
	private:
		QList<AbstractProtocol*> _protocols;

	protected slots:
		virtual void read(quint8 instruction, const Comm::Data& data) = 0;

		//Notice of receipt management
	private:

		class ARData
		{
		public:
			ARData(quint8 instruction, const Data& data, AbstractProtocol* protocol, int nbOfLoop = 1) :
				_instruction(instruction), _data(data), _protocol(protocol), _nb(nbOfLoop) {}

			quint8 _instruction;
			Data _data;
			AbstractProtocol* _protocol;
			int _nb;
		};

		int _timeOutAR;
		bool _arChecking;

		QQueue<ARData> _pendingAR;
		QSet<int> _receivedAR;

	private slots:
		void timeOut();

		/**
	 * \brief Connect the signal AbstractProtocol::message from \p protocol to the read method.
	 * \details Must be call only one time by protocol !!! Otherwise the read method will be called multiple times.
	 */
		void connectProtocol(AbstractProtocol* protocol);
	};
}

#endif // ABSTRACTROBOT_H
