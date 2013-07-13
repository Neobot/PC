#ifndef ZRANGE_H
#define ZRANGE_H

class ZRange
{
public:
    ZRange(double zBottom = 0.0, double zTop = 999.0);
    ZRange(const ZRange& other);

    bool isInRange(double z) const;
    bool intersected(const ZRange& range) const;
    ZRange intersection(const ZRange& range) const;

    bool operator==(const ZRange& other) const;

    double height() const;

private:
    double _zBottom;
    double _zTop;
};

#endif // ZRANGE_H
