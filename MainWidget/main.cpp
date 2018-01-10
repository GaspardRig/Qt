#include "MainWindow.h"
#include "ManJoystick.h"
#include "MyTcpSocket.h"
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
