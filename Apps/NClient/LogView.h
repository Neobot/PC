#ifndef LOGVIEW_H
#define LOGVIEW_H

#include "AbstractView.h"
#include "TextEditLogger.h"

namespace Ui {
	class LogView;
}

class LogView : public AbstractView, public NetworkClientCommListener, public Comm::CommListener
{
	Q_OBJECT
	
public:
	explicit LogView(NetworkConnection *connection, QWidget *parent = 0);
	~LogView();


	void logMessage(const QString& message);
	void logMessage(const QString& speaker, const QString& message, const QColor& color);

public slots:
	void connectionStatusChanged(NetworkConnection::ConnectionStatus status);

private:
	Ui::LogView *ui;
	Tools::TextEditLogger* _logger;

	QString formatText(const QString &text, const QColor &color);

	void serverAnnouncement(const QByteArray& message);
	void log(const QByteArray& text);

private slots:
	void clearLog();
};

#endif // LOGVIEW_H
