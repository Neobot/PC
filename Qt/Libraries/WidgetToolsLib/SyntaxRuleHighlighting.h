#ifndef SYNTAXRULESTRATEGY_H
#define SYNTAXRULESTRATEGY_H

#include "TextHighlighterStrategyInterface.h"
#include <QRegExp>
#include <QTextCharFormat>

namespace Tools
{
	/**
	* \class SyntaxRuleHighlighting
	* \brief A strategy of text highlighting that highlight the text using syntax rules based in regular expressions.
	* \details To be used with #TextHighlighter.
	* \see TextHighlighter
	*/
	class SyntaxRuleHighlighting : public TextHighlighterStrategyInterface
	{
	public:

		/**
		* \brief Constructs an SyntaxRuleHighlighting strategy.
		* \param parent The highlighter which use this strategy. If not null, the strategy is automatically added to the TextHighlighter.
		*/
		SyntaxRuleHighlighting(TextHighlighter* parent = 0);

		/**
		* \brief Add a syntax rule.
		* \param pattern The pattern of the rule.
		* \param format The format to apply to the text which match the pattern.
		*/
		void addSyntaxRule(const QRegExp& pattern, const QTextCharFormat& format);
		
		/**
		* \brief Add a syntax rule.
		*
		* This is an overloaded member function, provided for convenience.
		*
		* \param pattern The pattern of the rule.
		* \param color The color to apply to the text which match the pattern.
		* \param fontWeight The font weight to apply to the text which match the pattern.
		*/
		void addSyntaxRule(const QRegExp& pattern, const QColor& color, int fontWeight = QFont::Normal);
		
		/**
		* \brief Add a syntax rule on a keyword.
		* \param keyword The keyword. It cannot be a regular expression.
		* \param color The color to apply to the text which match the pattern.
		* \param fontWeight The font weight to apply to the text which match the pattern.
		*/
		void addSyntaxRule(const QString& keyword, const QColor& color, int fontWeight = QFont::Normal);

		/**
		* \brief Adds syntax rules for the shell language to \p highlighter.
		*/
		static SyntaxRuleHighlighting* shellSyntaxRules(TextHighlighter* parent = 0);

	protected:
		void highlightBlock(const QString &text);

	private:
		struct HighlightingRule
		{
			QRegExp _pattern;
			QTextCharFormat _format;
		};
		QVector<HighlightingRule> _highlightingRules;
	};
}

#endif // SYNTAXRULESTRATEGY_H
