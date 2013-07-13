#include "LogView.h"
#include "ui_LogView.h"

#include <QTextCursor>

LogView::LogView(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(), parent),
	ui(new Ui::LogView)
{
	ui->setupUi(this);
	_logger = new Tools::TextEditLogger(ui->textEdit);
	_logger->setRichText(true);

	connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(clearLog()));

	_connection->registerNetworkResponder(this);
	_connection->registerRobotResponder(this);
}

LogView::~LogView()
{
	delete _logger;
	delete ui;
}

void LogView::logMessage(const QString& message)
{
	*_logger << message << Tools::endl;
}

void LogView::logMessage(const QString& speaker, const QString& message, const QColor& color)
{
	*_logger << speaker << ": \"" << formatText(message, color) << "\"" << Tools::endl;
}

void LogView::connectionStatusChanged(NetworkConnection::ConnectionStatus status)
{
	Q_UNUSED(status);
}

void LogView::serverAnnouncement(const QByteArray &message)
{
	logMessage("Server", message, Qt::red);
}

void LogView::log(const QByteArray& text)
{
	logMessage("Robot", text, Qt::darkGreen);
}

void LogView::clearLog()
{
	_logger->clear();
}

QString LogView::formatText(const QString& text, const QColor& color)
{
	return QString("<text style=""color:%1;"">%2</text>").arg(color.name()).arg(text);
}
