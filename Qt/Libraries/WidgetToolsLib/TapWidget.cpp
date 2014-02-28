#include "TapWidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QPainter>

using namespace Tools;

TapWidget::TapWidget(QWidget *parent) :
	QWidget(parent), _isCheckable(false)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    _tableWidget = new QTableWidget(0, 1, this);
    layout->addWidget(_tableWidget);

    _tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _tableWidget->horizontalHeader()->hide();
    _tableWidget->verticalHeader()->setMinimumHeight(30);
    _tableWidget->verticalHeader()->hide();

	_tapDelegate = new TapDelegate(this);
	_tableWidget->setItemDelegate(_tapDelegate);

    _tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    _tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequested(QPoint)));

    connect(_tableWidget, SIGNAL(cellActivated(int, int)), this, SIGNAL(itemActivated(int)));
    connect(_tableWidget, SIGNAL(cellChanged(int,int)), this, SIGNAL(itemChanged(int)));
    connect(_tableWidget, SIGNAL(cellClicked(int,int)), this, SIGNAL(itemClicked(int)));
    connect(_tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SIGNAL(itemDoubleClicked(int)));
    connect(_tableWidget, SIGNAL(cellEntered(int,int)), this, SIGNAL(itemEntered(int)));
	connect(_tableWidget, SIGNAL(cellPressed(int,int)), this, SIGNAL(itemPressed(int)));
}

void TapWidget::clear()
{
	_tableWidget->clear();
	_tableWidget->setRowCount(0);
	_tableWidget->setColumnCount(1);
}

void TapWidget::addItem(const QString &text)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
	if (_isCheckable)
		setItemCheckable(item, true);
    addItem(item);
}

void TapWidget::addItem(QTableWidgetItem *item)
{
    _tableWidget->setRowCount(_tableWidget->rowCount() + 1);
    _tableWidget->setItem(_tableWidget->rowCount() - 1, 0, item);
}

void TapWidget::insertItem(int index, const QString &text)
{
     QTableWidgetItem *item = new QTableWidgetItem(text);
	 if (_isCheckable)
		 setItemCheckable(item, true);
	 insertItem(index, item);
}

void TapWidget::insertItem(int index, QTableWidgetItem *item)
{
	_tableWidget->insertRow(index);
	_tableWidget->setItem(index, 0, item);
}

void TapWidget::removeItem(int index)
{
	_tableWidget->removeRow(index);
}

void TapWidget::setEditTriggers(QAbstractItemView::EditTriggers triggers)
{
	_tableWidget->setEditTriggers(triggers);
}

QString TapWidget::getText(int index) const
{
	return _tableWidget->item(index, 0)->text();
}

int TapWidget::getCurrentIndex() const
{
	return _tableWidget->currentRow();
}

void TapWidget::setModicationEnabled(bool value)
{
	_tapDelegate->setArrowDisplayed(!value);
}

bool TapWidget::isModificationEnabled() const
{
	return !_tapDelegate->getArrowDisplayed();
}

void TapWidget::clearSelection()
{
	_tableWidget->clearSelection();
}

QList<int> TapWidget::getCheckedIndexes() const
{
	QList<int> results;

	int rowCount = _tableWidget->rowCount();
	for(int i = 0; i < rowCount; ++i)
	{
		QTableWidgetItem* item = _tableWidget->item(i, 0);
		if (item->checkState() == Qt::Checked)
			results << i;
	}

	return results;
}

void TapWidget::setCheckable(bool value)
{
	_isCheckable = value;
	int rowCount = _tableWidget->rowCount();
	for(int i = 0; i < rowCount; ++i)
		setItemCheckable(_tableWidget->item(i, 0), _isCheckable);

	setModicationEnabled(value);
}

bool TapWidget::isCheckable() const
{
	return _isCheckable;
}

void TapWidget::setItemCheckable(QTableWidgetItem *item, bool value)
{
	if (value)
	{
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Unchecked);
	}
	else
	{
		item->setFlags(item->flags() & ~Qt::ItemIsUserCheckable);
		item->setData(Qt::CheckStateRole, QVariant());
    }
}

void TapWidget::contextMenuRequested(const QPoint &pos)
{
    int itemIndex = _tableWidget->indexAt(pos).row();
    if (itemIndex >= 0)
        emit contextMenuRequestedForItem(itemIndex);
}

//------------------------------------------------------------------------

TapDelegate::TapDelegate(QObject *parent) : QItemDelegate(parent), _arrowDisplayed(true)
{
}

void TapDelegate::drawDisplay(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, const QString &text) const
{
    QRectF arrowRect(rect);
    arrowRect.setLeft(qMax(arrowRect.right() - 20, arrowRect.left()));

    QRect textRect(rect);
    textRect.setRight(qMax(arrowRect.right() - 20, arrowRect.left()));

    QItemDelegate::drawDisplay(painter, option, textRect, text);

	if (_arrowDisplayed)
	{
		painter->save();

		QFont f = painter->font();
		f.setBold(true);
		f.setPointSize(10);
		f.setFamily("Consolas");
		painter->setFont(f);
		painter->setPen(Qt::darkGray);
		painter->drawText(arrowRect, Qt::AlignHCenter | Qt::AlignCenter, QString(">"));

		painter->restore();
	}
}

void TapDelegate::setArrowDisplayed(bool value)
{
	_arrowDisplayed = value;
}

bool TapDelegate::getArrowDisplayed() const
{
	return _arrowDisplayed;
}
