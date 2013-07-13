#include "CommandView.h"
#include "ui_CommandView.h"

#include "RobotCommInterface.h"
#include "ToolsLib.h"

#include <QAction>
#include <QMenu>
#include <QLabel>

CommandView::CommandView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommandView)
{
    ui->setupUi(this);

	_macroSaveMapper = new QSignalMapper(this);
	connect(_macroSaveMapper, SIGNAL(mapped(int)), this, SLOT(saveMacro(int)));
	_macroSendMapper = new QSignalMapper(this);
	connect(_macroSendMapper, SIGNAL(mapped(int)), this, SLOT(macroSend(int)));
	_macroTeleportMapper = new QSignalMapper(this);
	connect(_macroTeleportMapper, SIGNAL(mapped(int)), this, SLOT(macroTeleport(int)));
	_macroClearMapper = new QSignalMapper(this);
	connect(_macroClearMapper, SIGNAL(mapped(int)), this, SLOT(macroClear(int)));


	ui->pointEditor->setPrefixes("x=", "y=", QString::fromUtf8("𝛳="));
	ui->pointEditor->setSuffixes("mm", "mm", QString::fromUtf8("°"));
	ui->pointEditor->setSeparator("   ");

	ui->btnSaveAs->setMenu(new QMenu(this));
	addMacro(1, ui->btnMGo1, ui->btnMTel1, ui->btnClear1, ui->btnSaveAs->menu()->addAction("Macro 1"));
	addMacro(2, ui->btnMGo2, ui->btnMTel2, ui->btnClear2, ui->btnSaveAs->menu()->addAction("Macro 2"));
	addMacro(3, ui->btnMGo3, ui->btnMTel3, ui->btnClear3, ui->btnSaveAs->menu()->addAction("Macro 3"));
	addMacro(4, ui->btnMGo4, ui->btnMTel4, ui->btnClear4, ui->btnSaveAs->menu()->addAction("Macro 4"));
	addMacro(5, ui->btnMGo5, ui->btnMTel5, ui->btnClear5, ui->btnSaveAs->menu()->addAction("Macro 5"));

	connect(ui->sendGoto, SIGNAL(clicked()), this, SLOT(send()));
	connect(ui->sendTeleport, SIGNAL(clicked()), this, SLOT(teleport()));
	connect(ui->sendFlush, SIGNAL(clicked()), this, SLOT(flush()));

	_robot = 0;

	_macroLabels[1] = ui->macroLabel1;
	_macroLabels[2] = ui->macroLabel2;
	_macroLabels[3] = ui->macroLabel3;
	_macroLabels[4] = ui->macroLabel4;
	_macroLabels[5] = ui->macroLabel5;

	_macroFrames[1] = ui->macroFrame1;
	_macroFrames[2] = ui->macroFrame2;
	_macroFrames[3] = ui->macroFrame3;
	_macroFrames[4] = ui->macroFrame4;
	_macroFrames[5] = ui->macroFrame5;

	updateMacros();
}

CommandView::~CommandView()
{
	delete ui;
}

void CommandView::setRobot(Comm::RobotCommInterface *robot)
{
	_robot = robot;
}

void CommandView::saveSettings(QSettings &settings)
{
	settings.beginGroup("CommandView");

	settings.setValue("point", ui->pointEditor->getPoint());
	settings.setValue("type", ui->cbType->currentIndex());
	settings.setValue("isStopPoint", ui->checkStopPoint->isChecked());

	settings.beginWriteArray("macro", _macros.count());
	int i = 0;
	for(QMap<int, Macro>::const_iterator it = _macros.constBegin(); it != _macros.constEnd(); ++it)
	{
		const Macro& m = *it;

		settings.setArrayIndex(i++);
		settings.setValue("index", it.key());
		settings.setValue("point", m.getPoint());
		settings.setValue("type", m.getType());
		settings.setValue("cbtype", m.getCbType());
		settings.setValue("isStopPoint", m.isStopPoint());
		settings.setValue("isSet", m.isSet());
	}

	settings.endArray();

	settings.endGroup();
}

void CommandView::loadSettings(QSettings &settings)
{
	settings.beginGroup("CommandView");

	ui->pointEditor->setPoint(qvariant_cast<QVector3D>(settings.value("point")));
	ui->cbType->setCurrentIndex(settings.value("type").toInt());
	ui->checkStopPoint->setChecked(settings.value("isStopPoint").toBool());

	int size = settings.beginReadArray("macro");
	for(int i = 0; i < size; ++i)
	{
		settings.setArrayIndex(i);
		int index = settings.value("index").toInt();
		QVector3D p = qvariant_cast<QVector3D>(settings.value("point"));
		int type = settings.value("type").toInt();
		int cbType = settings.value("cbtype").toInt();
		bool isStopPoint = settings.value("isStopPoint").toBool();
		bool isSet = settings.value("isSet").toBool();

		if (!isSet)
			_macros[index] = Macro();
		else
		{
			Macro m;
			m.set(p, type, cbType, isStopPoint);
			_macros[index] = m;
		}
	}
	updateMacros();
	settings.endArray();

	settings.endGroup();
}

void CommandView::updateMacros()
{
	for(QMap<int, QLabel*>::iterator it = _macroLabels.begin(); it != _macroLabels.end(); ++it)
	{
		const Macro& m = _macros.value(it.key());
		QLabel* l = *it;
		QFont f = l->font();

		if (m.isSet())
		{
			QString text;
			text += "(";
			text += QString::number(m.getPoint().x());
			text += ",";
			text += QString::number(m.getPoint().y());
			text += ",";
			text += QString::number(m.getPoint().z());
			text += ")\n";
			text += ui->cbType->itemText(m.getCbType());
			if (m.isStopPoint())
				text += " - stop";

			l->setText(text);
			f.setPointSize(8);
		}
		else
		{
			f.setPointSize(10);
			l->setText(QString("Macro").append(QString::number(it.key())));
		}

		l->setFont(f);

		QWidget* frame = _macroFrames[it.key()];
		frame->setEnabled(m.isSet());
	}
}

void CommandView::addMacro(int i, QPushButton *btnSend, QPushButton *btnTeleport, QPushButton* btnClear, QAction* saveAction)
{
	_macroSendMapper->setMapping(btnSend, i);
	connect(btnSend, SIGNAL(clicked()), _macroSendMapper, SLOT(map()));

	_macroTeleportMapper->setMapping(btnTeleport, i);
	connect(btnTeleport, SIGNAL(clicked()), _macroTeleportMapper, SLOT(map()));

	_macroClearMapper->setMapping(btnClear, i);
	connect(btnClear, SIGNAL(clicked()), _macroClearMapper, SLOT(map()));

	_macroSaveMapper->setMapping(saveAction, i);
	connect(saveAction, SIGNAL(triggered()), _macroSaveMapper, SLOT(map()));
}

void CommandView::macroSend(int i)
{
	const Macro& m = _macros[i];
	if (_robot && m.isSet())
	{
		const QVector3D& d = m.getPoint();
        Tools::RMovement mvt(Tools::RPoint(d.x(), d.y(), Tools::degreeToRadian(d.z())), m.getType(), 0, m.isStopPoint(), 100);
		_robot->setDestination(mvt);
	}
}

void CommandView::macroTeleport(int i)
{
	const Macro& m = _macros[i];
	if (_robot && m.isSet())
	{
		const QVector3D& d = m.getPoint();
		_robot->setPosition(d.x(), d.y(), Tools::degreeToRadian(d.z()));
	}
}

void CommandView::saveMacro(int i)
{
	Macro& m = _macros[i];
	m.set(ui->pointEditor->getPoint(), getMovementType(), ui->cbType->currentIndex(), ui->checkStopPoint->isChecked());
	updateMacros();
}

void CommandView::macroClear(int i)
{
	_macros.remove(i);
	updateMacros();
}

void CommandView::send()
{
	if (_robot)
	{
		QVector3D d = ui->pointEditor->getPoint();
        Tools::RMovement mvt(Tools::RPoint(d.x(), d.y(), Tools::degreeToRadian(d.z())), getMovementType(), 0, ui->checkStopPoint->isChecked(), 100);
		_robot->setDestination(mvt);
	}
}

void CommandView::teleport()
{
	if (_robot)
	{
		QVector3D d = ui->pointEditor->getPoint();
		_robot->setPosition(d.x(), d.y(), Tools::degreeToRadian(d.z()));
	}
}

void CommandView::flush()
{
	if (_robot)
		_robot->flush();
}


int CommandView::getMovementType() const
{
	switch(ui->cbType->currentIndex())
	{
		case 0:
		default:
			return Tools::AUTO;
		case 1:
			return Tools::Forward;
		case 2:
			return Tools::Backward;
	}
}
