#ifndef STRATEGYSCENE_H
#define STRATEGYSCENE_H

#include <QGraphicsScene>
#include <QHash>


class StrategyManager;

namespace Tools
{
	class DataObject;
	class DataMap;

	/**
	* \brief A scene which represents a DataMap
	* \details The order of the group inside DataMap defines the z-axis for each group
	*/

	class StrategyScene : public QGraphicsScene
	{
		Q_OBJECT
	public:
		enum TransformationOrigin
		{
			TopLeft,
			Center
		};

		explicit StrategyScene(QObject *parent = 0);

		void setMap(Tools::DataMap* map);
		void setManualOpponentEnabled(bool value);


		void clearGroupProperties();
		void setGroupAsLineObject(int group);
		void setGroupAsRectangleObject(int group);
		void setGroupPixmapProperty(int group, const QPixmap& pixmap);
		void setGroupDynamicProperty(int group, bool value);
		void setGroupDynamicSizeProperty(int group, bool value);
		void setGroupVisibleProperty(int group, bool visible);
		void setGroupPenProperty(int group, const QPen& pen);
		void setGroupBrushProperty(int group, const QBrush& brush);

		void setGroupTransformationOrigin(int group, TransformationOrigin origin);


		void refresh();

		static QRectF mapWithDataMap(const QRectF& point);
		static QPointF mapWithDataMap(const QPointF& point);
		static double mapAngleFromDataMap(double angleRadius);
		static double mapAngleToDataMap(double angleDegree);


	private:
		enum SpecialObjectType
		{
			None,
			Line,
			Rectangle,
			Pixmap
		};

		struct GroupProperties
		{
			GroupProperties() : type(Pixmap), dynamic(false), dynamicSize(false), transformOrigin(Center), pixmap(":/objects/obstacle"), visible(true) {}

			SpecialObjectType	type;
			bool			dynamic;
			bool			dynamicSize;
			TransformationOrigin	transformOrigin;
			QPixmap			pixmap;
			bool			visible;
			QPen			pen;
			QBrush			brush;
		};

		Tools::DataMap* _map;
		QGraphicsPixmapItem* _tableItem;
		QGraphicsPixmapItem* _robotItem;
		QGraphicsPixmapItem* _opponentItem;

		QHash<const Tools::DataObject*, QGraphicsItem*> _objectItems;
		QHash<const Tools::DataObject*, int> _objectGroups;
		QHash<const Tools::DataObject*, QGraphicsItem*> _dynamicObjectItems;
		QHash<const Tools::DataObject*, QGraphicsItem*> _dynamicSizeObjectItems;
		QHash<int, GroupProperties> _groupProperties;

		int _refreshInterval; //in ms
		bool _manualOpponent;


		void mapObjectPosition(const Tools::DataObject* object, QGraphicsItem* item) const;
		void mapObjectSize(const Tools::DataObject* object, QGraphicsItem* item) const;

	private slots:
		void objectAdded(const Tools::DataObject* o, int group);
		void objectRemoved(const Tools::DataObject* o, int group);
		void mapCleared();
	};
}

#endif // STRATEGYSCENE_H
