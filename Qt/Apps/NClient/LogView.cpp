#include "LogView.h"
#include "ui_LogView.h"

#include <QTextCursor>

LogView::LogView(NetworkConnection *connection, QWidget *parent) :
	AbstractView(connection, QImage(), parent),
	ui(new Ui::LogView), _nbLog(0)
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

void LogView::initReceived()
{
	logMessage("Info", "MicroC reset.", Qt::darkBlue);
}

void LogView::logMessage(const QString& message)
{
	*_logger << message << Tools::endl;
}

void LogView::logMessage(const QString& speaker, const QString& message, const QColor& color)
{
	if (ui->checkAutoClear->isChecked())
	{
		if (_nbLog > 200)
		{
			_logger->clear();
			_nbLog == 0;
		}

		++_nbLog;
	}

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
