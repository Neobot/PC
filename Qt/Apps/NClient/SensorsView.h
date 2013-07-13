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
	
private:
	Ui::SensorsView *ui;
	int _nbAvoidingCurvesInitialized;
	unsigned int _timeAvoiding;
	int _nbOtherCurvesInitialized;
	unsigned int _timeOther;

	QVector<double> _defaultTimeValues;

	QList<quint8> _lastAvoidingValues;
	QList<QColor> _colors;

	bool _isPaused;

	void saveSettings(QSettings *settings);
	void loadSettings(QSettings *settings);

	void avoidingSensors(const QList<quint8>& values);
	void otherSensors(const QList<quint8> &values);

private slots:
	void clear();
	void pauseButtonClicked();
};

#endif // SENSORSVIEW_H
