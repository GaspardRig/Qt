#include "VrBut.h"
#include "QPushButton"
#include "VrGrid.h"
#include "MyTcpSocket.h"
#include "VrDrag.h"
#include <QVBoxLayout>

VrBut::VrBut(VrGrid *vr_grid, VrDrag *dwid, QWidget *parent) :
    QWidget(parent)
{
    retry = new QPushButton("Reset", this);
    start = new QPushButton("GO!", this);
    retry->setEnabled(false);
    start->setEnabled(false);

    layout = new QVBoxLayout(this);
    layout->addWidget(retry);
    layout->addWidget(start);

    QObject::connect(retry, &QPushButton::clicked, vr_grid, &VrGrid::sl_reset);
    QObject::connect(retry, &QPushButton::clicked, this, &VrBut::sl_reset);
    QObject::connect(start, &QPushButton::clicked, this, &VrBut::sl_start);
    QObject::connect(retry, &QPushButton::clicked, dwid, &VrDrag::sl_reset);
    QObject::connect(vr_grid , SIGNAL(sg_enable(bool)), this , SLOT(sl_enable(bool)));
}

void VrBut::sl_start()
{
    MyTcpSocket::sendData(QString("matrice"));
    MyTcpSocket::recvData();
}

void VrBut::sl_reset()
{
    MyTcpSocket::sendData(QString("matrice vide"));
    MyTcpSocket::recvData();
}

void VrBut::sl_enable(bool state)
{
    retry->setEnabled(state);
    start->setEnabled(state);
}
