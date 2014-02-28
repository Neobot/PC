#include "Ax12View.h"
#include "ui_Ax12View.h"

#include "NetworkClientCommInterface.h"
#include "Ax12MovementManager.h"

#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <QFileDialog>

Ax12View::Ax12View(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(":/toolbar/ax12"), parent),
	ui(new Ui::Ax12View)
{
	ui->setupUi(this);

	connect(ui->btnAddAx12, SIGNAL(clicked()), this, SLOT(addAx12()));

	_removeAx12Mapper = new QSignalMapper(this);
	connect(_removeAx12Mapper, SIGNAL(mapped(QWidget*)), this, SLOT(removeAx12(QWidget*)));

	_sendAx12Mapper = new QSignalMapper(this);
	connect(_sendAx12Mapper, SIGNAL(mapped(QWidget*)), this, SLOT(sendAx12(QWidget*)));

	_increaseAx12Mapper = new QSignalMapper(this);
	connect(_increaseAx12Mapper, SIGNAL(mapped(QWidget*)), this, SLOT(increaseAx12(QWidget*)));

	_decreaseAx12Mapper = new QSignalMapper(this);
	connect(_decreaseAx12Mapper, SIGNAL(mapped(QWidget*)), this, SLOT(decreaseAx12(QWidget*)));

	connect(ui->btnRefreshPositions, SIGNAL(clicked()), this, SLOT(refreshAx12Positions()));

	connect(ui->btnRecord, SIGNAL(clicked()), this, SLOT(addRecord()));

	connect(ui->btnLoadMovements, SIGNAL(clicked()), this, SLOT(reloadMovements()));
	connect(ui->btnSaveMovements, SIGNAL(clicked()), this, SLOT(saveMovements()));
	connect(ui->btnExport, SIGNAL(clicked()), this, SLOT(exportMovements()));
	connect(ui->btnImport, SIGNAL(clicked()), this, SLOT(importMovements()));
	connect(ui->btnLockAll, SIGNAL(clicked()), this, SLOT(lockAll()));
	connect(ui->btnReleaseAll, SIGNAL(clicked()), this, SLOT(releaseAll()));
	
	connect(ui->movementEditor, SIGNAL(currentTabChanged(int)), this, SLOT(movementEditorTabChanged(int)));
	connect(ui->btnRunMovement, SIGNAL(clicked()), this, SLOT(runMovement()));

    connect(ui->movementEditor, SIGNAL(runCurrentMovementUntil(int)), this, SLOT(runCurrentMovementUntil(int)));
    connect(ui->movementEditor, SIGNAL(moveToCurrentMovementPosition(int)), this, SLOT(moveToCurrentMovementPosition(int)));

	connectionStatusChanged(_connection->getConnectionStatus());
	_connection->registerNetworkResponder(this);
}

Ax12View::~Ax12View()
{
	delete ui;
}

void Ax12View::loadSettings(QSettings *settings)
{
	settings->beginGroup("Ax12");

	int nbAx12 = settings->beginReadArray("ax12");
	for(int i = 0; i < nbAx12; ++i)
	{
		settings->setArrayIndex(i);
		int id = settings->value("id").toInt();
		float speed = settings->value("speed").toDouble();
		float torque = settings->value("torque").toDouble();

		addAx12(id, speed, torque);
	}
	settings->endArray();

	ui->spSpeedLimit->setValue(settings->value("movementSpeedLimit", 114).toDouble());

	settings->endGroup();
}

void Ax12View::saveSettings(QSettings *settings)
{
	settings->beginGroup("Ax12");

	settings->beginWriteArray("ax12", _ax12Widgets.count());
	int index = 0;
	foreach(Ax12DisplayWidget* ax12, _ax12Widgets)
	{
		settings->setArrayIndex(index++);
		settings->setValue("id", ax12->getId());
		settings->setValue("speed", ax12->getSpeed());
		settings->setValue("torque", ax12->getTorque());
	}
	settings->endArray();

	settings->setValue("movementSpeedLimit", ui->spSpeedLimit->value());

	settings->endGroup();
}

void Ax12View::connectionStatusChanged(NetworkConnection::ConnectionStatus status)
{
	switch(status)
	{
		case NetworkConnection::Lookup:
		case NetworkConnection::Disconnected:
			ui->btnExport->setEnabled(false);
			ui->btnImport->setEnabled(false);
			ui->movementEditor->setEnabled(false);
			ui->btnLoadMovements->setEnabled(false);
			ui->btnSaveMovements->setEnabled(false);
			ui->btnRecord->setEnabled(false);
			ui->widgetRunMovement->setEnabled(false);
			ui->btnLockAll->setEnabled(false);
			ui->btnReleaseAll->setEnabled(false);
			ui->btnRefreshPositions->setEnabled(false);
			foreach(Ax12DisplayWidget* ax12, _ax12Widgets)
				ax12->setSendEnabled(false);
			break;
		case NetworkConnection::Connected:
			reloadMovements();
			ui->btnExport->setEnabled(true);
			ui->btnImport->setEnabled(true);
			ui->movementEditor->setEnabled(true);
			ui->btnLoadMovements->setEnabled(true);
			ui->btnSaveMovements->setEnabled(true);
			ui->btnRecord->setEnabled(false);
			ui->widgetRunMovement->setEnabled(false);
			ui->btnLockAll->setEnabled(false);
			ui->btnReleaseAll->setEnabled(false);
			ui->btnRefreshPositions->setEnabled(false);
			foreach(Ax12DisplayWidget* ax12, _ax12Widgets)
				ax12->setSendEnabled(false);
			break;

		case NetworkConnection::Controled:
		{
			ui->btnExport->setEnabled(true);
			ui->btnImport->setEnabled(true);
			ui->movementEditor->setEnabled(true);
			ui->btnLoadMovements->setEnabled(true);
			ui->btnSaveMovements->setEnabled(true);
			ui->btnRecord->setEnabled(ui->movementEditor->getCurrentTab() == Tools::Ax12MovementEditor::Positions);
			ui->btnLockAll->setEnabled(true);
			ui->btnReleaseAll->setEnabled(true);
			ui->widgetRunMovement->setEnabled(ui->movementEditor->getCurrentTab() == Tools::Ax12MovementEditor::Positions);
			ui->btnRefreshPositions->setEnabled(true);
			foreach(Ax12DisplayWidget* ax12, _ax12Widgets)
				ax12->setSendEnabled(true);
			refreshAx12Positions();
			break;
		}
	}
}

void Ax12View::movementEditorTabChanged(int index)
{
	switch(index)
	{
		case Tools::Ax12MovementEditor::Groups:
		case Tools::Ax12MovementEditor::Movements:
			ui->btnRecord->setEnabled(false);
			ui->widgetRunMovement->setEnabled(false);
			break;
		case Tools::Ax12MovementEditor::Positions:
			ui->btnRecord->setEnabled(true);
			ui->widgetRunMovement->setEnabled(_connection->getConnectionStatus() == NetworkConnection::Controled);
			break;
	}
}

void Ax12View::addAx12(int id, float speed, float torque)
{
	Ax12DisplayWidget* ax12widget = new Ax12DisplayWidget(this);
	ax12widget->setId(id);
	ax12widget->setSpeed(speed);
	ax12widget->setTorque(torque);
	ax12widget->setSendEnabled(_connection->getConnectionStatus() == NetworkConnection::Controled);

	ui->Ax12BoxLayout->insertWidget(_ax12Widgets.count() + 1, ax12widget);

	_removeAx12Mapper->setMapping(ax12widget, ax12widget);
	connect(ax12widget, SIGNAL(remove()), _removeAx12Mapper, SLOT(map()));

	_sendAx12Mapper->setMapping(ax12widget, ax12widget);
	connect(ax12widget, SIGNAL(send()), _sendAx12Mapper, SLOT(map()));

	_increaseAx12Mapper->setMapping(ax12widget, ax12widget);
	connect(ax12widget, SIGNAL(increase()), _increaseAx12Mapper, SLOT(map()));

	_decreaseAx12Mapper->setMapping(ax12widget, ax12widget);
	connect(ax12widget, SIGNAL(decrease()), _decreaseAx12Mapper, SLOT(map()));

	_ax12Widgets.insert(id, ax12widget);

	QList<int> ids;
	ids << id;
	_connection->getComm()->askAx12Positions(ids);
}

void Ax12View::setAx12Positions(const QList<int> ids, const QList<float> &positions)
{
	for(int i = 0; i < ids.count(); ++i)
	{
		int id = ids.value(i);
		float pos = positions.value(i, 0.0);

		if (_ax12Widgets.contains(id))
			_ax12Widgets.value(id)->setAngle(pos);
	}
}

void Ax12View::setAx12MovementsFile(const QByteArray &data)
{
	Tools::Ax12MovementManager manager;

	QString strData(data);
	QTextStream stream(&strData);
	manager.readFromStream(stream);

	ui->movementEditor->edit(manager);
}

QMap<int, float> Ax12View::getAllPositions() const
{
	QMap<int, float> results;
	foreach(Ax12DisplayWidget* ax12, _ax12Widgets)
	{
		results[ax12->getId()] = ax12->getAngle();
	}

	return results;
}

void Ax12View::addAx12()
{
	int id = 0;

	while (_ax12Widgets.contains(id) && id <= 254)
		++id;

	bool ok = false;
	id = QInputDialog::getInt(this, "AX-12 identifier", "Select the id of the AX-12", id, 0, 253, 1, &ok);

	if (_ax12Widgets.contains(id))
	{
		QMessageBox::critical(this, "Cannot add this AX-12", QString("The Ax-12 ").append(id).append(" is already present."));
		return;
	}

	if (id < 254 && ok)
		addAx12(id, 50, 100);
}

void Ax12View::removeAx12(QWidget *ax12Widget)
{
	Ax12DisplayWidget* ax12 = static_cast<Ax12DisplayWidget*>(ax12Widget);
	if (_ax12Widgets.contains(ax12->getId()))
	{
		ui->Ax12BoxLayout->removeWidget(ax12);
		_ax12Widgets.remove(ax12->getId());
		delete ax12;
	}
}

void Ax12View::sendAx12(QWidget *ax12Widget)
{
	Ax12DisplayWidget* ax12 = static_cast<Ax12DisplayWidget*>(ax12Widget);

	Comm::Ax12Info info;
	info.id = ax12->getId();
	info.angle = ax12->getAngle();
	info.speed = ax12->getSpeed();
	info.torque = ax12->getTorque();

	_connection->getComm()->moveAx12(info);
}

void Ax12View::increaseAx12(QWidget *ax12Widget)
{
	Ax12DisplayWidget* ax12 = static_cast<Ax12DisplayWidget*>(ax12Widget);
	ax12->setAngle(ax12->getAngle() + 5);

	Comm::Ax12Info info;
	info.id = ax12->getId();
	info.angle = ax12->getAngle();
	info.speed = 10.0;
	info.torque = ax12->getTorque();

	_connection->getComm()->moveAx12(info);
}

void Ax12View::decreaseAx12(QWidget *ax12Widget)
{
	Ax12DisplayWidget* ax12 = static_cast<Ax12DisplayWidget*>(ax12Widget);
	ax12->setAngle(ax12->getAngle() - 5);

	Comm::Ax12Info info;
	info.id = ax12->getId();
	info.angle = ax12->getAngle();
	info.speed = 10.0;
	info.torque = ax12->getTorque();

	_connection->getComm()->moveAx12(info);
}

void Ax12View::refreshAx12Positions()
{
	QList<int> ids = _ax12Widgets.keys();
	_connection->getComm()->askAx12Positions(ids);
}

void Ax12View::lockAll()
{
	QMap<int, bool> lockInfo;
	foreach(Ax12DisplayWidget* ax12, _ax12Widgets)
	{
		lockInfo[ax12->getId()] = true;
	}

	_connection->getComm()->lockAx12(lockInfo);
}

void Ax12View::releaseAll()
{
	QMap<int, bool> lockInfo;
	foreach(Ax12DisplayWidget* ax12, _ax12Widgets)
	{
		lockInfo[ax12->getId()] = false;
	}

	_connection->getComm()->lockAx12(lockInfo);
}

void Ax12View::addRecord()
{
	if (!ui->movementEditor->addRecord(getAllPositions(), 110, 50))
	{
		QMessageBox::critical(this, "Record failed", "Please select a movement to record the position");
	}
}

void Ax12View::reloadMovements()
{
	_connection->getComm()->askAx12Movements();
}

void Ax12View::saveMovements()
{
	QString data;
	QTextStream stream(&data);
	ui->movementEditor->getEditedMovements().writeToStream(stream);

	_connection->getComm()->setAx12Movements(data.toLatin1());
}

void Ax12View::exportMovements()
{
	QString filename = QFileDialog::getSaveFileName(this, "Where do you want to export the recorded AX-12 movements ?", QString(), "AX-12 Movements (*.nax)");
	if (filename.isEmpty())
		return;

	if (!ui->movementEditor->getEditedMovements().writeToFile(filename))
		QMessageBox::critical(this, "AX-12 movements export error", QString("Cannot save in ").append(filename));
}

void Ax12View::importMovements()
{
	QString filename = QFileDialog::getOpenFileName(this, "Select an AX-12 movement file to import", QString(), "AX-12 Movements (*.nax)");
	if (filename.isEmpty())
		return;

	Tools::Ax12MovementManager manager = ui->movementEditor->getEditedMovements();
	if (!manager.readFromFile(filename))
		QMessageBox::critical(this, "AX-12 movements import error", QString("Cannot read from ").append(filename));

	ui->movementEditor->edit(manager);
}

void Ax12View::runMovement()
{
    runCurrentMovementUntil(-1);
}

void Ax12View::runCurrentMovementUntil(int positionIndex)
{
    QString group = ui->movementEditor->getCurrentGroup();
    QString mvt = ui->movementEditor->getCurrentMovement();

    Q_ASSERT(!group.isEmpty());
    Q_ASSERT(!mvt.isEmpty());

    saveMovements();
    _connection->getComm()->runAx12Movement(group, mvt, ui->spSpeedLimit->value(), positionIndex);
}

void Ax12View::moveToCurrentMovementPosition(int positionIndex)
{
    QString group = ui->movementEditor->getCurrentGroup();
    QString mvt = ui->movementEditor->getCurrentMovement();

    Q_ASSERT(!group.isEmpty());
    Q_ASSERT(!mvt.isEmpty());

    Tools::Ax12MovementManager::DetailedPosition pos = ui->movementEditor->getEditedMovements().getDetailedMovement(group, mvt).value(positionIndex);

    QMap<int, Tools::Ax12MovementManager::Ax12Position> ax12Pos = pos.first;

    QList<Comm::Ax12Info> ax12InfoList;

    for(QMap<int, Tools::Ax12MovementManager::Ax12Position>::const_iterator it = ax12Pos.constBegin(); it != ax12Pos.constEnd(); ++it)
    {
        Comm::Ax12Info info;
        info.id = it.key();
        info.angle = *it;
        info.speed = pos.second.maxSpeed;
        info.torque = pos.second.torque;

        ax12InfoList << info;
    }

    _connection->getComm()->moveAx12(ax12InfoList);
}

//----------------------------------------------------------------------

Ax12DisplayWidget::Ax12DisplayWidget(QWidget *parent) : QWidget(parent)
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	_spId = new QSpinBox(this);
	_spId->setMinimum(0);
	_spId->setMaximum(253);
	_spId->setEnabled(false);

	_spAngle = new QDoubleSpinBox(this);
	_spAngle->setSuffix(QString::fromUtf8("°"));
	_spAngle->setMinimum(0);
	_spAngle->setMaximum(300);
	_spSpeed = new QDoubleSpinBox(this);
	_spSpeed->setSuffix("rpm");
	_spSpeed->setMinimum(0);
	_spSpeed->setMaximum(114);
	_spTorque = new QDoubleSpinBox(this);
	_spTorque->setSuffix("%");
	_spTorque->setMinimum(0);
	_spTorque->setMaximum(100);

	_btnSend = new QPushButton("Send", this);
	connect(_btnSend, SIGNAL(clicked()), this, SIGNAL(send()));

	_btnIncrease = new QPushButton(" ^ ", this);
	_btnIncrease->setAutoRepeat(true);
	connect(_btnIncrease, SIGNAL(clicked()), this, SIGNAL(increase()));

	_btnDecrease = new QPushButton(" v ", this);
	_btnDecrease->setAutoRepeat(true);
	connect(_btnDecrease, SIGNAL(clicked()), this, SIGNAL(decrease()));

	QPushButton* btnRemove = new QPushButton(" - ", this);
	connect(btnRemove, SIGNAL(clicked()), this, SIGNAL(remove()));

	mainLayout->addStretch();
	QHBoxLayout* l = new QHBoxLayout;
	l->setSpacing(0);
	l->addWidget(new QLabel("ID:", this));
	l->addWidget(_spId);
	mainLayout->addLayout(l);

	l = new QHBoxLayout;
	l->setSpacing(0);
	l->addWidget(new QLabel("Angle:", this));
	l->addWidget(_spAngle);
	mainLayout->addLayout(l);

	l = new QHBoxLayout;
	l->setSpacing(0);
	l->addWidget(new QLabel("Speed:", this));
	l->addWidget(_spSpeed);
	mainLayout->addLayout(l);

	l = new QHBoxLayout;
	l->setSpacing(0);
	l->addWidget(new QLabel("Torque:", this));
	l->addWidget(_spTorque);
	mainLayout->addLayout(l);

	mainLayout->addWidget(_btnSend);
	mainLayout->addSpacing(10);
	mainLayout->addWidget(_btnIncrease);
	mainLayout->addWidget(_btnDecrease);
	mainLayout->addSpacing(10);
	mainLayout->addWidget(btnRemove);
	mainLayout->addStretch();
}

void Ax12DisplayWidget::setId(int id)
{
	_spId->setValue(id);
}

void Ax12DisplayWidget::setAngle(float angle)
{
	_spAngle->setValue(angle);
}

void Ax12DisplayWidget::setSpeed(float speed)
{
	_spSpeed->setValue(speed);
}

void Ax12DisplayWidget::setTorque(float torque)
{
	_spTorque->setValue(torque);
}

int Ax12DisplayWidget::getId() const
{
	return _spId->value();
}

float Ax12DisplayWidget::getAngle() const
{
	return _spAngle->value();
}

float Ax12DisplayWidget::getSpeed() const
{
	return _spSpeed->value();
}

float Ax12DisplayWidget::getTorque() const
{
	return _spTorque->value();
}

void Ax12DisplayWidget::setSendEnabled(bool value)
{
	_btnSend->setEnabled(value);
	_btnIncrease->setEnabled(value);
	_btnDecrease->setEnabled(value);
}
