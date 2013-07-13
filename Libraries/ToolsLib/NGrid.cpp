#include "NGrid.h"
#include "ToolsLib.h"
#include <cmath>

#include <QtDebug>

using namespace Tools;

const char NGRID_FILE_SEPARATOR = ';';

const char* Tools::NGrid::NODE_SECTION_TITLE = "nodes";
const char* Tools::NGrid::AREAS_SECTION_TITLE = "areas";
const char* Tools::NGrid::LINNKS_SECTION_TITLE = "links";

AbstractLogger& Tools::operator<<(AbstractLogger& logger, const NGridNode& node)
{
    logger << "Node(" << node.getPosition().x() << "," << node.getPosition().y() << ")";
    return logger;
}

Tools::NGridNode::NGridNode(const QPointF &position) : _pos(position)
{
}

Tools::NGridNode::~NGridNode()
{
}

void Tools::NGridNode::addReachableNode(Tools::NGridNode *node, int costToGo)
{
    _reachableNodes[node] = costToGo;
    node->_revertedLinks << this;
}

void Tools::NGridNode::removeReachableNode(Tools::NGridNode *node)
{
    _reachableNodes.remove(node);
    node->_revertedLinks.removeAll(this);
}

void Tools::NGridNode::removeAllConnections()
{
    QHash<NGridNode*, int> c(_reachableNodes);
    for(QHash<NGridNode*, int>::const_iterator it = c.constBegin(); it != c.constEnd(); ++it)
        removeReachableNode(it.key());

    foreach(NGridNode* node, _revertedLinks)
        node->removeReachableNode(this);

    _revertedLinks.clear();
}

void Tools::NGridNode::setReachableNodeCostToGo(Tools::NGridNode *node, int cost)
{
    if (_reachableNodes.contains(node))
        _reachableNodes[node] = cost;
}


int Tools::NGridNode::getCostToGo(Tools::NGridNode *node) const
{
    return _reachableNodes.value(node, -1);
}


const QPointF & Tools::NGridNode::getPosition() const
{
    return _pos;
}

void Tools::NGridNode::setPosition(const QPointF &position)
{
    _pos = position;
}

const QHash<Tools::NGridNode *, int> & Tools::NGridNode::getReachableNodes() const
{
    return _reachableNodes;
}

const QList<Tools::NGridNode *> & Tools::NGridNode::getRevertedConnections() const
{
    return _revertedLinks;
}

//------------------------------------------------------------------------------------------------

NGrid::NGrid()
{
}

Tools::NGrid::~NGrid()
{
    qDeleteAll(_areas);
    _areas.clear();

    qDeleteAll(_nodes);
    _nodes.clear();
}

void Tools::NGrid::addNode(Tools::NGridNode *node)
{
    _nodes << node;
    for(QHash<QString, NGridArea*>::iterator it = _areas.begin(); it != _areas.end(); ++it)
    {
        NGridArea* area = it.value();
        if (area->getRect().contains(node->getPosition()))
            area->_nodes.append(node);
    }
}

NGridNode* Tools::NGrid::addNode(const QPointF& position)
{
    NGridNode* node = new NGridNode(position);
    addNode(node);

    return node;
}

void Tools::NGrid::removeNode(Tools::NGridNode *node)
{
    node->removeAllConnections();
    _nodes.removeAll(node);
    removeAlias(node);
    for(QHash<QString, NGridArea*>::iterator it = _areas.begin(); it != _areas.end(); ++it)
    {
        NGridArea* area = it.value();
        if (area->contains(node))
            area->_nodes.removeAll(node);
    }
}

void Tools::NGrid::clear()
{
    _areas.clear();

    qDeleteAll(_nodes);
    _nodes.clear();
    _alias.clear();
}

void Tools::NGrid::addBidirectionalLink(Tools::NGridNode *n1, Tools::NGridNode *n2)
{
    n1->addReachableNode(n2);
    n2->addReachableNode(n1);
}

void Tools::NGrid::removeBidirectionalLink(Tools::NGridNode *n1, Tools::NGridNode *n2)
{
    n1->removeReachableNode(n2);
    n2->removeReachableNode(n1);
}

void NGrid::setGlobalCost(int cost)
{
    foreach(NGridNode* node, _nodes)
    {
        for(QHash<NGridNode*, int>::const_iterator it = node->getReachableNodes().constBegin(); it != node->getReachableNodes().constEnd(); ++it)
            node->setReachableNodeCostToGo(it.key(), cost);
    }
}

void Tools::NGrid::addAlias(Tools::NGridNode *node, const QString &alias)
{
    _alias.insert(alias, node);
}

void Tools::NGrid::removeAlias(NGridNode* node)
{
    _alias.remove(_alias.key(node));
}

QString Tools::NGrid::getAlias(Tools::NGridNode *node)
{
    return _alias.key(node);
}

Tools::NGridNode * Tools::NGrid::getNode(const QString &alias) const
{
    return _alias.value(alias, 0);
}

const QList<Tools::NGridNode *> & Tools::NGrid::getNodes() const
{
    return _nodes;
}

Tools::NGridNode * Tools::NGrid::getNearestNode(const QPointF &position) const
{
    NGridNode* nearestNode = 0;
    double smallerDistance = -1.0;

    if (!_nodes.isEmpty())
    {
        smallerDistance = (_nodes.first()->getPosition() - position).manhattanLength();
        nearestNode = _nodes.first();
    }

    foreach(NGridNode* node, _nodes)
    {
        QPointF diff(node->getPosition());
        diff -= position;

        if (diff.isNull())
            return node;

        double d = diff.manhattanLength();
        if (d < smallerDistance)
        {
            smallerDistance = d;
            nearestNode = node;
        }
    }

    return nearestNode;
}

void Tools::NGrid::makeStandardGrid(const QPointF &origin, double dx, double dy, const QSizeF &size, Tools::NGrid::StandardGridConnectionType connectionType)
{
    clear();

    int nbNodeX = floor(size.width()/dx);
    int nbNodeY = floor(size.height()/dy);

    QHash<QPoint, NGridNode*> tmpNodeMap;

    //1st pass: node creation
    for (int x = 0; x <= nbNodeX; ++x)
    {
        for (int y = 0; y <= nbNodeY; ++y)
        {
            NGridNode* insertedNode = addNode(origin + QPointF(x * dx, y * dy));
            tmpNodeMap.insert(QPoint(x, y), insertedNode);
        }
    }

    if (connectionType != NoConnection)
    {
        //2nd pass: Connections
        for (int x = 0; x <= nbNodeX; ++x)
        {
            for (int y = 0; y <= nbNodeY; ++y)
            {
                NGridNode* currentNode = tmpNodeMap.value(QPoint(x, y), 0);
                Q_ASSERT(currentNode);

                NGridNode* rightNode = tmpNodeMap.value(QPoint(x + 1, y), 0);
                if (rightNode)
                    addBidirectionalLink(currentNode, rightNode);

                NGridNode* upNode = tmpNodeMap.value(QPoint(x, y + 1), 0);
                if (upNode)
                    addBidirectionalLink(currentNode, upNode);

                if (connectionType == HeightConnections)
                {
                    NGridNode* rightUpNode = tmpNodeMap.value(QPoint(x + 1, y + 1), 0);
                    if (rightUpNode)
                        addBidirectionalLink(currentNode, rightUpNode);

                    NGridNode* leftUpNode = tmpNodeMap.value(QPoint(x - 1, y + 1), 0);
                    if (leftUpNode)
                        addBidirectionalLink(currentNode, leftUpNode);
                }
            }
        }
    }
}

bool Tools::NGrid::writeToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

    //Writes the nodes
    out << "nodes" << NGRID_FILE_SEPARATOR << _nodes.count() << NGRID_FILE_SEPARATOR << '\n';
    int nodeIndex = 0;
    foreach(NGridNode* node, _nodes)
    {
        out << nodeIndex++              << NGRID_FILE_SEPARATOR
            << node->getPosition().x()  << NGRID_FILE_SEPARATOR
            << node->getPosition().y()  << NGRID_FILE_SEPARATOR
            << _alias.key(node)         << NGRID_FILE_SEPARATOR
            << '\n';
    }

    out << "areas" << NGRID_FILE_SEPARATOR << _areas.count() << NGRID_FILE_SEPARATOR << '\n';
    for(QHash<QString, NGridArea*>::const_iterator it = _areas.constBegin(); it != _areas.constEnd(); ++it)
    {
        const NGridArea* a = it.value();
        out << a->getRect().x()      << NGRID_FILE_SEPARATOR
            << a->getRect().y()      << NGRID_FILE_SEPARATOR
            << a->getRect().width()  << NGRID_FILE_SEPARATOR
            << a->getRect().height() << NGRID_FILE_SEPARATOR
            << it.key()             << NGRID_FILE_SEPARATOR
            << '\n';
    }

    out << "links" << '\n';
    nodeIndex = 0;
    foreach(NGridNode* node, _nodes)
    {
        for(QHash<NGridNode*, int>::const_iterator it = node->getReachableNodes().constBegin();
            it != node->getReachableNodes().constEnd(); ++it)
        {
            out << nodeIndex                << NGRID_FILE_SEPARATOR
                << _nodes.indexOf(it.key()) << NGRID_FILE_SEPARATOR
                << it.value()               << NGRID_FILE_SEPARATOR
                << '\n';
        }

        ++nodeIndex;
    }

    return true;
}

bool Tools::NGrid::readFromFile(const QString &filename, bool reverse)
{
    clear();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    //Read the nodes
    QList<QByteArray> firstLineTokens = file.readLine().split(NGRID_FILE_SEPARATOR);
    if (firstLineTokens.count() < 2)
        return false;

    QString section = firstLineTokens[0];
    if (section.compare(NODE_SECTION_TITLE, Qt::CaseInsensitive) != 0)
        return false;

    int nbOfNode = firstLineTokens[1].toInt();

    int nodeIndex = 0;
    while (!file.atEnd() && nodeIndex < nbOfNode)
    {
        QList<QByteArray> tokens = file.readLine().split(NGRID_FILE_SEPARATOR);
        if (tokens.count() < NODES_NB_FIELDS)
        {
            //Invalid format, node ignored
            qDebug() << "Inconsistency found at node #" << nodeIndex;
        }
        else
        {
            NGridNode* node = addNode(Tools::autoMirror(QPointF(tokens.value(NODE_X).toDouble(), tokens.value(NODE_Y).toDouble()), reverse));
            QByteArray alias = tokens.value(NODE_ALIAS);
            if (!alias.isEmpty())
                addAlias(node, alias);
        }

        ++nodeIndex;
    }

    firstLineTokens = file.readLine().split(NGRID_FILE_SEPARATOR);
    if (firstLineTokens.count() < 1)
        return false;

    section = firstLineTokens[0];

    if (section.compare(AREAS_SECTION_TITLE, Qt::CaseInsensitive) == 0)
    {
        if (firstLineTokens.count() < 1)
            return false;
        int nbOfArea = firstLineTokens[1].toInt();

        int areaIndex = 0;
        while (!file.atEnd() && areaIndex < nbOfArea)
        {
            QList<QByteArray> tokens = file.readLine().split(NGRID_FILE_SEPARATOR);
            if (tokens.count() < AREA_NB_FIELDS)
            {
                //Invalid format, area ignored
                qDebug() << "Inconsistency found at area #" << areaIndex;
            }
            else
            {
                QString name = tokens.value(AREA_NAME);
		QPointF p = Tools::autoMirror(QPointF(tokens.value(AREA_POS_X).toDouble(), tokens.value(AREA_POS_Y).toDouble()), reverse);
		QSizeF s(tokens.value(AREA_WIDTH).toDouble(), tokens.value(AREA_HEIGHT).toDouble());
		if (reverse)
		    p.ry() -= s.height();

		QRectF rect(p, s);

                if (!name.isEmpty() && rect.isValid())
                    addArea(rect, name);
                else
                {
                    //Invalid data, area ignored
                    qDebug() << "Invalid area at area line #" << areaIndex;
                }
            }

            ++areaIndex;
        }
    }

    section = file.readLine(); //read the "links" line

    if (section.startsWith(LINNKS_SECTION_TITLE, Qt::CaseInsensitive))
    {
        //Read the links
        while (!file.atEnd())
        {
            QList<QByteArray> tokens = file.readLine().split(NGRID_FILE_SEPARATOR);
            if (tokens.count() < LINKS_NB_FIELDS)
            {
                qDebug() << "Inconsistency found on a link";
                continue; //Invalid format, node ignored
            }

            NGridNode* node = _nodes.value(tokens[LINK_START_NODE_INDEX].toInt());
            NGridNode* reachableNode = _nodes.value(tokens[LINK_END_NODE_INDEX].toInt());

            //qDebug() << node->getPosition() << "-" << reachableNode->getPosition();

            if (node && reachableNode)
                node->addReachableNode(reachableNode, tokens[COST_TO_GO].toInt());
            else
                qDebug() << "Warning: unconnected node";

        }
    }


    return true;
}

int Tools::NGrid::indexOf(Tools::NGridNode *node) const
{
    return _nodes.indexOf(node);
}

bool Tools::NGrid::contains(Tools::NGridNode *node) const
{
    return _nodes.contains(node);
}

bool Tools::NGrid::nodesAreAligned(Tools::NGridNode *c1, Tools::NGridNode *c2, Tools::NGridNode *c3) const
{
    return c1->getPosition().x() - c2->getPosition().x() == c2->getPosition().x() - c3->getPosition().x()
            && c1->getPosition().y() - c2->getPosition().y() == c2->getPosition().y() - c3->getPosition().y();
}

void NGrid::addArea(const QRectF &rect, const QString &name)
{
    _areas[name] = new NGridArea(rect, this);
}

NGridArea *NGrid::getArea(const QString &name) const
{
    return _areas[name];
}

void NGrid::updateAreas()
{
    for(QHash<QString, NGridArea*>::iterator it = _areas.begin(); it != _areas.end(); ++it)
    {
        NGridArea* area = it.value();
        area->update();
    }
}

const QHash<QString, NGridArea *> &NGrid::getAreas() const
{
    return _areas;
}

NGridArea* NGrid::removeArea(const QString &name)
{
    if (!_areas.contains(name))
        return 0;

    return _areas.take(name);
}

void NGrid::renameArea(const QString &oldName, const QString &newName)
{
    NGridArea* a = _areas.value(oldName, 0);
    if (!a)
        return;
    _areas.remove(oldName);
    _areas[newName] = a;
}



