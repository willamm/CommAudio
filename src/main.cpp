#include "mainwindow.h"
#include "mediaplayer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    MediaPlayer p;
//    p.show();

    return a.exec();
}
