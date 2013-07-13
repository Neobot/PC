/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Neobot wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy us a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <QCoreApplication>
#include "NeobotStrategyLauncher.h"

int main(int argc, char *argv[])
{
    NeobotMainOptions options;
    if (!options.parse(argc, argv))
	return 1;

    QCoreApplication a(argc, argv);

	NeobotStrategyLauncher launcher;
    return launcher.exec(options, a);
}
