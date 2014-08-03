#include "SyntaxRuleHighlighting.h"
#include "TextHighlighter.h"

using namespace Tools;

SyntaxRuleHighlighting::SyntaxRuleHighlighting(TextHighlighter* parent) : TextHighlighterStrategyInterface(parent)
{

}

void SyntaxRuleHighlighting::addSyntaxRule(const QRegExp& pattern, const QTextCharFormat& format)
{
	HighlightingRule rule;
	rule._pattern = pattern;
	rule._format = format;
	_highlightingRules.append(rule);
}

void SyntaxRuleHighlighting::addSyntaxRule(const QRegExp& pattern, const QColor& color, int fontWeight)
{
	HighlightingRule rule;
	rule._pattern = pattern;

	QTextCharFormat format;
	format.setForeground(color);
	format.setFontWeight(fontWeight);
	rule._format = format;
	_highlightingRules.append(rule);
}

void SyntaxRuleHighlighting::addSyntaxRule(const QString& keyword, const QColor& color, int fontWeight)
{
	HighlightingRule rule;
	rule._pattern = QRegExp("\\b" + keyword + "\\b");

	QTextCharFormat format;
	format.setForeground(color);
	format.setFontWeight(fontWeight);
	rule._format = format;
	_highlightingRules.append(rule);
}

void SyntaxRuleHighlighting::highlightBlock(const QString &text)
{
	foreach (HighlightingRule rule, _highlightingRules) 
	{
		QRegExp expression(rule._pattern);
		int index = text.indexOf(expression);
		while (index >= 0) 
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule._format);
			index = text.indexOf(expression, index + length + 1);
		}
	}
}

SyntaxRuleHighlighting* SyntaxRuleHighlighting::shellSyntaxRules(TextHighlighter* parent)
{
	SyntaxRuleHighlighting* highlighter = new SyntaxRuleHighlighting(parent);

	//keywords
	highlighter->addSyntaxRule("if", Qt::darkBlue, QFont::Bold);
	highlighter->addSyntaxRule("fi", Qt::darkBlue, QFont::Bold);
	highlighter->addSyntaxRule("then", Qt::darkBlue, QFont::Bold);
	highlighter->addSyntaxRule("else", Qt::darkBlue, QFont::Bold);
	highlighter->addSyntaxRule("echo", Qt::darkBlue, QFont::Bold);

	//operators
	highlighter->addSyntaxRule(QRegExp("\\&{1,}"), Qt::darkRed, QFont::Bold);
	highlighter->addSyntaxRule(QRegExp("\\|{1,}"), Qt::darkRed, QFont::Bold);
	highlighter->addSyntaxRule(QRegExp("\\={1,}"), Qt::darkRed, QFont::Bold);
	highlighter->addSyntaxRule(QRegExp("\\+{1,}"), Qt::darkRed, QFont::Bold);
	highlighter->addSyntaxRule(QRegExp("\\-{1,}"), Qt::darkRed, QFont::Bold);
	highlighter->addSyntaxRule(QRegExp("\\*{1,}"), Qt::darkRed, QFont::Bold);
	highlighter->addSyntaxRule(QRegExp("\\/{1,}"), Qt::darkRed, QFont::Bold);
	highlighter->addSyntaxRule(QRegExp("\\({1,}"), Qt::darkRed, QFont::Bold);
	highlighter->addSyntaxRule(QRegExp("\\){1,}"), Qt::darkRed, QFont::Bold);
	highlighter->addSyntaxRule(QRegExp("\\{{1,}"), Qt::darkRed, QFont::Bold);
	highlighter->addSyntaxRule(QRegExp("\\}{1,}"), Qt::darkRed, QFont::Bold);
	highlighter->addSyntaxRule(QRegExp("\\<{1,}"), Qt::darkRed, QFont::Bold);
	highlighter->addSyntaxRule(QRegExp("\\>{1,}"), Qt::darkRed, QFont::Bold);


	highlighter->addSyntaxRule(QRegExp("\\d+\\.?\\d*"), Qt::red);	//number
	highlighter->addSyntaxRule(QRegExp("\\$\\w*"), Qt::darkYellow);	//variable
	QRegExp quote("\".*\"");
	quote.setMinimal(true);
	highlighter->addSyntaxRule(quote, Qt::gray);	//quotation with "

	QRegExp quote2("\'.*\'");
	quote2.setMinimal(true);
	highlighter->addSyntaxRule(quote2, Qt::gray);	//quotation with '

	highlighter->addSyntaxRule(QRegExp("#[^\\n]*"), Qt::darkGreen);  //comment


	return highlighter;
}
