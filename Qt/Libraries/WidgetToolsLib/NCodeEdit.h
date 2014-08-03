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

	public:
		NCodeEdit(QWidget *parent);

		void lineNumberAreaPaintEvent(QPaintEvent *event);
		int lineNumberAreaWidth();
		
		void setCurrentLineBackgroundColor(const QColor& color);
		void setErrorFormat(const QTextCharFormat& format);
		
		void setSyntaxHighlighting(SyntaxRuleHighlighting* syntaxHighlighting);
		
		void clearErrors();
		void addError(int line, int col, int length, const QString& message);

	protected:
		void resizeEvent(QResizeEvent *event);
		bool event(QEvent* event);

	private slots:
		void updateLineNumberAreaWidth(int newBlockCount);
		void highlightCurrentLine();
		void updateLineNumberArea(const QRect &, int);

	private:
		QWidget* _leftArea;		
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
		
		class MarginArea : public QWidget
		{
		 public:
			 MarginArea(NCodeEdit* editor);
			 QSize sizeHint() const;

		 protected:
			 void paintEvent(QPaintEvent *event);

		 private:
			 NCodeEdit* _editor;
		};
	};
}

#endif // NCODEEDITOR_H
