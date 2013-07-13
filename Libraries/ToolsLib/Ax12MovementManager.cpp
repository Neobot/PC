#include "Ax12MovementManager.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

using namespace Tools;

const char AX12_FILE_SEPARATOR = ';';

Ax12MovementManager::Ax12MovementManager()
{
}

void Ax12MovementManager::setGroup(const QString& name, const QList<int>& ids)
{
	_groups[name] = ids;
}

void Ax12MovementManager::removeGroup(const QString& name)
{
	_groups.remove(name);
	removeMovements(name);
}

void Ax12MovementManager::renameGroup(const QString& oldName, const QString& newName)
{
	QList<int> ids = getGroupIds(oldName);
	_groups.remove(oldName);
	setGroup(newName, ids);
	
	Ax12Movements mvts = _movements[oldName];
	_movements.remove(oldName);
	_movements[newName] = mvts;
}

bool Ax12MovementManager::containsGroup(const QString& name) const
{
	return _groups.contains(name);
}

QList<int> Ax12MovementManager::getGroupIds(const QString& groupName) const
{
	return _groups.value(groupName);
}

void Ax12MovementManager::addEmptyMovement(const QString &groupName, const QString &movementName)
{
	_movements[groupName].insert(movementName, Ax12SingleMovement());
}

void Ax12MovementManager::setMovementTorque(const QString &groupName, const QString &movementName, int positionIndex, float torque)
{
	_movements[groupName][movementName][positionIndex].info.torque = torque;
}

void Ax12MovementManager::setMovementMaxSpeed(const QString &groupName, const QString &movementName, int positionIndex, float speed)
{
	_movements[groupName][movementName][positionIndex].info.maxSpeed = speed;
}

void Ax12MovementManager::setMovementPosition(const QString &groupName, const QString &movementName, int positionIndex, int ax12Index, float position)
{
	Ax12SingleGroupPosition& pos = _movements[groupName][movementName][positionIndex];
	if (ax12Index >= 0 && ax12Index < pos.positions.count())
		pos.positions[ax12Index] = position;
}

void Ax12MovementManager::addPositionToMovement(const QString& groupName, const QString& movementName, const Ax12SingleGroupPosition& movement)
{
	_movements[groupName][movementName].append(movement);
}

void Ax12MovementManager::insertPositionToMovement(const QString &groupName, const QString &movementName, const Ax12MovementManager::Ax12SingleGroupPosition &movement, int position)
{
	_movements[groupName][movementName].insert(position, movement);
}

void Ax12MovementManager::removeMovement(const QString& groupName, const QString& movementName)
{
	_movements[groupName].remove(movementName);
}

void Ax12MovementManager::removeMovements(const QString& groupName)
{
	_movements.remove(groupName);
}

void Ax12MovementManager::removePosition(const QString &groupName, const QString &movementName, int positionIndex)
{
	_movements[groupName][movementName].removeAt(positionIndex);
}

void Ax12MovementManager::renameMovement(const QString& groupName, const QString& oldMovementName, const QString& newMovementName)
{
	Ax12SingleMovement mvt = getMovement(groupName, oldMovementName);
	_movements[groupName].remove(oldMovementName);
	_movements[groupName][newMovementName] = mvt;
}

bool Ax12MovementManager::containsMovement(const QString& groupName, const QString& movementName) const
{
	return _movements.value(groupName).contains(movementName);
}

const Ax12MovementManager::Ax12SingleMovement &Ax12MovementManager::getMovement(const QString& groupName, const QString& movementName)
{
	return _movements[groupName][movementName];
}

QList<Ax12MovementManager::DetailedPosition> Ax12MovementManager::getDetailedMovement(const QString& groupName, const QString& movementName)
{
	QList<DetailedPosition> results;
	const QList<int>& ids = getGroupIds(groupName);

	foreach(const Ax12SingleGroupPosition& singleMove, getMovement(groupName, movementName))
	{
		QMap<int, Ax12Position> positions;
		DetailedPosition positionInfo;
		for(int i = 0; i < ids.count(); ++i)
		{
			int id = ids.value(i);
			Ax12Position pos = singleMove.positions.value(i);
			positions[id] = pos;
		}

		positionInfo.first = positions;
		positionInfo.second = singleMove.info;

		results << positionInfo;
	}

	return results;
}

QStringList Ax12MovementManager::getGroupNames() const
{
	return _groups.keys();
}

QStringList Ax12MovementManager::getMovementNames(const QString &groupName) const
{
	return _movements.value(groupName).keys();
}

void Ax12MovementManager::clear()
{
	_groups.clear();
	_movements.clear();
}

bool Ax12MovementManager::writeToFile(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream out(&file);
	return writeToStream(out);
}

bool Ax12MovementManager::readFromFile(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QTextStream in(&file);
	return readFromStream(in);
}

bool Ax12MovementManager::writeToStream(QTextStream &out)
{
	for(Ax12GroupMovements::const_iterator itGroups = _movements.constBegin(); itGroups != _movements.constEnd(); ++itGroups)
	{
		const QString& groupName = itGroups.key();
		out << "G" << AX12_FILE_SEPARATOR
			<< groupName << AX12_FILE_SEPARATOR;

		const QList<int>& ids = _groups.value(groupName);
		foreach(int id, ids)
			out << id << AX12_FILE_SEPARATOR;
		out << '\n';

		for(Ax12Movements::const_iterator itMov = itGroups->constBegin(); itMov != itGroups->constEnd(); ++itMov)
		{
			const QString& movementName = itMov.key();
			out << "M" << AX12_FILE_SEPARATOR
				<< movementName << '\n';

			foreach(const Ax12SingleGroupPosition& singleMove, *itMov)
			{
				out << "P" << AX12_FILE_SEPARATOR;
				foreach(const Ax12Position& p, singleMove.positions)
				{
					out << p << AX12_FILE_SEPARATOR;
				}
				out << singleMove.info.torque << AX12_FILE_SEPARATOR
					<< singleMove.info.maxSpeed << '\n';
			}
		}
	}

	return true;
}

bool Ax12MovementManager::readFromStream(QTextStream &in)
{
	clear();

	QString currentGroup;
	QString currentMovementName;

	while(!in.atEnd())
	{
		QString line = in.readLine();
		if (line.startsWith("#")) //comments
			continue;

		QStringList tokens = line.split(AX12_FILE_SEPARATOR);
		if (tokens.value(0) == "G")
		{
			if (tokens.count() >= 2)
			{
				currentGroup = tokens.value(1);
				QList<int> ids;
				for(int i = 2; i < tokens.count(); ++i)
				{
					if (!tokens.value(i).isEmpty())
						ids << tokens.value(i).toInt();
				}

				setGroup(currentGroup, ids);
			}
		}
		else if (tokens.value(0) == "M")
		{
			if (tokens.count() >= 2)
			{
				currentMovementName = tokens.value(1);
				addEmptyMovement(currentGroup, currentMovementName);
			}
		}
		else if (tokens.value(0) == "P")
		{
			if (!tokens.isEmpty() && tokens.last().isEmpty())
				tokens.erase(tokens.end() - 1);

			if (tokens.count() >= 3)
			{
				Ax12SingleGroupPosition singleMovement;
				for(int i = 1; i < tokens.count() - 2; ++i)
				{
					QString t = tokens.value(i);
					if (!t.isEmpty())
					{
						Ax12Position pos = t.toFloat();
						singleMovement.positions << pos;
					}
				}

				singleMovement.info.torque = tokens.value(tokens.count() - 2).toFloat();
				singleMovement.info.maxSpeed = tokens.last().toFloat();

				addPositionToMovement(currentGroup, currentMovementName, singleMovement);
			}
		}
	}

	return true;
}
