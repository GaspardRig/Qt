#include "ModeAuto.h"
#include "MyTcpSocket.h"
#include <QPushButton>
#include <QDebug>

ModeAuto::ModeAuto(QWidget *parent) : QWidget(parent)
{
    go = new QPushButton(QIcon (":/icon/go.png"),"GO!",this);
    go->setMinimumSize(266,266);
    go->setIconSize(QSize(266,266));
    go->setText("");
    go->setFlat(true);
    QObject::connect(go, &QPushButton::clicked, this , sl_go);
}

void ModeAuto::sl_go(){
    MyTcpSocket::sendData(QString("GO! AUTO"));
    MyTcpSocket::recvData();;
}
