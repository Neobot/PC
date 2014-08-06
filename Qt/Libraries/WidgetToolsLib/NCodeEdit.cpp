#include "NCodeEdit.h"
#include "TextHighlighter.h"
#include "LineHighlighting.h"
#include "SyntaxRuleHighlighting.h"

#include <QPainter>
#include <QTextBlock>
#include <QToolTip>

using namespace Tools;

NCodeEdit::NCodeEdit(QWidget *parent)
	: QPlainTextEdit(parent), _currentLineHighlighted(false), _lineAreaVisibled(true), _errorAreaVisibled(true), _errorHighlighting(nullptr)
{
	_lineNumberArea = new LineNumberArea(this);
	_errorArea = new ErrorNotificationArea(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateEditorLayout()));
	connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(updateAdditionalAreas(const QRect &, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	QColor lineColor = QColor(239,239,239);
	_currentLineFormat.setBackground(lineColor);
	_currentLineFormat.setProperty(QTextFormat::FullWidthSelection, true);

	_errorFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
	_errorFormat.setUnderlineColor(Qt::red);
	_errorFormat.setFontUnderline(true);

	_highlighter = new TextHighlighter(document());

	updateEditorLayout();
	highlightCurrentLine();

	setTabStopWidth(20);
	setLineWrapMode(QPlainTextEdit::NoWrap);

	connect(document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(textChangedAt(int,int,int)));

	setLineNumbersVisible(true);
	setErrorAreaVisible(true);
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

void NCodeEdit::setLineNumbersVisible(bool value)
{
	_lineNumberArea->setVisible(value);
	_lineAreaVisibled = value;
	updateEditorLayout();
}

void NCodeEdit::setErrorAreaVisible(bool value)
{
	_errorArea->setVisible(value);
	_errorAreaVisibled = value;
	updateEditorLayout();
}

void NCodeEdit::setCurrentLineHighlighted(bool value)
{
    _currentLineHighlighted = value;
    highlightCurrentLine();
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

	int space = 5 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
}

int NCodeEdit::errorAreaWidth()
{
	return 20;
}

void NCodeEdit::updateEditorLayout()
{
    int leftSize = 0;

	if (_lineAreaVisibled)
        leftSize += lineNumberAreaWidth();

	if (_errorAreaVisibled)
        leftSize += errorAreaWidth();

    setViewportMargins(leftSize, 0, 0, 0);
}

void NCodeEdit::updateAdditionalAreas(const QRect &rect, int dy)
{
    if (dy != 0)
	{
		_lineNumberArea->scroll(0, dy);
		_errorArea->scroll(0, dy);
	}
    else
	{
		_lineNumberArea->update(0, rect.y(), _lineNumberArea->width(), rect.height());
		_errorArea->update(0, rect.y(), _errorArea->width(), rect.height());
	}

    if (rect.contains(viewport()->rect()))
		updateEditorLayout();
}

void NCodeEdit::textChangedAt(int pos, int charsRemoved, int charsAdded)
{
	if (charsRemoved > 0 || charsAdded > 0)
	{
		emit codeChanged();

		QTextBlock block = document()->findBlock(pos);
		int lineNumber = block.blockNumber() + 1;

		if (_errorsPerLine.contains(lineNumber))
		{
			//int posInBlock = pos - block.pos();

			_errorsPerLine.remove(lineNumber);
			_errorHighlighting->clearSelection();

			for(auto it = _errorsPerLine.constBegin(); it != _errorsPerLine.constEnd(); ++it)
			{
				for(const Error& e : *it)
				{
					_errorHighlighting->addSelection(it.key(), e.pos, e.length);
				}
			}
		}
	}
}

void NCodeEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();

	int lineAreaPos = 0;
	if (_errorAreaVisibled)
		lineAreaPos += errorAreaWidth();
	_lineNumberArea->setGeometry(QRect(cr.left() + lineAreaPos, cr.top(), lineNumberAreaWidth(), cr.height()));
	_errorArea->setGeometry(QRect(cr.left(), cr.top(), errorAreaWidth(), cr.height()));
}

void NCodeEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (_currentLineHighlighted && !isReadOnly())
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
    painter.fillRect(event->rect(), QColor(235,235,235));

	QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

	while (block.isValid() && top <= event->rect().bottom()) 
	{
        if (block.isVisible() && bottom >= event->rect().top()) 
		{
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(167,167,167));
			painter.drawText(0, top, _lineNumberArea->width(), fontMetrics().height(),
							Qt::AlignCenter, number);
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
    painter.fillRect(event->rect(), QColor(235,235,235));

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
			QRect drawArea(0, top, _errorArea->width(), fontMetrics().height());
			QPixmap icon(":/other/error");
			//icon = icon.scaled(8,8, Qt::KeepAspectRatio);
			painter.drawPixmap(drawArea, icon);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		_errorLinesPos << bottom;
		++blockNumber;
	}
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
    {
		_errorArea->setToolTipWidthForText(message, 18);

		QString htmlMessage = "<p><img src=\":/other/error_croped\"> ";
        htmlMessage += message;
		htmlMessage += "</p>";

        QToolTip::showText(_errorArea->mapToGlobal(pos), htmlMessage, _errorArea);
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
    setToolTipWidthForText(QString(), 42);
}

QSize NCodeEdit::ErrorNotificationArea::sizeHint() const
{
    return QSize(_editor->errorAreaWidth(), 0);
}

void NCodeEdit::ErrorNotificationArea::setToolTipWidthForText(const QString& text, int fixedWidth)
{
	QFont f = QToolTip::font();
    f.setBold(true);
    QFontMetrics fm(f);
    int width = fm.width(text) + fixedWidth;

    QString ss = "QToolTip{background-color: rgb(255,172,154); color: black; font-weight: bold; min-width: ";
    ss += QString::number(width);
    ss += ";}";
    setStyleSheet(ss);
}

void NCodeEdit::ErrorNotificationArea::paintEvent(QPaintEvent *event)
{
	_editor->errorAreaPaintEvent(event, this);
}

void NCodeEdit::ErrorNotificationArea::mouseMoveEvent(QMouseEvent *event)
{
	_editor->showError(event->pos());
}

