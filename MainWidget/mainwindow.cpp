#include "MainWindow.h"
#include <QTcpSocket>
#include <QPushButton>
#include <QDebug>
#include <QGridLayout>
#include <QCloseEvent>
#include "ManJoystick.h"
#include "MyTcpSocket.h"
#include "ManButtons.h"
#include "VrGrid.h"
#include "VrDrag.h"
#include "VrBut.h"
#include "ModeAuto.h"
#include <QLineEdit>


#define TOP 40
#define WIDTH 800
#define HEIGHT 480
#define COLUMN_WIDTH 800/6

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(WIDTH,HEIGHT);

    //BUTTON
    autom = new QPushButton("Automatique", this);
    autom->setFont(QFont("DejaVu Sans"));
    manuel = new QPushButton("Manuel", this);
    manuel->setFont(QFont("DejaVu Sans"));
    virtuel = new QPushButton("Virtuel", this);
    virtuel->setFont(QFont("DejaVu Sans"));
    connect_state = new QPushButton("Connect", this);
    connect_state->setFont(QFont("DejaVu Sans"));
    calibration = new QPushButton("Calibration", this);
    calibration->setFont(QFont("DejaVu Sans"));
    control = new QPushButton("Control : OFF", this);
    control->setFont(QFont("DejaVu Sans"));

    line_edit = new QLineEdit("Server IP",this);

    //WIDGETS
    joy = new joystick(height()/2, this);
    rotBut = new rotateButtons(this);
    vr_grid = new VrGrid(360, this);
    vr_drag = new VrDrag(this);
    vr_but = new VrBut(vr_grid,vr_drag);
    auto_view = new ModeAuto(this);

    hide_all();
    autom->setEnabled(false);
    manuel->setEnabled(false);
    virtuel->setEnabled(false);
    control->setEnabled(false);
    calibration->setEnabled(false);

    QObject::connect(autom.data(), &QPushButton::clicked, this, &MainWindow::sl_autom);
    QObject::connect(virtuel.data(), &QPushButton::clicked, this, &MainWindow::sl_virtuel);
    QObject::connect(manuel.data(), &QPushButton::clicked, this, &MainWindow::sl_manuel);
    QObject::connect(connect_state.data(), &QPushButton::clicked, this, &MainWindow::sl_connect_state);
    QObject::connect(calibration.data(), &QPushButton::clicked, this, &MainWindow::sl_calibration);
    QObject::connect(control.data(), &QPushButton::clicked, this, &MainWindow::sl_control);


    //GRID
    layout = new QGridLayout();
    layout->setRowMinimumHeight(0,HEIGHT/10);
    layout->setRowMinimumHeight(1,HEIGHT*11/40);
    layout->setRowMinimumHeight(2,HEIGHT*21/40);
    layout->setRowMinimumHeight(3,HEIGHT/10);
    layout->setColumnMinimumWidth(0,COLUMN_WIDTH);
    layout->setColumnMinimumWidth(1,COLUMN_WIDTH);
    layout->setColumnMinimumWidth(2,COLUMN_WIDTH);
    layout->setColumnMinimumWidth(3,COLUMN_WIDTH);
    layout->setColumnMinimumWidth(4,COLUMN_WIDTH);
    layout->setColumnMinimumWidth(5,COLUMN_WIDTH);

    layout->addWidget(autom,0,0,1,2);
    layout->addWidget(virtuel,0,2,1,2);
    layout->addWidget(manuel,0,4,1,2);
    layout->addWidget(connect_state,4,2,1,1);
    layout->addWidget(line_edit,4,3,1,1);
    layout->addWidget(control,4,4,1,2);
    layout->addWidget(calibration,4,0,1,2);

    layout->addWidget(rotBut,2,0,2,2);
    layout->addWidget(joy,2,4,2,2);

    layout->addWidget(auto_view,1,2,2,2);

    layout->addWidget(vr_grid,1,0,3,4);
    layout->addWidget(vr_drag,1,4,1,1);
    layout->addWidget(vr_but,2,4,1,1);

    setLayout(layout);

}

void MainWindow::sl_calibration()
{
//    if(MyTcpSocket::sendData("", "<CALIBRATION>\n"))
//    {
//        if(MyTcpSocket::recvData()=="<OK_CAL>\n")
//            qDebug()<<"calibration ok";
//        else
//            qDebug()<<"calibration failed ";
//    }
}


void MainWindow::sl_autom()
{
    if (!MyTcpSocket::sendData("", "<AUTO_MODE>\n"))
        return;
    manuel->setEnabled(true);
    virtuel->setEnabled(true);
    autom->setEnabled(false);
    hide_all();
    auto_view->show();
}

void MainWindow::sl_virtuel()
{
    if (!MyTcpSocket::sendData("", "<VIRTUAL_MODE>\n"))
        return;
    autom->setEnabled(true);
    manuel->setEnabled(true);
    virtuel->setEnabled(false);
    hide_all();
    vr_grid->show();
    vr_drag->show();
    vr_but->show();
}

void MainWindow::sl_manuel()
{
    if (!MyTcpSocket::sendData("", "<MANUAL_MODE>\n"))
        return;
    autom->setEnabled(true);
    virtuel->setEnabled(true);
    manuel->setEnabled(false);
    hide_all();
    rotBut->show();
    joy->show();
}

void MainWindow::sl_connect_state()
{
    if (connect_state->text()=="Connect")
    {
        if(MyTcpSocket::sendData(line_edit->text(), "INIT")) //initialistion du client
        {
            QObject::connect(MyTcpSocket::get_socket().data() , &QTcpSocket::readyRead , this , &MainWindow::sl_recv);

            connect_state->setText("Disconnect");
            autom->setEnabled(true);
            manuel->setEnabled(true);
            virtuel->setEnabled(true);
            control->setEnabled(true);
            calibration->setEnabled(true);
        }
    }
    else
    {
        if(MyTcpSocket::sendData("", "STOP")) //initialistion du client
        {
            autom->setEnabled(false);
            manuel->setEnabled(false);
            virtuel->setEnabled(false);
            control->setEnabled(false);
            calibration->setEnabled(false);
            control->setText("Control : OFF");
            connect_state->setText("Connect");
            hide_all();
        }
    }
}

void MainWindow::sl_control()
{
    if (control->text()=="Control : OFF")
    {
        if(!MyTcpSocket::sendData("", "<TAKE_CONTROL>\n"))
            qDebug()<<"erreur d'écriture";
    }
    else
    {
        if(MyTcpSocket::sendData("", "<GIVE_CONTROL>\n"))
            qDebug()<<"erreur d'écriture";
    }
}

void MainWindow::hide_all()
{
    rotBut->hide();
    joy->hide();
    auto_view->hide();
    vr_grid->hide();
    vr_drag->hide();
    vr_but->hide();
}

void MainWindow::sl_recv()
{
    QString str = QString(MyTcpSocket::recvData());
    qDebug()<<"Server : "<<str;
    if (str == QString ("<REMOTE_CONTROL>\n"))
    {
        setWindowTitle("MainWindow : Remote Control Connected");
        calibration->setText("Remote Control Connected");
        return;
    }
    else if(str =="<OK_TAKE>\n")
    {
        control->setText("Control : ON");
        qDebug()<<"On prend le controle de la voiture";
    }
    else if(str =="<FAIL_TAKE>\n")
        qDebug()<<"impossible de prendre le controle de la voiture";
    else if(str =="<OK_GIVE>\n")
    {
        control->setText("Control : OFF");
        qDebug()<<"On rend le controle de la voiture";
    }
    else if(str =="<FAIL_GIVE>\n")
        qDebug()<<"impossible de rendre le controle de la voiture";
    else if(str =="<OK_AUTO>\n")
        qDebug()<<"On passse en mode : AUTO";
    else if(str =="<OK_VIRT>\n")
        qDebug()<<"On passse en mode : Virtuel";
    else if(str =="<OK_MANU>\n")
        qDebug()<<"On passse en mode : Manuel";
    else
    {
        setWindowTitle("MainWindow");
        calibration->setText("Calibration");
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (connect_state->text()=="Disconnect")
    {
        if(!MyTcpSocket::sendData("", "STOP")) // Disconnection
        {
            qDebug()<<"Cannot disconnect, try again.";
            event->ignore();
        }
    }
    event->accept();
    qDebug()<<"Application is closed.";
}
