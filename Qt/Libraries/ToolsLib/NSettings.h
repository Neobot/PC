#ifndef NSETTINGS_H
#define NSETTINGS_H

#include <QVariant>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace Tools
{
	class NSettings
	{
	public:
		struct Parameter
		{
			QString description;
			QVariant value;
			QVariant::Type contentType; //for list
			bool readOnly;
		};

		typedef QMap<QString, Parameter> ParameterGroup;
		typedef QMap<QString, ParameterGroup> ParameterMap;

		NSettings();

		bool loadFrom(const QString& file);
		bool writeTo(const QString& file);

		void beginGroup(const QString& groupName);
		void endGroup();

		void replaceValue(const QString& name, const QVariant& value);
		void setValue(const QString& name, const QVariant& value, const QString& description = QString(), bool readOnly = false);
		void setValueList(const QString& name, const QVariantList& value, QVariant::Type contentType, const QString& description = QString(), bool readOnly = false);
		QVariant value(const QString& name, const QVariant& defaultValue = QVariant()) const;
		QString description(const QString& name) const;
		bool contains(const QString& name) const;
		bool isReadOnly(const QString& name) const;

		void clear();

		const ParameterMap& getAllParameters() const;
		QStringList getAllGroups() const;
		QStringList getAllKeys() const;
		QString getCurrentGroup() const;

		bool hasChanges() const;
		void acceptChanges();

	private:
		ParameterMap _values;
		QString _currentGroup;
		bool _hasChanges;

		QString dataToString(const QByteArray& data) const;
		QByteArray stringToData(const QString& string) const;
	};
}

#endif // NSETTINGS_H
