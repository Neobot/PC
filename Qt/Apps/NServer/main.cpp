#include <QCoreApplication>

#include "NServer.h"


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Simulation);
	Q_INIT_RESOURCE(GraphicStrategy);

	QCoreApplication a(argc, argv);
	
	Tools::StdoutLogger logger;
	NServer server(&logger);
	server.start();

	return a.exec();
}
