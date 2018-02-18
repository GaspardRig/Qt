#include <QMouseEvent>
#include <QLabel>
#include <QDebug>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include "VrDrag.h"

VrDrag::VrDrag(QWidget *parent)
    : QFrame(parent)
{
    setFixedSize(200, 50);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(false);
    paint();
}

void VrDrag::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child)
        return;

    QPixmap pixmap = *child->pixmap();
    qDebug()<<child->accessibleName();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - child->pos())<<child->accessibleName();

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-png", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);//transformer le curseur par l'image
    drag->setHotSpot(event->pos() - child->pos());//garde la position du clic par rapport a l'image

    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();

    child->setPixmap(tempPixmap);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        child->close();
    } else {
        child->setPixmap(pixmap);
    }
}

void VrDrag::paint()
{
    boatIcon = new QLabel(this);
    boatIcon->setPixmap(QPixmap(":/icon/flag1.png"));
    boatIcon->move(40, 10);
    boatIcon->setAccessibleName("2");
    boatIcon->show();
    boatIcon->setAttribute(Qt::WA_DeleteOnClose);

    carIcon = new QLabel(this);
    carIcon->setPixmap(QPixmap(":/icon/car1.png"));
    carIcon->move(140, 10);
    carIcon->setAccessibleName("3");
    carIcon->show();
    carIcon->setAttribute(Qt::WA_DeleteOnClose);
}

void VrDrag::sl_reset()
{
    paint();
}
