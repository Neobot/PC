#ifndef DATAMAP_H
#define DATAMAP_H

#include "Curve.h"
#include "DataObject.h"
#include <QList>
#include <QHash>
#include <QPair>
#include <QVector2D>
#include <QObject>
#include <QQueue>

class DataMapTests;

/**
  * \brief A DataMap reprensents a table which contains a robot, an opponent and a list of object.
  * \details It provides the necessary methods to check collision between the different group of objects.
  */
namespace Tools
{

	class DataMap : public QObject
	{
		Q_OBJECT

		friend class DataMapTests;

	public:
		DataMap(const QSizeF& tableSize, double robotRadius, int robotGroup, double opponentRadius, int opponentGroup, QObject* parent = 0);
		virtual ~DataMap();

		void init(const QSizeF& tableSize, double robotRadius, int robotGroup, double opponentRadius, int opponentGroup);

		void moveRobot(const QPointF& pos);
		void moveRobot(const QPointF& pos, double angle);
		virtual void moveRobot(const RPoint& pos);
		void moveOpponent(const QPointF& pos);
		void moveOpponent(const RPoint& pos);
		RPoint getRobotPosition() const;
		RPoint getOpponentPosition() const;

		void addObject(DataObject* object, int group);
		bool removeObject(DataObject* object, int group);
		bool containsObject(DataObject* object, int group) const;

		void addTemporaryOpponent(const QPointF& pos, int ms);
		void addTemporaryObject(DataObject* object, int group, int ms);

		const CircleObject* getRobot() const;
		const CircleObject* getOpponent() const;

		bool isInTable(const QPointF& pos) const;

		void clear();
		void clear(int group);

		QList<DataObject*> getObjects(int group) const;
		const QHash<int, QList<DataObject*> >& getObjects() const;

		const QRectF& getTableRect() const;

		bool collideWith(DataObject* objects, int group) const;
		bool collideWith(DataObject* objects, int group, QList<DataObject*>& list) const;

		static bool collide(DataObject* o1, DataObject* o2);
		static bool collide(CircleObject* c1, CircleObject* c2);
		static bool collide(RectangleObject* r, CircleObject* c);
		static bool collide(LineObject* l, CircleObject* c);
		static bool collide(TrajectoryObject* t, CircleObject* c);

		void manageObjectOwnership(bool value);

		int getRobotGroup() const;
		int getOpponentGroup() const;

	protected:
		bool _manageObjectOwnership;
		QRectF _tableRect;

		QHash<int, QList<DataObject*> > _objects;
		QQueue<QPair<int, DataObject*> > _temporayObjects;

		CircleObject* _robot;
		int _robotGroup;

		CircleObject* _opponent;
		int _oppGroup;

	private:
		static bool compareLinePointDistance(const QVector2D& A, const QVector2D& B, const QVector2D& C, double distance);

		void removeContent();

	signals:
		void objectAdded(const Tools::DataObject* object, int group);
		void objectRemoved(const Tools::DataObject* object, int group);
		void cleared();

	private slots:
		void removeTempObject();
	};
}

#endif // DATAMAP_H
