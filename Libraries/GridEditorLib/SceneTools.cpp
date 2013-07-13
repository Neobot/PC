#include "SceneTools.h"
#include "NGrid.h"

QPointF SceneTools::mapWithStandardReference(const QPointF &point)
{
    return QPointF(point.y(), point.x());
}

QPointF SceneTools::getNodeScenePosition(Tools::NGridNode *node)
{
    return mapWithStandardReference(node->getPosition());
}

QSizeF SceneTools::mapWithStandardReference(const QSizeF &size)
{
    return QSizeF(size.height(), size.width());
}

QRectF SceneTools::mapWithStandardReference(const QRectF &rect)
{
    return QRectF(mapWithStandardReference(rect.topLeft()), mapWithStandardReference(rect.size()));
}
