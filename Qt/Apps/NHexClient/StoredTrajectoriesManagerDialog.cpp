#include "StoredTrajectoriesManagerDialog.h"
#include "ui_StoredTrajectoriesManagerDialog.h"

#include <QInputDialog>
#include <QMessageBox>

StoredTrajectoriesManagerDialog::StoredTrajectoriesManagerDialog(const QMap<QString, Tools::Trajectory> &trajectories, QWidget *parent) :
	QDialog(parent), ui(new Ui::StoredTrajectoriesManagerDialog),
	_trajectories(trajectories)
{
	ui->setupUi(this);
	populateList();

	connect(ui->btnRemove, SIGNAL(clicked()), this, SLOT(onRemove()));
	connect(ui->btnRename, SIGNAL(clicked()), this, SLOT(onRename()));
	connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(onClearAll()));
}

StoredTrajectoriesManagerDialog::~StoredTrajectoriesManagerDialog()
{
	delete ui;
}

QMap<QString, Tools::Trajectory> StoredTrajectoriesManagerDialog::getTrajectories() const
{
	return _trajectories;
}

void StoredTrajectoriesManagerDialog::populateList()
{
	ui->list->clear();
	for(QMap<QString, Tools::Trajectory>::const_iterator it = _trajectories.constBegin(); it != _trajectories.constEnd(); ++it)
	{
		ui->list->addItem(it.key());
	}

	if (!_trajectories.isEmpty())
		ui->list->setCurrentRow(0);
}

void StoredTrajectoriesManagerDialog::on_list_currentTextChanged(const QString &text)
{
	if (_trajectories.contains(text))
	{
		Tools::Trajectory& tr = _trajectories[text];
		QString description(text);
		description += "\n\n";
		foreach(const Tools::RPoint& p, tr)
		{
			description += "x=";
			description += QString::number(p.getX());
			description += "mm, y=";
			description += QString::number(p.getY());
			description += QString::fromUtf8("mm, 𝛳=");
			description += QString::number(p.getThetaDegree());
			description += QString::fromUtf8("°\n");
		}

		ui->lblContent->setText(description);

		ui->btnRemove->setEnabled(true);
		ui->btnRename->setEnabled(true);
		ui->btnClear->setEnabled(true);
	}
	else
	{
		ui->btnRemove->setEnabled(false);
		ui->btnRename->setEnabled(false);
		ui->btnClear->setEnabled(false);
	}
}

void StoredTrajectoriesManagerDialog::onRemove()
{
	QString currentTr = ui->list->currentItem()->text();
	if (_trajectories.contains(currentTr))
	{
		_trajectories.remove(currentTr);
		populateList();
	}
}

void StoredTrajectoriesManagerDialog::onRename()
{
	QString currentTr = ui->list->currentItem()->text();
	if (_trajectories.contains(currentTr))
	{
		bool ok = false;
		QString name = QInputDialog::getText(this, "Rename trajectory", "Please type a trajectory name", QLineEdit::Normal, currentTr, &ok);
		if (ok)
		{
			if (name == currentTr)
				return;

			if (_trajectories.contains(name))
			{
				if (QMessageBox::question(this, "Rename trajectory", "A trajectory with the same name already exist. Do you want to replace it ?",
									  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
				{
					return;
				}
			}

			Tools::Trajectory tr = _trajectories.take(currentTr);
			_trajectories.insert(name, tr);
		}
		populateList();
	}
}

void StoredTrajectoriesManagerDialog::onClearAll()
{
	if (!_trajectories.isEmpty())
	{
		_trajectories.clear();
		populateList();
	}
}
