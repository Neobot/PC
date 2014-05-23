#ifndef PARAMETERSVIEW_H
#define PARAMETERSVIEW_H

#include "AbstractView.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>

namespace Ui {
	class ParametersView;
}

class ParameterWidget : public QWidget
{
public:
	ParameterWidget(QWidget *parent = 0);

	void setValue(double value);
	void setName(const QString& name);
	double getValue() const;

private:
	QLabel* _lbl;
	QDoubleSpinBox* _spValue;
};

class ParametersView : public AbstractView, public Comm::CommListener, public NetworkClientCommListener
{
	Q_OBJECT
	
public:
	explicit ParametersView(NetworkConnection *connection, QWidget *parent = 0);
	~ParametersView();
	
public slots:
	void connectionStatusChanged(NetworkConnection::ConnectionStatus status);

	void parameters(const QList<float> &values);
	void parameterNames(const QStringList &names);

	void setStrategies(const QStringList &strategies);
	void setSerialPorts(const QStringList &ports);
	void autoStrategyInfo(bool enabled, int strategyNum, const QString &robotPort, const QString &ax12Port, bool simulation, bool mirror, int delayInSeconds);

private:
	Ui::ParametersView *ui;
	QList<ParameterWidget*> _params;

	int		_currentStrategyNum = -1;
	QString _currentAutoRobotPort;
	QString _currentAutoAx12Port;
	int		_currentAutoStartDelay = 20;

	void createParameterWidget();
	void updateAutoStrategyWidget();

private slots:
	void refreshParameters();
	void sendParameters();

	void editSendAutoStrategy();
	void refreshAutoStrategy();
};

#endif // PARAMETERSVIEW_H
