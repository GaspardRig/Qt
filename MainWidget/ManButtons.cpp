#include "ManButtons.h"
#include "MyTcpSocket.h"
#include <QPushButton>
#include <QDebug>
#include <QVBoxLayout>
#define MAIN_WINDOW_SIZE 480


rotateButtons::rotateButtons(QWidget *parent) : QWidget(parent)
{

    setFixedSize(MAIN_WINDOW_SIZE/2,MAIN_WINDOW_SIZE/2);

    rotate_left = new QPushButton(QIcon (":/icon/rotate_left"),"rotate_left",this);
    rotate_left->setMinimumSize(MAIN_WINDOW_SIZE/6,MAIN_WINDOW_SIZE/6);
    rotate_left->setIconSize(QSize(MAIN_WINDOW_SIZE/6,MAIN_WINDOW_SIZE/6));
    rotate_left->setText("");
    rotate_left->setFlat(true);
    rotate_left->setFont(QFont("DejaVu Sans"));
    QObject::connect(rotate_left.data(), &QPushButton::pressed, this , &rotateButtons::sl_left_press);
    QObject::connect(rotate_left.data(), &QPushButton::released, this , &rotateButtons::sl_relsd);

    rotate_right = new QPushButton(QIcon (":/icon/rotate_right"),"rotate_right",this);
    rotate_right->setMinimumSize(MAIN_WINDOW_SIZE/6,MAIN_WINDOW_SIZE/6);
    rotate_right->setIconSize(QSize(MAIN_WINDOW_SIZE/6,MAIN_WINDOW_SIZE/6));
    rotate_right->setText("");
    rotate_right->setFlat(true);
    rotate_right->setFont(QFont("DejaVu Sans"));
    QObject::connect(rotate_right.data(), &QPushButton::pressed, this , &rotateButtons::sl_right_press);
    QObject::connect(rotate_right.data(), &QPushButton::released, this , &rotateButtons::sl_relsd);

    v_layout = new QVBoxLayout(this);
    v_layout->addWidget(rotate_left);
    v_layout->addWidget(rotate_right);

}

void rotateButtons::sl_right_press(){
    if(!MyTcpSocket::sendData("",QString("RD\n")))
        qDebug()<<"erreur d'écriture";
}
void rotateButtons::sl_relsd(){
    if(!MyTcpSocket::sendData("",QString("ST\n")))
        qDebug()<<"erreur d'écriture";
}

void rotateButtons::sl_left_press(){
    if(!MyTcpSocket::sendData("",QString("RG\n")))
        qDebug()<<"erreur d'écriture";
}


