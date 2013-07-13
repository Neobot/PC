#ifndef NACQUISITOR_H
#define NACQUISITOR_H

#include <QMainWindow>
#include <QBoxLayout>
#include <QtGlobal>
#include <QSignalMapper>
#include <QSettings>

#include "CommListener.h"
#include "Acquisition.h"

class QCheckBox;
class QComboBox;
class ViewWidget;
class ViewInterface;

namespace Ui {
    class NAcquisitor;
}

class Simulator;
class SimProtocol;
class QextSerialPort;
class CommandView;
class LogView;

namespace Comm
{
	class RobotCommInterface;
	class AX12CommManager;
	class AbstractProtocol;
}


class NAcquisitor : public QMainWindow, public Comm::CommListener
{
    Q_OBJECT
    
public:
    explicit NAcquisitor(QWidget *parent = 0);
    ~NAcquisitor();

	enum DefinedView {XY, X, Y, THETA, RAW, SHARPS, RAW_SHARPS, COMMANDS, EMPTY};

private:
    Ui::NAcquisitor *ui;

	QList<QDockWidget*> _docks;
	Simulator* _simulator;

	QIODevice* _device;
	Comm::AbstractProtocol* _protocol;
	Comm::AX12CommManager* _ax12Manager;
	Comm::RobotCommInterface* _robot;

	QCheckBox* _checkSimulation;
	QComboBox* _cbPortName;
	QAction* _actionPlay;
	QAction* _actionStop;

	Acquisition _acquisition; //data

	CommandView* _command;
	LogView*	_log;

	QSettings* _settings;
	enum { MaxRecentFiles = 5 };
	QAction *recentFileActs[MaxRecentFiles];
	QAction* recentFileListSeparatorAction;
	QStringList _recentFileList;
	QSignalMapper* _recentFileActionsMapper;
	QString _currentFileName;
	bool _hasModifications;

	QList<QAction*> _notInRunActions; //actions to disable durring a run

	void open(const QString &file);
	void addToRecentFileList(const QString &file);
	void updateRecentFileActions();

	void updateTitle();

	void initDocks();
	void initActions();
	void initConnection(bool simulation);
	void deleteConnection();

	void coordinates(qint16 x, qint16 y, double theta, bool forward);
	void objective(qint16 x, qint16 y, double theta);
	void avoidingSensors(const QList<quint8>& values);
	bool microswitch(const quint8 values);
	void otherSensors(const QList<quint8>& values);
	bool initDone();
	bool go(bool mirrored);
	bool pingReceived();
	void noticeOfReceipt(quint8 instruction, bool result);
	void opponentPosition(qint16 x, qint16 y);
	bool restart();
	void quit();
	void isArrived();
	void isBlocked();
	void log(const QByteArray& text);

	void closeEvent(QCloseEvent *);
	void saveSettings();
	void loadSettings();

private slots:
	void play();
	void stop();
	void switchSimulationMode();

	void newAcquisition();
	void open();
	void openRecentFile(int index);
	void save();
	void saveAs();
};

#endif // NACQUISITOR_H
