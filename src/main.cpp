/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: main.cpp
--
-- PROGRAM: Qtify Audio Player
--
-- FUNCTIONS:
-- int main(int, char);
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Will Murphy, Matthew Shew
--
-- PROGRAMMER: Will Murphy, Calvin Lai
--
-- NOTES:
-- Main point of entry for application.
----------------------------------------------------------------------------------------------------------------------*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
