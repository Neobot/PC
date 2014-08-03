#ifndef NCODEEDITOR_H
#define NCODEEDITOR_H

#include <QPlainTextEdit>

namespace Tools
{
	class TextHighlighter;
	class SyntaxRuleHighlighting;
	class LineHighlighting;

	class NCodeEdit : public QPlainTextEdit
	{
		Q_OBJECT

		class LineNumberArea : public QWidget
		{
		 public:
			 LineNumberArea(NCodeEdit* editor);
			 QSize sizeHint() const;

		 protected:
			 void paintEvent(QPaintEvent *event);

		 private:
			 NCodeEdit* _editor;
		};

		class ErrorNotificationArea : public QWidget
		{
		 public:
			 ErrorNotificationArea(NCodeEdit* editor);
			 QSize sizeHint() const;

		 protected:
			 void paintEvent(QPaintEvent *event);
			 void mouseMoveEvent(QMouseEvent* event);

		 private:
			 NCodeEdit* _editor;
		};

		friend class LineNumberArea;
		friend class ErrorNotificationArea;

	public:
		NCodeEdit(QWidget *parent);
		
		void setCurrentLineBackgroundColor(const QColor& color);
		void setErrorFormat(const QTextCharFormat& format);
		
		void setSyntaxHighlighting(SyntaxRuleHighlighting* syntaxHighlighting);
		
		void clearErrors();
		void addError(int line, int col, int length, const QString& message);

	protected:
		void resizeEvent(QResizeEvent *event);
		void lineNumberAreaPaintEvent(QPaintEvent *event, LineNumberArea* area);
		void errorAreaPaintEvent(QPaintEvent *event, ErrorNotificationArea* area);

	private slots:
		void updateLineNumberAreaWidth(int newBlockCount);
		void highlightCurrentLine();
		void updateLineNumberArea(const QRect &, int);

	private:
		QWidget* _leftArea;		
		QWidget* _rightArea;
		QTextCharFormat _currentLineFormat;
		QTextCharFormat _errorFormat;
		
		TextHighlighter* _highlighter;
		SyntaxRuleHighlighting* _syntaxHighlighting;
		LineHighlighting*  _errorHighlighting;
		
		struct Error
		{
			int pos, length;
			QString error;
		};
		
		QHash<int, QList<Error>> _errorsPerLine;
		QList<int> _errorLinesPos;

		int lineNumberAreaWidth();
		int errorAreaWidth();
		void showError(const QPoint &pos);
	};
}

#endif // NCODEEDITOR_H
