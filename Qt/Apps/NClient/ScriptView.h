#ifndef SCRIPTVIEW_H
#define SCRIPTVIEW_H

#include "AbstractView.h"

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

private:
	Ui::ScriptView *ui;

};

#endif // SCRIPTVIEW_H
