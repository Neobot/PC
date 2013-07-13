#include "SpinBoxDelegate.h"
#include <QSpinBox>

SpinBoxDelegate::SpinBoxDelegate(int minValue, int maxValue, QObject *parent)
	: QItemDelegate(parent), _min(minValue), _max(maxValue)
 {
 }

 QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
 {
	 QSpinBox *editor = new QSpinBox(parent);
	 editor->setButtonSymbols(QSpinBox::NoButtons);
	 editor->setMinimum(_min);
	 editor->setMaximum(_max);

	 return editor;
 }

 void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
 {
	 int value = index.model()->data(index, Qt::EditRole).toInt();

	 QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
	 spinBox->setValue(value);
 }

 void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
 {
	 QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
	 spinBox->interpretText();
	 int value = spinBox->value();

	 model->setData(index, value, Qt::EditRole);
 }

 void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index) const
 {
	 Q_UNUSED(index);
	 editor->setGeometry(option.rect);
 }


 DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(double minValue, double maxValue, QObject *parent)
	 : QItemDelegate(parent), _min(minValue), _max(maxValue)
  {
  }

  QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
  {
	  QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
	  editor->setButtonSymbols(QSpinBox::NoButtons);
	  editor->setMinimum(_min);
	  editor->setMaximum(_max);

	  return editor;
  }

  void DoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
  {
	  double value = index.model()->data(index, Qt::EditRole).toDouble();

	  QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
	  spinBox->setValue(value);
  }

  void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
  {
	  QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
	  spinBox->interpretText();
	  int value = spinBox->value();

	  model->setData(index, value, Qt::EditRole);
  }

  void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index) const
  {
	  Q_UNUSED(index);
	  editor->setGeometry(option.rect);
  }
