#ifndef STOREDTRAJECTORIESMANAGERDIALOG_H
#define STOREDTRAJECTORIESMANAGERDIALOG_H

#include <QDialog>
#include <QMap>
#include "Curve.h"

namespace Ui {
	class StoredTrajectoriesManagerDialog;
}

class StoredTrajectoriesManagerDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit StoredTrajectoriesManagerDialog(const QMap<QString, Tools::Trajectory>& trajectories, QWidget *parent = 0);
	~StoredTrajectoriesManagerDialog();

	QMap<QString, Tools::Trajectory> getTrajectories() const;
	
private:
	Ui::StoredTrajectoriesManagerDialog *ui;
	QMap<QString, Tools::Trajectory> _trajectories;

	void populateList();

private slots:
	void on_list_currentTextChanged(const QString& text);
	void onRemove();
	void onRename();
	void onClearAll();
};

#endif // STOREDTRAJECTORIESMANAGERDIALOG_H
