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
class QLineEdit;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
private :
    QPointer<MyTcpSocket> my_socket;
    QPointer<joystick> joy;
    QPointer<rotateButtons> rotBut;
    QPointer<VrGrid> vr_grid;
    QPointer<VrDrag> vr_drag;
    QPointer<VrBut> vr_but;
    QPointer<ModeAuto> auto_view;

    QPointer<QPushButton> autom;
    QPointer<QPushButton> manuel;
    QPointer<QPushButton> virtuel;
    QPointer<QPushButton> connect_state;
    QPointer<QPushButton> calibration;
    QPointer<QPushButton> control;
    QPointer<QGridLayout> layout;
    QPointer<QLineEdit> line_edit;

    void hide_all();

private slots:
    void sl_autom();
    void sl_manuel();
    void sl_virtuel();
    void sl_connect_state();
    void sl_calibration();
    void sl_control();
    void sl_recv();
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
