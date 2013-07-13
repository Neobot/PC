#include "TextEditLogger.h"
#include <QtDebug>

using namespace Tools;

TextEditLogger::TextEditLogger(QTextEdit* textEdit) : _textEdit(textEdit), _quiet(false), _richText(false) {}

void TextEditLogger::log(const char* text)
{
	if (_quiet)
		return;

	_textEdit->moveCursor(QTextCursor::End);
	_richText ? _textEdit->insertHtml(text) : _textEdit->insertPlainText(text);
	_textEdit->ensureCursorVisible();
}

void TextEditLogger::log(char text)
{
	if (_quiet)
		return;

	if (text == Tools::endl)
	{
		log("<br>");
		return;
	}

	_textEdit->moveCursor(QTextCursor::End);
	_richText ? _textEdit->insertHtml(QChar(text)) : _textEdit->insertPlainText(QChar(text));
	_textEdit->ensureCursorVisible();
}

void TextEditLogger::clear()
{
	_textEdit->clear();
}

void TextEditLogger::setRichText(bool value)
{
	_richText = value;
}


//-------------------------------------------------

PlainTextEditLogger::PlainTextEditLogger(QPlainTextEdit* textEdit)
	: _quiet(false), _textEdit(textEdit)
{
}

PlainTextEditLogger::~PlainTextEditLogger()
{
}

void PlainTextEditLogger::log(const char* text)
{
	if (_quiet)
		return;

	_textEdit->moveCursor(QTextCursor::End);
	_textEdit->insertPlainText(text);
	_textEdit->ensureCursorVisible();
}

void PlainTextEditLogger::log(char text)
{
	if (_quiet)
		return;

	_textEdit->moveCursor(QTextCursor::End);
	_textEdit->insertPlainText(QChar(text));
	_textEdit->ensureCursorVisible();
}

void PlainTextEditLogger::clear()
{
	_textEdit->clear();
}
