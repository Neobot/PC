#include "StrategyScene.h"
#include "DataMap.h"
#include "DataObject.h"
#include "ToolsLib.h"
#include <QGraphicsPixmapItem>

#include <QtDebug>

using namespace Tools;

StrategyScene::StrategyScene(QObject *parent) :
	QGraphicsScene(parent), _map(0), _robotItem(0), _opponentItem(0), _manualOpponent(false)
{


}

void StrategyScene::setMap(Tools::DataMap* map)
{
	if (_map)
	{
		//reset
		disconnect(_map, SIGNAL(objectAdded(const Tools::DataObject*, int)), this, SLOT(objectAdded(const Tools::DataObject*, int)));
		disconnect(_map, SIGNAL(objectRemoved(const Tools::DataObject*, int)), this, SLOT(objectRemoved(const Tools::DataObject*, int)));
		disconnect(_map, SIGNAL(cleared()), this, SLOT(mapCleared()));

		qDeleteAll(_objectItems);
		_objectItems.clear();
	}


	QGraphicsScene::clear();
	_tableItem = new QGraphicsPixmapItem(QPixmap(":/tables/table2015"));
	_tableItem->setZValue(-1);
	_tableItem->setPos(0, 0);
	addItem(_tableItem);

	_map = map;
	setSceneRect(mapWithDataMap(_map->getTableRect()));
	double tableScaleFactor =  sceneRect().width() / _tableItem->boundingRect().width();
	_tableItem->setScale(tableScaleFactor);

	//set the map objects
	if (_objectItems.isEmpty())
	{
		for(QHash<int, QList<Tools::DataObject*> >::const_iterator it = _map->getObjects().constBegin(); it != _map->getObjects().constEnd(); ++it)
		{
			foreach(Tools::DataObject* o, it.value())
			{
				if (o == _map->getRobot())
				{
					//set the robot
					_robotItem = new QGraphicsPixmapItem(QPixmap(":/objects/robot2011"));
					addItem(_robotItem);


					double robotScaleFactor =  (_map->getRobot()->getRadius() * 2) / _robotItem->boundingRect().width();
					robotScaleFactor /= 1.21;

					_robotItem->setTransformOriginPoint((_robotItem->boundingRect().center() - _robotItem->pos()));
					_robotItem->setScale(robotScaleFactor);
					_robotItem->setZValue(_map->getRobotGroup());
					mapObjectPosition(_map->getRobot(), _robotItem);
				}
				else if (o == _map->getOpponent())
				{
					//set the opponent
					_opponentItem = new QGraphicsPixmapItem(QPixmap(":/objects/opponent"));
					addItem(_opponentItem);

					_opponentItem->setTransformOriginPoint(_opponentItem->boundingRect().center() - _opponentItem->pos());
					double oppScaleFactor =  (_map->getOpponent()->getRadius() * 2) / _opponentItem->boundingRect().width();
					_opponentItem->setScale(oppScaleFactor);
					_opponentItem->setZValue(_map->getOpponentGroup());
					mapObjectPosition(_map->getOpponent(), _opponentItem);
				}
				else
					objectAdded(o, it.key());
			}
		}
	}

	connect(_map, SIGNAL(objectAdded(const Tools::DataObject*, int)), this, SLOT(objectAdded(const Tools::DataObject*, int)));
	connect(_map, SIGNAL(objectRemoved(const Tools::DataObject*, int)), this, SLOT(objectRemoved(const Tools::DataObject*, int)));
	connect(_map, SIGNAL(cleared()), this, SLOT(mapCleared()));
}

QPointF StrategyScene::mapWithDataMap(const QPointF &point)
{
	return QPointF(point.y(), point.x());
}

QRectF StrategyScene::mapWithDataMap(const QRectF& rect)
{
	return QRectF(rect.topLeft(), QSizeF(rect.height(), rect.width()));
}

double StrategyScene::mapAngleFromDataMap(double angleRadius)
{
	return 90 - Tools::radianToDegree(angleRadius);
}

double StrategyScene::mapAngleToDataMap(double angleDegree)
{
	return Tools::radianToDegree(90 - angleDegree);
}

void StrategyScene::mapObjectPosition(const Tools::DataObject *object, QGraphicsItem* item) const
{
	QPointF p = mapWithDataMap(object->getPosition());
	GroupProperties properties = _groupProperties.value(_objectGroups.value(object));

	if (properties.transformOrigin == Center)
	{
		p.rx() -= item->boundingRect().width() / 2;
		p.ry() -= item->boundingRect().height() / 2;
	}
	item->setPos(p);
	item->setRotation(mapAngleFromDataMap(object->getRotation()));
}

void StrategyScene::mapObjectSize(const Tools::DataObject* object, QGraphicsItem* item) const
{
	GroupProperties properties = _groupProperties.value(_objectGroups.value(object));
	QRectF r = object->getBoundingRect();
	if (properties.type == Pixmap)
	{
		double scaleFactor =  r.width() / item->boundingRect().width();
		item->setScale(scaleFactor);
	}
	else if (properties.type == Line)
	{
		static_cast<QGraphicsLineItem*>(item)->setLine(QLine(0, 0, QVector2D(r.topLeft() - r.bottomRight()).length() , 0));
	}
	else if (properties.type == Rectangle)
	{
		static_cast<QGraphicsRectItem*>(item)->setRect(0, 0, r.width(), r.height());
	}
}

void StrategyScene::setManualOpponentEnabled(bool value)
{
	_manualOpponent = value;
}

void StrategyScene::objectAdded(const Tools::DataObject *o, int group)
{
	GroupProperties properties = _groupProperties.value(group);
	if (!properties.visible)
		return;

	_objectGroups.insert(o, group);

	QGraphicsItem* item = 0;
	QRectF r = o->getBoundingRect();

	if (properties.type == Line)
	{
		QGraphicsLineItem* lineItem = new QGraphicsLineItem(0, 0, 1 , 0);
		lineItem->setPen(properties.pen);
		item = lineItem;
	}
	else if (properties.type == Rectangle)
	{
		QGraphicsRectItem* rectItem = new QGraphicsRectItem(0, 0, 1 , 1);
		rectItem->setPen(properties.pen);
		rectItem->setBrush(properties.brush);
		item = rectItem;
	}
	else
		item = new QGraphicsPixmapItem(properties.pixmap);

	mapObjectSize(o, item);

	if (properties.transformOrigin == Center)
		item->setTransformOriginPoint(item->boundingRect().center() - item->pos());
	else
		item->setTransformOriginPoint(item->boundingRect().topLeft() - item->pos());

	if (properties.type == Pixmap)
	{
		double scaleFactor =  r.width() / item->boundingRect().width();
		item->setScale(scaleFactor);
	}

	mapObjectPosition(o, item);
	_objectItems[o] = item;

	if (properties.dynamicSize)
		_dynamicSizeObjectItems[o] = item;

	if (properties.dynamic)
		_dynamicObjectItems[o] = item;

	item->setZValue(group);

	addItem(item);
}

void StrategyScene::objectRemoved(const Tools::DataObject *o, int group)
{
	Q_UNUSED(group);
	QGraphicsItem* obstacleItem = _objectItems[o];
	removeItem(obstacleItem);
	_objectItems.remove(o);
	_objectGroups.remove(o);
	_dynamicObjectItems.remove(o);
	_dynamicSizeObjectItems.remove(o);
	delete obstacleItem;
}

void StrategyScene::setGroupPixmapProperty(int group, const QPixmap &pixmap)
{
	_groupProperties[group].pixmap = pixmap;
	_groupProperties[group].type = Pixmap;
}

void StrategyScene::setGroupVisibleProperty(int group, bool visible)
{
	_groupProperties[group].visible = visible;
}

void StrategyScene::setGroupPenProperty(int group, const QPen &pen)
{
	_groupProperties[group].pen = pen;
}

void StrategyScene::setGroupBrushProperty(int group, const QBrush &brush)
{
	_groupProperties[group].brush = brush;
}

void StrategyScene::setGroupAsLineObject(int group)
{
	_groupProperties[group].type = Line;
}

void StrategyScene::setGroupAsRectangleObject(int group)
{
	_groupProperties[group].type = Rectangle;
}

void StrategyScene::setGroupTransformationOrigin(int group, StrategyScene::TransformationOrigin origin)
{
	_groupProperties[group].transformOrigin = origin;
}

void StrategyScene::setGroupDynamicProperty(int group, bool value)
{
	_groupProperties[group].dynamic = value;
}

void StrategyScene::setGroupDynamicSizeProperty(int group, bool value)
{
	_groupProperties[group].dynamicSize = value;
}

void StrategyScene::clearGroupProperties()
{
	_groupProperties.clear();
	_objectGroups.clear();
	_dynamicObjectItems.clear();
	_dynamicSizeObjectItems.clear();
	_objectItems.clear();
}

void StrategyScene::mapCleared()
{
	for(QHash<const Tools::DataObject*, QGraphicsItem*>::iterator it = _objectItems.begin();
		it != _objectItems.end(); ++it)
	{
		delete it.value();
	}
	_objectItems.clear();
}

void StrategyScene::refresh()
{
	if (!_map)
		return;

	mapObjectPosition(_map->getRobot(), _robotItem);

	if (!_manualOpponent)
		mapObjectPosition(_map->getOpponent(), _opponentItem);

	for(QHash<const Tools::DataObject*, QGraphicsItem*>::const_iterator it = _dynamicObjectItems.constBegin();
		it != _dynamicObjectItems.constEnd(); ++it)
	{
		mapObjectPosition(it.key(), it.value());
	}

	for(QHash<const Tools::DataObject*, QGraphicsItem*>::const_iterator it = _dynamicSizeObjectItems.constBegin();
		it != _dynamicSizeObjectItems.constEnd(); ++it)
	{
		mapObjectSize(it.key(), it.value());
	}
}




