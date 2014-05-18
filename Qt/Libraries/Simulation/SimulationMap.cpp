#include "SimulationMap.h"
#include "ToolsLib.h"
#include <QtDebug>

using namespace Tools;


SimulationMap::SimulationMap() : Tools::DataMap(QSizeF(2000, 3000), 350/2, RobotGroup, 350/2, OpponentGroup),
    _colorMap(":/ColorMaps/2014ColorMap")
{
    _colorMap = _colorMap.scaled(getTableRect().size().toSize());
    configureRobotSensors();

    //cake
	//addObject(new CircleObject(400.0, QPointF(0, 1500), 0.0, ZRange(0.0, 250)), TableObstacleGroup);
    //addObject(new CircleObject(480.0, QPointF(0, 1500), 0.0, ZRange(0.0, 150)), TableObstacleGroup);
}

SimulationMap::~SimulationMap() {}

void SimulationMap::setConfiguration(const ChessUpConfiguration &config)
{
    clear(PawnsGroup);
    clear(QueensGroup);
    clear(KingsGroup);

    foreach(PawnObject* pawn, config.getPawns())
        addObject(pawn, PawnsGroup);

    foreach(KingObject* king, config.getKings())
        addObject(king, KingsGroup);

    foreach(QueenObject* queen, config.getQueens())
        addObject(queen, QueensGroup);
}

void SimulationMap::configureRobotSensors()
{
    //Avoiding sharps

	addObject(new MaxSonar_MB1240(QPointF(40, -40), Tools::degreeToRadian(-10), 280), AvoidingSharpGroup);

	addObject(new MaxSonar_MB1240(QPointF(-40, -40), Tools::degreeToRadian(-170), 280), AvoidingSharpGroup);
	addObject(new MaxSonar_MB1240(QPointF(-40, 40), Tools::degreeToRadian(170), 280), AvoidingSharpGroup);

	addObject(new MaxSonar_MB1240(QPointF(40, 40), Tools::degreeToRadian(10), 280), AvoidingSharpGroup);


    //    // front
    //    addObject(new Sharp10_80(QPointF(172, -125), Tools::degreeToRadian(-5), 280), AvoidingSharpGroup);
    //    addObject(new Sharp20_150(QPointF(63, 0), 0, 280), AvoidingSharpGroup);
    //    addObject(new Sharp10_80(QPointF(172, 125), Tools::degreeToRadian(5), 280), AvoidingSharpGroup);

    //    // rear
    //    addObject(new Sharp10_80(QPointF(-130, 138), Tools::degreeToRadian(175), 280), AvoidingSharpGroup);
    //    addObject(new Sharp20_150(QPointF(-130, 0), Tools::degreeToRadian(180), 280), AvoidingSharpGroup);
    //    addObject(new Sharp10_80(QPointF(-130, -138), Tools::degreeToRadian(-175), 280), AvoidingSharpGroup);

    //Detection sharps
    // right
    addObject(new Sharp20_150(QPointF(63, -170), Tools::degreeToRadian(-90), 100), OtherSharpGroup);
    addObject(new Sharp20_150(QPointF(15, -170), Tools::degreeToRadian(-90), 25), OtherSharpGroup);

    // left
    addObject(new Sharp20_150(QPointF(63, 170), Tools::degreeToRadian(90), 100), OtherSharpGroup);
    addObject(new Sharp20_150(QPointF(15, 170), Tools::degreeToRadian(90), 25), OtherSharpGroup);

    //color sensors
    addObject(new ColorSensorObject(QVector2D(0, -150)), ColorSensorGroup);
    addObject(new ColorSensorObject(QVector2D(0,  150)), ColorSensorGroup);
}

void SimulationMap::moveRobot(const RPoint& robot)
{
    _robot->setPosition(QPointF(robot.getX(), robot.getY()));
    _robot->setRotation(robot.getTheta());

    QPointF rPos = _robot->getPosition();
    double rRot = _robot->getRotation();

    QList<Tools::DataObject *> completeSharpList = getObjects(AvoidingSharpGroup);
    completeSharpList.append(getObjects(OtherSharpGroup));
    QList<Tools::DataObject *>::iterator object;
    for (object = completeSharpList.begin(); object != completeSharpList.end(); ++object)
    {
        SharpObject* sharp = static_cast<SharpObject*>(*object);
        sharp->setPositionAndRotation(RPoint(rPos.x(), rPos.y(), rRot));
    }

    QList<Tools::DataObject *> contactorList = getObjects(ContactorGroup);
    for (object = contactorList.begin(); object != contactorList.end(); ++object)
    {
        ContactorObject* contactor = static_cast<ContactorObject*>(*object);
        contactor->setPositionAndRotation(RPoint(rPos.x(), rPos.y(), rRot));
    }

    QList<Tools::DataObject *> colorSensorList = getObjects(ColorSensorGroup);
    for (object = colorSensorList.begin(); object != colorSensorList.end(); ++object)
    {
        ColorSensorObject* cs = static_cast<ColorSensorObject*>(*object);
        cs->setPositionAndRotation(RPoint(rPos.x(), rPos.y(), rRot));
    }
}

void SimulationMap::updateSensors()
{
    QList<Tools::DataObject *> completeSharpList = getObjects(AvoidingSharpGroup);
    completeSharpList.append(getObjects(OtherSharpGroup));
    QList<Tools::DataObject *>::iterator object;

    for (object = completeSharpList.begin(); object != completeSharpList.end(); ++object)
    {
        SharpObject* sharp = static_cast<SharpObject*>(*object);

        double d = getObstacleDistanceToSharp(sharp);
        sharp->setDistance(d);
    }

     QList<Tools::DataObject *> colorSensorList = getObjects(ColorSensorGroup);
     for (QList<Tools::DataObject *>::iterator itCs = colorSensorList.begin(); itCs != colorSensorList.end(); ++itCs)
     {
         ColorSensorObject* cs = static_cast<ColorSensorObject*>(*itCs);
         QPointF p = cs->getPosition();
         if (p.x() >= 0 && p.y() >= 0)
         {
             QColor c(_colorMap.pixel(p.x(), p.y()));
             cs->setColor(c);
         }
     }


//    QList<Tools::DataObject *> contactorList = getObjects(ContactorGroup);

//    for (object = contactorList.begin(); object != contactorList.end(); ++object)
//    {
//    ContactorObject* contactor = static_cast<ContactorObject*>(*object);

//    contactor->setStatus(getContactorStatus(contactor));
}

double SimulationMap::distanceTo(DataObject* o1, DataObject* o2)
{
    if (o1->getType() == DataObject::Line && o2->getType() == DataObject::Circle)
        return distanceTo(static_cast<LineObject*>(o1), static_cast<CircleObject*>(o2));

    return -1;
}

double SimulationMap::distanceTo(LineObject* l1, CircleObject* c2)
{
    QVector2D v(l1->getLine().p1() - c2->getPosition());
    double d = v.length() - c2->getRadius();
    return (d > 0 ? d : 0);
}

double SimulationMap::distanceTo(LineObject* l1, RectangleObject* r2)
{
    //TODO
    Q_UNUSED(l1);
    Q_UNUSED(r2);
    Q_ASSERT(false);
    return 0.0;
}

double SimulationMap::distanceTo(SharpObject* sharp, int group)
{
    double shortestDistance = 3000;
    double distance = 3000;

    QList<DataObject*> objectsToChecks = _objects[group];
    for(QList<DataObject*>::const_iterator it = objectsToChecks.constBegin(); it != objectsToChecks.constEnd(); ++it)
    {
        if (collide(sharp, *it))
        {
            distance = distanceTo(sharp, *it);

            if (distance < shortestDistance)
                shortestDistance = distance;
        }
    }

    return shortestDistance;
}

double SimulationMap::getObstacleDistanceToSharp(SharpObject* sharp)
{
    double shortestDistance = 3000;
    double distance = 3000;

    sharp->setLength(sharp->getRange());

    shortestDistance = distanceTo(sharp, PawnsGroup);

    distance = distanceTo(sharp, QueensGroup);
    if (distance < shortestDistance)
        shortestDistance = distance;

    distance = distanceTo(sharp, KingsGroup);
    if (distance < shortestDistance)
        shortestDistance = distance;

    distance = distanceTo(sharp, OpponentGroup);
    if (distance < shortestDistance)
        shortestDistance = distance;

    distance = distanceTo(sharp, ObstacleGroup);
    if (distance < shortestDistance)
        shortestDistance = distance;

    distance = distanceTo(sharp, TableObstacleGroup);
    if (distance < shortestDistance)
        shortestDistance = distance;

    return shortestDistance;
}

SharpObject* SimulationMap::getSharpInClaw()
{
    return static_cast<SharpObject*>(_objects[OtherSharpGroup].last());
}

void SimulationMap::takeChessUpObjects()
{
    /*DataObject* contactor = getObjects(ContactorGroup).takeFirst();
    QList<DataObject*> list;

    collideWith(contactor, PawnsGroup, list);
    collideWith(contactor, KingsGroup, list);
    collideWith(contactor, QueensGroup, list);

    for (QList<DataObject *>::iterator object = list.begin(); object != list.end(); ++object)
    static_cast<ChessUpObject*>(*object)->take();*/
}
