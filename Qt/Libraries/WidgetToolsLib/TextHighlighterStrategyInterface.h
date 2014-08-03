#ifndef TextHighlighterSTRATEGYINTERFACE_H
#define TextHighlighterSTRATEGYINTERFACE_H

#include <QString>
#include <QTextCharFormat>
#include <QTextBlockUserData>

namespace Tools
{
	class TextHighlighter;

	class TextHighlighterStrategyInterface
	{
		friend class TextHighlighter;

	public:
		TextHighlighterStrategyInterface(TextHighlighter* parent = 0);
		virtual ~TextHighlighterStrategyInterface() {}

		/**
		* \brief Enables or disables the highlighting.
		* \see isEnabled
		*/
		void setEnabled(bool value);

		/**
		* \brief Returns true if the syntax highlighting is enabled; otherwise returns false.
		* \see setEnabled
		*/
		bool isEnabled() const;

	protected:
		/**
		* \brief Performs the highlighting in \p text if the strategy is enabled.
		* \see highlightBlock
		* \see isEnabled
		*/
		void doHighlighting(const QString& text);

		/**
		* \brief Performs the highlighting in \p text.
		* \details To be reimplemented in a subclass.
		*/
		virtual void highlightBlock(const QString& text) = 0;

		/**
		* \brief Redo the highlighting.
		*/
		void rehighlight() const;

		/**
		* \brief Sets the format of the specified characters in the text.
		*/
		void setFormat(int start, int count, const QTextCharFormat& format);

		void setCurrentBlockUserData(QTextBlockUserData* data);
		QTextBlockUserData* currentBlockUserData() const;

		QTextBlock currentBlock() const;

	private:
		TextHighlighter* _highlighter;
		bool		 _enabled;
	};
}

#endif // TextHighlighterSTRATEGYINTERFACE_H
