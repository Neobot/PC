#ifndef SCENETOOLS_H
#define SCENETOOLS_H

#include <QPointF>
#include <QSizeF>
#include <QRectF>

namespace Tools
{
    class NGridNode;
}

class SceneTools
{
public:
    enum ConnectionType
    {
        Normal,        // 1  -> 2
        Reverted,      // 1 <-  2
        Both           // 1 <-> 2
    };

    enum Mode
    {
        SelectionMode,
        ConnectionMode,
        AreaMode
    };

    static QPointF mapWithStandardReference(const QPointF& point);
    static QPointF getNodeScenePosition(Tools::NGridNode* node);
    static QSizeF mapWithStandardReference(const QSizeF& size);
    static QRectF mapWithStandardReference(const QRectF& rect);
};

#endif // SCENETOOLS_H
