#ifndef GraphsView_H
#define GraphsView_H

#include "AbstractView.h"

#include <QGridLayout>

namespace Ui {
	class GraphsView;
}

namespace Tools
{
	class NPlotWidget;
}


class GraphTab : public QWidget
{
public:
	GraphTab(QWidget *parent = 0);

	void clear();
	void clearData();
	void addGraph(int id, Tools::NPlotWidget* widget);
	bool isFull() const;
	bool contains(int id) const;

	void addSample(int id, const QList<float>& values);
	void addSingleSample(int id, int paramId, float value);

private:
	QHash<int, Tools::NPlotWidget*> _graphs;
	QGridLayout* _layout;
};

class GraphsView : public AbstractView, public Comm::CommListener
{
	Q_OBJECT
	
public:
	explicit GraphsView(NetworkConnection *connection, QWidget *parent = 0);
	~GraphsView();
	
public slots:
	void connectionStatusChanged(NetworkConnection::ConnectionStatus status);

private:
	Ui::GraphsView *ui;
	QList<GraphTab*> _tabs;

	bool _isPaused;

	void saveSettings(QSettings *settings);
	void loadSettings(QSettings *settings);

	void registerGraph(int graphId, Comm::GraphType type, const QString& name, const QStringList& parameterNames);
	void graphValues(int graphId, const QList<float>& values);
	void graphSingleValues(int graphId, int parameterId, float value);

	Tools::NPlotWidget* getPlotWidget(Comm::GraphType type, const QString& name, const QStringList& parameterNames);
	GraphTab* getTabForGraphId(int graphId);

private slots:
	void clear();
	void pauseButtonClicked();
};

#endif // GraphsView_H
