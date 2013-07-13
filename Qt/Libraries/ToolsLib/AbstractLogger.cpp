#include "AbstractLogger.h"

#include <QFile>
#include <QTextStream>
#include <QtDebug>
#include <QPlainTextEdit>

using namespace Tools;

AbstractLogger::AbstractLogger() : _base(10), _displayCharsAsNumbers(true) {}

quint8 AbstractLogger::getBase() { return _base; }
void AbstractLogger::setBase(quint8 base) { _base = base; }
bool AbstractLogger::charsDisplayedAsNumbers() { return _displayCharsAsNumbers; }
void AbstractLogger::displayCharsAsNumbers(bool displayCharsAsNumbers) { _displayCharsAsNumbers = displayCharsAsNumbers; }

AbstractLogger& Tools::operator<<(AbstractLogger& logger, const QString& text)
{
    logger.log(text.toAscii().data());
    return logger;
}

AbstractLogger& Tools::operator<<(AbstractLogger& logger, const QByteArray& text)
{
    logger.log(text.data());
    return logger;
}

AbstractLogger& Tools::operator<<(AbstractLogger& logger, const QPointF& p)
{
    logger << "QPointF(" << p.x() << "," << p.y() << ")";
    return logger;
}

AbstractLogger& Tools::operator<<(AbstractLogger& logger, const char* text)
{
    logger.log(text);
    return logger;
}

AbstractLogger& Tools::operator<<(AbstractLogger& logger, char text)
{
	logger.log(text);
    return logger;
}

AbstractLogger& Tools::operator<<(AbstractLogger& logger, long n)
{
	logger << QString().setNum(n, logger.getBase());
	return logger;
}

AbstractLogger& Tools::operator<<(AbstractLogger& logger, int n)
{
	if (logger.charsDisplayedAsNumbers())
		logger << QString().setNum(n, logger.getBase());
	else
		logger.log(n);

    return logger;
}

AbstractLogger& Tools::operator<<(AbstractLogger& logger, unsigned int n)
{
	logger << QString().setNum(n, logger.getBase());
    return logger;
}

AbstractLogger& Tools::operator<<(AbstractLogger& logger, double n)
{
   logger << QString().setNum(n);
   return logger;
}

//-------------------------StdoutLogger------------------------

StdoutLogger::StdoutLogger() : _quiet(false) {}

void StdoutLogger::log(const char* text)
{
    if (_quiet)
        return;

    QFile info;
    info.open(stdout, QIODevice::WriteOnly);
    QTextStream out(&info);

    out << text;
}

void StdoutLogger::log(char text)
{
    if (_quiet)
        return;

    QFile info;
    info.open(stdout, QIODevice::WriteOnly);
    QTextStream out(&info);

    out << text;
}

//-------------------------QDebugLogger------------------------

QDebugLogger::QDebugLogger() : _quiet(false) {}

void QDebugLogger::log(const char* text)
{
    if (_quiet)
        return;

    qDebug() << text;
}

void QDebugLogger::log(char text)
{
    if (_quiet)
        return;

    qDebug() << text;
}

//-------------------------FileLogger------------------------

FileLogger::FileLogger(const QString& filename)
    : _quiet(false), file(filename)
{
    file.open(QIODevice::WriteOnly | QIODevice::Text);
}

FileLogger::~FileLogger()
{
    flush();
}

void FileLogger::log(const char* text)
{
    if (_quiet || !isOpen())
	return;

    file.write(text);
}

void FileLogger::log(char text)
{
    if (_quiet || !file.isOpen())
	return;

    QByteArray a;
    a.append(text);
    file.write(a);
}

bool FileLogger::isOpen() const
{
    return file.isOpen();
}

bool FileLogger::flush()
{
    return file.flush();
}

//-------------------------MultipleLogger------------------------

MultipleLogger::MultipleLogger() : _quiet(false) {}

void MultipleLogger::addLogger(AbstractLogger* logger)
{
    _loggers.append(logger);
}

void MultipleLogger::log(const char* text)
{
    if (_quiet)
        return;

    foreach(AbstractLogger* logger, _loggers)
        logger->log(text);
}

void MultipleLogger::log(char text)
{
    if (_quiet)
        return;

    foreach(AbstractLogger* logger, _loggers)
        logger->log(text);
}

void MultipleLogger::clear()
{
    foreach(AbstractLogger* logger, _loggers)
        logger->clear();
}

//-------------------------LoggerInterface------------------------

NullLogger* LoggerInterface::_nullLogger = 0;

LoggerInterface::LoggerInterface(AbstractLogger* logger)
{
    _logEnabled = true;
    setLogger(logger);
    beQuiet(false);
}

AbstractLogger& LoggerInterface::logger() const
{
    if (!_logger || !_logEnabled)
	return *getNullLogger();

    return *_logger;
}

void LoggerInterface::setLogger(AbstractLogger* logger)
{
    _logger = logger;
}

void LoggerInterface::beQuiet(bool value)
{
    if (_logger)
        _logger->noLogger(value);
}

void LoggerInterface::setLogEnabled(bool value)
{
    _logEnabled = value;
}

NullLogger* LoggerInterface::getNullLogger()
{
    if (!_nullLogger)
	_nullLogger = new NullLogger;

    return _nullLogger;
}
