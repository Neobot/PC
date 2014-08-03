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
	_leftArea = new LineNumberArea(this);
	_rightArea = new ErrorNotificationArea(this);

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

	 _rightArea->hide();
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
		
	_rightArea->hide();
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
	
	_rightArea->show();
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

int NCodeEdit::errorAreaWidth()
{
	return 20;
}

void NCodeEdit::showError(const QPoint& pos)
{
	int line = 0;
	for(int linePos : _errorLinesPos)
	{
		++line;
		if (pos.y() < linePos)
			break;
	}

	QString message;
	QList<Error> errors = _errorsPerLine.value(line);
	for(const Error& e : errors)
	{
		if (!message.isEmpty())
			message += '\n';
		message += e.error;
	}

	if (!message.isEmpty())
		QToolTip::showText(_rightArea->mapToGlobal(pos), message);
}

void NCodeEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
	setViewportMargins(lineNumberAreaWidth(), 0, _rightArea->isVisible() ? errorAreaWidth() : 0, 0);
}

void NCodeEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy != 0)
	{
        _leftArea->scroll(0, dy);
		_rightArea->scroll(0, dy);
	}
    else
	{
        _leftArea->update(0, rect.y(), _leftArea->width(), rect.height());
		_rightArea->update(rect.right() - _rightArea->width(), rect.y(), _rightArea->width(), rect.height());
	}

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
 }

void NCodeEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    _leftArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
	_rightArea->setGeometry(QRect(cr.right() - errorAreaWidth(), cr.top(), errorAreaWidth(), cr.height()));
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

void NCodeEdit::lineNumberAreaPaintEvent(QPaintEvent *event, LineNumberArea *area)
{
	QPainter painter(area);
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

void NCodeEdit::errorAreaPaintEvent(QPaintEvent *event, ErrorNotificationArea *area)
{
	QPainter painter(area);
	painter.fillRect(event->rect(), Qt::white);

	_errorLinesPos.clear();

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();
	_errorLinesPos << bottom;

	while (block.isValid() && top <= event->rect().bottom())
	{
		if (block.isVisible() && bottom >= event->rect().top() && !_errorsPerLine.value(blockNumber + 1).isEmpty())
		{
			painter.setPen(Qt::black);
			painter.drawText(0, top, _leftArea->width(), fontMetrics().height(),
							Qt::AlignRight, "e");
		}

		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		_errorLinesPos << bottom;
		++blockNumber;
	}
}

//----------------------------------------------------------------------------

NCodeEdit::LineNumberArea::LineNumberArea(NCodeEdit* editor) : QWidget(editor), _editor(editor)
{
}

QSize NCodeEdit::LineNumberArea::sizeHint() const
{
    return QSize(_editor->lineNumberAreaWidth(), 0);
}

void NCodeEdit::LineNumberArea::paintEvent(QPaintEvent *event)
{
	_editor->lineNumberAreaPaintEvent(event, this);
}

//----------------------------------------------------------------------------

NCodeEdit::ErrorNotificationArea::ErrorNotificationArea(NCodeEdit* editor) : QWidget(editor), _editor(editor)
{
	setMouseTracking(true);
}

QSize NCodeEdit::ErrorNotificationArea::sizeHint() const
{
	return QSize(_editor->errorAreaWidth(), 0);
}

void NCodeEdit::ErrorNotificationArea::paintEvent(QPaintEvent *event)
{
	_editor->errorAreaPaintEvent(event, this);
}

void NCodeEdit::ErrorNotificationArea::mouseMoveEvent(QMouseEvent *event)
{
	_editor->showError(event->pos());
}

