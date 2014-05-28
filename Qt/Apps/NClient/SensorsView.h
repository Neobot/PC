#ifndef SENSORSVIEW_H
#define SENSORSVIEW_H

#include "AbstractView.h"

namespace Ui {
	class SensorsView;
}

class SensorsView : public AbstractView, public Comm::CommListener
{
	Q_OBJECT
	
public:
	explicit SensorsView(NetworkConnection *connection, QWidget *parent = 0);
	~SensorsView();
	
public slots:
	void connectionStatusChanged(NetworkConnection::ConnectionStatus status);

private:
	Ui::SensorsView *ui;
	int _nbAvoidingCurvesInitialized;
	unsigned int _timeAvoiding;

	QVector<double> _defaultTimeValues;

	QList<quint8> _lastAvoidingValues;
	QList<QColor> _colors;

	bool _isPaused;

	void saveSettings(QSettings *settings);
	void loadSettings(QSettings *settings);

	void avoidingSensors(const QList<quint8>& values);
	void sensorEvent(Comm::SensorType type, int sensorId, int value);

private slots:
	void clear();
	void pauseButtonClicked();
	void enableSensors();
	void disableSensors();

	QColor getCommColor(Comm::ColorState colorState) const;
};

#endif // SENSORSVIEW_H
