#ifndef TELEPORTDIALOG_H
#define TELEPORTDIALOG_H

#include <QDialog>
#include <QVector3D>

namespace Ui {
	class TeleportDialog;
}

class TeleportDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit TeleportDialog(const QVector3D& defaultValue, QWidget *parent = 0);
	~TeleportDialog();

	QVector3D getPoint() const;
	
private:
	Ui::TeleportDialog *ui;
	QVector3D _point;

private slots:
	void select();
	void setOrigin();
	void setOrigin2();
	void setStart1();
	void setStart2();
	void setCenter();
};

#endif // TELEPORTDIALOG_H
