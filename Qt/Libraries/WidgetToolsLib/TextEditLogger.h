#ifndef TEXTEDITLOGGER_H
#define TEXTEDITLOGGER_H

#include "AbstractLogger.h"

#include <QTextEdit>
#include <QPlainTextEdit>

namespace Tools
{
	class TextEditLogger : public Tools::AbstractLogger
	{

	public:
		TextEditLogger(QTextEdit* textEdit);

		void log(const char* text);
		void log(char text);
		void clear();
		void noLogger(bool value) {_quiet = value;}

		void setRichText(bool value);

	private:
		QTextEdit* _textEdit;
		bool _quiet;
		bool _richText;
	};

	/**
	 * \brief Log text messages in a file().
	 */
	class PlainTextEditLogger : public  AbstractLogger
	{
	public:
		PlainTextEditLogger(QPlainTextEdit* textEdit);
		~PlainTextEditLogger();
		void log(const char* text);
		void log(char text);
		void noLogger(bool value) {_quiet = value;}
		void clear();

	private:
		bool _quiet;
		QPlainTextEdit* _textEdit;
	};
}
#endif // TEXTEDITLOGGER_H
