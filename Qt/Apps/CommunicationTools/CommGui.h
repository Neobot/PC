#ifndef COMMGUI_H
#define COMMGUI_H

#include "ui_CommGui.h"
#include "TextEditLogger.h"
#include "RobotProtocol.h"
#include "ProtocolAX12.h"
#include "ProtocolUM6.h"
#include "ProtocolRaw.h"
#include <QIODevice>
#include <QTimer>

class CommGui : public QDialog, Tools::LoggerInterface
{
    Q_OBJECT
public:
    CommGui(QWidget *parent = 0);
    ~CommGui();

	void openConnection();
	void closeConnection();

private:
	enum ProtocolType
	{
		None = -1,
		Robot = 0,
		AX12 = 1,
		Raw = 2,
		UM6 = 3
	};

    Ui::CommGui ui;
    Tools::PlainTextEditLogger* _logger;
    Comm::AbstractProtocol* _protocol;
    QIODevice* _port;
    QTimer* _timerSend;
	ProtocolType _openedProtocolType;

    bool _showValidMessages;
    bool _connected;

    void parseData(Comm::Data& data) const;
    void send(bool log = true);
	bool openPort(const QString& portname, const QString& baudrate, ProtocolType protocol);
    bool closePort();

private slots:
    void messageReceived(quint8 instruction, const Comm::Data&, quint8 id);
    void on_btnConnect_clicked();
    void on_btnSend_clicked();
    void on_cbQuiet_stateChanged(int state);
    void on_cbValidMessage_stateChanged(int state);
    void on_cbDebugMode_stateChanged(int state);
    void on_btnClear_clicked();
    void on_btnStopRun_clicked(bool value);
	void on_cbProtocol_currentIndexChanged(int index);
    void on_rbOutputDec_toggled(bool checked);
    void on_rbOutputHex_toggled(bool checked);
    void on_rbOutputASCII_toggled(bool checked);
    void on_cbSendContinuously_stateChanged(int state);
    void timerTick();
};

#endif // COMMGUI_H
