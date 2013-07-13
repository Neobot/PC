#ifndef NSETTINGSPROPERTYBROWSER_H
#define NSETTINGSPROPERTYBROWSER_H

#include <QtTreePropertyBrowser>
#include <QtVariantPropertyManager>
#include <QtGroupPropertyManager>

#include "NSettings.h"

namespace Tools
{
	class NSettings;


	class NSettingsPropertyManager : public QtVariantPropertyManager
	{
		Q_OBJECT
	public:
		NSettingsPropertyManager(QObject *parent = 0);
		~NSettingsPropertyManager();

		QtVariantProperty* addNSettingProperty(QVariant::Type dataType, int contentType, const QString& name, bool isReadOnly);
		virtual QVariant value(const QtProperty *property) const;
		virtual int valueType(int propertyType) const;
		virtual int contentType(const QtProperty *property) const;
		virtual bool isReadOnly(const QtProperty *property) const;
		virtual bool isPropertyTypeSupported(int propertyType) const;

		QString valueText(const QtProperty *property) const;

	public slots:
		virtual void setValue(QtProperty *property, const QVariant &val);

	protected:
		virtual void initializeProperty(QtProperty *property);
		virtual void uninitializeProperty(QtProperty *property);

	private slots:
		void slotValueChanged(QtProperty *property, const QVariant &value);
		void slotPropertyDestroyed(QtProperty *property);

	private:
		struct ListData
		{
			QVariant value;
			QtProperty* property;
		};

		typedef QList<ListData> Data;
		QHash<const QtProperty *, Data> _listProperties;
		QHash<const QtProperty *, QtProperty*> _subListPropertiesMap;
		QHash<const QtProperty *, int> _listPropertiesContentType;

		QSet<const QtProperty *> _readOnlyProperties;

		class ListDataPropertySearch
		{
		public:
			ListDataPropertySearch(QtProperty* p) : _p(p) {}
			bool operator()(const ListData& ld) const {return _p == ld.property;}
		private:
			QtProperty* _p;
		};
	};

	class NSettingsEditorFactory : public QtAbstractEditorFactory<NSettingsPropertyManager>
	{
		Q_OBJECT
	   public:
		   NSettingsEditorFactory(QObject *parent = 0);
		   ~NSettingsEditorFactory();

	   protected:
		   void connectPropertyManager(NSettingsPropertyManager *manager);
		   QWidget *createEditor(NSettingsPropertyManager *manager, QtProperty *property,
					   QWidget *parent);
		   void disconnectPropertyManager(NSettingsPropertyManager *manager);

	   private:
		   QtVariantEditorFactory *_originalFactory;
		   QHash<QWidget*, QtProperty *> _editorToProperty;
		   QHash<QtProperty *, QList<QWidget*> > _createdEditors;

	private slots:
		   void slotEditorDestroyed(QObject* object);
		   void listCountChanged(int value);
	};

	class NSettingsPropertyBrowser : public QtTreePropertyBrowser
	{
		Q_OBJECT
	public:
		explicit NSettingsPropertyBrowser(QWidget *parent = 0);

		void setSettings(NSettings settings);
		void setAllItemExpanded(bool expanded);

		NSettings getOriginalSettings() const;
		NSettings getSettings() const;

	private:
		NSettingsPropertyManager* _manager;
		QtGroupPropertyManager* _groupManager;
		QtVariantEditorFactory* _factory;
		NSettings _settings;

		QColor _groupTitleColor;
		QList<QColor> _groupColors;

		void setAllChildrenExpanded(QtBrowserItem* item, bool expanded);
	};
}

#endif // NSETTINGSPROPERTYBROWSER_H
