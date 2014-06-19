#include "GraphsView.h"
#include "ui_GraphsView.h"
#include "NetworkClientCommInterface.h"
#include "NPlotWidget.h"
#include "ToolsLib.h"

const int PLOT_MAX_NB_VALUES = 1000;

const int NB_COLUMNS = 2;
const int NB_ROWS = 3;

GraphsView::GraphsView(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(), parent),
	ui(new Ui::GraphsView), _isPaused(false)
{
	ui->setupUi(this);
	ui->tabWidget->clear();

	_connection->registerRobotResponder(this);

	connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui->btnPause, SIGNAL(clicked()), this, SLOT(pauseButtonClicked()));
}

GraphsView::~GraphsView()
{
	delete ui;
}

void GraphsView::connectionStatusChanged(NetworkConnection::ConnectionStatus status)
{
	if (status == NetworkConnection::Controled)
		_connection->getComm()->askGraphs();
}

void GraphsView::clear()
{
	for(GraphTab* tab: _tabs)
	{
		tab->clearData();
	}
}

void GraphsView::pauseButtonClicked()
{
	_isPaused = !_isPaused;
	ui->btnPause->setText(_isPaused ? "Resume" : "Pause");
}

void GraphsView::saveSettings(QSettings *settings)
{
	settings->beginGroup("Graphs");

	settings->setValue("Paused", _isPaused);

	settings->endGroup();
}

void GraphsView::loadSettings(QSettings *settings)
{
	settings->beginGroup("Graphs");

	_isPaused = settings->value("Paused", false).toBool();
	ui->btnPause->setChecked(_isPaused);

	settings->endGroup();
}

void GraphsView::registerGraph(int graphId, Comm::GraphType type, const QString &name, const QStringList &parameterNames)
{
	Tools::NPlotWidget* graph = getPlotWidget(type, name, parameterNames);

	GraphTab* currentTab = getTabForGraphId(graphId);

	if (currentTab == nullptr)
	{
		if (_tabs.isEmpty() || _tabs.last()->isFull())
		{
			currentTab = new GraphTab(this);
			QString tabName = QString("Page ").append(QString::number(_tabs.count() + 1));
			ui->tabWidget->addTab(currentTab, tabName);
			_tabs << currentTab;
		}
		else
		{
			currentTab = _tabs.last();
		}
	}


	Q_ASSERT(currentTab);
	if (currentTab)
		currentTab->addGraph(graphId, graph);
}

void GraphsView::graphValues(int graphId, const QList<float> &values)
{
	if (!_isPaused)
	{
		GraphTab* currentTab = getTabForGraphId(graphId);
		if (currentTab)
			currentTab->addSample(graphId, values);
	}
}

void GraphsView::graphSingleValues(int graphId, int parameterId, float value)
{
	if (!_isPaused)
	{
		GraphTab* currentTab = getTabForGraphId(graphId);
		if (currentTab)
			currentTab->addSingleSample(graphId, parameterId, value);
	}
}

Tools::NPlotWidget *GraphsView::getPlotWidget(Comm::GraphType type, const QString &name, const QStringList &parameterNames)
{
	if (type == Comm::CurveGraph)
	{
		Tools::MultiCurveWidget* curveGraph = new Tools::MultiCurveWidget(this);

		curveGraph->setAxisValuesVisible(false, true);
		curveGraph->setTitle(name);

		for(const QString& param : parameterNames)
		{
			curveGraph->addCurve(param);
		}

		curveGraph->setDataNumberLimit(PLOT_MAX_NB_VALUES);

		curveGraph->rescale();

		return curveGraph;
	}
	else if (type == Comm::BarGraph)
	{
		Tools::MultiBarWidget* barGraph = new Tools::MultiBarWidget(this);

		barGraph->setAxisValuesVisible(false, true);
		barGraph->setTitle(name);

		for(const QString& param : parameterNames)
		{
			barGraph->addBar(param);
		}

		return barGraph;
	}

	return nullptr;
}

GraphTab *GraphsView::getTabForGraphId(int graphId)
{
	for(GraphTab* tab : _tabs)
	{
		if (tab->contains(graphId))
			return tab;
	}

	return nullptr;
}

//------------------------------------------------------------------------------------------

GraphTab::GraphTab(QWidget *parent) : QWidget(parent)
{
	_layout = new QGridLayout(this);
	setLayout(_layout);
}

void GraphTab::clear()
{
	for(Tools::NPlotWidget* w: _graphs)
		_layout->removeWidget(w);

	qDeleteAll(_graphs);
	_graphs.clear();
}

void GraphTab::clearData()
{
	for(Tools::NPlotWidget* w: _graphs)
		w->clear();
}

void GraphTab::addGraph(int id, Tools::NPlotWidget *widget)
{
	if (_graphs.contains(id))
	{
		Tools::NPlotWidget* existingWidget = _graphs[id];
		_layout->replaceWidget(existingWidget, widget, Qt::FindDirectChildrenOnly);
		_graphs[id] = widget;
		_layout->invalidate();

		delete existingWidget;
	}
	else if (!isFull())
	{
		int num = _graphs.count();
		int x = num % NB_ROWS;
		int y = floor((double)num / (double)NB_ROWS);

		_layout->addWidget(widget, x, y);
		_graphs[id] = widget;
	}
}

bool GraphTab::isFull() const
{
	return _graphs.count() >= NB_COLUMNS * NB_ROWS;
}

bool GraphTab::contains(int id) const
{
	return _graphs.contains(id);
}

void GraphTab::addSample(int id, const QList<float> &values)
{
	Tools::NPlotWidget* plot = _graphs[id];
	QList<double> dValues;
	for(float f: values)
		dValues << f;

	plot->addTimeSample(dValues);
}

void GraphTab::addSingleSample(int id, int paramId, float value)
{
	Tools::NPlotWidget* plot = _graphs[id];
	plot->addSingleTimeSample(paramId, value);
}
