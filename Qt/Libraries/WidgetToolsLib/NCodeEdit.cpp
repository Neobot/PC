#include "NCodeEdit.h"
#include "TextHighlighter.h"
#include "LineHighlighting.h"
#include "SyntaxRuleHighlighting.h"

#include <QPainter>
#include <QTextBlock>
#include <QToolTip>

using namespace Tools;

NCodeEdit::NCodeEdit(QWidget *parent)
	: QPlainTextEdit(parent), _errorHighlighting(nullptr)
{
	_leftArea = new MarginArea(this);

     connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
     connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(updateLineNumberArea(const QRect &, int)));
     connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	QColor lineColor = QColor(Qt::yellow).lighter(160);
	_currentLineFormat.setBackground(lineColor);
	_currentLineFormat.setProperty(QTextFormat::FullWidthSelection, true);

	_errorFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
	_errorFormat.setUnderlineColor(Qt::red);
	_errorFormat.setFontUnderline(true);
	
	_highlighter = new TextHighlighter(document());
		
     updateLineNumberAreaWidth(0);
     highlightCurrentLine();

	 setTabStopWidth(20);
	 setLineWrapMode(QPlainTextEdit::NoWrap);
}

void NCodeEdit::setCurrentLineBackgroundColor(const QColor& color)
{
	_currentLineFormat.setBackground(color);
}

void NCodeEdit::setErrorFormat(const QTextCharFormat& format)
{
	_errorFormat = format;
	if (_errorHighlighting)
		_errorHighlighting->setSelectionFormat(_errorFormat);
}

void NCodeEdit::setSyntaxHighlighting(SyntaxRuleHighlighting* syntaxHighlighting)
{
	_highlighter->addHighlightingStrategy(syntaxHighlighting);
}

void NCodeEdit::clearErrors()
{
	_errorsPerLine.clear();
	
	if (_errorHighlighting)
		_errorHighlighting->clearSelection();
		
	_highlighter->rehighlight();
}

void NCodeEdit::addError(int line, int col, int length, const QString& message)
{
	Error e;
	e.pos = col;
	e.length = length;
	e.error = message;
	_errorsPerLine[line] << e;
	
	if (!_errorHighlighting)
	{
		_errorHighlighting = new LineHighlighting(_highlighter);
		_errorHighlighting->setSelectionFormat(_errorFormat);
	}
		
	_errorHighlighting->addSelection(line, col, length);
	
	_highlighter->rehighlight();
}

int NCodeEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) 
	{
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void NCodeEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void NCodeEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy != 0)
        _leftArea->scroll(0, dy);
    else
        _leftArea->update(0, rect.y(), _leftArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
 }

void NCodeEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    _leftArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void NCodeEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) 
	{
        QTextEdit::ExtraSelection selection;
		selection.format = _currentLineFormat;
       
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void NCodeEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(_leftArea);
    painter.fillRect(event->rect(), Qt::lightGray);

	QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

	while (block.isValid() && top <= event->rect().bottom()) 
	{
        if (block.isVisible() && bottom >= event->rect().top()) 
		{
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, _leftArea->width(), fontMetrics().height(),
                            Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

bool NCodeEdit::event(QEvent* event)
{
	if (event->type() == QEvent::ToolTip)
	{
		QHelpEvent* helpEvent = static_cast <QHelpEvent*>(event);
		QTextCursor cursor = cursorForPosition(helpEvent->pos());
		cursor.select(QTextCursor::WordUnderCursor);

		int line = cursor.block().blockNumber();
		const QList<Error>& errors = _errorsPerLine.value(line + 1);

		bool found = false;
		int posInBlock = cursor.position() - cursor.block().position();
		for(const Error& e : errors)
		{
			if (posInBlock >= e.pos && posInBlock < e.pos + e.length)
			{
				QToolTip::showText(helpEvent->globalPos(), e.error);
				found = true;
				break;

			}
		}

		if (!found)
			QToolTip::hideText();

		return true;
	}

	return QPlainTextEdit::event(event);
}

//----------------------------------------------------------------------------

NCodeEdit::MarginArea::MarginArea(NCodeEdit* editor) : QWidget(editor), _editor(editor)
{
}

QSize NCodeEdit::MarginArea::sizeHint() const
{
    return QSize(_editor->lineNumberAreaWidth(), 0);
}

void NCodeEdit::MarginArea::paintEvent(QPaintEvent *event)
{
    _editor->lineNumberAreaPaintEvent(event);
}
