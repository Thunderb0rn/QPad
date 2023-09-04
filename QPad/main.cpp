#include "qpad.h"
//#include "fontform.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPad w;
    w.show();
    return a.exec();
}
