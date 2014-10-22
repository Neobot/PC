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

void SyntaxRuleHighlighting::addSyntaxRule(const QString& keyword, const QColor& color, int fontWeight, Qt::CaseSensitivity sensitivity)
{
	HighlightingRule rule;
	rule._pattern = QRegExp("\\b" + keyword + "\\b", sensitivity);

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

SyntaxRuleHighlighting *SyntaxRuleHighlighting::nsSyntaxRules(TextHighlighter *parent)
{
	SyntaxRuleHighlighting* syntax = new SyntaxRuleHighlighting(parent);

	syntax->addSyntaxRule(QRegExp("\\b\\d+\\.?\\d*\\b"), Qt::red);	//numbers
	syntax->addSyntaxRule(QRegExp("\\b\\d+\\.?\\d*\\s*(s|ms|deg|rad)\\b"), Qt::red); //numbers + unit

	QColor keywordColor = Qt::darkBlue;
	int fontWeigth = QFont::Bold;

	syntax->addSyntaxRule("declare", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("teleport", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("go", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("to", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("speed", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("set", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("enable", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("disable", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("all", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("wait", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("as", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("circle", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("rect", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("do", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("action", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("move", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("ax12", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("async", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("make", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("movement", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("concurrent", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("if", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("else", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("is", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("in", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("not", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("while", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("function", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("rotate", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("timeout", keywordColor, fontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("import", keywordColor, fontWeigth, Qt::CaseInsensitive);

	QColor enumColor = Qt::darkRed;
	int enumFontWeigth = QFont::Bold;
	syntax->addSyntaxRule("forward", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("backward", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("auto", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("AllActionFinished", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("OneActionFinished", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("FirstActionFinished", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("LastActionFinished", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("position", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("orientation", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("opponent", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("strategy", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("reversed", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("sharp", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("microswitch", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("color", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("sensor", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("true", enumColor, enumFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("false", enumColor, enumFontWeigth, Qt::CaseInsensitive);

	QColor valueColor = Qt::darkGreen;
	int valueFontWeigth = QFont::Bold;
	syntax->addSyntaxRule("unknown", valueColor, valueFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("red", valueColor, valueFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("green", valueColor, valueFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("blue", valueColor, valueFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("yellow", valueColor, valueFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("white", valueColor, valueFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("black", valueColor, valueFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("far", valueColor, valueFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("detected", valueColor, valueFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("close", valueColor, valueFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("on", valueColor, valueFontWeigth, Qt::CaseInsensitive);
	syntax->addSyntaxRule("off", valueColor, valueFontWeigth, Qt::CaseInsensitive);



//	QColor unitColor = QColor(230,100,0);
//	int unitFontWeigth = QFont::Normal;

//	syntax->addSyntaxRule(QRegExp("ms\\b", Qt::CaseInsensitive), unitColor, unitFontWeigth);
//	syntax->addSyntaxRule(QRegExp("s\\b", Qt::CaseInsensitive), unitColor, unitFontWeigth);
//	syntax->addSyntaxRule(QRegExp("rad\\b", Qt::CaseInsensitive), unitColor, unitFontWeigth);
//	syntax->addSyntaxRule(QRegExp("deg\\b", Qt::CaseInsensitive), unitColor, unitFontWeigth);
//	syntax->addSyntaxRule(QRegExp("\\%\\b", Qt::CaseInsensitive), unitColor, unitFontWeigth);

	QRegExp quote("\".*\"");
	quote.setMinimal(true);
	syntax->addSyntaxRule(quote, Qt::gray);	//quotation with "

	syntax->addSyntaxRule(QRegExp("//[^\\n]*"), Qt::darkGreen);  //comment

	return syntax;
}


