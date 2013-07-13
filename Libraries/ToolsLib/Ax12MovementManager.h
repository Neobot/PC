#ifndef AX12MOVEMENTMANAGER_H
#define AX12MOVEMENTMANAGER_H

#include <QHash>
#include <QMap>
#include <QTextStream>


namespace Tools
{
	class Ax12MovementManager
	{

	public:
		typedef float Ax12Position;
		
		struct MovementInfo
		{
			MovementInfo() : torque(100), maxSpeed(60) {}
			
			float torque;
			float maxSpeed;
		};

		struct Ax12SingleGroupPosition
		{
			MovementInfo info;
			QList<Ax12Position> positions;
		};
		

		typedef QList<Ax12SingleGroupPosition> Ax12SingleMovement;

		typedef QMap<QString, Ax12SingleMovement> Ax12Movements;
		typedef QHash<QString, Ax12Movements> Ax12GroupMovements;
		typedef QPair<QMap<int, Ax12Position>, MovementInfo> DetailedPosition;

		Ax12MovementManager();

		void setGroup(const QString& name, const QList<int>& ids);
		void removeGroup(const QString& name);
		void renameGroup(const QString& oldName, const QString& newName);
		bool containsGroup(const QString& name) const;
		
		QList<int> getGroupIds(const QString& groupName) const;

		void addEmptyMovement(const QString& groupName, const QString& movementName);
		void setMovementTorque(const QString& groupName, const QString& movementName, int positionIndex, float torque);
		void setMovementMaxSpeed(const QString &groupName, const QString &movementName, int positionIndex, float speed);
		void setMovementPosition(const QString& groupName, const QString& movementName, int positionIndex, int ax12Index, float position);
		void addPositionToMovement(const QString& groupName, const QString& movementName, const Ax12SingleGroupPosition& movement);
		void insertPositionToMovement(const QString& groupName, const QString& movementName, const Ax12SingleGroupPosition& movement, int position);
		void removeMovement(const QString& groupName, const QString& movementName);
		void removeMovements(const QString& groupName);
		void removePosition(const QString& groupName, const QString& movementName, int positionIndex);
		void renameMovement(const QString& groupName, const QString& oldMovementName, const QString& newMovementName);
		bool containsMovement(const QString& groupName, const QString& movementName) const;

		const Ax12SingleMovement& getMovement(const QString& groupName, const QString& movementName);
		QList<DetailedPosition> getDetailedMovement(const QString& groupName, const QString& movementName);

		QStringList getGroupNames() const;
		QStringList getMovementNames(const QString& groupName) const;

		bool writeToFile(const QString& filename);
		bool readFromFile(const QString& filename);

		bool writeToStream(QTextStream& out);
		bool readFromStream(QTextStream& in);

		void clear();

	private:
		QHash<QString, QList<int> > _groups;
		Ax12GroupMovements _movements;
	};
}

#endif // AX12MOVEMENTMANAGER_H
