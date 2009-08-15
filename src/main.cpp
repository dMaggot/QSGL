#include <QtGui/QApplication>
#include "qsglwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSGLWindow w;
    w.show();
    return a.exec();
}
