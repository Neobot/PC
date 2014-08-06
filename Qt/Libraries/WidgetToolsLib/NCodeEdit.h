#ifndef NCODEEDITOR_H
#define NCODEEDITOR_H

#include <QPlainTextEdit>
#include <QLabel>

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
             void setToolTipWidthForText(const QString& text, int fixedWidth);

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

        void setLineNumbersVisible(bool value);
        void setErrorAreaVisible(bool value);
        void setCurrentLineHighlighted(bool value);

	protected:
		void resizeEvent(QResizeEvent *event);
		void lineNumberAreaPaintEvent(QPaintEvent *event, LineNumberArea* area);
		void errorAreaPaintEvent(QPaintEvent *event, ErrorNotificationArea* area);

	private slots:
		void updateEditorLayout();
		void highlightCurrentLine();
		void updateAdditionalAreas(const QRect &, int);
        void textChangedAt(int pos, int charsRemoved, int charsAdded);

	private:
		QWidget* _lineNumberArea;
        ErrorNotificationArea* _errorArea;
		QTextCharFormat _currentLineFormat;
		QTextCharFormat _errorFormat;

        bool _currentLineHighlighted;
		bool _lineAreaVisibled;
		bool _errorAreaVisibled;
		
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

	signals:
		void codeChanged();
	};
}

#endif // NCODEEDITOR_H
