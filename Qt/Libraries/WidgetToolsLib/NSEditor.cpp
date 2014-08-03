#include "NSEditor.h"
#include "SyntaxRuleHighlighting.h"

#include <QBoxLayout>
#include <QToolBar>

using namespace Tools;

NSEditor::NSEditor(QWidget *parent) :
	QWidget(parent)
{
	setupUi();
}

NSEditor::~NSEditor()
{

}

NCodeEdit *NSEditor::codeEdit() const
{
	return _codeEdit;
}

void Tools::NSEditor::setupUi()
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0,0,0,0);
	layout->setSpacing(0);

	QToolBar* toolBar = new QToolBar("NSEditor", this);
	toolBar->addAction("Open");
	toolBar->addAction("Save");
	toolBar->addSeparator();
	toolBar->addAction("Check", this, SIGNAL(check()));

	_codeEdit = new NCodeEdit(this);

	SyntaxRuleHighlighting* syntax = new SyntaxRuleHighlighting;
	_codeEdit->setSyntaxHighlighting(syntax);

	addNSKeywordSyntax("declare", syntax);
	addNSKeywordSyntax("teleport", syntax);
	addNSKeywordSyntax("go", syntax);
	addNSKeywordSyntax("to", syntax);
	addNSKeywordSyntax("set", syntax);
	addNSKeywordSyntax("enable", syntax);
	addNSKeywordSyntax("disable", syntax);
	addNSKeywordSyntax("all", syntax);
	addNSKeywordSyntax("wait", syntax);
	addNSKeywordSyntax("as", syntax);
	addNSKeywordSyntax("rect", syntax);

	syntax->addSyntaxRule(QRegExp("\\d+\\.?\\d*"), Qt::red);	//numbers

	QRegExp quote("\".*\"");
	quote.setMinimal(true);
	syntax->addSyntaxRule(quote, Qt::gray);	//quotation with "

	syntax->addSyntaxRule(QRegExp("#[^\\n]*"), Qt::darkGreen);  //comment

	layout->addWidget(toolBar);
	layout->addWidget(_codeEdit);
}

void NSEditor::addNSKeywordSyntax(const QString& keyword, SyntaxRuleHighlighting* syntax)
{
	QRegExp exp("\\b" + keyword + "\\b", Qt::CaseInsensitive);
	syntax->addSyntaxRule(exp, Qt::darkBlue, QFont::Bold);
}
