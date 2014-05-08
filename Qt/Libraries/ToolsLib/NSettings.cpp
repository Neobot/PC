#include "NSettings.h"

#include <QFile>
#include <QStringList>
#include <QRectF>

using namespace Tools;

const QString VERSION = "1.0";

NSettings::NSettings() : _hasChanges(false)
{
}

bool NSettings::loadFrom(const QString &file)
{
	QFile f(file);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QXmlStreamReader in(&f);

	while (!in.atEnd())
	{
		in.readNext();

		if (in.isStartElement())
		{
			const QStringRef& startName = in.name();
			const QStringRef& startVersion = in.attributes().value("version");

			if (startName == "NSettings" && startVersion == VERSION)
			{
				while (!in.atEnd())
				{
					in.readNext();

					if (in.isStartElement() && in.name() == "Group")
					{
						QString groupName = in.attributes().value("name").toString();
						beginGroup(groupName);
					}

					if (in.isEndElement() && in.name() == "Group")
						endGroup();

					if (in.isStartElement() && in.name() == "Parameter")
					{
						QString name = in.attributes().value("name").toString();
						QString description = in.attributes().value("description").toString();
						int type = in.attributes().value("content_type").toString().toInt();
						bool readOnly = in.attributes().value("read-only").toString() == "yes";

						QVariant value;
						QByteArray valueData = stringToData(in.readElementText());
						QDataStream dataStream(valueData);
						dataStream.setVersion(QDataStream::Qt_4_6);
						dataStream >> value;

						Parameter p;
						p.value = value;
						p.description = description;
						p.contentType = (QVariant::Type)type;
						p.readOnly = readOnly;

						_values[_currentGroup][name] = p;
					}
				}
			}
			else
				return false;
		}
	}

	return in.error();
}

bool NSettings::writeTo(const QString &file)
{
	QFile f(file);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QXmlStreamWriter out(&f);
	out.writeStartDocument();

	out.writeStartElement("NSettings");
	out.writeAttribute("version", "1.0");

	for(ParameterMap::const_iterator itGroup = _values.constBegin(); itGroup != _values.constEnd(); ++itGroup)
	{
		const QString& group = itGroup.key();
		bool isGroup = !group.isEmpty();

		if (isGroup)
		{
			out.writeStartElement("Group");
			out.writeAttribute("name", group);
		}

		for(ParameterGroup::const_iterator it = itGroup.value().constBegin(); it != itGroup.value().constEnd(); ++it)
		{
			out.writeStartElement("Parameter");

			const QString& name = it.key();
			const Parameter& p = *it;

			out.writeAttribute("name", name);
			out.writeAttribute("description", p.description);
			out.writeAttribute("content_type", QString::number(p.contentType));
			if (p.readOnly)
				out.writeAttribute("read-only", "yes");

			QByteArray valueData;
			QDataStream dataStream(&valueData, QIODevice::WriteOnly);
			dataStream.setVersion(QDataStream::Qt_4_6);
			dataStream << p.value;
			out.writeCharacters(dataToString(valueData));

			out.writeEndElement();
		}

		if (isGroup)
			out.writeEndElement();
	}

	out.writeEndElement();
	out.writeEndDocument();

	return true;
}

void NSettings::beginGroup(const QString &groupName)
{
	_currentGroup = groupName;
}

void NSettings::endGroup()
{
	_currentGroup.clear();
}

void NSettings::replaceValue(const QString &name, const QVariant &value)
{
	Parameter& p = _values[_currentGroup][name];
	p.value = value;

	_hasChanges = true;
}

void NSettings::setValue(const QString &name, const QVariant &value, const QString &description, bool readOnly)
{
	Q_ASSERT_X(value.type() != QVariant::List, "NSettings", "Use #setValueList to add a list parameter.");

	Parameter& p = _values[_currentGroup][name];

	p.value = value;
	p.contentType = value.type();
	p.description = description;
	p.readOnly = readOnly;

	_hasChanges = true;
}

void NSettings::setValueList(const QString &name, const QVariantList &value, QVariant::Type contentType, const QString &description, bool readOnly)
{
	Parameter p;
	p.value = value;
	p.description = description;
	p.contentType = contentType;
	p.readOnly = readOnly;

	_values[_currentGroup][name] = p;

	_hasChanges = true;
}

QVariant NSettings::value(const QString &name, const QVariant &defaultValue) const
{
	if (!contains(name))
		return defaultValue;

	return _values[_currentGroup].value(name).value;
}

QString NSettings::description(const QString &name) const
{
	return _values[_currentGroup].value(name).description;
}

void NSettings::updateDescription(const QString &name, const QString &description)
{
	if (contains(name))
	{
		QString oldDescription = _values[_currentGroup][name].description;
		if (oldDescription != description)
		{
			_values[_currentGroup][name].description = description;
			_hasChanges = true;
		}
	}
}

bool NSettings::contains(const QString &name) const
{
	return _values[_currentGroup].contains(name);
}

bool NSettings::isReadOnly(const QString &name) const
{
	return _values[_currentGroup].value(name).readOnly;
}

void NSettings::setDeprecated(const QString &name)
{
	if (contains(name))
	{
		Parameter& p = _values[_currentGroup][name];
		p.readOnly = true;
		p.value = "DEPRECATED";
		p.description = "DEPRECATED";

		_hasChanges = true;
	}
}

void NSettings::clear()
{
	_values.clear();
	_currentGroup.clear();

	_hasChanges = true;
}

QStringList NSettings::getAllGroups() const
{
	return _values.keys();
}

QStringList NSettings::getAllKeys() const
{
	return _values[_currentGroup].keys();
}

QString NSettings::getCurrentGroup() const
{
	return _currentGroup;
}

bool NSettings::hasChanges() const
{
	return _hasChanges;
}

void NSettings::acceptChanges()
{
	_hasChanges = false;
}

const NSettings::ParameterMap &NSettings::getAllParameters() const
{
	return _values;
}

QString NSettings::dataToString(const QByteArray &data) const
{
	QString result;
	foreach(unsigned const char& c, data)
	{
		QString num = QString::number((int)c, 16).rightJustified(2, QChar('0'));
		result += num;
	}

	return result;
}

QByteArray NSettings::stringToData(const QString &string) const
{
	QByteArray result;
	QString sub(string);
	for(int i = 0; i < string.count(); i += 2)
	{
		QString n = sub.left(2);
		result.append((unsigned char)n.toInt(0, 16));
		sub.remove(0, 2);
	}

	return result;
}
