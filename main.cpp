#include "introwindow.h"
#include "browserdialog.h"
#include "mainwindow.h"

#include <QApplication>
#include <QKeyEvent>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IntroWindow w;
    w.show();

    return a.exec();
}
