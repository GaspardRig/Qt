#include "MyTcpSocket.h"
#include "QTcpSocket"
#include <QDataStream>

QString MyTcpSocket::Id_code= "azertyuiop1234\n";
QString MyTcpSocket::Ip_adrr = "172.20.10.14";
qint16 MyTcpSocket::Id_port = 975;
QPointer<QTcpSocket> MyTcpSocket::socket = new QTcpSocket();

QString MyTcpSocket::recvData()
{
    if(!(socket->waitForReadyRead()))
        qDebug()<<"pas de donnée recu";
    return QString(socket->readAll());
}

bool MyTcpSocket::sendData(QString data)
{
    if (data==QString("INIT"))
    {
        qDebug() << "Connection in progress";
        socket->connectToHost(Ip_adrr,Id_port); //IP Adress, Port ...This ain't blocking call
        if (!socket->waitForConnected(5000)) //5s waiting
        {
            qDebug() << "Error: " << socket->errorString();
            return false;
        }
        else
        {
            qDebug()<< "Connected";

            qDebug()<<recvData(); //confirmtion de la connection

            sendData(Id_code.toStdString().c_str()); //envoie du code ID

            qDebug()<<recvData(); //confirmtion de l'ID

            //qDebug()<<recvData(); //message d'acceuil
        }
    }
    else if (data==QString("STOP"))
    {
        sendData("<STOP>\n");
        qDebug() << "Disconnection in progress";
        socket->disconnectFromHost();
        qDebug()<< "Disconnected";
        return true;

    }else{
        socket->write(data.toUtf8().data());
        if(!(socket->waitForBytesWritten()))
        {
            qDebug()<<"erreur d'écriture";
            return false;
        }
        return true;
    }
}
