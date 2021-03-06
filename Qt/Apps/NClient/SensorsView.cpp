#include "SensorsView.h"
#include "ui_SensorsView.h"
#include "NetworkClientCommInterface.h"


const int PLOT_MAX_NB_VALUES = 1000;

SensorsView::SensorsView(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(), parent),
	ui(new Ui::SensorsView), _nbAvoidingCurvesInitialized(0), _timeAvoiding(PLOT_MAX_NB_VALUES - 1),
	_isPaused(false)
{
	ui->setupUi(this);

	ui->avoidingSensorsPlot->setDataNumberLimit(PLOT_MAX_NB_VALUES);
	ui->avoidingSensorsPlot->setAutoResize(false);
	ui->avoidingSensorsPlot->setAxisScale(0, 1000, 0, 255);
    //ui->avoidingSensorsPlot->legend->setPositionStyle(QCPLegend::psTopLeft);
	ui->avoidingSensorsPlot->setAxisValuesVisible(false, true);
	ui->avoidingSensorsPlot->setDefaultInteractionEnabled(false);

	ui->avoidingSensorsBars->setAutoResize(false);
	ui->avoidingSensorsBars->setAxisScale(0, 6, 0, 255);
	ui->avoidingSensorsBars->setAxisValuesVisible(false, true);
	ui->avoidingSensorsBars->setDefaultInteractionEnabled(false);

	_connection->registerRobotResponder(this);

	_colors << Qt::blue << Qt::red << Qt::green << Qt::yellow << Qt::darkBlue << Qt::darkRed << Qt::darkGreen << Qt::darkYellow;

	connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui->btnPause, SIGNAL(clicked()), this, SLOT(pauseButtonClicked()));
	connect(ui->btnEnableSensors, SIGNAL(clicked()), this, SLOT(enableSensors()));
	connect(ui->btnDisableSensors, SIGNAL(clicked()), this, SLOT(disableSensors()));
}

SensorsView::~SensorsView()
{
	delete ui;
}

void SensorsView::connectionStatusChanged(NetworkConnection::ConnectionStatus status)
{
	ui->btnEnableSensors->setEnabled(status == NetworkConnection::Controled);
	ui->btnDisableSensors->setEnabled(status == NetworkConnection::Controled);
}

void SensorsView::activatedSensors(const QList<quint8> &values)
{
	if (_nbAvoidingCurvesInitialized < values.count())
	{
		int nbExistingSensors = _nbAvoidingCurvesInitialized;
		_nbAvoidingCurvesInitialized = values.count();
		for(int i = nbExistingSensors; i < _nbAvoidingCurvesInitialized; ++i)
		{
			QColor c = _colors.value(i);
			ui->avoidingSensorsPlot->addCurve(QString("Sensor").append(QString::number(i)), QPen(c));
			ui->avoidingSensorsBars->addBar(QString("Sensor").append(QString::number(i)), QPen(Qt::black), QBrush(c));
		}
	}

	if (_isPaused)
		return;

	QList<double> doubleValues;
	foreach(quint8 value, values)
	{
		doubleValues << value;
	}

	ui->avoidingSensorsPlot->addValues(_timeAvoiding, doubleValues);
	if (isViewActive())
		ui->avoidingSensorsBars->setValues(doubleValues);

	_timeAvoiding++;
}

void SensorsView::sensorEvent(Comm::SensorType type, int sensorId, int value)
{
	switch(type)
	{
		case Comm::ColorSensor:
			ui->colorWidget->setColor(sensorId-1, getCommColor((Comm::ColorState)value));
			break;
		case Comm::SharpSensor: //TODO
		case Comm::MicroswitchSensor: //TODO
			break;
		case Comm::UnknownedSensor:
			break;
	}
}

void SensorsView::clear()
{
	ui->avoidingSensorsBars->clear();
	ui->avoidingSensorsPlot->clear();
}

void SensorsView::pauseButtonClicked()
{
	_isPaused = !_isPaused;
	ui->btnPause->setText(_isPaused ? "Resume" : "Pause");
}

void SensorsView::enableSensors()
{
	if (getConnection()->getConnectionStatus() == NetworkConnection::Controled)
	{
		_connection->getComm()->enableSensor(Comm::ColorSensor, 0);
		_connection->getComm()->enableSensor(Comm::MicroswitchSensor, 0);
		_connection->getComm()->enableSensor(Comm::SharpSensor, 0);
	}
}

void SensorsView::disableSensors()
{
	if (getConnection()->getConnectionStatus() == NetworkConnection::Controled)
	{
		_connection->getComm()->disableSensor(Comm::ColorSensor, 0);
		_connection->getComm()->disableSensor(Comm::MicroswitchSensor, 0);
		_connection->getComm()->disableSensor(Comm::SharpSensor, 0);
	}
}

QColor SensorsView::getCommColor(Comm::ColorState colorState) const
{
	switch(colorState)
	{
		case Comm::ColorGreen: return Qt::green;
		case Comm::ColorRed: return Qt::red;
		case Comm::ColorBlue: return Qt::blue;
		case Comm::ColorYellow: return Qt::yellow;
		case Comm::ColorWhite: return Qt::white;
		case Comm::ColorBlack:
		default:
			return Qt::black;
	}
}

void SensorsView::saveSettings(QSettings *settings)
{
	settings->beginGroup("Sensors");

	settings->setValue("Paused", _isPaused);

	settings->endGroup();
}

void SensorsView::loadSettings(QSettings *settings)
{
	settings->beginGroup("Sensors");

	_isPaused = settings->value("Paused", false).toBool();
	ui->btnPause->setChecked(_isPaused);

	settings->endGroup();
}
