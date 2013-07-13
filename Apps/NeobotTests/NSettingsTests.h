#ifndef NSETTINGSTESTS_H
#define NSETTINGSTESTS_H

#include <QObject>
#include <QRectF>

class NSettingsTests : public QObject
{
	Q_OBJECT
public:
	explicit NSettingsTests(QObject *parent = 0);
	
private slots:
	void testReadWrite();

private:
	void compareRect(const QRectF& r1, const QRectF& r2);
	
};

#endif // NSETTINGSTESTS_H
