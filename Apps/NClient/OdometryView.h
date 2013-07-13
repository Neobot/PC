#ifndef ODOMETRYVIEW_H
#define ODOMETRYVIEW_H

#include "AbstractView.h"
#include "RPoint.h"

namespace Ui {
	class OdometryView;
}

class OdometryView : public AbstractView, public Comm::CommListener
{
	Q_OBJECT
public:
	explicit OdometryView(NetworkConnection *connection, QWidget *parent = 0);

private:
	Ui::OdometryView *ui;

	bool _robotOK, _objectiveOK;
	Tools::RPoint _lastRobotPoint;
	Tools::RPoint _lastObjectivePoint;

	unsigned int _time;
	bool _isPaused;

	void saveSettings(QSettings *settings);
	void loadSettings(QSettings *settings);

	void coordinates(qint16 x, qint16 y, double theta, quint8 forward);
	void objective(qint16 x, qint16 y, double theta);
	
	void addValuesToPlots();

private slots:
	void clear();
	void pauseButtonClicked();
};

#endif // ODOMETRYVIEW_H
