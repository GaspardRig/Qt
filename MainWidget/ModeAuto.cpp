#include "ModeAuto.h"
#include "MyTcpSocket.h"
#include <QPushButton>
#include <QDebug>
#define MAIN_WINDOW_SIZE 480


ModeAuto::ModeAuto(QWidget *parent) : QWidget(parent)
{
    setFixedSize(MAIN_WINDOW_SIZE/2,MAIN_WINDOW_SIZE/2);
    go = new QPushButton(QIcon (":/icon/go.png"),"GO!",this);
    go->setMinimumSize(MAIN_WINDOW_SIZE/2,MAIN_WINDOW_SIZE/2);
    go->setIconSize(QSize(MAIN_WINDOW_SIZE/2,MAIN_WINDOW_SIZE/2));
    go->setText("");
    go->setFlat(true);
    go->setFont(QFont("DejaVu Sans"));
    QObject::connect(go.data(), &QPushButton::clicked, this , &ModeAuto::sl_go);
}

void ModeAuto::sl_go(){
    if(!MyTcpSocket::sendData(QString("GO! AUTO")))
        qDebug()<<"erreur d'écriture";
}
