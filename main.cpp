#include "mainwindow.h"

#include <QApplication>
#include <QDateTime>
#include <QLocale>


int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    app.setApplicationName("Qivrit");

    QLocale::setDefault(QLocale(QLocale::Hebrew, QLocale::Israel));

    qsrand(QDateTime::currentDateTime().toTime_t());

    MainWindow window;
    window.show();

    return app.exec();
}
