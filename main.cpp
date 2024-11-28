#include "cartmainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOverrideCursor(Qt::BlankCursor);
    a.changeOverrideCursor(Qt::BlankCursor);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "1806-RobotCart_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    CartMainWindow w;
    w.show();
    w.showFullScreen();
    //w.PushMessageToConsole("UI init complete.");
    return a.exec();
}
