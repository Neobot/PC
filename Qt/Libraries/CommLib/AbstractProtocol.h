#ifndef ABSTRACTPROTOCOL_H
#define ABSTRACTPROTOCOL_H

#include "Data.h"
#include <QIODevice>


namespace Comm
{
	/**
	 * \brief The base interface for communication protocol.
	 *
	 * In order to communicate with the robot, the program must use a communication protocol.
	 * Reimplements this class to define it.
	 */
	class AbstractProtocol : public QObject, public Tools::LoggerInterface
	{
		Q_OBJECT

	public:
		/**
		 * \brief Creates a new instance of the protocol.
		 * \param device The device used for the communication.
		 * \param logger The logger to write error and debugging information.
		 * \note The device must be opened before to start the communication.
		 */
		AbstractProtocol(QIODevice* device, Tools::AbstractLogger* logger = 0);

		virtual ~AbstractProtocol() {}

		/**
		 * \brief Open the device in Read/Write mode.
		 */
		bool open();

		/**
		 * \brief Close the device.
		 */
		bool close();

		/**
		 * \brief Returns true if the device is opened. i.e. ready to read and write data.
		 */
		bool isOpened() const;

			/**
		 * \brief Return the associated device.
		 * \note The device must be opened before to start the communication.
		 */
		QIODevice* getIODevice() const;

		/**
		 * \brief Send a message to the robot through the device.
		 * \param address Address of the device.
		 * \param instruction An 1 byte code which indicates the instruction.
		 * \param data The message associated to the instruction.
		 */
		virtual void sendMessage(quint8 address, quint8 instruction, const Data& data) const = 0;

		/**
		 * \brief Send a message to the robot through the device.
		 * \param instruction An 1 byte code which indicates the instruction.
		 * \param data The message associated to the instruction.
		 */
		void sendMessage(quint8 instruction, const Data& data) const;

		/**
		 * \brief Send a message to the robot through the device.
		 * \param data The message associated to the instruction.
		 */
		void sendMessage(const Data& data) const;


		/**
		  * \brief Debug mode.
		  *
		  * Display every bytes read.
		  */
		void setDebugMode(bool errors, bool messages, bool frames);

		/**
		  * \brief Stop the reading.
		  *
		  * Disconnect the Protocol from the readyRead signal of the device.
		  */
		void stop();

		/**
		  * \brief Start the reading.
		  *
		  * Connect the Protocol to the readyRead signal of the device.
		  */
		void run();

		void beQuiet(bool value);

	signals:
		/**
		 * \brief This signal must be send when a new instruction has arrived from the robot through the device.
		 * \param instruction An 1 byte code which indicates the instruction.
		 * \param data The message associated to the instruction.
		 */
		void message(quint8 instruction, const Comm::Data& data, quint8 id = 0);

	protected:
		QIODevice* _device;
		bool _messages;
		bool _frames;
		bool _showError;
		int _errorCounter;

		void resetErrorCounter();
		void incrementErrorCounter();
		int getErrorsCount() const;
	};
}

#endif // ABSTRACTPROTOCOL_H
