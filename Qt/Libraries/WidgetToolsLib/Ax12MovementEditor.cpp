#include "Ax12MovementEditor.h"
#include "ui_Ax12MovementEditor.h"

#include <QInputDialog>
#include <QHeaderView>
#include <QMenu>

#include "SpinBoxDelegate.h"

using namespace Tools;

Ax12MovementEditor::Ax12MovementEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Ax12MovementEditor)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(Groups);
	tabChanged(Groups);
	ui->stackedWidget->setAnimation(QEasingCurve::OutCubic);
	ui->stackedWidget->setSpeed(200);

	connect(ui->btnAdd, SIGNAL(clicked()), this, SLOT(add()));
	connect(ui->btnPrev, SIGNAL(clicked()), this, SLOT(prev()));
	connect(ui->btnModify, SIGNAL(clicked()), this, SLOT(modify()));
	connect(ui->btnRemove, SIGNAL(clicked()), this, SLOT(remove()));

	connect(ui->mvtIdsTable, SIGNAL(cellChanged(int,int)), this, SLOT(idAdded(int,int)));
	connect(ui->movementTree, SIGNAL(cellChanged(int,int)), this, SLOT(positionAdded(int,int)));

	connect(ui->groupTapWidget, SIGNAL(itemClicked(int)), this, SLOT(groupSelected(int)));
	connect(ui->movementsTapWidget, SIGNAL(itemClicked(int)), this, SLOT(movementSelected(int)));
	
	connect(ui->groupTapWidget, SIGNAL(itemChanged(int)), this, SLOT(groupRenamed(int)));
	connect(ui->movementsTapWidget, SIGNAL(itemChanged(int)), this, SLOT(movementRenamed(int)));

	connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
	connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SIGNAL(currentTabChanged(int)));

	ui->mvtIdsTable->setItemDelegate(new SpinBoxDelegate(0, 254, this));

    ui->movementTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->movementTree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(movementContextMenuRequested(QPoint)));
}

Ax12MovementEditor::~Ax12MovementEditor()
{
	delete ui;
}

void Ax12MovementEditor::edit(Ax12MovementManager manager)
{
	ui->stackedWidget->setCurrentIndex(Groups);
	ui->groupTapWidget->clear();

	_manager = manager;

	ui->groupTapWidget->blockSignals(true);
	foreach(const QString& group, manager.getGroupNames())
		ui->groupTapWidget->addItem(group);
	ui->groupTapWidget->blockSignals(false);
}

bool Ax12MovementEditor::addRecord(const QMap<int, float> &ax12Positions, float maxSpeed, float torque)
{
	if (_currentGroup.isEmpty() || _currentMovement.isEmpty())
		return false;

	QList<int> ids = _manager.getGroupIds(_currentGroup);

	QList<float> positionsForExistingIds;
	foreach(int id, ids)
		positionsForExistingIds << ax12Positions.value(id, -1);

	int pos = insertPosition(positionsForExistingIds, maxSpeed, torque, 0.0, ui->movementTree->currentRow());


	Ax12MovementManager::Ax12SingleGroupPosition p;
	p.positions = positionsForExistingIds;
	p.info.torque = torque;
	p.info.maxSpeed = maxSpeed;

	_manager.insertPositionToMovement(_currentGroup, _currentMovement, p, pos);

	return true;
}

Ax12MovementManager Ax12MovementEditor::getEditedMovements() const
{
	return _manager;
}

void Ax12MovementEditor::refreshMovementTableIds()
{
	QList<int> ids = _manager.getGroupIds(_currentGroup);

	ui->movementTree->clear();
    ui->movementTree->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->movementTree->setColumnCount(ids.count() + 3);
	for(int i = 0; i < ids.count(); ++i)
	{
		delete ui->movementTree->itemDelegateForColumn(i);
		ui->movementTree->setItemDelegateForColumn(i, new DoubleSpinBoxDelegate(-1, 300, this));
	}

	delete ui->movementTree->itemDelegateForColumn(ui->movementTree->columnCount() - 1);
	ui->movementTree->setItemDelegateForColumn(ui->movementTree->columnCount() - 2, new DoubleSpinBoxDelegate(0, 100, this));

	delete ui->movementTree->itemDelegateForColumn(ui->movementTree->columnCount() - 2);
	ui->movementTree->setItemDelegateForColumn(ui->movementTree->columnCount() - 1, new DoubleSpinBoxDelegate(0, 100, this));

	delete ui->movementTree->itemDelegateForColumn(ui->movementTree->columnCount() - 3);
	ui->movementTree->setItemDelegateForColumn(ui->movementTree->columnCount() - 2, new DoubleSpinBoxDelegate(0, 114, this));



	QStringList headers;
	for(int i = 0; i < ids.count(); ++i)
		headers << QString::number(ids.value(i));
	headers << "Max Speed" << "Torque" << "Load Limit";
	ui->movementTree->setHorizontalHeaderLabels(headers);

	if (ui->movementsTapWidget->getCurrentIndex() > 0)
		ui->movementTree->setRowCount(1);
}

QString Ax12MovementEditor::getUniqueGroupeName(const QString &name) const
{
	QString result = name;
	QString base(name);
	int i = 1;
	while (_manager.containsGroup(result))
	{
		result = base;
		result += QString::number(i);
		++i;
	}

	return result;
}

QString Ax12MovementEditor::getUniqueMovementName(const QString &groupName, const QString &name) const
{
	QString result = name;
	QString base(name);
	int i = 1;
	while (_manager.containsMovement(groupName, result))
	{
		result = base;
		result += QString::number(i);
		++i;
	}

	return result;
}

void Ax12MovementEditor::addPosition(const QList<float> &positions, float maxSpeed, float torque, float loadLimit)
{
	insertPosition(positions, maxSpeed, torque, loadLimit, ui->movementTree->rowCount() - 1);
}

int Ax12MovementEditor::insertPosition(const QList<float> &positions, float maxSpeed, float torque, float loadLimit, int position)
{
	ui->movementTree->blockSignals(true);
	int row = position;
	ui->movementTree->insertRow(row);

	for(int i = 0; i < ui->movementTree->columnCount() - 3; ++i)
	{
		QTableWidgetItem* item = new QTableWidgetItem(QString::number(positions.value(i, -1.)));
		ui->movementTree->setItem(row, i, item);
	}

	QTableWidgetItem* speedItem = new QTableWidgetItem(QString::number(maxSpeed));
	ui->movementTree->setItem(row, ui->movementTree->columnCount() - 3, speedItem);

	QTableWidgetItem* torqueItem = new QTableWidgetItem(QString::number(torque));
	ui->movementTree->setItem(row, ui->movementTree->columnCount() - 2, torqueItem);

	QTableWidgetItem* loadItem = new QTableWidgetItem(QString::number(loadLimit));
	ui->movementTree->setItem(row, ui->movementTree->columnCount() - 1, loadItem);

	ui->movementTree->blockSignals(false);

	return row;
}

void Ax12MovementEditor::groupSelected(int index)
{
	_currentGroup = ui->groupTapWidget->getText(index);
	
	if (ui->groupTapWidget->isModificationEnabled() || index < 0)
		return;

	ui->lblMovementTab->setText(QString(_currentGroup));

	ui->movementsTapWidget->clear();
	QStringList groups = _manager.getMovementNames(_currentGroup);
	foreach(const QString& group, groups)
	{
		ui->movementsTapWidget->blockSignals(true);
		ui->movementsTapWidget->addItem(group);
		ui->movementsTapWidget->blockSignals(false);
	}

	QList<int> ids = _manager.getGroupIds(_currentGroup);

	ui->mvtIdsTable->clear();
    ui->mvtIdsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui->mvtIdsTable->setColumnCount(ids.count() + 1);
	ui->mvtIdsTable->setRowCount(1);

	for(int i = 0; i < ids.count(); ++i)
	{
		QString text = QString::number(ids.value(i));
		QTableWidgetItem* item = new QTableWidgetItem(text);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
		ui->mvtIdsTable->setItem(0, i, item);
	}

	QTableWidgetItem* lastItem = new QTableWidgetItem(QString());
	lastItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);

	ui->stackedWidget->slideInIndex(Movements);
}

void Ax12MovementEditor::movementSelected(int index)
{
	_currentMovement = ui->movementsTapWidget->getText(index);

	if (ui->movementsTapWidget->isModificationEnabled() || index < 0)
		return;

	ui->lblPositionTab->setText(QString(_currentGroup).append("/").append(_currentMovement));

	ui->movementTree->clearContents();
	ui->movementTree->setRowCount(1);
	
	refreshMovementTableIds();

	Ax12MovementManager::Ax12SingleMovement mvt = _manager.getMovement(_currentGroup, _currentMovement);
	foreach(const Ax12MovementManager::Ax12SingleGroupPosition& p, mvt)
		addPosition(p.positions, p.info.maxSpeed, p.info.torque, p.info.loadLimit);

	ui->stackedWidget->slideInIndex(Positions);
}

void Ax12MovementEditor::prev()
{
	if (ui->stackedWidget->currentIndex() != Groups)
		ui->stackedWidget->slideInPrev();
}

void Ax12MovementEditor::add()
{
	switch(ui->stackedWidget->currentIndex())
	{
		case Groups:
			addGroup();
			break;
		case Movements:
			addMovement();
			break;
	}
}

void Ax12MovementEditor::addGroup()
{
	bool ok = false;
	QString name = QInputDialog::getText(this, "New Group", "Please select a group name", QLineEdit::Normal, getUniqueGroupeName("Group"), &ok);
	if (ok && ! name.isEmpty())
	{
		name = getUniqueGroupeName(name);
		_manager.setGroup(name, QList<int>());
		ui->groupTapWidget->addItem(name);
	}
}

void Ax12MovementEditor::addMovement()
{
	bool ok = false;
	QString name = QInputDialog::getText(this, "New Movement", "Please select a movement name", QLineEdit::Normal, getUniqueMovementName(_currentGroup, "Mvt"), &ok);
	if (ok && ! name.isEmpty())
	{
		name = getUniqueMovementName(_currentGroup, name);
		_manager.addEmptyMovement(_currentGroup, name);
		ui->movementsTapWidget->addItem(name);
	}
}

void Ax12MovementEditor::modify()
{
	ui->btnRemove->setVisible(ui->btnModify->isChecked());

	switch(ui->stackedWidget->currentIndex())
	{
		case Groups:
			ui->groupTapWidget->setCheckable(!ui->groupTapWidget->isCheckable());
			break;
		case Movements:
			ui->movementsTapWidget->setCheckable(!ui->movementsTapWidget->isCheckable());
			break;
		case Positions:
			break;
	}
}

void Ax12MovementEditor::remove()
{
	switch(ui->stackedWidget->currentIndex())
	{
		case Groups:
		{
			QList<int> items = ui->groupTapWidget->getCheckedIndexes();
			qSort(items.begin(), items.end(), std::greater<int>());
			foreach(int i, items)
			{
				_manager.removeGroup(ui->groupTapWidget->getText(i));
				ui->groupTapWidget->removeItem(i);
			}
			ui->btnModify->setChecked(false);
			modify();
			break;
		}
		case Movements:
		{
			QList<int> items = ui->movementsTapWidget->getCheckedIndexes();
			qSort(items.begin(), items.end(), std::greater<int>());
			foreach(int i, items)
			{
				_manager.removeMovement(_currentGroup, ui->movementsTapWidget->getText(i));
				ui->movementsTapWidget->removeItem(i);
			}
			ui->btnModify->setChecked(false);
			modify();
			break;
		}
		case Positions:
		{
			QModelIndexList  rowIndexes = ui->movementTree->selectionModel()->selectedRows();
			int lastRow = ui->movementTree->rowCount() - 1;
			QList<int> rows;
			foreach(const QModelIndex& index, rowIndexes)
			{
				int r = index.row();
				if (r >= 0 && r < lastRow)
					rows << r;
			}
			qSort(rows);

			ui->movementTree->blockSignals(true);
			for(QList<int>::const_iterator it = rows.constEnd() - 1; it != rows.constBegin() - 1; --it)
			{
				int row = *it;
				ui->movementTree->removeRow(row);
				_manager.removePosition(_currentGroup, _currentMovement, row);
			}
			ui->movementTree->blockSignals(false);

			break;
		}
	}
}

void Ax12MovementEditor::tabChanged(int index)
{
	switch(index)
	{
		case Groups:
			_currentGroup.clear();
			ui->btnRemove->hide();
			ui->btnModify->show();
			ui->btnAdd->show();
			ui->btnPrev->setEnabled(false);
			ui->movementsTapWidget->clearSelection();
			break;
		case Movements:
			_currentMovement.clear();
			ui->btnRemove->hide();
			ui->btnModify->show();
			ui->btnAdd->show();
			ui->btnPrev->setEnabled(true);
			ui->groupTapWidget->clearSelection();
			break;
		case Positions:
			ui->btnRemove->show();
			ui->btnModify->hide();
			ui->btnAdd->hide();
			ui->movementsTapWidget->clearSelection();
			break;
	}
}

void Ax12MovementEditor::idAdded(int row, int column)
{
	Q_UNUSED(row);
	if (column == ui->mvtIdsTable->columnCount() - 1)
	{
		ui->mvtIdsTable->setColumnCount(ui->mvtIdsTable->columnCount() + 1);
		QList<int> ids;
		for(int i = 0; i < ui->mvtIdsTable->columnCount() - 1; ++i)
			ids << ui->mvtIdsTable->item(0, i)->text().toInt();
		
		_manager.setGroup(_currentGroup, ids);
	}
	else
	{
		QList<int> ids = _manager.getGroupIds(_currentGroup);
		ids[column] = ui->mvtIdsTable->item(0, column)->text().toInt();
		_manager.setGroup(_currentGroup, ids);
	}
}

void Ax12MovementEditor::positionAdded(int row, int column)
{
	Q_UNUSED(row);
	ui->movementTree->blockSignals(true);
	int loadIndex = ui->movementTree->columnCount() - 1;
	int torqueIndex = ui->movementTree->columnCount() - 2;
	int speedIndex = ui->movementTree->columnCount() - 3;

	if (row == ui->movementTree->rowCount() - 1)
	{
		//new position

		Ax12MovementManager::Ax12SingleGroupPosition p;
		QList<float> positions;

		for(int i = 0; i < speedIndex; ++i)
		{
			QTableWidgetItem* item = ui->movementTree->item(row, i);
			if (i != column)
			{
				Q_ASSERT(!item);
				item = new QTableWidgetItem;
				ui->movementTree->setItem(row, i, item);
				item->setText("-1");
				positions << -1;
			}
			else
			{
				Q_ASSERT(item);
				positions << item->text().toFloat();
			}
		}

		QTableWidgetItem* loadItem = ui->movementTree->item(row, loadIndex);
		if (!loadItem)
		{
			loadItem = new QTableWidgetItem;
			ui->movementTree->setItem(row, loadIndex, loadItem);
		}
		if (column != loadIndex)
			loadItem->setText(QString::number(p.info.loadLimit));
		else
			p.info.loadLimit = loadItem->text().toFloat();

		QTableWidgetItem* torqueItem = ui->movementTree->item(row, torqueIndex);
		if (!torqueItem)
		{
			torqueItem = new QTableWidgetItem;
			ui->movementTree->setItem(row, torqueIndex, torqueItem);
		}
		if (column != torqueIndex)
			torqueItem->setText(QString::number(p.info.torque));
		else
			p.info.torque = torqueItem->text().toFloat();
		
		QTableWidgetItem* speedItem = ui->movementTree->item(row, speedIndex);
		if (!speedItem)
		{
			speedItem = new QTableWidgetItem;
			ui->movementTree->setItem(row, speedIndex, speedItem);
		}
		if (column != speedIndex)
			speedItem->setText(QString::number(p.info.maxSpeed));
		else
			p.info.maxSpeed = speedItem->text().toFloat();

		p.positions = positions;
		_manager.addPositionToMovement(_currentGroup, _currentMovement, p);

		ui->movementTree->setRowCount(ui->movementTree->rowCount() + 1);
	}
	else
	{
		//existing position edited

		float value = ui->movementTree->item(row, column)->text().toFloat();
		if (column < torqueIndex - 1)
			_manager.setMovementPosition(_currentGroup, _currentMovement, row, column, value);
		else if (column == torqueIndex - 1)
			_manager.setMovementMaxSpeed(_currentGroup, _currentMovement, row, value);
		else if (column == torqueIndex)
			_manager.setMovementTorque(_currentGroup, _currentMovement, row, value);
		else
			_manager.setMovementLoadLimit(_currentGroup, _currentMovement, row, value);
	}
    ui->movementTree->blockSignals(false);
}

void Ax12MovementEditor::movementContextMenuRequested(QPoint pos)
{
    QMenu menu;

    int positionIndex = ui->movementTree->indexAt(pos).row();
    if (positionIndex >= 0 && positionIndex < ui->movementTree->rowCount() - 1)
    {
        QAction* action;
        action = menu.addAction("Run until here", this, SLOT(runUntilHere()));
        action->setData(positionIndex);

        action = menu.addAction("Move to this position", this, SLOT(moveToPosition()));
        action->setData(positionIndex);
    }

    if (!menu.isEmpty())
        menu.exec(ui->movementTree->viewport()->mapToGlobal(pos));
}

void Ax12MovementEditor::runUntilHere()
{
    QAction* action = (QAction*)sender();
    emit runCurrentMovementUntil(action->data().toInt());
}

void Ax12MovementEditor::moveToPosition()
{
    QAction* action = (QAction*)sender();
    emit moveToCurrentMovementPosition(action->data().toInt());
}

void Ax12MovementEditor::groupRenamed(int index)
{
	_manager.renameGroup(_currentGroup, ui->groupTapWidget->getText(index));
}

void Ax12MovementEditor::movementRenamed(int index)
{
	_manager.renameGroup(_currentMovement, ui->movementsTapWidget->getText(index));
}

Ax12MovementEditor::Tabs Ax12MovementEditor::getCurrentTab() const
{
	return static_cast<Ax12MovementEditor::Tabs>(ui->stackedWidget->currentIndex());
}

QString Ax12MovementEditor::getCurrentGroup() const
{
	return _currentGroup;
}

QString Ax12MovementEditor::getCurrentMovement() const
{
	return _currentMovement;
}
