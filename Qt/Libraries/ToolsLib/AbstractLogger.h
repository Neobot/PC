#ifndef ABSTRACTLOGGER_H
#define ABSTRACTLOGGER_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QPointF>

class QPlainTextEdit;


namespace Tools
{
    /**
     * \brief The base interface for logging management.
     *
     * You can ihnerit from AbstractLogger to implements your own logger or you can use one of the predefined logger.
     *
     * \see LoggerInterface
     * \see StdoutLogger
     * \see QDebugLogger
     * \see MultipleLogger
     */
    class AbstractLogger
    {
    public:
        AbstractLogger();
        virtual ~AbstractLogger() {}

        /**
         * \brief Log a text.
         */
        virtual void log(const char* text) = 0;
        /**
         * \brief Log a single char.
         */
        virtual void log(char text) = 0;
        /**
         * \brief Clear the text.
         */
        virtual void clear() = 0;

        /**
         * \brief Make the logger quiet i.e do not log anything.
         */
        virtual void noLogger(bool value) = 0;

        quint8 getBase();
        void setBase(quint8 base);
        bool charsDisplayedAsNumbers();
        void displayCharsAsNumbers(bool displayCharsAsNumbers);

    protected:
        quint8 _base; // base for print numbers
        bool _displayCharsAsNumbers; // base for print numbers
    };

    const char endl = '\n';

    AbstractLogger& operator<<(AbstractLogger& logger, const QString& text);
    AbstractLogger& operator<<(AbstractLogger& logger, const QByteArray& text);
    AbstractLogger& operator<<(AbstractLogger& logger, const QPointF& p);
    AbstractLogger& operator<<(AbstractLogger& logger, const char* text);
    AbstractLogger& operator<<(AbstractLogger& logger, char text);
    AbstractLogger& operator<<(AbstractLogger& logger, unsigned int n);
    AbstractLogger& operator<<(AbstractLogger& logger, long n);
    AbstractLogger& operator<<(AbstractLogger& logger, int n);
    AbstractLogger& operator<<(AbstractLogger& logger, double n);

    /**
     * \brief Log text messages into stdout.
     */
    class StdoutLogger : public AbstractLogger
    {
    public:
        StdoutLogger();
        void log(const char* text);
        void log(char text);
        void noLogger(bool value) {_quiet = value;}

        /**
         * \brief Do nothing.
         */
        void clear() {}
    private:
        bool _quiet;
    };

    /**
     * \brief No logging.
     */
    class NullLogger : public AbstractLogger
    {
    public:
        NullLogger() {}
        void log(const char* text) {Q_UNUSED(text);}
        void log(char text) {Q_UNUSED(text);}
        void noLogger(bool value) {Q_UNUSED(value);}
        void clear() {}
    private:
    };


    /**
     * \brief Log text messages using qDebug().
     */
    class QDebugLogger : public  AbstractLogger
    {
    public:
        QDebugLogger();
        void log(const char* text);
        void log(char text);
        void noLogger(bool value) {_quiet = value;}

        /**
         * \brief Do nothing.
         */
        void clear() {}
    private:
        bool _quiet;
    };

    /**
     * \brief Log text messages in a file().
     */
    class FileLogger : public  AbstractLogger
    {
    public:
        FileLogger(const QString& filename);
        ~FileLogger();
        void log(const char* text);
        void log(char text);
        void noLogger(bool value) {_quiet = value;}

        bool isOpen() const;
        bool flush();
        /**
         * \brief Do nothing.
         */
        void clear() {}
    private:
        bool _quiet;
        QFile file;
    };

    /**
     * \brief Log text messages in multiple loggers.
     */
    class MultipleLogger : public AbstractLogger
    {
    public:
        MultipleLogger();
        void log(const char* text);
        void log(char text);
        void clear();
        void noLogger(bool value) {_quiet = value;}

        void addLogger(AbstractLogger* logger);
    private:
        QVector<AbstractLogger*> _loggers;
        bool _quiet;
    };

    /**
     * \brief Easy access logging feature.
     *
     * This class helps for implementing.
     * In order to get a logger in your own class, simply inherit it from LoggerInterface.
     */
    class LoggerInterface
    {
    public:
        LoggerInterface(AbstractLogger* logger = 0);

        /**
         * \brief If \p value is True, the logger won't write anything.
         */
        void beQuiet(bool value);

        void setLogEnabled(bool value);

    protected:
        /**
         * \brief Get the logger.
         *
         * Use this fonction to log something.
         * \example logger() << "hello world! :" << 42 << endl;
         */
        AbstractLogger& logger() const;

        /**
         * \brief Set the logger.
         */
        void setLogger(AbstractLogger* logger);

    private:
        AbstractLogger* _logger;
        bool _logEnabled;

        static NullLogger* _nullLogger;
        static NullLogger* getNullLogger();
    };
}

#endif // ABSTRACTLOGGER_H
