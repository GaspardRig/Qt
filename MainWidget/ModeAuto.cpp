#include "ModeAuto.h"
#include "MyTcpSocket.h"
#include "VrGrid.h"
#include <QPushButton>
#include <QDebug>
#define MAIN_WINDOW_SIZE 480


ModeAuto::ModeAuto(VrGrid *auto_grid,QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(MAIN_WINDOW_SIZE/2,MAIN_WINDOW_SIZE/2);
    go = new QPushButton(QIcon (":/icon/go.png"),"GO!",this);
    go->setMinimumSize(MAIN_WINDOW_SIZE/2,MAIN_WINDOW_SIZE/2);
    go->setIconSize(QSize(MAIN_WINDOW_SIZE/2,MAIN_WINDOW_SIZE/2));
    go->setText("");
    go->setFlat(true);
    go->setFont(QFont("DejaVu Sans"));
    QObject::connect(go.data(), &QPushButton::clicked, this , &ModeAuto::sl_go);
    QObject::connect(go.data(), &QPushButton::clicked, auto_grid, &VrGrid::sl_start_auto);
}

void ModeAuto::sl_go(){
    if(!MyTcpSocket::sendData(QString("GO! AUTO")))
        qDebug()<<"erreur d'écriture";
}
