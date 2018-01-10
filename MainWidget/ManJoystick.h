#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QWidget>
#include <QPointer>
class MyTcpSocket;
class QTimer;
class QGraphicsView;

class joystick:public QWidget
{
public:
    joystick(int size, QWidget *parent = 0);
private :
    QPointer <QGraphicsView> view;
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    int x, y,drag,WidgetSize,Rjoystick, Rcircle, Rshadow;
    QPointer <QPainter> peintre;
    QPointer <QTimer> tmp;
    QString m_data;

private slots:
    void clock();
};

#endif // JOYSTICK_H
