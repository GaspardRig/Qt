#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPointer>

class QPushButton;
class QPixmap;
class QGridLayout;
class MyTcpSocket;
class rotateButtons;
class joystick;
class VrDrag;
class VrGrid;
class VrBut;
class ModeAuto;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private :
    MyTcpSocket *my_socket;
    joystick *joy;
    rotateButtons *rotBut;
    VrGrid *vr_grid;
    VrDrag *vr_drag;
    VrBut *vr_but;
    ModeAuto *auto_view;

    QPointer<QPushButton> autom;
    QPointer<QPushButton> manuel;
    QPointer<QPushButton> virtuel;
    QPointer<QPushButton> connect_state;
    QPointer<QGridLayout> layout;

    bool connect;
private slots:
    void sl_autom();
    void sl_manuel();
    void sl_virtuel();
    void sl_connect_state();

};

#endif // MAINWINDOW_H
