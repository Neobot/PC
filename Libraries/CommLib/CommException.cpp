#include "CommException.h"

using namespace Comm;

CommException::CommException(const QByteArray& description) : _description(description)
{
}

const QByteArray& CommException::what() const
{
    return _description;
}
