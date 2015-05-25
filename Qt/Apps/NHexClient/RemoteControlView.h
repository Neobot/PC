#ifndef REMOTECONTROLVIEW_H
#define REMOTECONTROLVIEW_H

#include "AbstractView.h"
#include "RPoint.h"

#include <QBoxLayout>
#include <QVector3D>
#include <QSignalMapper>
#include "Curve.h"

class QLabel;
class QToolButton;
class QComboBox;

namespace Ui {
	class RemoteControlView;
}

namespace Tools
{
	class DataMap;
	class LineObject;
	class PointEdit;
}

class TeleportDialog;

class TrajectoryPointWidget : public QWidget
{
	Q_OBJECT

public:
	explicit TrajectoryPointWidget(const QVector3D& defaultValue, QWidget *parent = 0);

	Tools::PointEdit* getPointEditor() const;
	void setNum(int i);
	void setCurrent(bool isCurrent);
	void disableEdition(bool value);


signals:
	void removeAsked();

private:
	Tools::PointEdit* _pointEditor;
	QLabel* _lbl;
	QToolButton* _btn;
};

class RemoteControlView : public AbstractView, public Comm::CommListener
{
	Q_OBJECT
	
public:
	explicit RemoteControlView(CommInterface* connection, QWidget *parent = 0);
	~RemoteControlView();

public slots:
	void connectionStatusChanged(bool connected);
	void activeStatusChanged(bool isActive);

private:
	Ui::RemoteControlView *ui;
	Tools::DataMap* _map;

	Tools::LineObject* _objectiveLine;

	enum CollisionGroup
	{
		Objective = 1,
		RobotGroup = 2000,
		OpponentGroup = 3

	};

	QList<TrajectoryPointWidget*> _trajectoryEditors;
	QSignalMapper* _trPointRemoveMapper;

	QMap<QString, Tools::Trajectory> _storedTrajectories;
	QSignalMapper* _storedTrajectoriesLoadMapper;

	bool _isMoving;
	int _currentObjectiveIndex;

	TeleportDialog* _teleportDialog;

	void saveSettings(QSettings *settings);
	void loadSettings(QSettings *settings);

	void coordinates(qint16 x, qint16 y, double theta, quint8 forward);
	void objective(qint16 x, qint16 y, double theta);
	void isArrived();

	void addPoint(const QVector3D &defaultValue = QVector3D());
	void clearTrajectory();
	void updateTrajectoryNumeratation();

	void refreshStoredTrajectoriesMenu();

private slots:
	void addTrajectoryPoint();
	void removeTrajectoryPoint(QWidget* trajectoryWidget);

	void stopRobot();
	void sendTrajectory();
	void teleport();

	void saveCurrentTrajectoryAs();
	void loadTrajectory(const QString& name);
	void manageTrajectories();
};

#endif // REMOTECONTROLVIEW_H
