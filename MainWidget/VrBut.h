#ifndef BUTTON_H
#define BUTTON_H

#include <QWidget>
#include <QPointer>

class QPushButton;
class VrGrid;
class MyTcpSocket;
class VrDrag;
class QVBoxLayout;

class VrBut : public QWidget
{
    Q_OBJECT
public:
    VrBut(VrGrid *vr_grid,VrDrag *dwid, QWidget *parent =0);
private:
    QPointer<QPushButton> retry;
    QPointer<QPushButton> start;
    QPointer<QVBoxLayout> layout;

private slots:
    void sl_start();
    void sl_reset();
    void sl_enable(bool state);
};

#endif // BUTTON_H
