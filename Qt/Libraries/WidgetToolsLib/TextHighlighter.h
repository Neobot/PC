#ifndef TEXTHIGHLIGHTER_H
#define TEXTHIGHLIGHTER_H

#include <QSyntaxHighlighter>

namespace Tools
{
	class TextHighlighterStrategyInterface;

	/**
	* \brief Main class for syntax highligting.
	* \details Based on QSyntaxHighlighter. It can highlight a text in different way depending of the strategies which have been added.
	* \note Without strategy nothing is highlighted.
	* \see HighlighterStrategyInterface
	* \see addHighlightingStrategy
	*/
	class TextHighlighter : public QSyntaxHighlighter
	{
		Q_OBJECT
		friend class TextHighlighterStrategyInterface;

	public:
		TextHighlighter(QTextDocument *parent = 0);
		virtual ~TextHighlighter();

		/**
		* \brief Adds a strategy to define the behavior of the highlighting.
		* \details The TextHighlighter takes the responsability of destruction of \p strategy.
		*/
		void addHighlightingStrategy(TextHighlighterStrategyInterface* strategy);

		/**
		* \brief Enables or disables the highlighting.
		*/
		void setEnabled(bool value);

		/**
		* \brief Returns true if the syntax highlighting is enabled; otherwise returns false.
		*/
		bool isEnabled() const;

	protected:
		/**
		* \brief Performs the highlighting of \p text with the strategies that have been added.
		* \see addHighlightingStrategy
		*/
		virtual void highlightBlock(const QString &text);

	private:
		bool _enabled;
		QList<TextHighlighterStrategyInterface*> _strategies;
		
	};
}

#endif // TEXTHIGHLIGHTER_H
