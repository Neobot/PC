#include "AbstractAction.h"

AbstractAction::AbstractAction(QObject* parent) : QObject(parent)
{
}

void AbstractAction::succeed()
{
    emit finished(true);
}

void AbstractAction::failed()
{
    emit finished(false);
}
