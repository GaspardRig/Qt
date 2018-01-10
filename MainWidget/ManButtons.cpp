#include "ManButtons.h"
#include "MyTcpSocket.h"
#include "QPushButton"
#include "QDebug"
#include "QVBoxLayout"

rotateButtons::rotateButtons(QWidget *parent) : QWidget(parent)
{

    rotate_left = new QPushButton(QIcon (":/icon/rotate_left"),"rotate_left",this);
    rotate_left->setMinimumSize(100,100);
    rotate_left->setIconSize(QSize(100,100));
    rotate_left->setText("");
    rotate_left->setFlat(true);
    QObject::connect(rotate_left, &QPushButton::pressed, this , sl_left_press);
    QObject::connect(rotate_left, &QPushButton::released, this , sl_left_relsd);

    rotate_right = new QPushButton(QIcon (":/icon/rotate_right"),"rotate_right",this);
    rotate_right->setMinimumSize(100,100);
    rotate_right->setIconSize(QSize(100,100));
    rotate_right->setText("");
    rotate_right->setFlat(true);
    QObject::connect(rotate_right, &QPushButton::pressed, this , sl_right_press);
    QObject::connect(rotate_right, &QPushButton::released, this , sl_right_relsd);

    v_layout = new QVBoxLayout(this);
    v_layout->addWidget(rotate_left);
    v_layout->addWidget(rotate_right);

}

void rotateButtons::sl_right_press(){
    MyTcpSocket::sendData(QString("rotate_right_press"));
    MyTcpSocket::recvData();
}
void rotateButtons::sl_right_relsd(){
    MyTcpSocket::sendData(QString("rotate_right_released"));
    MyTcpSocket::recvData();
}

void rotateButtons::sl_left_press(){
    MyTcpSocket::sendData(QString("rotate_right_press"));
    MyTcpSocket::recvData();
}

void rotateButtons::sl_left_relsd(){
    MyTcpSocket::sendData(QString("rotate_right_released"));
    MyTcpSocket::recvData();
}
