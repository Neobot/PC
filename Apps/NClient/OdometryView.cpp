#include "OdometryView.h"
#include "ui_OdometryView.h"

OdometryView::OdometryView(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(), parent), ui(new Ui::OdometryView),
	_robotOK(false), _objectiveOK(false), _time(0), _isPaused(false)
{
	ui->setupUi(this);

	_connection->registerRobotResponder(this);

	ui->xyDiagram->addCurve("Robot", QPen(Qt::red));
	ui->xDiagram->addCurve("Robot", QPen(Qt::red));
	ui->yDiagram->addCurve("Robot", QPen(Qt::red));
	ui->thetaDiagram->addCurve("Robot", QPen(Qt::red));

	QCPCurve* c = ui->xyDiagram->addCurve("Objective", QPen(Qt::blue));
	c->setScatterStyle(QCP::ssCircle);
	c->setScatterSize(6);
	c->setLineStyle(QCPCurve::lsNone);
	ui->xDiagram->addCurve("Objective", QPen(Qt::blue));
	ui->yDiagram->addCurve("Objective", QPen(Qt::blue));
	ui->thetaDiagram->addCurve("Objective", QPen(Qt::blue));

	ui->xyDiagram->setTitle("X/Y");
	ui->xDiagram->setTitle("X/t");
	ui->yDiagram->setTitle("Y/t");
	ui->thetaDiagram->setTitle(QString::fromUtf8("𝛳/t"));

	ui->xDiagram->setAxisValuesVisible(false, true);
	ui->yDiagram->setAxisValuesVisible(false, true);
	ui->thetaDiagram->setAxisValuesVisible(false, true);

	connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui->btnPause, SIGNAL(clicked()), this, SLOT(pauseButtonClicked()));
}

void OdometryView::saveSettings(QSettings *settings)
{
	settings->beginGroup("Odometry");

	settings->setValue("Paused", _isPaused);

	settings->endGroup();
}

void OdometryView::loadSettings(QSettings *settings)
{
	settings->beginGroup("Odometry");

	_isPaused = settings->value("Paused", false).toBool();
	ui->btnPause->setChecked(_isPaused);

	settings->endGroup();
}

void OdometryView::coordinates(qint16 x, qint16 y, double theta, quint8 forward)
{
	Q_UNUSED(forward);

	Tools::RPoint newPoint = Tools::RPoint(x, y, theta);
	if (!_isPaused && newPoint != _lastRobotPoint)
	{
		_lastRobotPoint = newPoint;

		if (_robotOK != _objectiveOK)
		{
			addValuesToPlots();
			_robotOK = false;
			_objectiveOK = false;
		}
		else if (!_robotOK)
		{
			_robotOK = true;
		}
		else
		{
			//_robotOK == _objectiveOK == false -> should never happend
			Q_ASSERT(false);
			_robotOK = false; //in case it happens, we try to recover from it
			_objectiveOK = false;
		}
	}
}

void OdometryView::objective(qint16 x, qint16 y, double theta)
{
	Tools::RPoint newPoint = Tools::RPoint(x, y, theta);
	if (!_isPaused && newPoint != _lastObjectivePoint)
	{
		_lastObjectivePoint = newPoint;

		if (_robotOK != _objectiveOK)
		{
			addValuesToPlots();
			_robotOK = false;
			_objectiveOK = false;
		}
		else if (!_objectiveOK)
		{
			_objectiveOK = true;
		}
		else
		{
			//_robotOK == _objectiveOK == false -> should never happend
			Q_ASSERT(false);
			_robotOK = false; //in case it happens, we try to recover from it
			_objectiveOK = false;
		}
	}
}

void OdometryView::addValuesToPlots()
{
	_time++;

	QList<double> xValues, yValues, xtValues, ytValues, thetatValues;
	xValues << _lastRobotPoint.getX() << _lastObjectivePoint.getX();
	yValues << _lastRobotPoint.getY() << _lastObjectivePoint.getY();
	xtValues << _lastRobotPoint.getX() << _lastObjectivePoint.getX();
	ytValues << _lastRobotPoint.getY() << _lastObjectivePoint.getY();
	thetatValues << _lastRobotPoint.getThetaDegree() << _lastObjectivePoint.getThetaDegree();

	ui->xyDiagram->addValues(xValues, yValues);
	ui->xDiagram->addValues(_time, xtValues);
	ui->yDiagram->addValues(_time, ytValues);
	ui->thetaDiagram->addValues(_time, thetatValues);
}

void OdometryView::clear()
{
	ui->xyDiagram->clear();
	ui->xDiagram->clear();
	ui->yDiagram->clear();
	ui->thetaDiagram->clear();
}

void OdometryView::pauseButtonClicked()
{
	_isPaused = !_isPaused;
	ui->btnPause->setText(_isPaused ? "Resume" : "Pause");
}
