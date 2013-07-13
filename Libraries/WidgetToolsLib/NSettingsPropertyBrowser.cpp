#include "NSettingsPropertyBrowser.h"

#include <QSpinBox>
#include <algorithm>

using namespace Tools;

NSettingsPropertyBrowser::NSettingsPropertyBrowser(QWidget *parent) :
	QtTreePropertyBrowser(parent)
{
	_manager = new NSettingsPropertyManager(this);
	_groupManager = new QtGroupPropertyManager(this);
	setFactoryForManager(_manager, new NSettingsEditorFactory);

	_groupTitleColor = QColor(191, 191, 191);
	_groupColors << QColor(255, 255, 188) << QColor(189, 255, 189) << QColor(255, 191, 190) << QColor(190, 206, 255);
}

void NSettingsPropertyBrowser::setSettings(NSettings settings)
{
	_settings = settings;
	clear();;

	Q_ASSERT(!_groupColors.isEmpty());
	QList<QColor>::iterator itColor = _groupColors.begin();

	for(NSettings::ParameterMap::const_iterator itGroup = _settings.getAllParameters().constBegin(); itGroup != _settings.getAllParameters().constEnd(); ++itGroup)
	{
		const QString& group = itGroup.key();
		bool isGroup = !group.isEmpty();

		QtProperty* groupProperty = 0;
		QtBrowserItem* groupItem = 0;

		if (isGroup)
		{
			groupProperty = _groupManager->addProperty(group);
			groupItem = addProperty(groupProperty);
		}
		else
		{
			groupProperty = _groupManager->addProperty("Global");
			groupItem = addProperty(groupProperty);
		}

		for(NSettings::ParameterGroup::const_iterator it = itGroup.value().constBegin(); it != itGroup.value().constEnd(); ++it)
		{
			const QString& name = it.key();
			QVariant value = it->value;
			QString description = it->description;
			int contentType = it->contentType;

			QtVariantProperty* property = _manager->addNSettingProperty(value.type(), contentType, name, it->readOnly);
			if (property)
			{
				property->setToolTip(description);
				property->setValue(value);
				if (groupProperty)
					groupProperty->addSubProperty(property);
				else
					addProperty(property);
			}
		}

		if (groupItem)
		{
			setBackgroundColor(groupItem, _groupTitleColor);
			if (itColor == _groupColors.end())
				itColor = _groupColors.begin();

			foreach(QtBrowserItem* child, groupItem->children())
				setBackgroundColor(child, *itColor);

			setAllChildrenExpanded(groupItem, false);
			setExpanded(groupItem, true);
			++itColor;
		}
	}

	setAlternatingRowColors(true);
}

void NSettingsPropertyBrowser::setAllItemExpanded(bool expanded)
{
	QList<QtBrowserItem*> items = topLevelItems();
	foreach(QtBrowserItem* item, items)
		setAllChildrenExpanded(item, expanded);
}

NSettings NSettingsPropertyBrowser::getOriginalSettings() const
{
	return _settings;
}

NSettings NSettingsPropertyBrowser::getSettings() const
{
	NSettings updatedSettings = _settings;

	QList<QtBrowserItem*> items = topLevelItems();
	if (!items.isEmpty())
	{
		foreach(QtBrowserItem* groupItem, items)
		{
			QString group = groupItem->property()->propertyName();
			bool isGlobalGroup = group == "Global" && groupItem == items.first();

			if (!isGlobalGroup)
				updatedSettings.beginGroup(group);

			QList<QtBrowserItem*> children = groupItem->children();
			foreach(QtBrowserItem* item, children)
			{
				QString name = item->property()->propertyName();
				QVariant value = _manager->value(item->property());

				updatedSettings.replaceValue(name, value);
			}

			if (!isGlobalGroup)
				updatedSettings.endGroup();
		}
	}

	return updatedSettings;
}

void NSettingsPropertyBrowser::setAllChildrenExpanded(QtBrowserItem *item, bool expanded)
{
	QList<QtBrowserItem*> childrenList = item->children();
	foreach(QtBrowserItem* child, childrenList)
		setAllChildrenExpanded(child, expanded);

	setExpanded(item, expanded);
}

//----------------------------------------------------------------------------------------------------------------

NSettingsPropertyManager::NSettingsPropertyManager(QObject *parent)
	: QtVariantPropertyManager(parent)
{
	connect(this, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
			this, SLOT(slotValueChanged(QtProperty *, const QVariant &)));
	connect(this, SIGNAL(propertyDestroyed(QtProperty *)),
			this, SLOT(slotPropertyDestroyed(QtProperty *)));
}

NSettingsPropertyManager::~NSettingsPropertyManager()
{
}

QtVariantProperty *NSettingsPropertyManager::addNSettingProperty(QVariant::Type dataType, int contentType, const QString &name, bool isReadOnly)
{
	QtVariantProperty* property = addProperty(dataType, name);
	if (dataType == QVariant::List)
		_listPropertiesContentType[property] = contentType;

	if (isReadOnly)
		_readOnlyProperties.insert(property);

	return property;
}

void NSettingsPropertyManager::slotValueChanged(QtProperty *property, const QVariant &value)
{
	if (_subListPropertiesMap.contains(property))
	{
		QtProperty *listProperty = _subListPropertiesMap[property];
		QVariant v = this->value(listProperty);
		QVariantList list = v.toList();

		Data& d = _listProperties[listProperty];
		Data::iterator it = std::find_if(d.begin(), d.end(), ListDataPropertySearch(property));
		if (it != d.end())
		{
			int indexInList = it - d.begin();
			list[indexInList] = value;
		}

		setValue(listProperty, list);
	}
}

void NSettingsPropertyManager::slotPropertyDestroyed(QtProperty *property)
{
	if (_subListPropertiesMap.contains(property))
	{
		QtProperty *listProperty = _subListPropertiesMap[property];
		//		QVariant v = this->value(listProperty);
		//		QVariantList list = v.toList();

		Data& d = _listProperties[listProperty];
		Data::iterator it = std::find_if(d.begin(), d.end(), ListDataPropertySearch(property));
		if (it != d.end())
		{
			d.erase(it);
		}

		_subListPropertiesMap.remove(property);
	}
}

bool NSettingsPropertyManager::isPropertyTypeSupported(int propertyType) const
{
	if (propertyType == QVariant::List)
		return true;

	return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}

int NSettingsPropertyManager::valueType(int propertyType) const
{
	if (propertyType == QVariant::List)
		return QVariant::List;

	return QtVariantPropertyManager::valueType(propertyType);
}

int NSettingsPropertyManager::contentType(const QtProperty *property) const
{
	return _listPropertiesContentType.value(property, -1);
}

bool NSettingsPropertyManager::isReadOnly(const QtProperty *property) const
{
	return _readOnlyProperties.contains(property);
}

QVariant NSettingsPropertyManager::value(const QtProperty *property) const
{
	if (_listProperties.contains(property))
	{
		const Data& d = _listProperties[property];
		QVariantList list;
		foreach(const ListData& ld, d)
			list.append(ld.value);

		return list;

	}

	return QtVariantPropertyManager::value(property);
}

QString NSettingsPropertyManager::valueText(const QtProperty *property) const
{
	if (_listProperties.contains(property))
	{
		QVariant v = value(property);
		QVariantList list = v.toList();

		return QString(tr("[%1 element(s)]").arg(list.count()));
	}

	return QtVariantPropertyManager::valueText(property);
}

void NSettingsPropertyManager::setValue(QtProperty *property, const QVariant &val)
{
	if (_listProperties.contains(property))
	{
		if (val.type() != QVariant::List && !val.canConvert(QVariant::List))
			return;

		QVariantList list = val.toList();
		Data& d = _listProperties[property];

		int index = 0;
		foreach(const QVariant& v, list)
		{
			if (index < d.count())
			{
				ListData& listdata = d[index];
				listdata.value = v;
			}
			else
			{
				ListData newListData;
				newListData.value = v;
				newListData.property = addProperty(v.type(), QString("value ").append(QString::number(index)));
				d.append(newListData);
				property->addSubProperty(newListData.property);
				setValue(newListData.property, v);
				_subListPropertiesMap[newListData.property] = property;
				initializeProperty(newListData.property);
			}

			++index;
		}

		if (index < d.count())
		{
			for(Data::iterator it = d.begin() + index; it != d.end();)
			{
				//remove the additional properties
				ListData& additionalData = *it;
				_subListPropertiesMap.remove(additionalData.property);
				property->removeSubProperty(additionalData.property);
				uninitializeProperty(additionalData.property);

				it = d.erase(it);
			}
		}

		emit propertyChanged(property);
		emit valueChanged(property, list);

		return;
	}

	QtVariantPropertyManager::setValue(property, val);
}

void NSettingsPropertyManager::initializeProperty(QtProperty *property)
{
	if (propertyType(property) == QVariant::List)
	{
		Data d;
		_listProperties[property] = d;
	}
	QtVariantPropertyManager::initializeProperty(property);
}

void NSettingsPropertyManager::uninitializeProperty(QtProperty *property)
{
	if (_listProperties.contains(property))
	{
		Data& d = _listProperties[property];
		foreach(const ListData& ld, d)
		{
			_subListPropertiesMap.remove(ld.property);
		}

		_listProperties.remove(property);
		_listPropertiesContentType.remove(property);
		_readOnlyProperties.remove(property);
	}

	QtVariantPropertyManager::uninitializeProperty(property);
}

//----------------------------------------------------------------------------------------------------------------

NSettingsEditorFactory::NSettingsEditorFactory(QObject *parent)
	: QtAbstractEditorFactory<NSettingsPropertyManager>(parent)
{
	_originalFactory = new QtVariantEditorFactory(this);
}

NSettingsEditorFactory::~NSettingsEditorFactory()
{
	// not need to delete editors because they will be deleted by originalFactory in its destructor
	for(QHash<QtProperty *, QList<QWidget*> >::iterator it = _createdEditors.begin(); it != _createdEditors.end(); ++it)
	{
		qDeleteAll(it.value());
		it.value().clear();
	}
}

void NSettingsEditorFactory::connectPropertyManager(NSettingsPropertyManager *manager)
{
	_originalFactory->addPropertyManager(manager);
}

QWidget *NSettingsEditorFactory::createEditor(NSettingsPropertyManager *manager, QtProperty *property, QWidget *parent)
{
	if (manager->isReadOnly(property))
		return 0;

	QVariant v = manager->value(property);
	if (v.type() == QVariant::List)
	{
		QSpinBox* editor =  new QSpinBox(parent);
		editor->setMinimum(0);
		editor->setMaximum(9999);
		editor->setPrefix("[");
		editor->setSuffix(" element(s)]");

		QVariantList list = v.toList();
		editor->setValue(list.count());

		_createdEditors[property].append(editor);
		_editorToProperty[editor] = property;

		connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		connect(editor, SIGNAL(valueChanged(int)), this, SLOT(listCountChanged(int)));

		return editor;
	}
	else
	{
		QtAbstractEditorFactoryBase *base = _originalFactory;
		return base->createEditor(property, parent);
	}
}

void NSettingsEditorFactory::disconnectPropertyManager(NSettingsPropertyManager *manager)
{
	_originalFactory->removePropertyManager(manager);
}

void NSettingsEditorFactory::slotEditorDestroyed(QObject *object)
{
	QWidget* editor = qobject_cast<QWidget*>(object);
	if (editor)
	{
		QtProperty *property = _editorToProperty.value(editor, 0);
		if (property)
		{
			_editorToProperty.remove(editor);
			_createdEditors[property].removeAll(editor);
			if (_createdEditors[property].isEmpty())
				_createdEditors.remove(property);
		}
	}
}

void NSettingsEditorFactory::listCountChanged(int value)
{
	QSpinBox* editor = qobject_cast<QSpinBox*>(sender());
	if (editor)
	{
		QtProperty *property = _editorToProperty.value(editor, 0);
		if (property)
		{
			NSettingsPropertyManager* manager = propertyManager(property);
			QVariant v = manager->value(property);
			if (v.type() == QVariant::List)
			{
				int contentType = manager->contentType(property);
				if (contentType >= 0)
				{
					QVariantList list = v.toList();
					for(int i = list.count(); i < value; ++i)
						list << QVariant((QVariant::Type)contentType);

					if (value < list.count())
						list.erase(list.begin() + value, list.end());

					manager->setValue(property, list);
				}
			}
		}
	}
}
