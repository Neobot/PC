#ifndef NSEDITOR_H
#define NSEDITOR_H

#include "NCodeEdit.h"

#include <QWidget>

namespace Tools
{
	class NSEditor : public QWidget
	{
		Q_OBJECT

	public:
		explicit NSEditor(QWidget *parent = 0);
		~NSEditor();

		NCodeEdit* codeEdit() const;

	private:
		NCodeEdit* _codeEdit;

		void setupUi();
		void addNSKeywordSyntax(const QString &keyword, SyntaxRuleHighlighting *syntax);

	signals:
		void check();
	};
}

#endif // NSEDITOR_H
