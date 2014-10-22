#ifndef SCRIPTVIEW_H
#define SCRIPTVIEW_H

#include "AbstractView.h"
#include "NetworkClientCommListener.h"


namespace Ui {
	class ScriptView;
}

class ScriptView : public AbstractView, public NetworkClientCommListener
{
	Q_OBJECT

public:
	explicit ScriptView(NetworkConnection *connection, QWidget *parent = 0);
	~ScriptView();

public slots:
	void connectionStatusChanged(NetworkConnection::ConnectionStatus status);

private:
	Ui::ScriptView *ui;

	void loadSettings(QSettings *settings);
	void saveSettings(QSettings *settings);
};

#endif // SCRIPTVIEW_H
