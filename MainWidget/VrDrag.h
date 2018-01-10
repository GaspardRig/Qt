#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QFrame>
#include <QPointer>

class QLabel;

class VrDrag : public QFrame
{
    Q_OBJECT
public:
    VrDrag(QWidget *parent = 0);
public slots :
    void sl_reset();
private:
    QPointer <QLabel> boatIcon;
    QPointer <QLabel> carIcon;
    void paint();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // DRAGWIDGET_H
