#include "ManJoystick.h"
#include "MyTcpSocket.h"
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QtWidgets>
#include <QPainter>
#include <qdebug.h>

#define THICK 5
#define PI 3.14159265
#define TO_DEGRES 180/PI

joystick::joystick(int size, QWidget *parent)
    : QWidget(parent)
{
    installEventFilter(this);
    WidgetSize = size;
    y=x=WidgetSize/2;
    Rcircle = WidgetSize/2;
    Rjoystick =Rcircle/3;
    Rshadow = Rjoystick/2;
    resize(WidgetSize,WidgetSize); //dépendra de la taille de l'écran
    drag=0;
    tmp = new QTimer();
    QObject::connect(tmp,&QTimer::timeout,this,&joystick::clock);
}

void joystick::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter peintre(this);
    QPen pinceau(Qt::black, THICK, Qt::SolidLine);
    QPen pinceau2(Qt::NoBrush, THICK, Qt::SolidLine);
    peintre.setRenderHint(QPainter::Antialiasing);
    peintre.setRenderHint(QPainter::HighQualityAntialiasing);
    peintre.setPen(pinceau);
    peintre.drawEllipse(WidgetSize/2-Rcircle+THICK+Rjoystick/2, WidgetSize/2-Rcircle+THICK+Rjoystick/2, Rcircle*2-Rjoystick-THICK*2 , Rcircle*2-Rjoystick-THICK*2);
    peintre.setPen(pinceau2);
    peintre.setBrush(Qt::gray);
    peintre.drawEllipse(WidgetSize/2-Rshadow, WidgetSize/2-Rshadow, Rshadow*2, Rshadow*2);
    peintre.setBrush(Qt::black);
    peintre.setPen(pinceau);
    peintre.drawEllipse(x-Rjoystick, y-Rjoystick, Rjoystick*2, Rjoystick*2);
}

void joystick::clock()
{
    static int xx;
    static int yy;
    static QString l_data;
    if( xx!=x || yy != y){ //discutable...?
        if(x<WidgetSize/2-Rjoystick/2 || x>WidgetSize/2+Rjoystick/2
                || y<WidgetSize/2-Rjoystick/2 || y>WidgetSize/2+Rjoystick/2)
        {
            if(x<WidgetSize/2-Rjoystick/2 && y<WidgetSize/2-Rjoystick/2)
                l_data="HG\n";
            else if(x>WidgetSize/2+Rjoystick/2 && y<WidgetSize/2-Rjoystick/2)
                l_data="HD\n";
            else if(x<WidgetSize/2-Rjoystick/2 && y>WidgetSize/2+Rjoystick/2 )
                l_data="BG\n";
            else if(x>WidgetSize/2+Rjoystick/2  && y>WidgetSize/2+Rjoystick/2 )
                l_data="BD\n";
            else if(x<WidgetSize/2-Rjoystick/2 )
                l_data="GA\n";
            else if(x>WidgetSize/2+Rjoystick/2)
                l_data="DR\n";
            else if(y>WidgetSize/2+Rjoystick/2 )
                l_data="BA\n";
            else if(y<WidgetSize/2-Rjoystick/2 )
                l_data="HA\n";
        }
    }
    if(l_data!=m_data || m_data=="ST\n")
    {
        if(!MyTcpSocket::sendData(l_data))
            qDebug()<<"erreur d'écriture";
    }
    m_data=l_data;
    yy =y;
    xx =x;
}

bool joystick::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);

    if (event->type() == QEvent::MouseMove|| event->type() == QEvent::MouseButtonRelease|| event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        {//calcul de l'angle du joystick
            float ex=((float)mouseEvent->pos().x()-WidgetSize/2)/(Rcircle-THICK-Rjoystick/2);
            float ey=(-((float)mouseEvent->pos().y()-WidgetSize/2))/(Rcircle-THICK-Rjoystick/2);
            if(ey>0)
                angle = atan2(ey,ex)*TO_DEGRES;
            else
                angle=360+atan2(ey,ex)*TO_DEGRES;
            //qDebug()<<"atan2 : " << int(angle);
        }

        if (mouseEvent->type() == QEvent::MouseButtonPress)
        {
            drag = 1;
            tmp->start(100);//temps minimale entre deux envoie TCP
        }
        if (mouseEvent->type() == QEvent::MouseButtonRelease)
        {
            drag = 0;
            x =y= WidgetSize/2;
            tmp->stop();
            if (m_data!="ST\n")
            {
                m_data="ST\n";
                if(!MyTcpSocket::sendData(m_data))
                    qDebug()<<"erreur d'écriture";
            }
        }
        if(drag)
        {
            x= mouseEvent->pos().x();
            y = mouseEvent->pos().y();

            if(mouseEvent->pos().x()<Rjoystick+THICK && mouseEvent->pos().y()<Rjoystick+THICK)
            {
                x=Rjoystick+THICK;
                y=Rjoystick+THICK;
            }
            else if(mouseEvent->pos().x()<Rjoystick+THICK && mouseEvent->pos().y()>WidgetSize-Rjoystick-THICK)
            {
                x=Rjoystick+THICK;
                y=WidgetSize-Rjoystick-THICK;
            }
            else if(mouseEvent->pos().x()>WidgetSize-Rjoystick-THICK && mouseEvent->pos().y()<Rjoystick+THICK)
            {
                x=WidgetSize-Rjoystick-THICK;
                y=Rjoystick+THICK;
            }
            else if(mouseEvent->pos().x()>WidgetSize-Rjoystick-THICK && mouseEvent->pos().y()>WidgetSize-Rjoystick-THICK)
            {
                x=WidgetSize-Rjoystick-THICK;
                y=WidgetSize-Rjoystick-THICK;
            }
            else if(mouseEvent->pos().x()<Rjoystick+THICK)
            {
                x=Rjoystick+THICK;
                y=mouseEvent->pos().y();
            }
            else if(mouseEvent->pos().x()>WidgetSize-Rjoystick-THICK)
            {
                x=WidgetSize-Rjoystick-THICK;
                y=mouseEvent->pos().y();
            }
            else if(mouseEvent->pos().y()<Rjoystick+THICK)
            {
                y=Rjoystick+THICK;
                x=mouseEvent->pos().x();
            }
            else if(mouseEvent->pos().y()>WidgetSize-Rjoystick-THICK)
            {
                y=WidgetSize-Rjoystick-THICK;
                x=mouseEvent->pos().x();
            }
        }
        this->repaint();
    }
    return false;
}
