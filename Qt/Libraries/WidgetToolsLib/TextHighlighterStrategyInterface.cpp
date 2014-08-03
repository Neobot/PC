#include "TextHighlighterStrategyInterface.h"
#include "TextHighlighter.h"

using namespace Tools;

TextHighlighterStrategyInterface::TextHighlighterStrategyInterface(TextHighlighter* parent)
{
	_enabled = true;
	_highlighter = parent;
	if (parent)
		_highlighter->_strategies << this;
}

void TextHighlighterStrategyInterface::rehighlight() const
{
	if (_highlighter)
		_highlighter->rehighlight();
}

void TextHighlighterStrategyInterface::setFormat(int start, int count, const QTextCharFormat& format)
{
	if (_highlighter)
		_highlighter->setFormat(start, count, format);
}

void TextHighlighterStrategyInterface::setCurrentBlockUserData(QTextBlockUserData* data)
{
	if (_highlighter)
		_highlighter->setCurrentBlockUserData(data);
}

QTextBlockUserData* TextHighlighterStrategyInterface::currentBlockUserData () const
{
	if (_highlighter)
		return _highlighter->currentBlockUserData();
	else
		return 0;
}

QTextBlock TextHighlighterStrategyInterface::currentBlock() const
{
	return _highlighter->currentBlock();
}

void TextHighlighterStrategyInterface::doHighlighting(const QString& text)
{
	if (_enabled)
		highlightBlock(text);
}

void TextHighlighterStrategyInterface::setEnabled(bool value)
{
	_enabled = value;
	rehighlight();
}

bool TextHighlighterStrategyInterface::isEnabled() const
{
	return _enabled;
}