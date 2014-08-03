#include "LineHighlighting.h"
#include "TextHighlighter.h"

using namespace Tools;

LineHighlighting::LineHighlighting(TextHighlighter* parent) 
	: TextHighlighterStrategyInterface(parent)
{
	_format.setBackground(Qt::yellow);
	clearSelection();
}

void LineHighlighting::setSelection(int line, int col, int length)
{
	_selections.clear();
	addSelection(line, col, length);
}

void LineHighlighting::addSelection(int line, int col, int length)
{
	Selection s;
	s.selectionLine = line;
	s.selectionCol = col;
	s.selectionLength = length;

	_selections << s;

	rehighlight();
}

void LineHighlighting::clearSelection()
{
	_selections.clear();
	rehighlight();
}

void LineHighlighting::setSelectionFormat(const QTextCharFormat& format)
{
	_format = format;
}

void LineHighlighting::highlightBlock(const QString &text)
{
	Q_UNUSED(text);

	int currentLine = currentBlock().blockNumber();
	foreach(const Selection& s, _selections)
	{
		if (s.isValid())
		{
			if (s.selectionLine - 1 == currentLine)
			{
				if (s.selectionCol >= 0)
					setFormat(s.selectionCol, s.selectionLength, _format);
			}
		}
	}
}
