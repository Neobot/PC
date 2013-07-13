#include <QApplication>
#include "NAcquisitor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/app/AppIcon"));
    NAcquisitor w;
    w.show();
    
    return a.exec();
}
