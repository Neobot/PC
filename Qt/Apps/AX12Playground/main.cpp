#include <QtGui/QApplication>

#ifndef Q_OS_WIN32
#include "AX12Playground.h"
#endif

int main(int argc, char *argv[])
{
#ifndef Q_OS_WIN32
	QApplication a(argc, argv);

	AX12Playground *gui = new AX12Playground();
	gui->show();

	return a.exec();
#endif
}
