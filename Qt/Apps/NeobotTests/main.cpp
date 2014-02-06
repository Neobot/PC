#include <QtCore/QCoreApplication>
#include <QTest>
#include <iostream>

#include "CommException.h"
#include "DataTests.h"
#include "PointTests.h"
#include "CurveTests.h"
#include "ProtocolTests.h"
#include "PatherTests.h"
#include "GraphicStrategyTest.h"
#include "ZRangeTests.h"
#include "DataMapTests.h"
#include "NGridTests.h"
#include "NSettingsTests.h"
#include "Ax12MovementManagerTest.h"

void runAll(const QList<QObject*>& testObjects)
{
        int nbTests=0, nbPassed=0;
        QStringList failedTests;
        foreach(QObject *it, testObjects)
        {
            nbTests++;
            if (QTest::qExec(it) == 0)
                nbPassed++;
            else
                failedTests.append(it->metaObject()->className());
        }

        std::cout << std::endl << "Global test results: "
                  << nbPassed << " passed, " << failedTests.count() << " failed" << std::endl;

        if (nbPassed != nbTests)
            std::cout << std::endl << "Failed tests: " << failedTests.join(", ").toLatin1().data() << std::endl;

}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    try
    {
        QList<QObject*> testObjects;

        //CommLib Tests
        testObjects.push_back(new DataTests(&app));
//        testObjects.push_back(new ProtocolTests(&app));

//        //ToolsLib Tests
//        testObjects.push_back(new PointTests(&app));
//        testObjects.push_back(new CurveTests(&app));
//        testObjects.push_back(new NGridTests(&app));
//        testObjects.push_back(new ZRangeTests(&app));
//        testObjects.push_back(new DataMapTests(&app));
//  	  testObjects.push_back(new NSettingsTests(&app));
        //testObjects.push_back(new Ax12MovementManagerTest(&app));

        //GraphicStrategy Tests
		//testObjects.push_back(new PatherTests(&app));
        //testObjects.push_back(new GraphicStrategyTests(&app));

        runAll(testObjects);
    }
    catch(const Comm::CommException& e)
    {
        std::cout << e.what().data();
    }
    catch(QString e)
    {
        std::cout << e.toLatin1().data();
    }
    catch(...)
    {
        std::cout << "Unexpected error";
    }

    return 0;
}
