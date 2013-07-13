#ifndef UTILITYSELECTOR_H
#define UTILITYSELECTOR_H

#include <QDialog>
#include "CommGui.h"
#include "AX12Utilities.h"

namespace Ui {
	class UtilitySelector;
}

class UtilitySelector : public QDialog
{
	Q_OBJECT
	
public:
	explicit UtilitySelector(QWidget *parent = 0);
	~UtilitySelector();
	
private:
	Ui::UtilitySelector *ui;

	CommGui* _protocolUtilities;
	AX12Utilities* _ax12utilities;
	QButtonGroup* _group;

	void closeProtocolUtilities();
	void closeAx12Utilities();

private slots:
	void on_btnProtocolUtilities_toggled(bool checked);
	void on_btnAx12Utilities_toggled(bool checked);
};

#endif // UTILITYSELECTOR_H
