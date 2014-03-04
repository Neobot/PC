/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Neobot wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy us a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <QApplication>
#include "NPlayer.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(WidgetToolsLib);
    Q_INIT_RESOURCE(Simulation);

    QApplication a(argc, argv);
	a.setWindowIcon(QIcon(":/app/AppIcon"));
    NPlayer w;
    w.show();

    return a.exec();
}
