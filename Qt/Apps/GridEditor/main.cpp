/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Neobot wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy us a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <QApplication>
#include "GridEditor.h"
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/app/AppIcon"));
	GridEditor w;

	if (a.arguments().count() > 0)
	{
		QString file = a.arguments().value(1);
		if (QFile::exists(file))
			w.open(file);

		QString table = a.arguments().value(2);
        if (table == "2014")
            w.set2014Table();
        else if (table == "2013")
			w.set2013Table();
		else if (table == "2012")
			w.set2012Table();
		else if (table == "2011")
			w.set2011Table();
	}

	w.show();
	w.setFocus();

    return a.exec();
}
