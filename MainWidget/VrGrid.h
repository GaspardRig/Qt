#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPointer>

#define grid_size 24

class QLabel;

class VrGrid : public QWidget
{
    Q_OBJECT

public:
    explicit VrGrid(int size, QWidget *parent = 0);
    int matrice [grid_size][grid_size];
private:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    QPointer <QLabel> newIcon1,newIcon2;
    void fill_matrice(int x, int y, int n);
    int reset_enable;
    int block_size;
    bool setArrival , setCar;
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
public slots :
    void sl_reset();
signals :
    void sg_enable(bool state);
};

#endif // WIDGET_H
