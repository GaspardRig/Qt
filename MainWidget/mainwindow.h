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
    //Manual mode widgets
    QPointer<joystick> joy;
    QPointer<rotateButtons> rotBut;

    // virtual mode widgets
    QPointer<VrGrid> vr_grid;
    QPointer<VrDrag> vr_drag;
    QPointer<VrBut> vr_but;

    //auto mode widgets
    QPointer<VrGrid> auto_grid;
    QPointer<ModeAuto> auto_view;

    //main window widgets
    QPointer<QPushButton> autom;
    QPointer<QPushButton> manuel;
    QPointer<QPushButton> virtuel;
    QPointer<QPushButton> connect_state;
    QPointer<QPushButton> calibration;
    QPointer<QPushButton> control;

    //layout for all widgets
    QPointer<QGridLayout> layout;

    //private method
    void hide_all();
    void readJson(QString strJson, VrGrid *grid);
    void read_subobj(QJsonObject *obj, int value, VrGrid *grid);
    void read_ip();
private slots:
    //slots for all buttons on the main window
    void sl_autom();
    void sl_manuel();
    void sl_virtuel();
    void sl_connect();
    void sl_calibration();
    void sl_control();

    // reception slots
    void sl_recv();

    // slot call when the application is close
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
