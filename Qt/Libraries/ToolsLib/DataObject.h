#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <QPointF>
#include <QRectF>
#include <QLineF>

#include "ZRange.h"
#include "Curve.h"

namespace Tools
{
    class DataObject
    {
    public:
	enum ObjectType
	{
	    Circle,
	    Rectangle,
	    Line,
	    Trajectory,
	    NoType
	};

	DataObject(const ZRange& zrange = ZRange());
	DataObject(const DataObject& other);
	virtual ~DataObject() {}

	ObjectType getType() const;
	const ZRange& getZRange() const;
	bool isCollideable() const;
	void collideable();
	void notCollideable();

	virtual double getRotation() const = 0;
	virtual void setRotation(double angle) = 0;
	virtual QPointF getPosition() const = 0;
	virtual void setPosition(const QPointF& position) = 0;
	virtual QRectF getBoundingRect() const = 0;
	virtual bool contains(const QPointF& point) const = 0;

    protected:
	 ObjectType _type;
	 ZRange	_zRange;
	 bool _collideable; // true = can collide with another object
    };


    class CircleObject : public DataObject
    {
    public:
	CircleObject(double radius, const QPointF& position, double rotation = 0.0, const ZRange& zrange = ZRange());
	CircleObject(const CircleObject& other);

	void setRadius(double radius);
	double getRadius() const;

	double getRotation() const;
	void setRotation(double angle);
	QPointF getPosition() const;
	void setPosition(const QPointF& position);
	QRectF getBoundingRect() const;
	bool contains(const QPointF& point) const;

    private:
	double _r;
	double _angle;
	QPointF _position;
    };

    /**
      * \brief Rectangle. No rotation possible.
      */
    class RectangleObject : public DataObject
    {
    public:
	RectangleObject(const QRectF rect, const ZRange& zrange = ZRange());
	RectangleObject(const RectangleObject& other);

	const QRectF& getRectangle() const;

	double getRotation() const;
	void setRotation(double angle);
	QPointF getPosition() const;
	void setPosition(const QPointF& position);
	QRectF getBoundingRect() const;
	bool contains(const QPointF& point) const;

    private:
	QRectF _rect;
	double _angle;
    };

    /**
      * \brief Line. Rotation on the first point.
      */
    class LineObject : public DataObject
    {
    public:
	LineObject(const QLineF& line, const ZRange& zrange = ZRange());
	LineObject(const LineObject& other);

	const QLineF& getLine() const;

	double getRotation() const;
	void setRotation(double angle);

	QPointF getPosition() const;
	void setPosition(const QPointF& position);

	QRectF getBoundingRect() const;
	bool contains(const QPointF& point) const;

	void setP2(const QPointF& position);
	void setLength(double length);

    private:
	QLineF _line;
    };

    class TrajectoryObject : public DataObject
    {
    public:
	TrajectoryObject(const CircleObject* patternCircle, const Tools::Trajectory& tr, const ZRange& zrange = ZRange());
	TrajectoryObject(const CircleObject* patternCircle, const ZRange& zrange = ZRange());
	TrajectoryObject(const TrajectoryObject& other);
	~TrajectoryObject();

	const CircleObject* getPatternCircle() const;
	const Tools::Trajectory& getTrajectory() const;

	void append(const Tools::RPoint& point);
	void prepend(const Tools::RPoint& point);

	void removeFirst();
	void removeLast();

	void clear();
	void setTrajectory(const Tools::Trajectory& tr);

	double getRotation() const;
	void setRotation(double angle);
	QPointF getPosition() const;
	void setPosition(const QPointF& position);
	QRectF getBoundingRect() const;
	bool contains(const QPointF& point) const;

    private:
	const CircleObject* _pattern;
	Tools::Trajectory _trajectory;
    };
}

#endif // DATAOBJECT_H
