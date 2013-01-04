#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include "teachreg.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
	
	app.setApplicationName("TeachReg");
	app.setApplicationVersion(VERSION);
	
	QTranslator qtTranslator;
#ifdef Q_OS_LINUX
	qtTranslator.load("qt_" + QLocale::system().name(), "/usr/share/qt/translations");
#endif
#ifdef Q_OS_WIN32
	qtTranslator.load("qt_" + QLocale::system().name(), QApplication::applicationDirPath () + "/translations");
#endif
	app.installTranslator(&qtTranslator);
	
    TeachReg mw;
    mw.show();
    return app.exec();
}
