#include "ZRange.h"
#include <QtGlobal>

ZRange::ZRange(double zBottom, double zTop) : _zBottom(zBottom), _zTop(zTop)
{
}

ZRange::ZRange(const ZRange& other) : ZRange(other._zBottom, other._zTop)
{
}

bool ZRange::isInRange(double z) const
{
    return z >= _zBottom && z <= _zTop;
}

bool ZRange::intersected(const ZRange& range) const
{
    return !(range._zBottom >= _zTop || _zBottom >= range._zTop);
}

ZRange ZRange::intersection(const ZRange& range) const
{
    if (intersected(range))
        return ZRange(qMax(_zBottom, range._zBottom), qMin(_zTop, range._zTop));
    else
        return ZRange(0.0, 0.0);
}

bool ZRange::operator==(const ZRange& other) const
{
    return other._zBottom == _zBottom && other._zTop == _zTop;
}

double ZRange::height() const
{
    return _zTop - _zBottom;
}
