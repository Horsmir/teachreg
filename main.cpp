#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include "teachreg.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
	
	app.setApplicationName("TeachReg");
	app.setApplicationVersion(VERSION);
	
	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(), "/usr/share/qt/translations");
	app.installTranslator(&qtTranslator);
	
    TeachReg mw;
    mw.show();
    return app.exec();
}
