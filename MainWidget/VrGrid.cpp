#include <QDragEnterEvent>
#include <QLabel>
#include <QDebug>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include "VrGrid.h"
#include "MyTcpSocket.h"
#include <QJsonDocument>
#include <QJsonObject>


VrGrid::VrGrid(int size, QWidget *parent) :
    QWidget(parent)
{
    setArrival=setCar=true;
    block_size = size/grid_size;
    setAcceptDrops(true);
    reset_enable = 0;
    resize(block_size*grid_size,block_size*grid_size + 2 * block_size);
    RAZ_matrice();
}

void VrGrid::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);
    painter.setPen(Qt::black);
    for (int x = 0 ; x<=(block_size*grid_size); x+=block_size){
        painter.drawLine(x,0,x,block_size*grid_size);
    }
    for (int y = 0 ; y<=(block_size*grid_size); y=y+block_size){
        painter.drawLine(0,y,block_size*grid_size,y);
    }

    for (int x = 0 ; x<(grid_size); ++x){
        for (int y = 0 ; y<(grid_size); ++y){
            switch (matrice[x][y]){
            case 0:
                break;
            case 1:
                painter.fillRect(x*block_size,y*block_size,block_size,block_size,Qt::blue);
                break;
            case 2:
                painter.fillRect(x*block_size,y*block_size,block_size,block_size,Qt::green);
                break;
            case 3:
                painter.fillRect(x*block_size,y*block_size,block_size,block_size,Qt::red);
                break;
            default:
                break;
            }
        }
    }
}

void VrGrid::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-png")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void VrGrid::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-png")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void VrGrid::dropEvent(QDropEvent *event)
{
    if(event->pos().y() < block_size*grid_size && event->pos().x() < grid_size*block_size)
    {
        if (event->mimeData()->hasFormat("application/x-png")){
            QByteArray itemData = event->mimeData()->data("application/x-png");
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);

            QPixmap pixmap;
            dataStream >> pixmap;

            QPoint pos((event->pos().x()/block_size),(event->pos().y()/block_size));
            if(pos.x() == 0)
                pos.setX(pos.x() + 1);
            if(pos.x() == grid_size-1)
                pos.setX(pos.x() - 1);
            if(pos.y() == 0)
                pos.setY(pos.y() + 1);
            if(pos.y() == grid_size-1)
                pos.setY(pos.y() - 1);
            pos.setX(pos.x()*block_size);
            pos.setY(pos.y()*block_size);
            if(itemData.at(itemData.size()-1) == '2')
            {
                if(matrice[pos.x()/block_size+1][pos.y()/block_size+1] == 3 ||
                        matrice[pos.x()/block_size-1][pos.y()/block_size-1] == 3 ||
                        matrice[pos.x()/block_size+1][pos.y()/block_size-1] == 3 ||
                        matrice[pos.x()/block_size-1][pos.y()/block_size+1] == 3 )
                    return;
                newIcon1 = new QLabel(this);
                newIcon1->setPixmap(pixmap);
                newIcon1->move(pos);
                newIcon1->show();
                newIcon1->setAttribute(Qt::WA_DeleteOnClose);
                for (int x = 0 ; x<(grid_size); ++x){
                    for (int y = 0 ; y<(grid_size); ++y){
                        if(matrice[x][y] == STOP ){
                            matrice[x][y]=EMPTY;
                        }
                    }
                }
                if (setArrival)
                {
                    reset_enable++;
                    setArrival=false;
                }
                fill_matrice(pos.x()/block_size,pos.y()/block_size,2);
            }else if(itemData.at(itemData.size()-1) == '3'){
                if(matrice[pos.x()/block_size+1][pos.y()/block_size+1] == 2 ||
                        matrice[pos.x()/block_size-1][pos.y()/block_size-1] == 2 ||
                        matrice[pos.x()/block_size-1][pos.y()/block_size+1] == 2 ||
                        matrice[pos.x()/block_size+1][pos.y()/block_size-1] == 2)
                    return;
                newIcon2 = new QLabel(this);
                newIcon2->setPixmap(pixmap);
                newIcon2->move(pos);
                newIcon2->show();
                newIcon2->setAttribute(Qt::WA_DeleteOnClose);
                for (int x = 0 ; x<(grid_size); ++x){
                    for (int y = 0 ; y<(grid_size); ++y){
                        if(matrice[x][y] == START ){
                            matrice[x][y]=EMPTY;
                        }
                    }
                }
                if (setCar)
                {
                    reset_enable++;
                    setCar=false;
                }
                fill_matrice(pos.x()/block_size,pos.y()/block_size,3);
            }
            else
            {
                qDebug()<<"icone inconnu";
            }
            update();
            event->setDropAction(Qt::MoveAction);
            event->accept();
            if(reset_enable == 2){
                emit sg_enable(true);
                reset_enable=0;
                setArrival=setCar=true;
            }
        }else{
            event->ignore();
        }
    }else{
        event->ignore();
    }
}

void VrGrid::mousePressEvent(QMouseEvent *e) {

    if(enableMouse)
    {
        QLabel *child = static_cast<QLabel*>(childAt(e->pos()));
        if (child){
            QPixmap pixmap = *child->pixmap();
            QByteArray itemData;
            QDataStream dataStream(&itemData, QIODevice::WriteOnly);
            dataStream << pixmap << QPoint(e->pos() - child->pos()) << QString::number(matrice[e->pos().x()/block_size][e->pos().y()/block_size]);

            QMimeData *mimeData = new QMimeData;
            mimeData->setData("application/x-png", itemData);

            QDrag *drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap);//transformer le curseur par l'image
            drag->setHotSpot(e->pos() - child->pos());//garde la position du clic par rapport a l'image

            QPixmap tempPixmap = pixmap;
            QPainter painter;
            painter.begin(&tempPixmap);
            painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
            painter.end();

            child->setPixmap(tempPixmap);

            if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
                child->close();
            } else {
                //child->show();
                child->setPixmap(pixmap);
            }
        } else {
            //        qDebug()<<"X"<<e->x()<<"valeur : "<<(e->x()/block_size)*block_size;
            //        qDebug()<<"Y"<<e->y()<<"valeur : "<<(e->y()/block_size)*block_size;
            if(e->y() < block_size*grid_size && e->x() < grid_size*block_size){
                switch(matrice[e->x()/block_size][e->y()/block_size]){
                case 0:
                    fill_matrice(e->x()/block_size,e->y()/block_size,1);
                    break;
                case 1:
                    matrice[e->x()/block_size][e->y()/block_size] = 0;
                    break;
                case 2:
                    break;
                case 3:
                    break;
                default:
                    break;
                }
                this->update();
            }else
                qDebug()<<"Click hors de la Zone !";
        }
    }
}

void VrGrid::fill_matrice(int x, int y, int n)
{
    matrice[x][y] = n;
    if((x + 1 < grid_size) && (y + 1) < grid_size && (matrice[x + 1][y + 1] < 2))
        matrice[x + 1][y + 1] = n;
    if((x - 1 >= 0) && (y + 1) < grid_size && (matrice[x - 1][y + 1] < 2))
        matrice[x - 1][y + 1] = n;
    if((x + 1 < grid_size) && (y - 1) >= 0 && (matrice[x + 1][y - 1] < 2))
        matrice[x + 1][y - 1] = n;
    if((x - 1 >= 0) && (y - 1) >= 0 && (matrice[x - 1][y - 1] < 2))
        matrice[x - 1][y - 1] = n;
    if((x + 1 < grid_size) && (matrice[x + 1][y] < 2))
        matrice[x + 1][y] = n;
    if((x - 1 >= 0) && (matrice[x - 1][y] < 2))
        matrice[x - 1][y] = n;
    if((y + 1) < grid_size && (matrice[x][y + 1] < 2))
        matrice[x][y + 1] = n;
    if((y - 1) >= 0 && (matrice[x][y - 1] < 2))
        matrice[x][y - 1] = n;
}

void VrGrid::sl_reset(){
    reset_enable = 0;
    emit sg_enable(false);
    RAZ_matrice();
    if(!newIcon1.isNull())
        newIcon1->setPixmap(QPixmap());
    if(!newIcon2.isNull())
        newIcon2->setPixmap(QPixmap());
    this->update();
}


//Slot call when the start button
//of the VIRTUAL mode is clicked
void VrGrid::sl_start_vr()
{
    MyTcpSocket::sendData(writeJson());
}

//Slot call when the start button
//of the AUTO mode is clicked
void VrGrid::sl_start_auto()
{
    RAZ_matrice();
}

void VrGrid::RAZ_matrice()
{
    int iteX,iteY;
    iteY=iteX=0;
    for(iteX=0;iteX<= grid_size-1 ; iteX++)
    {
        for(iteY=0;iteY<= grid_size-1 ; iteY++)
        {
            matrice[iteX][iteY]=EMPTY;
        }
        iteY=0;
    }
}

QString VrGrid::writeJson()
{

    //création du json
      QJsonObject file;

    //taille de l'arène
    file.insert("Scene", QString::number(grid_size));

    {//position des blocs
          QJsonObject bloc;
          QJsonObject bloc_pos;
        bloc.insert("Number", QString::number(writeItems(&bloc_pos,BLOC)));
        bloc.insert("Position", bloc_pos);
        file.insert("Bloc", bloc);
    }
    {//position du départ
          QJsonObject start;
        writeItem(&start,START);
        file.insert("Start", start);
    }
    {//position d'arrivee
          QJsonObject stop;
        writeItem(&stop,STOP);
        file.insert("Stop", stop);
    }

    {//position du chemin
          QJsonObject path;
          QJsonObject path_pos;
        path.insert("Number", QString::number(writeItems(&path_pos,PATH)));
        path.insert("Position", path_pos);
        file.insert("Path", path);
    }

    qDebug()<<"Json write : \n" << QJsonDocument(file).toJson(QJsonDocument::Compact);
    return (QJsonDocument(file).toJson(QJsonDocument::Compact));
}

void VrGrid::writeItem(QJsonObject *obj , int value)
{
      int iteratorX,iteratorY;
    for(iteratorX=0;iteratorX<= grid_size-1 ; iteratorX++)
    {
        for(iteratorY=0;iteratorY<= grid_size-1 ; iteratorY++)
        {
            if(matrice[iteratorX][iteratorY]==value){
                obj->insert("X:",QString::number(iteratorX));
                obj->insert("Y:",QString::number(iteratorY));
            }
        }
        iteratorY=0;
    }
}

int VrGrid::writeItems(QJsonObject *obj , int value){
    int iteratorX,iteratorY,count;
    count=0;
    for(iteratorY=grid_size-1;iteratorY>=0  ; iteratorY--)
    {
        for(iteratorX=0;iteratorX<= grid_size-1 ; iteratorX++)
        {
            if(matrice[iteratorX][iteratorY]==value){
                QString data;
                data.append(QString::number(iteratorX));
                data.append(",");
                data.append(QString::number(iteratorY));
                obj->insert(QString::number(count),data);
                count++;
            }
        }
        iteratorX=0;
    }
    return count;
}
