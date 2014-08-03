#ifndef SELECTIONHIGHLIGHTING_H
#define SELECTIONHIGHLIGHTING_H

#include "TextHighlighterStrategyInterface.h"
#include <QTextCharFormat>

namespace Tools
{
	/**
	* \brief A strategy of text highlighting that highlight a fixed part in the text.
	* \details To be used with #TextHighlighter.
	* \see TextHighlighter
	*/
	class LineHighlighting : public TextHighlighterStrategyInterface
	{
	public:

		/**
		* \param parent The highlighter which use this strategy. If not null, the strategy is automatically added to the TextHighlighter.
		*/
		LineHighlighting(TextHighlighter* parent = 0);

		/**
		* \brief Sets text to be select.
		* \details Any previous selections are cleared.
		*/
		void setSelection(int line, int col, int length);

		/**
		* \brief Add a text to be select to the list of selected text.
		*/
		void addSelection(int line, int col, int length);

		/**
		* \brief Clear the highlighting of the selection.
		*/
		void clearSelection();

		/**
		* \brief Sets the format of the highlighted selection.
		* \details Default is bold red.
		*/
		void setSelectionFormat(const QTextCharFormat& format);

	protected:
		virtual void highlightBlock(const QString &text);

		struct Selection
		{
			int	selectionLine;
			int	selectionCol;
			int	selectionLength;

			bool isValid() const {return selectionLine >= 0 && selectionCol >= 0 && selectionLength > 0;}
		};

		QList<Selection> _selections;
		QTextCharFormat _format;
	};
}

#endif // SELECTIONHIGHLIGHTING_H
