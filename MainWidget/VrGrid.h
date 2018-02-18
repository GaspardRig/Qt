#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPointer>

#define EMPTY 0
#define BLOC 1
#define STOP 2
#define START 3
#define PATH 4
#define grid_size 24

class QLabel;

class VrGrid : public QWidget
{
    Q_OBJECT
public:
    explicit VrGrid(int size, QWidget *parent = 0);
    int matrice [grid_size][grid_size];
    bool enableMouse=true;
private:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    QPointer <QLabel> newIcon1,newIcon2;
    void fill_matrice(int x, int y, int n);
    void RAZ_matrice();
    int reset_enable , block_size;
    bool setArrival , setCar;
    //Parser json
    QString writeJson();
    void writeItem(QJsonObject *obj , int value);
    int writeItems(QJsonObject *obj , int value);
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
public slots :
    void sl_reset();
    void sl_start_vr();
    void sl_start_auto();
signals :
    void sg_enable(bool state);
};

#endif // WIDGET_H
