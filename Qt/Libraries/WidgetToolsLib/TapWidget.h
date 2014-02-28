#ifndef TAPWIDGET_H
#define TAPWIDGET_H

#include <QTableWidget>
#include <QItemDelegate>

namespace Tools
{
	class TapDelegate : public QItemDelegate
	{
	public:
		TapDelegate(QObject * parent = 0);
		void drawDisplay(QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect, const QString & text) const;

		void setArrowDisplayed(bool value);
		bool getArrowDisplayed() const;

	private:
		bool _arrowDisplayed;
	};

	class TapWidget : public QWidget
	{
		Q_OBJECT

	signals:
		void itemActivated(int);
		void itemChanged(int);
		void itemClicked(int);
		void itemDoubleClicked(int);
		void itemEntered(int);
		void itemPressed(int);
        void contextMenuRequestedForItem(int);

	public:
		explicit TapWidget(QWidget *parent = 0);

		void clear();
		void clearSelection();

		void addItem(const QString& text);
		void addItem(QTableWidgetItem* item);
		void insertItem(int index, const QString& text);
		void insertItem(int index, QTableWidgetItem* item);
		void removeItem(int index);

		void setEditTriggers(QAbstractItemView::EditTriggers triggers);

		QString getText(int index) const;
		int getCurrentIndex() const;

		void setModicationEnabled(bool value);
		bool isModificationEnabled() const;

		void setCheckable(bool value);
		bool isCheckable() const;

		QList<int> getCheckedIndexes() const;

	private:
		QTableWidget* _tableWidget;
		TapDelegate* _tapDelegate;
		bool _isCheckable;

		void setItemCheckable(QTableWidgetItem* item, bool value);

    private slots:
        void contextMenuRequested(const QPoint & pos);
	};
}

#endif // TAPWIDGET_H
