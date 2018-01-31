#include "VrBut.h"
#include "QPushButton"
#include "VrGrid.h"
#include "MyTcpSocket.h"
#include "VrDrag.h"
#include <QDebug>
#include <QVBoxLayout>

VrBut::VrBut(VrGrid *vr_grid, VrDrag *dwid, QWidget *parent) :
    QWidget(parent)
{
    retry = new QPushButton("Reset", this);
    retry->setFont(QFont("DejaVu Sans"));
    start = new QPushButton("GO!", this);
    start->setFont(QFont("DejaVu Sans"));
    retry->setEnabled(false);
    start->setEnabled(false);

    layout = new QVBoxLayout(this);
    layout->addWidget(retry);
    layout->addWidget(start);

    QObject::connect(retry.data(), &QPushButton::clicked, vr_grid, &VrGrid::sl_reset);
    QObject::connect(retry.data(), &QPushButton::clicked, this, &VrBut::sl_reset);
    QObject::connect(retry.data(), &QPushButton::clicked, dwid, &VrDrag::sl_reset);
    QObject::connect(start.data(), &QPushButton::clicked, this, &VrBut::sl_start);
    QObject::connect(vr_grid , SIGNAL(sg_enable(bool)), this , SLOT(sl_enable(bool)));
}

void VrBut::sl_start()
{
    if(!MyTcpSocket::sendData("",QString("matrice")))
        qDebug()<<"erreur d'écriture";
}

void VrBut::sl_reset()
{
    if(!MyTcpSocket::sendData("",QString("matrice vide")))
        qDebug()<<"erreur d'écriture";
}

void VrBut::sl_enable(bool state)
{
    retry->setEnabled(state);
    start->setEnabled(state);
}
