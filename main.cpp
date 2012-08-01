#include <QtGui/QApplication>
#include "teachreg.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    TeachReg foo;
    foo.show();
    return app.exec();
}
