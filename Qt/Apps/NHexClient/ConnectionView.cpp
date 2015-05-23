#include "ConnectionView.h"
#include "ui_ConnectionView.h"

#include "Instructions.h"

#include <QDateTime>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QtDebug>

ConnectionView::ConnectionView(CommInterface* connection, QWidget *parent) :
	AbstractView(connection, QImage(":/toolbar/connection"), parent),
	ui(new Ui::ConnectionView)
{
	ui->setupUi(this);

	_robotPingTimer = new QTimer(this);
	_robotPingTimer->setSingleShot(true);
	connect(_robotPingTimer, SIGNAL(timeout()), this, SLOT(pingRobotTimeOut()));

	refreshSerialPorts();
	connectionStatusChanged(false);

	if (!ui->cbRobotPorts->currentText().isEmpty())
		_preferedRobotPort = ui->cbRobotPorts->currentText();

	connect(ui->btnPingRobot, SIGNAL(clicked()), this, SLOT(sendPingToRobot()));
	_connection->registerResponder(this);

	connect(ui->btnConnectRobot, SIGNAL(clicked()), this, SLOT(openPort()));
	connect(ui->btnDisconnectRobot, SIGNAL(clicked()), this, SLOT(closePort()));
}

ConnectionView::~ConnectionView()
{
	delete ui;
}

void ConnectionView::loadSettings(QSettings *settings)
{
	settings->beginGroup("Connection");

	_preferedRobotPort = settings->value("robotPort").toString();

	settings->endGroup();
}

void ConnectionView::saveSettings(QSettings *settings)
{
	settings->beginGroup("Connection");

	settings->setValue("robotPort", _preferedRobotPort);

	settings->endGroup();
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

void ConnectionView::sendPingToRobot()
{
	_connection->getComm()->ping();
	_robotPingTimeCounter.start();
	_robotPingTimer->start(5000);
	ui->btnPingRobot->setEnabled(false);
}

void ConnectionView::pingRobotTimeOut()
{
	QString text("Last ping to the robot at ");
	text += QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
	text += " timeout...";
	ui->lblPingRobotResult->setText(text);
	ui->lblPingRobotResult->setStyleSheet("color: red");
	ui->btnPingRobot->setEnabled(true);
}

void ConnectionView::openPort()
{
	_connection->openPort(ui->cbRobotPorts->currentText());
}

void ConnectionView::closePort()
{
	_connection->closePort();
}

void ConnectionView::connectionStatusChanged(bool connected)
{
	if (connected)
	{
		ui->btnPingRobot->setEnabled(true);
		ui->lblPingRobotResult->clear();
		ui->btnConnectRobot->setEnabled(false);
		ui->btnDisconnectRobot->setEnabled(true);
		ui->cbRobotPorts->setEnabled(false);
	}
	else
	{
		ui->btnPingRobot->setEnabled(false);
		ui->lblPingRobotResult->clear();
		ui->btnConnectRobot->setEnabled(true);
		ui->btnDisconnectRobot->setEnabled(false);
		ui->cbRobotPorts->setEnabled(true);
	}

	if (!ui->cbRobotPorts->currentText().isEmpty())
		_preferedRobotPort = ui->cbRobotPorts->currentText();
}

void ConnectionView::refreshSerialPorts()
{
	QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
	ui->cbRobotPorts->clear();

	foreach(const QSerialPortInfo& port, ports)
		ui->cbRobotPorts->insertItem(0, port.portName());
}
