#include "MainWindow.h"
#include <QPushButton>
#include <QDebug>
#include <QPixmap>
#include <QGridLayout>
#include "ManJoystick.h"
#include "MyTcpSocket.h"
#include "ManButtons.h"
#include "VrGrid.h"
#include "VrDrag.h"
#include "VrBut.h"
#include "ModeAuto.h"

#define TOP 40

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    layout = new QGridLayout();
    autom = new QPushButton("Automatique", this);
    manuel = new QPushButton("Manuel", this);
    virtuel = new QPushButton("Virtuel", this);
    connect_state = new QPushButton("Connect", this);
    connect=false;
    autom->setEnabled(false);
    manuel->setEnabled(false);
    virtuel->setEnabled(false);

    resize(800,480);

    joy = new joystick(height()*3/4, this);
    joy->hide();
    rotBut = new rotateButtons(this); //have to : add the TCP client to the class rotateButton
    rotBut->hide();

    layout->setRowMinimumHeight(1,200);
    layout->setRowMinimumHeight(2,200);
    layout->setColumnMinimumWidth(0,266);
    layout->setColumnMinimumWidth(1,266);
    layout->setColumnMinimumWidth(2,266);

    vr_grid = new VrGrid(360, this);
    vr_grid->hide();
    vr_drag = new VrDrag(this);
    vr_drag->hide();
    vr_but = new VrBut(vr_grid,vr_drag);
    vr_but->hide();

    auto_view = new ModeAuto(this);
    auto_view->hide();
    layout->addWidget(autom,0,0,1,1);
    layout->addWidget(virtuel,0,1,1,1);
    layout->addWidget(manuel,0,2,1,1);
    layout->addWidget(connect_state,3,1,1,1);

    layout->addWidget(rotBut,1,0,2,1);
    layout->addWidget(joy,1,1,2,2);

    layout->addWidget(vr_grid,1,0,2,2);
    layout->addWidget(vr_drag,1,2,1,1);
    layout->addWidget(vr_but,2,2,1,1);

    layout->addWidget(auto_view,1,1,2,2);

    setLayout(layout);

    QObject::connect(autom, &QPushButton::clicked, this, sl_autom);
    QObject::connect(virtuel, &QPushButton::clicked, this, sl_virtuel);
    QObject::connect(manuel, &QPushButton::clicked, this, sl_manuel);
    QObject::connect(connect_state, &QPushButton::clicked, this, sl_connect_state);
}

void MainWindow::sl_autom()
{
    autom->setEnabled(false);
    virtuel->setEnabled(true);
    manuel->setEnabled(true);

    auto_view->show();
    rotBut->hide();
    joy->hide();
    vr_grid->hide();
    vr_drag->hide();
    vr_but->hide();

    MyTcpSocket::sendData("<AUTO>\n");
    MyTcpSocket::recvData();
}

void MainWindow::sl_virtuel()
{
    autom->setEnabled(true);
    virtuel->setEnabled(false);
    manuel->setEnabled(true);

    vr_grid->show();
    vr_drag->show();
    vr_but->show();
    auto_view->hide();
    rotBut->hide();
    joy->hide();

    MyTcpSocket::sendData("<VIRTUAL>\n");
    MyTcpSocket::recvData();
}

void MainWindow::sl_manuel()
{
    autom->setEnabled(true);
    virtuel->setEnabled(true);
    manuel->setEnabled(false);

    rotBut->show();
    joy->show();
    auto_view->hide();
    vr_grid->hide();
    vr_drag->hide();
    vr_but->hide();

    MyTcpSocket::sendData("<MANUAL>\n");
    MyTcpSocket::recvData();
    MyTcpSocket::sendData("<TAKE_CONTROL>\n");
    MyTcpSocket::recvData();


}

void MainWindow::sl_connect_state()
{
    if (connect==false)
    {
        if(MyTcpSocket::sendData("INIT")) //initialistion du client
        {
        connect=true;
        connect_state->setText("Disconnect");
        autom->setEnabled(true);
        manuel->setEnabled(true);
        virtuel->setEnabled(true);
        }
    }
    else
    {
        autom->setEnabled(false);
        manuel->setEnabled(false);
        virtuel->setEnabled(false);
        MyTcpSocket::sendData("STOP"); //initialistion du client
        connect=false;
        connect_state->setText("Connect");

        rotBut->hide();
        joy->hide();
        auto_view->hide();
        vr_grid->hide();
        vr_drag->hide();
        vr_but->hide();
    }

}
