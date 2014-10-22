#include "RemoteControlView.h"
#include "ui_RemoteControlView.h"

#include "StrategyScene.h"
#include "DataMap.h"

#include "PointEdit.h"
#include "NetworkClientCommInterface.h"
#include "TeleportDialog.h"
#include "ToolsLib.h"

#include "StoredTrajectoriesManagerDialog.h"

#include <QToolButton>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>

RemoteControlView::RemoteControlView(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(), parent),
	ui(new Ui::RemoteControlView), _isMoving(false), _currentObjectiveIndex(-1), _strategyRunning(false)
{
	ui->setupUi(this);

    addColorToCombobox(Qt::red, ui->cbColor);
    addColorToCombobox(Qt::yellow, ui->cbColor);

	_connection->registerRobotResponder(this);
	_connection->registerNetworkResponder(this);

	ui->lePosition->setReadOnly(true);
	ui->leObjective->setReadOnly(true);

    ui->nsEditor->addSearchDirectory(_connection->getGlobalScriptDirectory());

	_map = new Tools::DataMap(QSizeF(2000, 3000), 350/2, RobotGroup, 350/2, OpponentGroup);
	ui->graphicsView->getScene()->setMap(_map);
	ui->graphicsView->setRefreshInterval(100);
	ui->graphicsView->setRefreshEnabled(false);
	ui->graphicsView->getScene()->setGroupAsLineObject(Objective);
	ui->graphicsView->getScene()->setGroupDynamicProperty(Objective, true);
	ui->graphicsView->getScene()->setGroupDynamicSizeProperty(Objective, true);
	ui->graphicsView->getScene()->setGroupTransformationOrigin(Objective, Tools::StrategyScene::TopLeft);

	QPen trajectoryPen(Qt::darkBlue);
	trajectoryPen.setWidth(10);
	trajectoryPen.setStyle(Qt::DotLine);
	ui->graphicsView->getScene()->setGroupPenProperty(Objective, trajectoryPen);

	_objectiveLine = new Tools::LineObject(QLineF(0, 0, 100, 100));
	_map->addObject(_objectiveLine, Objective);

	_teleportDialog = new TeleportDialog(QVector3D(), this);

	refreshConnectionStatusGui();

	ui->btnLoadTrajectory->setPopupMode(QToolButton::InstantPopup);
	ui->btnLoadTrajectory->setMenu(new QMenu(this));
	_storedTrajectoriesLoadMapper = new QSignalMapper(this);
	connect(_storedTrajectoriesLoadMapper, SIGNAL(mapped(const QString&)), this, SLOT(loadTrajectory(const QString&)));
	refreshStoredTrajectoriesMenu();

	_trPointRemoveMapper = new QSignalMapper(this);
	connect(_trPointRemoveMapper, SIGNAL(mapped(QWidget*)), this, SLOT(removeTrajectoryPoint(QWidget*)));

	connect(ui->btnAddPoint, SIGNAL(clicked()), this, SLOT(addTrajectoryPoint()));
	connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(stopRobot()));
	connect(ui->btnSend, SIGNAL(clicked()), this, SLOT(sendTrajectory()));
	connect(ui->btnTeleport, SIGNAL(clicked()), this, SLOT(teleport()));
	connect(ui->btnSaveTrajectory, SIGNAL(clicked()), this, SLOT(saveCurrentTrajectoryAs()));
	connect(ui->btnManageTrajectories, SIGNAL(clicked()), this, SLOT(manageTrajectories()));
	connect(ui->btnStart, SIGNAL(clicked()), this, SLOT(startStopStrategy()));
	connect(ui->btnRunScript, SIGNAL(clicked()), this, SLOT(sendScript()));
}

RemoteControlView::~RemoteControlView()
{
	delete _map;
	delete ui;
}

void RemoteControlView::connectionStatusChanged(NetworkConnection::ConnectionStatus status)
{
	ui->graphicsView->setEnabled(status == NetworkConnection::Controled);
	ui->btnSend->setEnabled(status == NetworkConnection::Controled && !_trajectoryEditors.isEmpty() && !_isMoving && !_strategyRunning);
	ui->btnTeleport->setEnabled(status == NetworkConnection::Controled && !_strategyRunning);
	ui->btnStop->setEnabled(status == NetworkConnection::Controled && !_strategyRunning);
	ui->btnAddPoint->setEnabled(!_isMoving && !_strategyRunning);

	ui->cbStrategies->setEnabled(status == NetworkConnection::Controled && !_strategyRunning && !_isMoving);
	ui->cbColor->setEnabled(status == NetworkConnection::Controled && !_strategyRunning && !_isMoving);
	ui->btnStart->setEnabled(status == NetworkConnection::Controled && !_isMoving);
	ui->btnStart->setText(_strategyRunning ? "Stop" : "Start");

	foreach(TrajectoryPointWidget* tr, _trajectoryEditors)
		tr->disableEdition(_isMoving || _strategyRunning);

	ui->btnRunScript->setEnabled(status == NetworkConnection::Controled && !_strategyRunning && !_isMoving);
}

void RemoteControlView::activeStatusChanged(bool isActive)
{
	ui->graphicsView->setRefreshEnabled(isActive);
}

void RemoteControlView::saveSettings(QSettings *settings)
{
	settings->beginGroup("RemoteControl");

	settings->beginWriteArray("StoredTrajectory", _storedTrajectories.count());
	int i = 0;
	for(QMap<QString, Tools::Trajectory>::const_iterator it = _storedTrajectories.constBegin(); it != _storedTrajectories.constEnd(); ++it)
	{
		settings->setArrayIndex(i++);
		settings->setValue("name", it.key());
	}
	settings->endArray();

	settings->beginWriteArray("Trajectory", _trajectoryEditors.count());
	int j = 0;
	foreach(TrajectoryPointWidget* trWidget, _trajectoryEditors)
	{
		QVector3D p = trWidget->getPointEditor()->getPoint();
		settings->setArrayIndex(j++);
		settings->setValue("x", p.x());
		settings->setValue("y", p.y());
		settings->setValue("theta", p.z());
	}
	settings->endArray();

	settings->endGroup();


	settings->remove("RemoteControlTrajectories");
	settings->beginGroup("RemoteControlTrajectories");
	for(QMap<QString, Tools::Trajectory>::const_iterator it = _storedTrajectories.constBegin(); it != _storedTrajectories.constEnd(); ++it)
	{
		const Tools::Trajectory& tr = it.value();
		settings->beginWriteArray(it.key(), tr.count());
		int k = 0;
		foreach(const Tools::RPoint& p, tr)
		{
			settings->setArrayIndex(k++);
			settings->setValue("x", p.getX());
			settings->setValue("y", p.getY());
			settings->setValue("theta", p.getTheta());
		}
		settings->endArray();
	}
	settings->endGroup();

	settings->setValue("DeplacementType", ui->cbMoveType->currentIndex());

	settings->setValue("tabIndex", ui->tabControler->currentIndex());
}

void RemoteControlView::loadSettings(QSettings *settings)
{
	settings->beginGroup("RemoteControl");

	QStringList storedTrajectoriesName;
	int nbStoredTrajectories = settings->beginReadArray("StoredTrajectory");
	for(int i = 0; i < nbStoredTrajectories; ++i)
	{
		settings->setArrayIndex(i);
		storedTrajectoriesName << settings->value("name").toString();
	}
	settings->endArray();

	int nbTrajPoint = settings->beginReadArray("Trajectory");
	for(int j = 0; j < nbTrajPoint; ++j)
	{
		QVector3D p;
		settings->setArrayIndex(j);
		p.setX(settings->value("x").toDouble());
		p.setY(settings->value("y").toDouble());
		p.setZ(settings->value("theta").toDouble());

		addPoint(p);
	}
	settings->endArray();

	settings->endGroup();

	settings->beginGroup("RemoteControlTrajectories");
	foreach(const QString& name, storedTrajectoriesName)
	{
		Tools::Trajectory tr;
		int nbTrajPoint = settings->beginReadArray(name);
		for(int k = 0; k < nbTrajPoint; ++k)
		{
			Tools::RPoint p;
			settings->setArrayIndex(k);
			p.setX(settings->value("x").toDouble());
			p.setY(settings->value("y").toDouble());
			p.setTheta(settings->value("theta").toDouble());

			tr.append(p);
		}

		_storedTrajectories.insert(name, tr);
		settings->endArray();
	}
	refreshStoredTrajectoriesMenu();

	settings->endGroup();

	ui->cbMoveType->setCurrentIndex(settings->value("DeplacementType").toInt());

	ui->tabControler->setCurrentIndex(settings->value("tabIndex", 0).toInt());
}

void RemoteControlView::coordinates(qint16 x, qint16 y, double theta, quint8 forward)
{
	Q_UNUSED(forward);
	if (isViewActive())
	{
		_map->moveRobot(QPointF(x, y), theta);
		ui->lePosition->setText(QString::fromUtf8("x=%1  y=%2  𝛳=%3").arg(x).arg(y).arg((int)Tools::radianToDegree(theta)));

		if (_isMoving)
			_objectiveLine->setPosition(QPointF(x, y));
		else
			_objectiveLine->setPosition(QPointF(-1000, -1000));


	}
}

void RemoteControlView::objective(qint16 x, qint16 y, double theta)
{
	if (isViewActive())
	{
		ui->leObjective->setText(QString::fromUtf8("x=%1  y=%2  𝛳=%3").arg(x).arg(y).arg((int)Tools::radianToDegree(theta)));
		if (_isMoving)
			_objectiveLine->setP2(QPointF(x, y));
		else
			_objectiveLine->setLength(0);

	}
}

void RemoteControlView::opponentPosition(qint16 x, qint16 y)
{
	if (isViewActive())
	{
		_map->moveOpponent(QPointF(x, y));
	}
}

void RemoteControlView::robotEvent(Comm::RobotEvent event)
{
	if (event == Comm::EVENT_IS_ARRIVED)
		isArrived();
}

void RemoteControlView::isArrived()
{
	TrajectoryPointWidget* tr = _trajectoryEditors.value(_currentObjectiveIndex);
	if (tr)
	{
		tr->setCurrent(false);
		++_currentObjectiveIndex;
		if (_currentObjectiveIndex == _trajectoryEditors.count())
		{
			//end of trajectory
			_isMoving = false;
			refreshConnectionStatusGui();
		}
		else
			_trajectoryEditors.value(_currentObjectiveIndex)->setCurrent(true);
	}
}

void RemoteControlView::setStrategies(const QStringList &strategies)
{
	ui->cbStrategies->clear();
	ui->cbStrategies->addItems(strategies);
}

void RemoteControlView::strategyStatusUpdated(int strategyNum, bool isRunning)
{
	ui->cbStrategies->setCurrentIndex(strategyNum);
	_strategyRunning = isRunning;
	refreshConnectionStatusGui();
}

void RemoteControlView::networkNoticeOfReceipt(quint8 instruction, bool result)
{
	if (instruction == Comm::RUN_SCRIPT && !result)
	{
		QMessageBox::critical(this, "Error", "The script contains some errors and cannot be run.");
	}
}

void RemoteControlView::addPoint(const QVector3D& defaultValue)
{
	TrajectoryPointWidget* trWidget = new TrajectoryPointWidget(defaultValue, this);
	ui->pointsLayout->insertWidget(ui->pointsLayout->count() - 3, trWidget);
	trWidget->setNum(_trajectoryEditors.count() + 1);
	trWidget->disableEdition(_isMoving);
	_trajectoryEditors << trWidget;

	connect(trWidget, SIGNAL(removeAsked()), _trPointRemoveMapper, SLOT(map()));
	_trPointRemoveMapper->setMapping(trWidget, trWidget);

	ui->btnSend->setEnabled(_connection->getConnectionStatus() == NetworkConnection::Controled && !_isMoving);
}

void RemoteControlView::clearTrajectory()
{
	foreach(TrajectoryPointWidget* tr, _trajectoryEditors)
		delete tr;

	_trajectoryEditors.clear();
	refreshConnectionStatusGui();
}

void RemoteControlView::updateTrajectoryNumeratation()
{
	int i = 0;
	foreach(TrajectoryPointWidget* tr, _trajectoryEditors)
	{
		tr->setNum(++i);
	}
}

void RemoteControlView::refreshStoredTrajectoriesMenu()
{
	ui->btnLoadTrajectory->menu()->clear();
	for(QMap<QString, Tools::Trajectory>::const_iterator it = _storedTrajectories.constBegin(); it != _storedTrajectories.constEnd(); ++it)
	{
		const QString& trName = it.key();
		QAction* action = ui->btnLoadTrajectory->menu()->addAction(trName, _storedTrajectoriesLoadMapper, SLOT(map()));
		_storedTrajectoriesLoadMapper->setMapping(action, trName);
	}

	ui->btnLoadTrajectory->setEnabled(!_storedTrajectories.isEmpty());
	ui->btnManageTrajectories->setEnabled(!_storedTrajectories.isEmpty());
}

void RemoteControlView::addTrajectoryPoint()
{
	addPoint(_trajectoryEditors.isEmpty() ? QVector3D() : _trajectoryEditors.last()->getPointEditor()->getPoint());
}

void RemoteControlView::removeTrajectoryPoint(QWidget *trajectoryWidget)
{
	_trajectoryEditors.removeAll(static_cast<TrajectoryPointWidget*>(trajectoryWidget));
	delete trajectoryWidget;

	refreshConnectionStatusGui();

	updateTrajectoryNumeratation();
}

void RemoteControlView::stopRobot()
{
	_connection->getComm()->flush();
	_isMoving = false;

	if (_currentObjectiveIndex >= 0 && _trajectoryEditors.count() > _currentObjectiveIndex)
		_trajectoryEditors.value(_currentObjectiveIndex)->setCurrent(false);
	_currentObjectiveIndex = -1;

	refreshConnectionStatusGui();
}

void RemoteControlView::sendTrajectory()
{
	_isMoving = true;
	Tools::Deplacement dep = Tools::TURN_THEN_MOVE;
	switch(ui->cbMoveType->currentIndex())
	{
		case 1: dep = Tools::TURN_AND_MOVE; break;
		case 2: dep = Tools::TURN_ONLY; break;
		case 3: dep = Tools::MOVE_ONLY; break;
	}

	foreach(TrajectoryPointWidget* tr, _trajectoryEditors)
	{
		QVector3D p = tr->getPointEditor()->getPoint();
		Tools::RMovement mvt(Tools::RPoint(p.x(), p.y(), Tools::degreeToRadian(p.z())), Tools::AUTO, dep, tr == _trajectoryEditors.last(), 100);
		_connection->getComm()->setDestination(mvt);
	}

	if (!_trajectoryEditors.isEmpty())
	{
		_currentObjectiveIndex = 0;
		_trajectoryEditors.first()->setCurrent(true);
		ui->btnSend->setEnabled(false);
	}

	refreshConnectionStatusGui();
}

void RemoteControlView::teleport()
{
	if (_teleportDialog->exec() == QDialog::Accepted)
	{
		QVector3D p = _teleportDialog->getPoint();
		_connection->getComm()->setPosition(p.x(), p.y(), Tools::degreeToRadian(p.z()));
	}
}

void RemoteControlView::saveCurrentTrajectoryAs()
{
	//Get a unique default tr name
	QString defaultName;
	QString basename = "Trajectory";
	int nb = 1;

	do
	{
		defaultName = basename;
		defaultName.append(QString::number(nb));
		++nb;
	}
	while (_storedTrajectories.contains(defaultName));


	bool ok = false;
	QString name = QInputDialog::getText(this, "New trajectory", "Please type a trajectory name", QLineEdit::Normal, defaultName, &ok);
	if (ok)
	{
		if (_storedTrajectories.contains(name))
		{
			if (QMessageBox::question(this, "New trajectory", "A trajectory with the same name already exist. Do you want to replace it ?",
								  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
			{
				return;
			}
		}

		Tools::Trajectory t;
		foreach(TrajectoryPointWidget* tr, _trajectoryEditors)
		{
			QVector3D p = tr->getPointEditor()->getPoint();
			t.append(Tools::RPoint(p.x(), p.y(), Tools::degreeToRadian(p.z())));
		}

		_storedTrajectories.insert(name, t);

		refreshStoredTrajectoriesMenu();
	}
}

void RemoteControlView::loadTrajectory(const QString &name)
{
	if (_storedTrajectories.contains(name))
	{
		clearTrajectory();
		Tools::Trajectory& t = _storedTrajectories[name];

		foreach(const Tools::RPoint& p, t)
		{
			addPoint(QVector3D(p.getX(), p.getY(), p.getThetaDegree()));
		}
	}
}

void RemoteControlView::manageTrajectories()
{
	StoredTrajectoriesManagerDialog dialog(_storedTrajectories, this);
	if (dialog.exec() == QDialog::Accepted)
	{
		_storedTrajectories = dialog.getTrajectories();
		refreshStoredTrajectoriesMenu();
	}
}

void RemoteControlView::startStopStrategy()
{
	if (!_strategyRunning)
	{
		if (ui->cbStrategies->currentIndex() >= 0)
			_connection->getComm()->startStrategy(ui->cbStrategies->currentIndex(), ui->cbColor->currentIndex() > 0);
	}
	else
		_connection->getComm()->stopStrategy();
}

void RemoteControlView::sendScript()
{
	QString script = ui->nsEditor->getScript();
	if (!script.trimmed().isEmpty())
		_connection->getComm()->runScript(script.toLatin1());
}

void RemoteControlView::addColorToCombobox(const QColor& color, QComboBox* combo) const
{
	int size = ui->cbColor->style()->pixelMetric(QStyle::PM_SmallIconSize);
	QPixmap pixmap(size,size);

	combo->addItem("");
	pixmap.fill(color);
	combo->setItemData(combo->count() - 1, pixmap, Qt::DecorationRole);//Setting color palettes
}

//-----------------------------------

TrajectoryPointWidget::TrajectoryPointWidget(const QVector3D &defaultValue, QWidget *parent) : QWidget(parent)
{
	QHBoxLayout* vl = new QHBoxLayout(this);
	vl->setContentsMargins(0, 0, 0, 0);
	vl->setSpacing(6);

	_lbl = new QLabel("", this);
	_lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	_lbl->hide();

	_pointEditor = new Tools::PointEdit(defaultValue, this);
	_pointEditor->setPrefixes("x=", "y=", QString::fromUtf8("𝛳="));
	//_pointEditor->setSuffixes("mm", "mm", QString::fromUtf8("°"));
	_pointEditor->setSeparator(" ");

	_btn = new QToolButton(this);
	_btn->setText("-");

	vl->addWidget(_lbl);
	vl->addWidget(_pointEditor);
	vl->addWidget(_btn);

	connect(_btn, SIGNAL(clicked()), this, SIGNAL(removeAsked()));

	_pointEditor->setFocus();
}

Tools::PointEdit *TrajectoryPointWidget::getPointEditor() const
{
	return _pointEditor;
}

void TrajectoryPointWidget::setNum(int i)
{
	if (i >= 0)
	{
		_lbl->setText(QString::number(i));
		_lbl->show();
	}
	else
	{
		_lbl->hide();
	}
}

void TrajectoryPointWidget::setCurrent(bool isCurrent)
{
	if (isCurrent)
	{
		setStyleSheet("color: red");
	}
	else
		setStyleSheet("");
}

void TrajectoryPointWidget::disableEdition(bool value)
{
	_btn->setEnabled(!value);
	_pointEditor->setEnabled(!value);
}
