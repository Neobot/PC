#include "NPlayer.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QSettings>

#include "Strategies/StrategyEnumerator.h"
#include "Simulator.h"
#include "StrategyManager.h"
#include "NMicropather.h"
#include "SimProtocol.h"
#include "RobotCommInterface.h"
#include "StrategyMap.h"
#include "RPoint.h"
#include "TextEditLogger.h"

#include "StrategyScene.h"
#include <math.h>

const int REFRESH = 50; //ms

NPlayer::NPlayer(QWidget *parent) :
	QMainWindow(parent), _playing(false)
{
	ui.setupUi(this);
	setWindowIcon(QIcon(":/app/AppIcon"));
	ui.splitter->setSizes(QList<int>());
	ui.spSpeed->setValue(0.4);
	ui.comboStrats->addItems(StrategyEnumerator::getAvailableStrategies());

	ui.graphicsView->setRefreshInterval(REFRESH);

	_configSelector = new ChessUpConfigurationSelector(this);

    addColorToCombobox(Qt::red, ui.cbColor);
    addColorToCombobox(Qt::yellow, ui.cbColor);

	_viewModeButtonGroup = new QButtonGroup(this);
	_viewModeButtonGroup->setExclusive(true);
	_viewModeButtonGroup->addButton(ui.btnSimulation);
	_viewModeButtonGroup->addButton(ui.btnStrategy);

	_modeActionGroup = new QActionGroup(this);
	_modeActionGroup->setExclusive(true);
	_modeActionGroup->addAction(ui.actionSimulation);
	_modeActionGroup->addAction(ui.actionReplay);

	_logger = new Tools::PlainTextEditLogger(ui.textLogger);

	_protocol = new SimProtocol;
	_simulator = new Simulator(_protocol, REFRESH, ui.spSpeed->value(), _logger);
	_simulator->setConfiguration(ChessUpConfiguration(false));

	_ax12Manager = new Comm::AX12CommManager();

	_robot = new Comm::RobotCommInterface (_protocol, _ax12Manager, _logger);
	_robot->disableNoticeOfReceiptChecking();


	_pather = new NMicropather(_logger, NMicropather::Euclidean, 1000);

	_strategy = 0;
	_manager = new StrategyManager(_robot, _pather, _logger);
	_manager->connectToRobot();

	strategyChanged();
	viewModeChanged();
	colorChanged();
	setDebugMode();

	connect(ui.spSpeed, SIGNAL(valueChanged(double)), this, SLOT(speedChanged(double)));
	connect(ui.checkShowIntermediates, SIGNAL(toggled(bool)), this, SLOT(setDebugMode()));
	connect(ui.checkShowPositions, SIGNAL(toggled(bool)), this, SLOT(setDebugMode()));
	connect(ui.chexkShowTrajectories, SIGNAL(toggled(bool)), this, SLOT(setDebugMode()));
	connect(ui.checkShowCaptors, SIGNAL(toggled(bool)), this, SLOT(setDebugMode()));
	connect(ui.checkShowStrategyLog, SIGNAL(toggled(bool)), this, SLOT(setDebugMode()));
	connect(_viewModeButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(viewModeChanged()));
	connect(ui.comboStrats, SIGNAL(currentIndexChanged(int)), this, SLOT(strategyChanged()));
	connect(ui.cbColor, SIGNAL(currentIndexChanged(int)), this, SLOT(colorChanged()));
	connect(_modeActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(viewModeChanged()));
	connect(ui.sliderReplaySpeed, SIGNAL(valueChanged(int)), this, SLOT(replaySpeedChanged(int)));

	initActions();
	loadSettings();

	ui.graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.graphicsView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(viewContextMenuRequested(QPoint)));

	_player = 0;
	_playedFile = 0;
	_playSpeed = 100;

	connect(&_playTimer, SIGNAL(timeout()), this, SLOT(play()));
	_playTimer.setInterval(_playSpeed);


	connect(&_matchTimer, SIGNAL(timeout()), this, SLOT(refreshMatchTime()));
	_matchTimer.setInterval(1000);
	_matchTimer.start();

	_playTime.setHMS(0, 0, 0, 0);
}

NPlayer::~NPlayer()
{
	delete _manager;
	delete _strategy;
	delete _pather;
	delete _robot;
	delete _ax12Manager;
	delete _simulator;
	delete _protocol;
	delete _logger;
	delete _player;
	delete _playedFile;
}

void NPlayer::addColorToCombobox(const QColor& color, QComboBox* combo) const
{
	int size = ui.cbColor->style()->pixelMetric(QStyle::PM_SmallIconSize);
	QPixmap pixmap(size,size);

	combo->addItem("");
	pixmap.fill(color);
	combo->setItemData(combo->count() - 1, pixmap, Qt::DecorationRole);//Setting color palettes
}

void NPlayer::run(bool value)
{   
	if (value)
	{
		ui.actionRun->setText("Pause");
		ui.actionRun->setIcon(QIcon(":/gui/pause"));
		setConfigWidgetEnabled(false);


		if (ui.actionReplay->isChecked())
			_playTimer.start();
		else if (_strategy)
		{
			_manager->setPause(false);
			_simulator->start();
		}

	}
	else
	{
		ui.actionRun->setText("Run");
		ui.actionRun->setIcon(QIcon(":/gui/play"));

		if (ui.actionReplay->isChecked())
			_playTimer.stop();
		else if (_strategy)
		{
			_manager->setPause(true);
			_simulator->pause();
		}
	}
	ui.graphicsView->setRefreshEnabled(value);
}

void NPlayer::stop()
{
	run(false);
	ui.actionRun->setChecked(false);

	if (ui.actionReplay->isChecked())
	{
		_playedFile->close();
		_playedFile->open(QIODevice::ReadOnly);
		_simulator->getMap()->moveRobot(_simulator->getDefaultRobotPosition());
		_playTime.setHMS(0, 0, 0, 0);
	}
	else
	{
		setConfigWidgetEnabled(true);
		_simulator->stop();
		viewModeChanged();
	}

	ui.graphicsView->setRefreshEnabled(false);
	ui.graphicsView->refresh();
}

void NPlayer::speedChanged(double value)
{
	_simulator->setSpeed(value);
}

void NPlayer::on_btnClearLog_clicked()
{
	_logger->clear();
}

void NPlayer::on_btnSaveLogAs_clicked()
{
	QString filename = QFileDialog::getSaveFileName(this, "Please select a name for the log file.", QString(), "*.txt");
	if (!filename.isEmpty())
	{
		QFile file(filename);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::critical(this, "Error", QString("Cannot open the file \"").append(filename).append("\"."));
			return;
		}

		file.write(ui.textLogger->toPlainText().toAscii());
		file.close();

		QMessageBox::information(this, "Log saved", QString("The log has been saved in \"").append(QFileInfo(filename).absoluteFilePath()).append("\"."));
	}
}


void NPlayer::setDebugMode()
{
	_manager->setDebugMode(ui.checkShowIntermediates->isChecked(),
						   ui.checkShowPositions->isChecked(),
						   ui.chexkShowTrajectories->isChecked(),
						   ui.checkShowCaptors->isChecked());
	_strategy->setLogEnabled(ui.checkShowStrategyLog->isChecked());

}

void NPlayer::initActions()
{
	ui.actionRun->setShortcut(QKeySequence(Qt::Key_Space));
	connect(ui.actionRun, SIGNAL(triggered(bool)), this, SLOT(run(bool)));
	ui.btnRun->setDefaultAction(ui.actionRun);

	ui.actionStop->setShortcut(QKeySequence(Qt::Key_Escape));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stop()));
	ui.btnStop->setDefaultAction(ui.actionStop);

	connect(ui.actionRecord, SIGNAL(triggered(bool)), this, SLOT(record(bool)));
	ui.btnRecord->setDefaultAction(ui.actionRecord);

	ui.actionNew_configuration->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	connect(ui.actionNew_configuration, SIGNAL(triggered()), this, SLOT(newConfiguration()));

	ui.actionNew_random_configuration->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_N));
	connect(ui.actionNew_random_configuration, SIGNAL(triggered()), this, SLOT(newRandomConfiguration()));

	connect(ui.actionEmpty_configuration, SIGNAL(triggered()), this, SLOT(newEmptyConfiguration()));

	connect(ui.actionOpenReplay, SIGNAL(triggered()), this, SLOT(openReplay()));

	//view actions
	_viewContextMenu = new QMenu(this);
	_viewContextMenu->addAction("Set opponent here", this, SLOT(setOpponentHere()));
	_viewContextMenu->addAction("Move opponent here", this, SLOT(moveOpponentHere()));

	ui.graphicsView->refresh();
}

void NPlayer::customizeView()
{
	if (ui.actionSimulation->isChecked())
	{
		if (ui.btnSimulation->isChecked())
		{
			ui.graphicsView->getScene()->clearGroupProperties();
			ui.graphicsView->getScene()->setGroupPixmapProperty(SimulationMap::KingsGroup, QPixmap(":/objects/king"));
			ui.graphicsView->getScene()->setGroupPixmapProperty(SimulationMap::QueensGroup, QPixmap(":/objects/queen"));
			ui.graphicsView->getScene()->setGroupAsLineObject(SimulationMap::AvoidingSharpGroup);
			ui.graphicsView->getScene()->setGroupAsLineObject(SimulationMap::OtherSharpGroup);
			ui.graphicsView->getScene()->setGroupAsRectangleObject(SimulationMap::ContactorGroup);
			ui.graphicsView->getScene()->setGroupTransformationOrigin(SimulationMap::AvoidingSharpGroup, Tools::StrategyScene::TopLeft);
			ui.graphicsView->getScene()->setGroupTransformationOrigin(SimulationMap::OtherSharpGroup, Tools::StrategyScene::TopLeft);
			ui.graphicsView->getScene()->setGroupTransformationOrigin(SimulationMap::ContactorGroup, Tools::StrategyScene::TopLeft);
			ui.graphicsView->getScene()->setGroupDynamicProperty(SimulationMap::AvoidingSharpGroup, true);
			ui.graphicsView->getScene()->setGroupDynamicProperty(SimulationMap::OtherSharpGroup, true);
			ui.graphicsView->getScene()->setGroupDynamicProperty(SimulationMap::ContactorGroup, true);
			ui.graphicsView->getScene()->setGroupDynamicSizeProperty(SimulationMap::AvoidingSharpGroup, true);
			ui.graphicsView->getScene()->setGroupDynamicSizeProperty(SimulationMap::OtherSharpGroup, true);
			ui.graphicsView->getScene()->setGroupDynamicSizeProperty(SimulationMap::ContactorGroup, true);
			ui.graphicsView->getScene()->setGroupVisibleProperty(SimulationMap::TableObstacleGroup, false);
			ui.graphicsView->getScene()->setMap(_simulator->getMap());
		}
		else
		{
			ui.graphicsView->getScene()->clearGroupProperties();
			ui.graphicsView->getScene()->setGroupDynamicProperty(StrategyMap::SharpGroup, true);
			ui.graphicsView->getScene()->setGroupAsRectangleObject(StrategyMap::NoDetectionZone);
			//ui.graphicsView->getScene()->setGroupTransformationOrigin(StrategyMap::NoDetectionZone, StrategyScene::TopLeft);
			ui.graphicsView->getScene()->setMap(_manager->getMap());
		}

		ui.widgetSimuControl->setVisible(true);
		ui.widgetReplayControl->setVisible(false);
		ui.actionRun->setEnabled(true);
		ui.actionStop->setEnabled(true);

		ui.graphicsView->getScene()->setGroupVisibleProperty(SimulationMap::OpponentGroup, true);
	}
	else
	{
		ui.widgetSimuControl->setVisible(false);
		ui.widgetReplayControl->setVisible(true);
		ui.actionRun->setEnabled(false);
		ui.actionStop->setEnabled(false);

		ui.graphicsView->getScene()->clearGroupProperties();
		ui.graphicsView->getScene()->setGroupPixmapProperty(SimulationMap::KingsGroup, QPixmap(":/objects/king"));
		ui.graphicsView->getScene()->setGroupPixmapProperty(SimulationMap::QueensGroup, QPixmap(":/objects/queen"));
		ui.graphicsView->getScene()->setGroupAsLineObject(SimulationMap::AvoidingSharpGroup);
		ui.graphicsView->getScene()->setGroupAsLineObject(SimulationMap::OtherSharpGroup);
		ui.graphicsView->getScene()->setGroupAsRectangleObject(SimulationMap::ContactorGroup);
		ui.graphicsView->getScene()->setGroupTransformationOrigin(SimulationMap::AvoidingSharpGroup, Tools::StrategyScene::TopLeft);
		ui.graphicsView->getScene()->setGroupTransformationOrigin(SimulationMap::OtherSharpGroup, Tools::StrategyScene::TopLeft);
		ui.graphicsView->getScene()->setGroupTransformationOrigin(SimulationMap::ContactorGroup, Tools::StrategyScene::TopLeft);
		ui.graphicsView->getScene()->setGroupDynamicProperty(SimulationMap::AvoidingSharpGroup, true);
		ui.graphicsView->getScene()->setGroupDynamicProperty(SimulationMap::OtherSharpGroup, true);
		ui.graphicsView->getScene()->setGroupDynamicProperty(SimulationMap::ContactorGroup, true);
		ui.graphicsView->getScene()->setGroupDynamicSizeProperty(SimulationMap::AvoidingSharpGroup, true);
		ui.graphicsView->getScene()->setGroupDynamicSizeProperty(SimulationMap::OtherSharpGroup, true);
		ui.graphicsView->getScene()->setGroupDynamicSizeProperty(SimulationMap::ContactorGroup, true);
		ui.graphicsView->getScene()->setGroupVisibleProperty(SimulationMap::TableObstacleGroup, false);
		ui.graphicsView->getScene()->setMap(_simulator->getMap());
		_simulator->getMap()->moveOpponent(QPointF(-1000, -1000));
	}
}

void NPlayer::viewModeChanged()
{
	if (!_strategy)
		return;

	customizeView();

	ui.graphicsView->refresh();
}

void NPlayer::setConfigWidgetEnabled(bool value)
{
	ui.comboStrats->setEnabled(value);
	ui.cbColor->setEnabled(value);
	ui.actionNew_configuration->setEnabled(value);
	ui.actionNew_random_configuration->setEnabled(value);
	ui.actionEmpty_configuration->setEnabled(value);
}

void NPlayer::strategyChanged()
{
	StrategyEnumerator strategies;
	_strategy = strategies.start((StrategyEnumerator::Strategy)ui.comboStrats->currentIndex(), _logger);
	_strategy->setLogEnabled(ui.checkShowStrategyLog->isChecked());

	_manager->setStrategy(_strategy);
	//_manager->init();

	customizeView();

	ui.graphicsView->refresh();
}

void NPlayer::colorChanged()
{
	_manager->setReversedStrategy(ui.cbColor->currentIndex() != 0);
	//_manager->init();

    _simulator->setRobotDefaultColor(ui.cbColor->currentIndex() == 0 ? Simulator::Red : Simulator::Yellow);

	ui.graphicsView->refresh();
}

void NPlayer::openReplay()
{
	QString filename = QFileDialog::getOpenFileName(this, "Please select the record file to open.", QString(), "*.nrec");

	ui.actionReplay->setChecked(true);
	viewModeChanged();

	if (!filename.isEmpty())
		startPlaying(filename);
}

void NPlayer::startPlaying(const QString& filename)
{
	_playing = true;

	if (_playedFile)
	{
		_playedFile->close();
		_playedFile->setFileName(filename);
	}
	else
		_playedFile = new QFile(filename);

	_playedFile->open(QIODevice::ReadOnly);

	if (_player)
		_player->setDevice(_playedFile);
	else
		_player = new QDataStream(_playedFile);

	ui.actionRun->setEnabled(true);
	ui.actionStop->setEnabled(true);
}

void NPlayer::stopPlaying()
{
	if (_playing == true)
	{
		_playedFile->close();
		_player->setDevice(0);
		_playing = false;
		_playTimer.stop();

		ui.actionRun->setEnabled(false);
		ui.actionStop->setEnabled(false);
	}
}

void NPlayer::play()
{
	if (!_player->atEnd())
	{
		_playTime = _playTime.addMSecs(_playSpeed);

		double x, y, theta, r;
		int nbObjects;
		QList<double> sharps;

		*_player >> x
				 >> y
				 >> theta
				 >> sharps;

		_simulator->getMap()->moveRobot(Tools::RPoint(x, y, theta));

		QList<Tools::DataObject *> avoidingSharpList = _simulator->getMap()->getObjects(SimulationMap::AvoidingSharpGroup);
		for (QList<Tools::DataObject *>::iterator object = avoidingSharpList.begin(); object != avoidingSharpList.end(); ++object)
		{
			SharpObject* sharp = static_cast<SharpObject*>(*object);
			sharp->setDistance(sharps.takeFirst());
		}

		QList<Tools::DataObject *> otherSharpList = _simulator->getMap()->getObjects(SimulationMap::OtherSharpGroup);
		for (QList<Tools::DataObject *>::iterator object = otherSharpList.begin(); object != otherSharpList.end(); ++object)
		{
			SharpObject* sharp = static_cast<SharpObject*>(*object);
			sharp->setDistance(sharps.takeFirst());
		}


		*_player >> nbObjects;
		_simulator->getMap()->clear(SimulationMap::ObstacleGroup);

		for (int i = 0; i < nbObjects; i++)
		{
			*_player >> r >> x >> y >> theta;
			_simulator->getMap()->addObject(new Tools::CircleObject(r, QPointF(x, y), theta), SimulationMap::ObstacleGroup);
		}

	}
	else
	{
		ui.actionRun->setChecked(false);
		_playTimer.stop();
	}
}



void NPlayer::record(bool value)
{
	if (value)
	{
		if (!_manager->isRecording())
		{
			QString filename = QFileDialog::getSaveFileName(this, "Please select a name for the record file.", QString(), "*.nrec");

			if (!filename.isEmpty())
				_manager->startRecording(filename);
		}
	}
	else
		_manager->stopRecording();

}

void NPlayer::on_btnBlock_clicked()
{
	if (_simulator)
		_simulator->block();
}

void NPlayer::newConfiguration()
{
	//    if (_configSelector->exec() == QDialog::Accepted)
	//    {
	//	_simulator->setConfiguration(ChessUpConfiguration(_configSelector->getCard1(), _configSelector->getCard2(), _configSelector->getCard3()));
	//	ui.graphicsView->refresh();
	//    }
}

void NPlayer::newRandomConfiguration()
{
	//    _simulator->setConfiguration(ChessUpConfiguration(true));
	//    ui.graphicsView->refresh();
}

void NPlayer::newEmptyConfiguration()
{
	//    _simulator->setConfiguration(ChessUpConfiguration(false));
	//    ui.graphicsView->refresh();
}

void NPlayer::viewContextMenuRequested(const QPoint &pos)
{
	_lastContextMenuPos = pos;
	_viewContextMenu->exec(ui.graphicsView->mapToGlobal(pos));
}

void NPlayer::setOpponentHere()
{
	_simulator->getMap()->moveOpponent(Tools::StrategyScene::mapWithDataMap(ui.graphicsView->mapToScene(_lastContextMenuPos)));
	ui.graphicsView->refresh();
}

void NPlayer::moveOpponentHere()
{
	_simulator->addOpponentMovement(Tools::StrategyScene::mapWithDataMap(ui.graphicsView->mapToScene(_lastContextMenuPos)));
	ui.graphicsView->refresh();
}

void NPlayer::refreshMatchTime()
{
	if (ui.actionSimulation->isChecked())
	{
		QTime t(0, 0, 0);
		t = t.addSecs(_manager->getMatchTime());
		ui.labelMatchTimer->setText(t.toString("mm:ss"));
	}
	else
		ui.labelMatchTimer->setText(_playTime.toString("mm:ss"));
}

void NPlayer::replaySpeedChanged(int value)
{
	_playSpeed = 12.5 * pow(2.0, 8 - value);
	_playTimer.setInterval(_playSpeed);
}


void NPlayer::closeEvent(QCloseEvent *e)
{
	saveSettings();

	QMainWindow::closeEvent(e);
}

void NPlayer::saveSettings()
{
	QSettings settings;

	settings.setValue("MainWindowGeometry", saveGeometry());
	settings.setValue("MainWindowState", saveState());
}

void NPlayer::loadSettings()
{
	QSettings settings;

	restoreGeometry(settings.value("MainWindowGeometry").toByteArray());
	restoreState(settings.value("MainWindowState").toByteArray());
}
