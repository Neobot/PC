#ifndef DATA_H
#define DATA_H

#include <QByteArray>
#include <QBitArray>
#include <QDataStream>
#include <QTextStream>
#include "AbstractLogger.h"

namespace Comm
{
    enum Endianness
    {
        BigEndian,
        LittleEndian
    };

    const Endianness DEFAULT_ENDIANNESS = BigEndian;


    /**
     * \brief Data is a list of bytes.
     *
     * It provides easy access functions to read and write any kind of data and behaves like a FIFO.
     * It also manage the endianness. The default endianness is LittleEndian.
     * It can also calculates the checksum of the bytes it contains.
     */
    class Data : public QByteArray
    {
    public:
        /**
         * \brief Create a instance of Data in LittleEndian.
         */
        Data();

        /**
         * \brief Create a instance of Data in \p endianness.
         */
        Data(Endianness endianness);

        Endianness getEndianess() const;
		void setEndianness(Endianness endianness);

        Data& add(const Data& value);

        /**
         * \brief Add a byte at the end of the data.
		 */
        Data& add(qint8   value);
        Data& add(qint16   value);
        Data& add(qint32   value);
		Data& add(qint64  value);
        Data& add(quint8   value);
        Data& add(quint16  value);
        Data& add(quint32  value);
		Data& add(quint64  value);
        Data& add(const QBitArray&  value);
        Data& add(bool b1, bool b2 = false, bool b3 = false,
                  bool b4 = false, bool b5 = false, bool b6 = false,
                  bool b7 = false, bool b8 = false);
		Data& add(const QByteArray &value);
		Data& add(float  value);

        /**
         * \brief read and removes a byte at the begining of the data.
         */
        Data& take(qint8&    value);
        Data& take(qint16&   value);
        Data& take(qint32&   value);
		Data& take(qint64&   value);
        Data& take(quint8&   value);
        Data& take(quint16&  value);
        Data& take(quint32&  value);
		Data& take(quint64&  value);
        Data& take(QBitArray&  value);
        Data& take(bool& b1);
        Data& take(bool& b1, bool& b2);
        Data& take(bool& b1, bool& b2, bool& b3);
        Data& take(bool& b1, bool& b2, bool& b3, bool& b4);
        Data& take(bool& b1, bool& b2, bool& b3, bool& b4, bool& b5);
        Data& take(bool& b1, bool& b2, bool& b3, bool& b4, bool& b5, bool& b6);
        Data& take(bool& b1, bool& b2, bool& b3, bool& b4, bool& b5, bool& b6, bool& b7);
        Data& take(bool& b1, bool& b2, bool& b3, bool& b4, bool& b5, bool& b6,bool& b7, bool& b8);
		Data& take(QByteArray& value, int size);
		Data& take(QByteArray& value);
		Data& take(float&  value);

        /**
         * \brief read a byte at \p pos.
         */
        bool readAt(int pos, qint8& value);
        bool readAt(int pos, qint16& value);
        bool readAt(int pos, qint32& value);
		bool readAt(int pos, qint64& value);
        bool readAt(int pos, quint8& value);
        bool readAt(int pos, quint16& value);
        bool readAt(int pos, quint32& value);
		bool readAt(int pos, quint64& value);
        bool readAt(int pos, QBitArray&  value);
		bool readAt(int pos, QByteArray&  value, int len);
		bool readAt(int pos, float&  value);

        void intToBitArray(int value, int nbBit, QBitArray& array, int arrayStartIndex) const;
        int bitArrayToInt(const QBitArray& array, int nbBit, int arrayStartIndex) const;

        /**
         * \brief Convert an integer codded on \p nbBit bits in an array of bit.
         * \see bitArrayToInt
         */
        static void intToBitArray(int value, int nbBit, QBitArray& array, int arrayStartIndex, Endianness endianness);

        /**
         * \brief Convert an \p nbBit of \p array bits in an integer.
         * \see intToBitArray
         */
        static void bitArrayToInt(const QBitArray& array, int nbBit, int& value, int arrayStartIndex, Endianness endianness);

        /**
         * \brief AX-12 Checksum
         *
         * The inverse of the sum of the bytes.
         * \see http://www.agaverobotics.com/products/servos/robotis/ax12/ax12-protocol.aspx
         */
        quint8 checksum() const;

		quint16 checksum16bits() const;

    private:
        Endianness _endianness;
    };

    QDataStream& operator<<(QDataStream& out, const Data& data);
    QTextStream& operator<<(QTextStream& out, const Data& data);
    Tools::AbstractLogger& operator<<(Tools::AbstractLogger& out, const Data& data);
}

#endif // DATA_H
