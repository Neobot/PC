#ifndef SCRIPTVIEW_H
#define SCRIPTVIEW_H

#include "AbstractView.h"
#include "SignalDelayer.h"

namespace Ui {
	class ScriptView;
}

class ScriptView : public AbstractView
{
	Q_OBJECT

public:
	explicit ScriptView(NetworkConnection *connection, QWidget *parent = 0);
	~ScriptView();

public slots:
	void connectionStatusChanged(NetworkConnection::ConnectionStatus status);

	void check();
private:
	Ui::ScriptView *ui;
	Tools::SignalDelayer* _codeChangedSignalDelayer;
};

#endif // SCRIPTVIEW_H
