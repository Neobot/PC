#include <QApplication>
#include <QStringList>
#include "CommTools.h"
#include "UtilitySelector.h"

void help(Tools::StdoutLogger& out)
{
    out << "------------Neobot RS232 Communication Tools------------" << Tools::endl
			<< "Usage: nct portname baudrate Mode(-B|-S|-L) [options]" << Tools::endl
			<< "Generic Options :" << Tools::endl
            << "   -q : Be quiet. Do not display the values that has been read" << Tools::endl
            << "MODE -B : Baudrate test : Send continuously the same value in order to adjust the baudrate of the targeted PC." << Tools::endl
            << "   -v value=42 : The value sent to the targeted PC. The default value is 42." << Tools::endl
            << "MODE -S : Speed test : Display the time used to send and receive some frames. The frame contains random values." << Tools::endl
            << "The targeted PC must re-send exactly what it has received." << Tools::endl
            << "   -v value=42 : The first value to send." << Tools::endl
            << "   -n number=1000 : The number of frame to time. The default value is 1000." << Tools::endl
            << "   -g group=1 : The number of byte sent in 1 frame. The default value is 1." << Tools::endl
            << "MODE -L : Listening mode. Listen to the port." << Tools::endl
            << "   -s value : quit when value has been received. (Not implemented)" << Tools::endl
            << Tools::endl
            << "The default settings of the port are :" << Tools::endl
            << " - Data : 8 bits" << Tools::endl
            << " - Parity : None" << Tools::endl
            << " - Stop bits : 1 bit" << Tools::endl
            << " - Flow Control : OFF" << Tools::endl
            << Tools::endl
            << "The baudrates supported by this tool are :" << Tools::endl
			<< "300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 76800, 115200, 230400, 500000, 576000, 921600, 1000000, 1152000, 150000, 2000000, 2500000, 3000000, 3500000, 4000000" << Tools::endl
            << Tools::endl
            << "Examples :" << Tools::endl
            << "nct /dev/ttyS0 115200 -B -v 21" << Tools::endl
            << "nct /dev/ttyUSB0 19200 -S -q -n 10000 -g 10" << Tools::endl
            << "nct /dev/ttyUSB0 9600 -L -q -s 42" << Tools::endl
            << Tools::endl;
}

int stopWithError(const QString& errorMessage, Tools::StdoutLogger& out)
{
    out << errorMessage.toLatin1().data() << Tools::endl << Tools::endl;
    help(out);
    return 1;
}

int console(int argc, char *argv[], Tools::StdoutLogger& stdLogger)
{
    QCoreApplication a(argc, argv);

    QStringList arguments = a.arguments();

    if (arguments.count() < 4)
    {
        help(stdLogger);
        return 1;
    }

    arguments.takeFirst();  //0
    QString portname = arguments.takeFirst(); //1
    QString baudrate = arguments.takeFirst(); //2
    QString mode     = arguments.takeFirst(); //3

    int firstValue = 42;
    int group = 1;
    int nbFrame = 1000;

    CommTools tools(portname, baudrate, &stdLogger);

    /* Parse options
     * Note : Work only with single char option.
     * Options which does't require a value can be grouped
     */
    QString optionsWithValue = "ngsv"; //Options which require a value set by the next parameter
    while(!arguments.isEmpty())
    {
        QString arg =  arguments.takeFirst();

        if (arg.count() <= 1 || arg.left(1) != "-")
            return stopWithError("Invalid options.", stdLogger);

        for(int i = 1; i < arg.count(); ++i)
        {
            char option = arg.at(i).toLatin1();
            if (optionsWithValue.contains(option))
            {
                if (arg.count() > 2 || arguments.isEmpty() || arguments.value(0).left(1) == "-")
                     return stopWithError("Invalid options.", stdLogger);
            }

            switch(option)
            {
            case 'v':
                firstValue = arguments.takeFirst().toInt();
                break;
            case 'n':
                nbFrame = arguments.takeFirst().toInt();
                break;
            case 'g':
                group = arguments.takeFirst().toInt();
                break;
            case 'q':
                tools.beQuiet(true);
                break;
            case 's':
                tools.setStopValue(arguments.takeFirst().toInt());
                break;
            default:
                return stopWithError(QString("Invalid option '-").append(option).append("'"), stdLogger);
            }

        }  
    }

    if (mode == "-B")
        tools.startBaudrateTest(firstValue, &a);
    else if (mode == "-S")
        tools.startSpeedTest(firstValue, nbFrame, group);
    else if (mode == "-L")
        {}
    else
        return stopWithError("Invalid mode ....", stdLogger);



    return a.exec();
}

int main(int argc, char *argv[])
{
    Tools::StdoutLogger stdLogger;

	if  (argc <= 1)
	{
		QApplication a(argc, argv);

		UtilitySelector *gui = new UtilitySelector();
		gui->show();

		return a.exec();
	}
	else if (argc >= 4)
    {
		return console(argc, argv, stdLogger);
	}
	else
	{
        help(stdLogger);
        return 1;
    }
}
