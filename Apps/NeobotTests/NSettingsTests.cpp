#include "NSettingsTests.h"

#include "NSettings.h"
#include "ToolsLib.h"

#include <QDir>
#include <QRectF>
#include <QFile>
#include <QTest>

NSettingsTests::NSettingsTests(QObject *parent) :
	QObject(parent)
{
}

void NSettingsTests::testReadWrite()
{
	Tools::NSettings s;

	int i = 42;
	QRect rect(400, 300, 3, 44);
	QString k = "string random";
	QPointF p(200.3, 43.1);
	QList<QRectF> rectList;
	rectList << QRectF(400.4, 300, 3, 22) << QRectF(555, 1, 3, 0) << QRectF(323, 55, 90, 199);

	s.setValue("int", i, "comm1");
	s.setValue("rect", rect, "comm2");

	s.beginGroup("g1");
	s.setValue("k", k);
	s.endGroup();

	s.beginGroup("g2");
	s.setValue("point", p);

	s.setValue("recList", Tools::convertListToVariantList<QRectF>(rectList), "List of rect");
	s.endGroup();

	QDir current;
	QString file1 = current.absoluteFilePath("file1.txt");
	QString file2 = current.absoluteFilePath("file2.txt");

	s.writeTo(file1);
	s.clear();
	s.loadFrom(file1);
	s.writeTo(file2);

	QFile f1(file1);
	if (!f1.open(QIODevice::ReadOnly))
		QFAIL("Cannot open file 1.");

	QFile f2(file2);
	if (!f2.open(QIODevice::ReadOnly))
		QFAIL("Cannot open file 1.");

	QByteArray content1 = f1.readAll();
	QByteArray content2 = f2.readAll();

	QCOMPARE(content1, content2);

	QCOMPARE(s.value("int").toInt(), i);
	QCOMPARE(s.description("int"), QString("comm1"));
	compareRect(rect, s.value("rect").toRectF());
	s.beginGroup("g1");
	QCOMPARE(s.value("k").toString(), k);
	s.endGroup();
	s.beginGroup("g2");
	QCOMPARE(s.value("point").toPointF(), p);
	QList<QRectF> outRectList = Tools::convertVariantListToList<QRectF>(s.value("recList").toList());
	s.endGroup();

	QCOMPARE(outRectList.count(), rectList.count());
	for(int j = 0; j < outRectList.count(); ++j)
	{
		compareRect(outRectList.value(j), rectList.value(j));
	}
}

void NSettingsTests::compareRect(const QRectF &r1, const QRectF &r2)
{
	QCOMPARE(r1.x(), r2.x());
	QCOMPARE(r1.y(), r2.y());
	QCOMPARE(r1.width(), r2.width());
	QCOMPARE(r1.height(), r2.height());
}
