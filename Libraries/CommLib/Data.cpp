#include "Data.h"
#include <QtDebug>

using namespace Comm;

Data::Data() : _endianness(DEFAULT_ENDIANNESS)
{
}

Data::Data(Endianness endianness) : _endianness(endianness)
{
}

Endianness Data::getEndianess() const
{
    return _endianness;
}

void Data::setEndianness(Endianness endianness)
{
	_endianness = endianness;
}

quint8 Data::checksum() const
{
    int checksum = 0;
    for(int i = 0; i < size(); ++i)
        checksum += at(i);

    quint8 trucatedChecksum = checksum;
    return 255 - trucatedChecksum;
}

quint16 Data::checksum16bits() const
{
	quint16 checksum = 0;
	for(int i = 0; i < size(); ++i)
		checksum += (quint8)at(i);

	return checksum;
}

//-----------

Data& Data::add(const Data& value)
{
    append(value);
	return *this;
}

Data& Data::add(qint8   value)
{
   append((char)value);
   return *this;
}

Data& Data::add(qint16   value)
{
    qint8 b1 = value;
    qint8 b2 = value >> 8;

    switch(_endianness)
    {
    case LittleEndian:
        add(b1).add(b2); break;
    case BigEndian:
        add(b2).add(b1); break;
    }
    return *this;
}

Data& Data::add(qint32   value)
{
    qint8 b1 = value;
    qint8 b2 = value >> 8;
    qint8 b3 = value >> 16;
    qint8 b4 = value >> 24;

    switch(_endianness)
    {
    case LittleEndian:
        add(b1).add(b2).add(b3).add(b4); break;
    case BigEndian:
        add(b4).add(b3).add(b2).add(b1); break;
    }
	return *this;
}

Data &Data::add(qint64 value)
{
	qint8 b1 = value;
	qint8 b2 = value >> 8;
	qint8 b3 = value >> 16;
	qint8 b4 = value >> 24;
	qint8 b5 = value >> 32;
	qint8 b6 = value >> 40;
	qint8 b7 = value >> 48;
	qint8 b8 = value >> 56;

	switch(_endianness)
	{
	case LittleEndian:
		add(b1).add(b2).add(b3).add(b4).add(b5).add(b6).add(b7).add(b8); break;
	case BigEndian:
		add(b8).add(b7).add(b6).add(b5).add(b4).add(b3).add(b2).add(b1); break;
	}
	return *this;
}

Data& Data::add(quint8   value)
{
    return add((qint8) value);
}

Data& Data::add(quint16  value)
{
    return add((qint16) value);
}

Data& Data::add(quint32  value)
{
	return add((qint32) value);
}

Data &Data::add(quint64 value)
{
	return add((qint64) value);
}

Data& Data::add(const QBitArray&  value)
{
    int size = value.size();
    int result;

    bitArrayToInt(value, size, result, 0, _endianness);

    if (size <= 8)
        return add((qint8)result);
    else if (size <= 16)
        return add((qint16)result);
    else
        return add((qint32)result);
}

Data& Data::add(bool b1, bool b2, bool b3,
                bool b4, bool b5, bool b6,
                bool b7, bool b8)
{
    QBitArray bits(8, false);
    bits[0] = b1;
    bits[1] = b2;
    bits[2] = b3;
    bits[3] = b4;
    bits[4] = b5;
    bits[5] = b6;
    bits[6] = b7;
    bits[7] = b8;

	return add(bits);
}

Data &Data::add(const QByteArray &value)
{
	add((quint8)value.count());
	append(value);
	return *this;
}

Data &Data::add(float value)
{
	quint32 intValue = *((quint32*)&value);
	return add(intValue);
}

//------------

Data& Data::take(qint8&    value)
{
    if (readAt(0, value))
        remove(0,1);
    return *this;
}

Data& Data::take(qint16&   value)
{
    if (readAt(0, value))
        remove(0,2);
    return *this;
}

Data& Data::take(qint32&   value)
{
    if (readAt(0, value))
        remove(0,4);
	return *this;
}

Data &Data::take(qint64 &value)
{
	if (readAt(0, value))
		remove(0,8);
	return *this;
}

Data& Data::take(quint8&   value)
{
    return take((qint8&)value);
}

Data& Data::take(quint16&  value)
{
    return take((qint16&)value);
}

Data& Data::take(quint32&  value)
{
	return take((qint32&)value);
}

Data &Data::take(quint64 &value)
{
	return take((qint64&)value);
}

Data& Data::take(QBitArray&  value)
{
    if (readAt(0, value))
        remove(0,qRound((qreal)value.size() / 8.0) + 0.5);
    return *this;
}

Data& Data::take(bool& b1)
{
	QBitArray bits(8, false);
    take(bits);
    b1 = bits[0];
    return *this;
}
Data& Data::take(bool& b1, bool& b2)
{
    QBitArray bits(8, false);
    take(bits);
    b1 = bits[0];
    b2 = bits[1];
    return *this;
}
Data& Data::take(bool& b1, bool& b2, bool& b3)
{
    QBitArray bits(8, false);
    take(bits);
    b1 = bits[0];
    b2 = bits[1];
    b3 = bits[2];
    return *this;
}
Data& Data::take(bool& b1, bool& b2, bool& b3, bool& b4)
{
    QBitArray bits(8, false);
    take(bits);
    b1 = bits[0];
    b2 = bits[1];
    b3 = bits[2];
    b4 = bits[3];
    return *this;
}
Data& Data::take(bool& b1, bool& b2, bool& b3, bool& b4, bool& b5)
{
    QBitArray bits(8, false);
    take(bits);
    b1 = bits[0];
    b2 = bits[1];
    b3 = bits[2];
    b4 = bits[3];
    b5 = bits[4];
    return *this;
}
Data& Data::take(bool& b1, bool& b2, bool& b3, bool& b4, bool& b5, bool& b6)
{
    QBitArray bits(8, false);
    take(bits);
    b1 = bits[0];
    b2 = bits[1];
    b3 = bits[2];
    b4 = bits[3];
    b5 = bits[4];
    b6 = bits[5];
    return *this;
}
Data& Data::take(bool& b1, bool& b2, bool& b3, bool& b4, bool& b5, bool& b6, bool& b7)
{
    QBitArray bits(8, false);
    take(bits);
    b1 = bits[0];
    b2 = bits[1];
    b3 = bits[2];
    b4 = bits[3];
    b5 = bits[4];
    b6 = bits[5];
    b7 = bits[6];
    return *this;
}
Data& Data::take(bool& b1, bool& b2, bool& b3, bool& b4, bool& b5, bool& b6, bool& b7, bool& b8)
{
    QBitArray bits(8, false);
    take(bits);
    b1 = bits[0];
    b2 = bits[1];
    b3 = bits[2];
    b4 = bits[3];
    b5 = bits[4];
    b6 = bits[5];
    b7 = bits[6];
    b8 = bits[7];
	return *this;
}

Data &Data::take(QByteArray &value, int size)
{
	if (readAt(0, value, size))
		remove(0, size);
	return *this;
}

Data &Data::take(QByteArray &value)
{
	quint8 size = 0;
	if (readAt(0, size) && readAt(1, value, size))
		remove(0, size + 1);
	return *this;
}

Data &Data::take(float &value)
{
	if (readAt(0, value))
		remove(0,4); //32 bits
	return *this;
}

//------------

bool Data::readAt(int pos, qint8& value)
{
    if (pos < 0 || size() <= pos)
        return false;

    value = at(pos);
    return true;
}

bool Data::readAt(int pos, qint16& value)
{
    if (pos < 0 || size() <= pos + 1)
        return false;

    quint8 b1 = at(pos);
    quint8 b2 = at(pos + 1);

    switch(_endianness)
    {
    case LittleEndian:
        value = (b2 << 8) + b1; break;
    case BigEndian:
        value = (b1 << 8) + b2; break;
    }

    return true;
}

bool Data::readAt(int pos, qint32& value)
{
    if (pos < 0 || size() <= pos + 3)
        return false;

    quint8 b1 = at(pos);
    quint8 b2 = at(pos + 1);
    quint8 b3 = at(pos + 2);
    quint8 b4 = at(pos + 3);

    switch(_endianness)
    {
    case LittleEndian:
        value = (b4 << 24) + (b3 << 16) + (b2 << 8) + b1; break;
    case BigEndian:
        value = (b1 << 24) + (b2 << 16) + (b3 << 8) + b4; break;
    }

	return true;
}

bool Data::readAt(int pos, qint64 &value)
{
	if (pos < 0 || size() <= pos + 7)
		return false;

	quint8 b1 = at(pos);
	quint8 b2 = at(pos + 1);
	quint8 b3 = at(pos + 2);
	quint8 b4 = at(pos + 3);
	quint8 b5 = at(pos + 4);
	quint8 b6 = at(pos + 5);
	quint8 b7 = at(pos + 6);
	quint8 b8 = at(pos + 7);

	quint64 v1 = 0;
	quint64 v2 = 0;

	switch(_endianness)
	{
	case LittleEndian:
		v1 = (b4 << 24) + (b3 << 16) + (b2 << 8) + b1;
		v2 = (b8 << 24) + (b7 << 16) + (b6 << 8) + b5;
		value = v2 * (1 << 31) * 2 + v1;
	case BigEndian:
		v1 = (b1 << 24) + (b2 << 16) + (b3 << 8) + b4;
		v2 = (b5 << 24) + (b6 << 16) + (b7 << 8) + b8; break;
		value = v1 * (1 << 31) * 2 + v2;
	}

	return true;
}

bool Data::readAt(int pos, quint8& value)
{
    return readAt(pos, (qint8&)value);
}

bool Data::readAt(int pos, quint16& value)
{
    return readAt(pos, (qint16&)value);
}

bool Data::readAt(int pos, quint32& value)
{
	return readAt(pos, (qint32&)value);
}

bool Data::readAt(int pos, quint64 &value)
{
	return readAt(pos, (qint64&)value);
}

bool Data::readAt(int pos, QBitArray&  value)
{  
    if (pos < 0 || size() <= pos)
        return false;

    if (value.size() == 0)
        return false;

    int nbBitToRead = value.size();

    int currentPos = pos;
    int currentbit = 0;
    while(currentPos < size() && currentbit < nbBitToRead)
    {
        quint8 byte = at(currentPos);
        int nbBitToGo = qMin(8, nbBitToRead - currentbit);
        intToBitArray(byte, nbBitToGo, value, currentbit, _endianness);
        currentbit += nbBitToGo;
        ++currentPos;
    }

    return true;
}

bool Data::readAt(int pos, QByteArray&  value, int len)
{
	if (pos < 0 || size() < pos)
		return false;

	value = mid(pos, len);
	return true;
}

bool Data::readAt(int pos, float &value)
{
	quint32 intValue;
	if (!readAt(pos, intValue))
		return false;

	value = *((float*)&intValue);

	return true;
}

//------------

void Data::intToBitArray(int value, int nbBit, QBitArray& array, int arrayStartIndex, Endianness endianness)
{
    if (array.size() < arrayStartIndex + nbBit - 1)
        array.resize(arrayStartIndex + nbBit);

    int powerOfTwo = 1;    
	int index = (endianness == BigEndian ? arrayStartIndex : arrayStartIndex + nbBit - 1);
    for(int i = 0; i < nbBit; ++i)
    {

        array.setBit(index, (value & powerOfTwo) == powerOfTwo);
        powerOfTwo *= 2;
		if (endianness == BigEndian)
            ++index;
        else
            --index;       
    }
}

void Data::bitArrayToInt(const QBitArray& array, int nbBit, int& value, int arrayStartIndex, Endianness endianness)
{
    int size = array.size();

    value = 0;
    int powerOfTwo = 1;
    int lastIndex = qMin(size - arrayStartIndex, nbBit);
	int index = (endianness == BigEndian ? arrayStartIndex : arrayStartIndex + lastIndex - 1);
    for(int i = 0; i < lastIndex; ++i)
    {
        value += array.at(index) * powerOfTwo;
        powerOfTwo *= 2;
		if (endianness == BigEndian)
            ++index;
        else
            --index;
    }
}

void Data::intToBitArray(int value, int nbBit, QBitArray& array, int arrayStartIndex) const
{
    intToBitArray(value, nbBit, array, arrayStartIndex, _endianness);
}

int Data::bitArrayToInt(const QBitArray& array, int nbBit, int arrayStartIndex) const
{
    int value;
    bitArrayToInt(array, nbBit, value, arrayStartIndex, _endianness);
    return value;
}

//------------

QDataStream& Comm::operator<<(QDataStream& out, const Data& data)
{
    for(int i = 0; i < data.size(); ++i)
        out << (quint8)data.at(i);

    return out;
}

QTextStream& Comm::operator<<(QTextStream& out, const Data& data)
{
    for(int i = 0; i < data.size(); ++i)
        out << (quint8)data.at(i) << " ";

    return out;
}

Tools::AbstractLogger& Comm::operator<<(Tools::AbstractLogger& out, const Data& data)
{
    for(int i = 0; i < data.size(); ++i)
        out << (quint8)data.at(i) << " ";

    return out;
}
