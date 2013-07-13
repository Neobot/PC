#include "NAcquisitor.h"
#include "ui_NAcquisitor.h"

#include "ViewInterface.h"
#include "ViewWidget.h"
#include "Views/RawDataView.h"
#include "Views/CommandView.h"
#include "Views/SimpleCurveView.h"
#include "Views/DoubleCurveView.h"
#include "Views/LogView.h"
#include "Views/SharpView.h"

#include "Simulator.h"
#include "SimProtocol.h"
#include "RobotCommInterface.h"
#include "RobotProtocol.h"

#include <qextserialenumerator.h>

#include <QCheckBox>
#include <QComboBox>
#include <QDockWidget>
#include <QSettings>
#include <QFileDialog>

const int SIMULATION_REFRESH = 50; //ms
const double SIMULATION_SPEED = 0.4;

NAcquisitor::NAcquisitor(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::NAcquisitor), _simulator(0), _device(0), _protocol(0), _robot(0), _hasModifications(false)
{
	qApp->setOrganizationName("Neobot");
	qApp->setApplicationName("NAcquisitor");

	//setDockNestingEnabled(true);
	//setDockOptions();

    setWindowIcon(QIcon(":/app/AppIcon"));
    ui->setupUi(this);
	_checkSimulation = new QCheckBox("simulation", this);
	_cbPortName = new QComboBox(this);

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
	foreach(const QextPortInfo& port, ports)
		_cbPortName->addItem(port.portName);

	initDocks();

	_settings = new QSettings(QSettings::UserScope, "Neobot", "Acquisitor", this);
	loadSettings();

	initActions();

	_recentFileList = _settings->value("RecentFiles").toStringList();
	updateRecentFileActions();

	_ax12Manager = new Comm::AX12CommManager;

	newAcquisition();
}

NAcquisitor::~NAcquisitor()
{
	deleteConnection();
	delete _ax12Manager;
    delete ui;
}

void NAcquisitor::initDocks()
{
	XYCurveView* i1 = new XYCurveView;
	ui->mainView->setView(i1);
	_acquisition.connectView(i1);

	QDockWidget* d1 = new QDockWidget("Commands", this);
	_command = new CommandView;
	d1->setWidget(new ViewWidget(this, _command));
	d1->setObjectName("CommandDock");
	addDockWidget(Qt::RightDockWidgetArea, d1);

	RawDataView* i2 = new RawDataView;
	//ui->view2->setView(i2);
	_acquisition.connectView(i2);
	QDockWidget* d2 = new QDockWidget("Raw data", this);
	d2->setWidget(new ViewWidget(this, i2));
	d2->setObjectName("RawDataDock");
	addDockWidget(Qt::RightDockWidgetArea, d2);

	XCurveView* i4 = new XCurveView;
	//ui->view4->setView(i4);
	_acquisition.connectView(i4);
	QDockWidget* d4 = new QDockWidget("X(t)", this);
	d4->setWidget(new ViewWidget(this, i4));
	d4->setObjectName("XDock");
	addDockWidget(Qt::BottomDockWidgetArea, d4);

	YCurveView* i5 = new YCurveView;
	//ui->view5->setView(i5);
	_acquisition.connectView(i5);
	QDockWidget* d5 = new QDockWidget("y(t)", this);
	d5->setWidget(new ViewWidget(this, i5));
	d5->setObjectName("YDock");
	addDockWidget(Qt::BottomDockWidgetArea, d5);

	ThetaCurveView* i3 = new ThetaCurveView;
	//ui->view3->setView(i3);
	_acquisition.connectView(i3);
	QDockWidget* d3 = new QDockWidget("Theta(t)", this);
	d3->setWidget(new ViewWidget(this, i3));
	d3->setObjectName("ThetaDock");
	addDockWidget(Qt::BottomDockWidgetArea, d3);

	QDockWidget* d6 = new QDockWidget("Log", this);
	_log = new LogView;
	d6->setWidget(new ViewWidget(this, _log));
	d6->setObjectName("LogDock");
	addDockWidget(Qt::LeftDockWidgetArea, d6);
	d6->setFloating(true);
	d6->hide();

	SharpView* i7 = new SharpView;
	_acquisition.connectView(i7);
	QDockWidget* d7 = new QDockWidget("Avoiding sharps", this);
	d7->setWidget(new ViewWidget(this, i7));
	d7->setObjectName("SharpDock");
	addDockWidget(Qt::LeftDockWidgetArea, d7);
	d7->setFloating(true);
	d7->hide();

	_docks << d1 << d2 << d3 << d4 << d5 << d6 << d7;
}

void NAcquisitor::newAcquisition()
{
	_currentFileName.clear();
	_acquisition.clear();
	updateTitle();
	_hasModifications = false;
}

void NAcquisitor::addToRecentFileList(const QString &file)
{
	if (!_recentFileList.isEmpty() && _recentFileList.first() == file)
		return;

	if (_recentFileList.contains(file))
		_recentFileList.removeAll(file);

	_recentFileList.prepend(file);
	if (_recentFileList.count() > MaxRecentFiles)
		_recentFileList.removeLast();

	_settings->setValue("RecentFiles", _recentFileList);

	updateRecentFileActions();
}

void NAcquisitor::updateRecentFileActions()
{
	for (int i = 0; i < MaxRecentFiles; ++i)
	{
		const QString& file = _recentFileList.value(i);
		if (!file.isEmpty())
		{
			recentFileActs[i]->setVisible(true);

			QString actionText("&");
			actionText += QString::number(i + 1);
			actionText += " ";
			actionText += QFileInfo(file).fileName();
			recentFileActs[i]->setText(actionText);
		}
		else
		{
			recentFileActs[i]->setVisible(false);
			recentFileActs[i]->setText(QString());
		}
	}

	recentFileListSeparatorAction->setVisible(!_recentFileList.isEmpty());
}

void NAcquisitor::updateTitle()
{
	QString text;
	if (_currentFileName.isEmpty())
		text += "New acquisition";
	else
		text += _currentFileName;

	if (_hasModifications)
		text += "*";

	text += " - Neobot Acquisitor";

	setWindowTitle(text);
}

void NAcquisitor::open(const QString& file)
{
	_currentFileName = file;
	addToRecentFileList(file);
	if (_acquisition.read(file))
		_acquisition.refreshViews();

	_hasModifications = false;
	updateTitle();
}

void NAcquisitor::open()
{
	QString file = QFileDialog::getOpenFileName(this, "Open a dataset", QString(), "Acquisition Dataset (*.nacq)");
	if (!file.isEmpty())
		open(file);
}

void NAcquisitor::openRecentFile(int index)
{
	QString file = _recentFileList.value(index);
	if (!file.isEmpty())
		open(file);
}

void NAcquisitor::save()
{
	if (_currentFileName.isEmpty())
		saveAs();
	else
	{
		_acquisition.write(_currentFileName);
		_hasModifications = false;
	}
}

void NAcquisitor::saveAs()
{
	QString file = QFileDialog::getSaveFileName(this, "Save a dataset as...", QString(), "Acquisition Dataset (*.nacq)");
	if (!file.isEmpty())
	{
		addToRecentFileList(file);
		_acquisition.write(file);
		_currentFileName = file;
		_hasModifications = false;
		updateTitle();
	}
}

void NAcquisitor::initActions()
{
	//file menus
	_notInRunActions << ui->menuFile->addAction("New acquisition", this, SLOT(newAcquisition()), QKeySequence::New);
	_notInRunActions << ui->menuFile->addAction("Open acquisition", this, SLOT(open()), QKeySequence::Open);
	_notInRunActions <<  ui->menuFile->addAction("Save acquisition", this, SLOT(save()), QKeySequence::Save);
	_notInRunActions << ui->menuFile->addAction("Save acquisition as...", this, SLOT(saveAs()), QKeySequence::SaveAs);

	//recent files
	ui->menuFile->addSeparator();

	_recentFileActionsMapper = new QSignalMapper(this);
	connect(_recentFileActionsMapper, SIGNAL(mapped(int)), this, SLOT(openRecentFile(int)));

	for (int i = 0; i < MaxRecentFiles; ++i)
	{
		recentFileActs[i] = new QAction(this);
		recentFileActs[i]->setVisible(false);
		connect(recentFileActs[i], SIGNAL(triggered()), _recentFileActionsMapper, SLOT(map()));
		_recentFileActionsMapper->setMapping(recentFileActs[i], i);
		ui->menuFile->addAction(recentFileActs[i]);
	}
	recentFileListSeparatorAction = ui->menuFile->addSeparator();
	recentFileListSeparatorAction->setVisible(false);

	//run menu
	ui->mainToolBar->setWindowTitle("Run toolbar");

	_actionPlay = ui->menuRun->addAction(QIcon(":/toolbar/play"), "Play", this, SLOT(play()));
	ui->mainToolBar->addAction(_actionPlay);

	_actionStop = ui->menuRun->addAction(QIcon(":/toolbar/stop"), "Stop", this, SLOT(stop()));
	_actionStop->setEnabled(false);
	ui->mainToolBar->addAction(_actionStop);

	ui->mainToolBar->addSeparator();
	ui->mainToolBar->addWidget(_checkSimulation);
	ui->mainToolBar->addSeparator();
	ui->mainToolBar->addWidget(_cbPortName);
	connect(_checkSimulation, SIGNAL(clicked(bool)), _cbPortName, SLOT(setDisabled(bool)));

	//view
	QMenu* viewMenu = createPopupMenu();
	viewMenu->setTitle("View");
	ui->menuBar->addMenu(viewMenu);
}

void NAcquisitor::play()
{
	_checkSimulation->setEnabled(false);
	_cbPortName->setEnabled(false);
	_actionPlay->setEnabled(false);
	_actionStop->setEnabled(true);

	foreach(QAction* a, _notInRunActions)
		a->setEnabled(false);

	_acquisition.start();

	bool simu = _checkSimulation->isChecked();
	initConnection(simu);

	_hasModifications = true;
}


void NAcquisitor::stop()
{
	_acquisition.stop();
	deleteConnection();

	bool simu = _checkSimulation->isChecked();
	_cbPortName->setEnabled(!simu);

	_checkSimulation->setEnabled(true);
	_actionPlay->setEnabled(true);
	_actionStop->setEnabled(false);

	foreach(QAction* a, _notInRunActions)
		a->setEnabled(true);
}

void NAcquisitor::switchSimulationMode()
{
	bool simu = _checkSimulation->isChecked();
	_cbPortName->setEnabled(!simu);
}


void NAcquisitor::deleteConnection()
{
	delete _robot;
	_robot = 0;
	_command->setRobot(0);

	delete _simulator;
	_simulator = 0;

	delete _protocol;
	_protocol = 0;

	delete _device;
	_device = 0;
}

void NAcquisitor::initConnection(bool simulation)
{
	deleteConnection();

	if (simulation)
	{
		SimProtocol* p = new SimProtocol;
		_protocol = p;

		_simulator = new Simulator(p, SIMULATION_REFRESH, SIMULATION_SPEED);

		_device = 0;

		_simulator->start();
		_simulator->setPosition(0, 0, 0.0);
	}
	else
	{
		QextSerialPort* port = new QextSerialPort("/dev/ttyUSB0",  QextSerialPort::EventDriven);
		port->setBaudRate(BAUD115200);
		port->setFlowControl(FLOW_OFF);
		port->setParity(PAR_NONE);
		port->setDataBits(DATA_8);
		port->setStopBits(STOP_1);
		_device = port;

		_protocol = new Comm::RobotProtocol(_device);
		_simulator = 0;
	}

	_robot = new Comm::RobotCommInterface (_protocol, _ax12Manager, 0, this);
	_robot->disableNoticeOfReceiptChecking();
	_command->setRobot(_robot);
}

//--

void NAcquisitor::coordinates(qint16 x, qint16 y, double theta, bool forward)
{
	Q_UNUSED(forward);

	PositionSample sample(x, y, theta);
	_acquisition.addPositionAcquisition(sample);
}

void NAcquisitor::objective(qint16 x, qint16 y, double theta)
{
	CommandSample sample(x, y, theta);
	_acquisition.addCommandAcquisition(sample);
}

void NAcquisitor::avoidingSensors(const QList<quint8> &values)
{
	SharpSample sample(values);
	_acquisition.addAvoidingSharpAcquisition(sample);
}

bool NAcquisitor::microswitch(const quint8 values)
{
	Q_UNUSED(values);
	return true;
}

void NAcquisitor::otherSensors(const QList<quint8>& values)
{
	Q_UNUSED(values);
}

bool NAcquisitor::initDone()
{
	return true;
}

bool NAcquisitor::go(bool mirrored)
{
	Q_UNUSED(mirrored);
	return true;
}

bool NAcquisitor::pingReceived()
{
	return true;
}

void NAcquisitor::noticeOfReceipt(quint8 instruction, bool result)
{
	Q_UNUSED(instruction);
	Q_UNUSED(result);
}

void NAcquisitor::opponentPosition(qint16 x, qint16 y)
{
	Q_UNUSED(x);
	Q_UNUSED(y);
}

bool NAcquisitor::restart()
{
	return false;
}

void NAcquisitor::quit()
{
	quit();
}

void NAcquisitor::isArrived()
{
}

void NAcquisitor::isBlocked()
{
}

void NAcquisitor::log(const QByteArray &text)
{
	_log->appendLog(text);
}

void NAcquisitor::closeEvent(QCloseEvent *e)
{
	saveSettings();

	QMainWindow::closeEvent(e);
}

void NAcquisitor::saveSettings()
{
	_settings->setValue("MainWindowGeometry", saveGeometry());
	_settings->setValue("MainWindowState", saveState());

	_settings->setValue("Simulation", _checkSimulation->isChecked());
	_settings->setValue("PortIndex", _cbPortName->currentIndex());

	_command->saveSettings(*_settings);
}

void NAcquisitor::loadSettings()
{
	restoreGeometry(_settings->value("MainWindowGeometry").toByteArray());
	restoreState(_settings->value("MainWindowState").toByteArray());

	_checkSimulation->setChecked(_settings->value("Simulation").toBool());
	_cbPortName->setCurrentIndex(_settings->value("PortIndex").toInt());
	_cbPortName->setEnabled(!_checkSimulation->isChecked());
	_command->loadSettings(*_settings);
}
