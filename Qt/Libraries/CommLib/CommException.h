#ifndef COMMEXCEPTION_H
#define COMMEXCEPTION_H

#include <QByteArray>

namespace Comm
{
    class CommException
    {
    public:
        CommException(const QByteArray& description);
        const QByteArray& what() const;

    private:
        QByteArray _description;
    };
}

#endif // COMMEXCEPTION_H
