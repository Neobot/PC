#include "TextHighlighter.h"
#include "TextHighlighterStrategyInterface.h"

using namespace Tools;

TextHighlighter::TextHighlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent), _enabled(true)
{
}

TextHighlighter::~TextHighlighter()
{
	qDeleteAll(_strategies);
	_strategies.clear();
}

void TextHighlighter::setEnabled(bool value)
{
	_enabled = value;
	rehighlight();
}

bool TextHighlighter::isEnabled() const
{
	return _enabled;
}

void TextHighlighter::addHighlightingStrategy(TextHighlighterStrategyInterface* strategy)
{
	_strategies << strategy;
	strategy->_highlighter = this;
}

void TextHighlighter::highlightBlock(const QString &text)
{
	if (!_enabled)
		return;

	foreach (TextHighlighterStrategyInterface* strategy, _strategies) 
	{
		strategy->doHighlighting(text);
	}
}
