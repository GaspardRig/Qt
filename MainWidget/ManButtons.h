#ifndef ROTATEBUTTONS_H
#define ROTATEBUTTONS_H

#include <QWidget>
#include <QPointer>

class QPushButton;
class QVBoxLayout;

class rotateButtons : public QWidget
{
    Q_OBJECT
public:
    explicit rotateButtons(QWidget *parent = nullptr);
private :
    QPointer <QPushButton> rotate_left ,rotate_right;
    QPointer <QVBoxLayout> v_layout;
private slots :
    void sl_left_press();
    void sl_right_press();
    void sl_relsd();
signals:
    void sg_recv();
};

#endif // ROTATEBUTTONS_H
