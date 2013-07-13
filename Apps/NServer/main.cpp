#include <QCoreApplication>

#include "NServer.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	
	Tools::StdoutLogger logger;
	NServer server(&logger);
	server.start();

	return a.exec();
}
