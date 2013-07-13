#ifndef COMMANDVIEW_H
#define COMMANDVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QSignalMapper>
#include <QVector3D>
#include <QMap>
#include <QSettings>

class QLabel;

namespace Ui {
    class CommandView;
}

namespace Comm
{
	class RobotCommInterface;
}


class CommandView : public QWidget
{
    Q_OBJECT
    
public:
    explicit CommandView(QWidget *parent = 0);
    ~CommandView();

	void setRobot(Comm::RobotCommInterface* robot);

	void saveSettings(QSettings& settings);
	void loadSettings(QSettings& settings);
    
private:
    Ui::CommandView *ui;
	Comm::RobotCommInterface* _robot;


	class Macro
	{
	public:
		Macro() : _type(0), _isSet(false) {}

		void set(const QVector3D& p, int type, int cbType, bool stop) {_point = p; _type = type; _cbType = cbType; _isSet = true; _stopPoint = stop;}
		void clear() {_isSet = false;}
		const QVector3D& getPoint() const {return _point;}
		int getType() const {return _type;}
		int getCbType() const {return _cbType;}
		bool isStopPoint() const {return _stopPoint;}
		bool isSet() const {return _isSet;}

	private:
		QVector3D _point;
		int _type;
		int _cbType;
		bool _stopPoint;
		bool _isSet;
	};

	QSignalMapper* _macroSaveMapper;
	QSignalMapper* _macroSendMapper;
	QSignalMapper* _macroClearMapper;
	QSignalMapper* _macroTeleportMapper;

	QMap<int, Macro> _macros;
	QMap<int, QLabel*> _macroLabels;
	QMap<int, QWidget*> _macroFrames;

	void updateMacros();
	int getMovementType() const;
	void addMacro(int i, QPushButton* btnSend, QPushButton* btnTeleport, QPushButton *btnClear, QAction *saveAction);

private slots:
	void macroSend(int i);
	void macroTeleport(int i);
	void saveMacro(int i);
	void macroClear(int i);

	void send();
	void teleport();
	void flush();
};

#endif // COMMANDVIEW_H
