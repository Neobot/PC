#ifndef AX12MOVEMENTEDITOR_H
#define AX12MOVEMENTEDITOR_H

#include <QWidget>
#include "Ax12MovementManager.h"

namespace Ui {
	class Ax12MovementEditor;
}

namespace Tools
{
	class Ax12MovementEditor : public QWidget
	{
		Q_OBJECT

	signals:
		void currentTabChanged(int index);
		
	public:
		enum Tabs
		{
			Groups,
			Movements,
			Positions
		};
		
		explicit Ax12MovementEditor(QWidget *parent = 0);
		~Ax12MovementEditor();

		void edit(Ax12MovementManager manager);
		bool addRecord(const QMap<int, float> &ax12Positions, float maxSpeed, float torque);

		Ax12MovementManager getEditedMovements() const;
		QString getCurrentGroup() const;
		QString getCurrentMovement() const;
		
		Tabs getCurrentTab() const;

	private:
		
		Ui::Ax12MovementEditor *ui;
		Ax12MovementManager _manager;

		QString _currentGroup;
		QString _currentMovement;

		void refreshMovementTableIds();

		QString getUniqueGroupeName(const QString& name) const;
		QString getUniqueMovementName(const QString& groupName, const QString& name) const;

		void addPosition(const QList<float> &positions, float maxSpeed, float torque);
		int insertPosition(const QList<float> &positions, float maxSpeed, float torque, int position);

	private slots:
		void groupSelected(int index);
		void movementSelected(int index);
		void groupRenamed(int index);
		void movementRenamed(int index);

		void prev();

		void add();
		void addGroup();
		void addMovement();

		void modify();
		void remove();

		void tabChanged(int index);

		void idAdded(int row, int column);
		void positionAdded(int row, int column);
	};

}
#endif // AX12MOVEMENTEDITOR_H
