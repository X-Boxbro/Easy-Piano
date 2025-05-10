#include "startwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    startwindow startWin;
    startWin.show();
    return a.exec();
}
