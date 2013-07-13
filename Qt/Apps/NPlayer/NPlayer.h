#ifndef NPLAYER_H
#define NPLAYER_H

#include "ui_NPlayer.h"
#include "ChessUpConfigurationSelector.h"
#include <QTime>

class Simulator;
class StrategyManager;
class StrategyInterface;
class SimProtocol;
class NMicropather;

namespace Comm
{
    class RobotCommInterface;
    class AX12CommManager;
}

namespace Tools
{
    class PlainTextEditLogger;
}

class NPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit NPlayer(QWidget *parent = 0);
    ~NPlayer();

    void startPlaying(const QString& filename);
    void stopPlaying();

private:
    Ui::NPlayer ui;
    ChessUpConfigurationSelector* _configSelector;

    Simulator* _simulator;
    StrategyManager* _manager;
    StrategyInterface* _strategy;
    SimProtocol* _protocol;
    Comm::AX12CommManager* _ax12Manager;
    Comm::RobotCommInterface* _robot;
    NMicropather* _pather;
    Tools::PlainTextEditLogger* _logger;

    QButtonGroup* _viewModeButtonGroup;
    QMenu* _viewContextMenu;
    QPoint _lastContextMenuPos; //in widget coordinates of the view
    QActionGroup* _modeActionGroup;

    void initActions();
    void addColorToCombobox(const QColor& color, QComboBox* combo) const;
    void customizeView();

    bool _playing;
    QDataStream* _player;
    QFile* _playedFile;

    QTimer _playTimer;
    QTimer _matchTimer;
    QTime _playTime;
    double _playSpeed;

	void closeEvent(QCloseEvent *);
	void saveSettings();
	void loadSettings();

private slots:
    void newConfiguration();
    void newRandomConfiguration();
    void newEmptyConfiguration();

    void on_btnClearLog_clicked();
    void on_btnSaveLogAs_clicked();

    void run(bool value);
    void stop();
    void record(bool value);

    void on_btnBlock_clicked();

    void speedChanged(double value);
    void setDebugMode();
    void viewModeChanged();

    void setConfigWidgetEnabled(bool value);
    void strategyChanged();
    void colorChanged();

    void viewContextMenuRequested(const QPoint & pos);

    void setOpponentHere();
    void moveOpponentHere();

    void play();
    void openReplay();
    void refreshMatchTime();
    void replaySpeedChanged(int value);
};

#endif // NPLAYER_H
