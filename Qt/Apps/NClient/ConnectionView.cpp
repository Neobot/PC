#include "ConnectionView.h"
#include "ui_ConnectionView.h"

#include "NetworkClientCommInterface.h"
#include "Instructions.h"

#include <QDateTime>
#include <QMessageBox>

ConnectionView::ConnectionView(NetworkConnection* connection, QWidget *parent) :
	AbstractView(connection, QImage(":/toolbar/connection"), parent),
	ui(new Ui::ConnectionView), _simulation(false)
{
	ui->setupUi(this);
	ui->lblPingServerResult->clear();
	ui->lblPingRobotResult->clear();
	ui->leServerAddress->setValidator(new QRegExpValidator(QRegExp("\\d{0,3}\\.\\d{0,3}\\.\\d{0,3}\\.\\d{0,3}"), this));

	_serverPingTimer = new QTimer(this);
	_serverPingTimer->setSingleShot(true);
	connect(_serverPingTimer, SIGNAL(timeout()), this, SLOT(pingServerTimeOut()));
	
	_robotPingTimer = new QTimer(this);
	_robotPingTimer->setSingleShot(true);
	connect(_robotPingTimer, SIGNAL(timeout()), this, SLOT(pingRobotTimeOut()));

	connectionStatusChanged(_connection->getConnectionStatus());

	connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
	connect(ui->btnDisconnect, SIGNAL(clicked()), this, SLOT(disconnectFromServer()));
	connect(ui->btnPingServer, SIGNAL(clicked()), this, SLOT(sendPingToServer()));

	connect(ui->btnPingRobot, SIGNAL(clicked()), this, SLOT(sendPingToRobot()));
	connect(ui->btnAskControl, SIGNAL(clicked()), this, SLOT(connectToRobot()));
	connect(ui->btnReleaseControl, SIGNAL(clicked()), this, SLOT(disconnectFromRobot()));

	_connection->registerNetworkResponder(this);
	_connection->registerRobotResponder(this);

	connect(_connection, SIGNAL(socketError(QString)), this, SLOT(connectionError(QString)));

	connect(ui->btnRefreshSerial, SIGNAL(clicked()), this, SLOT(refreshSerialPorts()));
}

ConnectionView::~ConnectionView()
{
	delete ui;
}

void ConnectionView::loadSettings(QSettings *settings)
{
	settings->beginGroup("Connection");

	ui->leServerAddress->setText(settings->value("adress", "192.168.0.42").toString());
	ui->spPort->setValue(settings->value("port", 15042).toInt());
	ui->checkLocalHost->setChecked(settings->value("localhost", false).toBool());
	ui->checkSimulated->setChecked(settings->value("simutated", false).toBool());

	_preferedRobotPort = settings->value("robotPort").toString();
	_preferedAx12Port = settings->value("ax12Port").toString();

	settings->endGroup();
}

void ConnectionView::saveSettings(QSettings *settings)
{
	settings->beginGroup("Connection");

	settings->setValue("adress", ui->leServerAddress->text());
	settings->setValue("port", ui->spPort->value());
	settings->setValue("localhost", ui->checkLocalHost->isChecked());
	settings->setValue("simutated", ui->checkSimulated->isChecked());

	QString pRobot, pAx12;
	if (_connection->getConnectionStatus() == NetworkConnection::Connected)
	{
		pRobot = ui->cbRobotPorts->currentText();
		pAx12 = ui->cbAX12Ports->currentText();
	}
	else
	{
		pRobot = _preferedRobotPort;
		pAx12 = _preferedAx12Port;
	}

	settings->setValue("robotPort", pRobot);
	settings->setValue("ax12Port", pAx12);


	settings->endGroup();
}

void ConnectionView::connectionStatusChanged(NetworkConnection::ConnectionStatus status)
{
	switch(status)
	{
		case NetworkConnection::Lookup:
			ui->lblServerStatus->setText("searching...");
			ui->lblServerStatus->setStyleSheet("color: blue");
			ui->lblRobotStatus->setText("offline");
			ui->lblRobotStatus->setStyleSheet("color: red");
			ui->btnPingServer->setEnabled(false);
			ui->lblPingServerResult->clear();

			ui->leServerAddress->setEnabled(false);
			ui->spPort->setEnabled(false);
			ui->checkLocalHost->setEnabled(false);
			ui->btnConnect->setEnabled(false);
			ui->btnDisconnect->setEnabled(false);
			ui->btnAskControl->setEnabled(false);
			ui->checkSimulated->setEnabled(false);
			ui->btnReleaseControl->setEnabled(false);
			ui->btnPingRobot->setEnabled(false);
			ui->lblPingRobotResult->clear();
			ui->cbRobotPorts->setEnabled(false);
			ui->cbAX12Ports->setEnabled(false);
			ui->btnRefreshSerial->setEnabled(false);
			break;

		case NetworkConnection::Disconnected:
			ui->lblServerStatus->setText("offline");
			ui->lblServerStatus->setStyleSheet("color: red");
			ui->lblRobotStatus->setText("offline");
			ui->lblRobotStatus->setStyleSheet("color: red");
			ui->btnPingServer->setEnabled(false);
			ui->lblPingServerResult->clear();

			ui->spPort->setEnabled(true);
			ui->checkLocalHost->setEnabled(true);
			ui->leServerAddress->setEnabled(!ui->checkLocalHost->isChecked());
			ui->btnConnect->setEnabled(true);
			ui->btnDisconnect->setEnabled(false);
			ui->btnAskControl->setEnabled(false);
			ui->checkSimulated->setEnabled(false);
			ui->btnReleaseControl->setEnabled(false);
			ui->btnPingRobot->setEnabled(false);
			ui->lblPingRobotResult->clear();
			ui->cbRobotPorts->setEnabled(false);
			ui->cbAX12Ports->setEnabled(false);
			ui->btnRefreshSerial->setEnabled(false);
			break;

		case NetworkConnection::Connected:
			ui->lblServerStatus->setText("online");
			ui->lblServerStatus->setStyleSheet("color: green");
			ui->lblRobotStatus->setText("offline");
			ui->lblRobotStatus->setStyleSheet("color: red");
			ui->btnPingServer->setEnabled(true);
			ui->lblPingServerResult->clear();

			ui->leServerAddress->setEnabled(false);
			ui->spPort->setEnabled(false);
			ui->checkLocalHost->setEnabled(false);
			ui->btnConnect->setEnabled(false);
			ui->btnDisconnect->setEnabled(true);
			ui->btnAskControl->setEnabled(true);
			ui->checkSimulated->setEnabled(true);
			ui->btnReleaseControl->setEnabled(false);
			ui->btnPingRobot->setEnabled(false);
			ui->lblPingRobotResult->clear();
			ui->cbRobotPorts->setEnabled(!ui->checkSimulated->isChecked());
			ui->cbAX12Ports->setEnabled(true);
			ui->btnRefreshSerial->setEnabled(true);

			refreshSerialPorts();
			break;

		case NetworkConnection::Controled:
			ui->lblServerStatus->setText("online");
			ui->lblServerStatus->setStyleSheet("color: green");
			ui->lblRobotStatus->setText(_simulation ? "simulated" : "online");
			ui->lblRobotStatus->setStyleSheet(_simulation ? "color: orange" : "color: green");
			ui->btnPingServer->setEnabled(true);
			ui->lblPingServerResult->clear();

			ui->leServerAddress->setEnabled(true);
			ui->spPort->setEnabled(true);
			ui->btnConnect->setEnabled(false);
			ui->btnDisconnect->setEnabled(true);
			ui->btnAskControl->setEnabled(false);
			ui->checkSimulated->setEnabled(false);
			ui->btnReleaseControl->setEnabled(true);
			ui->btnPingRobot->setEnabled(true);
			ui->lblPingRobotResult->clear();
			ui->cbRobotPorts->setEnabled(false);
			ui->cbAX12Ports->setEnabled(false);
			ui->btnRefreshSerial->setEnabled(false);
			break;
	}

	if (!ui->cbRobotPorts->currentText().isEmpty())
		_preferedRobotPort = ui->cbRobotPorts->currentText();

	if (!ui->cbAX12Ports->currentText().isEmpty())
		_preferedAx12Port = ui->cbAX12Ports->currentText();
}

void ConnectionView::serverAnnouncement(const QByteArray &message)
{
	Q_UNUSED(message);
}

void ConnectionView::noticeOfReceipt(quint8 instruction, bool result)
{
	if (instruction == Comm::PING && result)
	{
		_robotPingTimer->stop();
		int ms = _robotPingTimeCounter.elapsed();

		QString text("Last ping to the robot at ");
		text += QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
		text += " - ";
		text += QString::number(ms);
		text += " ms";
		ui->lblPingRobotResult->setText(text);
		ui->lblPingRobotResult->setStyleSheet("color: green");
		ui->btnPingRobot->setEnabled(true);
	}
}

void ConnectionView::setSerialPorts(const QStringList &ports)
{
	ui->cbAX12Ports->clear();
	ui->cbAX12Ports->addItems(ports);
	ui->cbAX12Ports->addItem(QString());;
	if (!_preferedAx12Port.isEmpty())
	{
		int i =  ui->cbAX12Ports->findText(_preferedAx12Port);
		if (i < 0)
			ui->cbAX12Ports->setEditText(_preferedAx12Port);
		else
			ui->cbAX12Ports->setCurrentIndex(i);
	}
	else
		ui->cbAX12Ports->setCurrentIndex(1);

	ui->cbRobotPorts->clear();
	ui->cbRobotPorts->addItems(ports);
	if (!_preferedRobotPort.isEmpty())
	{
		int j =  ui->cbRobotPorts->findText(_preferedRobotPort);
		if (j < 0)
			ui->cbRobotPorts->setEditText(_preferedRobotPort);
		else
			ui->cbRobotPorts->setCurrentIndex(j);
	}
	else
		ui->cbRobotPorts->setCurrentIndex(0);
}

void ConnectionView::networkNoticeOfReceipt(quint8 instruction, bool result)
{
	if (instruction == Comm::PING_SERVER && result)
	{
		_serverPingTimer->stop();
		int ms = _serverPingTimeCounter.elapsed();

		QString text("Last ping to the server at ");
		text += QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
		text += " - ";
		text += QString::number(ms);
		text += " ms";
		ui->lblPingServerResult->setText(text);
		ui->lblPingServerResult->setStyleSheet("color: green");
		ui->btnPingServer->setEnabled(true);
	}
}

void ConnectionView::sendPingToServer()
{
	_connection->getComm()->sendPingToServer();
	_serverPingTimeCounter.start();
	_serverPingTimer->start(1000);
	ui->btnPingServer->setEnabled(false);
}

void ConnectionView::pingServerTimeOut()
{
	QString text("Last ping to the server at ");
	text += QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
	text += " timeout...";
	ui->lblPingServerResult->setText(text);
	ui->lblPingServerResult->setStyleSheet("color: red");
	ui->btnPingServer->setEnabled(true);
}

void ConnectionView::connectToServer()
{
	_connection->connectToServer(ui->checkLocalHost->isChecked() ? QString() : ui->leServerAddress->text(), ui->spPort->value());
}

void ConnectionView::disconnectFromServer()
{
	_connection->disconnectFromServer();
}

void ConnectionView::connectToRobot()
{
	_simulation = ui->checkSimulated->isChecked();
	_connection->askRobotControl(_simulation, ui->cbRobotPorts->currentText(), ui->cbAX12Ports->currentText());
}

void ConnectionView::disconnectFromRobot()
{
	_connection->closeRobotControl();
}

void ConnectionView::sendPingToRobot()
{
	_connection->getComm()->ping();
	_robotPingTimeCounter.start();
	_robotPingTimer->start(1000);
	ui->btnPingRobot->setEnabled(false);
}

void ConnectionView::pingRobotTimeOut()
{
	QString text("Last ping to the server at ");
	text += QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
	text += " timeout...";
	ui->lblPingRobotResult->setText(text);
	ui->lblPingRobotResult->setStyleSheet("color: red");
	ui->btnPingRobot->setEnabled(true);
}

void ConnectionView::refreshSerialPorts()
{
	_connection->getComm()->askSerialPorts();
}

void ConnectionView::connectionError(const QString &message)
{
	QMessageBox::critical(this, "Connection error", message);
}
