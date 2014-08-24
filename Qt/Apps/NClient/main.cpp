#include "NClient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(WidgetToolsLib);
	Q_INIT_RESOURCE(GraphicStrategy);

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/app/AppIcon"));
    NClient w;
	w.show();
	
	return a.exec();
}
