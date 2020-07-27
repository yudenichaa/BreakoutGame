#include "launcherwidget.h"
#include <QApplication>
#include <QStyleFactory>
#include <QSqlDatabase>
#include <QFile>
#include <QDataStream>

int main(int argc, char *argv[]) {

    //set library paths
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");
    paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);

    // create database connection
    QSqlDatabase::addDatabase("QSQLITE", "breakout");

    // create highscore.dat file if not exists
    if(!QFile::exists("highscore.dat")) {
        QFile file("highscore.dat");
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        out << quint32(0);
        file.close();
    }

    // launch application
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    LauncherWidget launcher;
    launcher.show();
    return a.exec();
}
