#include "MyTcpSocket.h"
#include <QTcpSocket>
#include "MainWindow.h"

QString MyTcpSocket::Id_code= "azertyuiop1234\n";
QString MyTcpSocket::Ip_adrr = "192.168.2.1"; //ip local host
int MyTcpSocket::Id_port = 975;
bool MyTcpSocket::connect=false;
QPointer<QTcpSocket> MyTcpSocket::socket = new QTcpSocket(nullptr);

bool MyTcpSocket::send(const char * str)
{
    socket->write(str);
    qDebug()<<"Send : "<< str;
    if(!(socket->waitForBytesWritten(500)))
    {
        qDebug()<<"erreur d'écriture";
        return false;
    }
    return true;
}

QString MyTcpSocket::recvData()
{
    if(!connect)
    {
        if(!(socket->waitForReadyRead(500)))
        {
            qDebug()<<"NO DATA TO READ";
            return "empty";
        }
    }
    return  QString(socket->readAll());
}

bool MyTcpSocket::sendData(QString IP,QString data)
{
    if (data==QString("INIT"))
    {
        qDebug() << "Start connection to : " << IP << ":" << Id_port;
        socket->open(QIODevice::ReadWrite);
        socket->connectToHost(IP,Id_port); //IP Adress, Port ...This ain't blocking call
        if (!socket->waitForConnected(500)) //0,5s waiting
            qDebug() << "Error: " << socket->errorString();
        else
        {
            //the fisrt step of connection always done !!
            QString id = recvData();
            qDebug()<<"Server : " << id;
            if ( id == "<ID_CODE>")
            {
                if(!MyTcpSocket::send(Id_code.toStdString().c_str()))
                    qDebug()<< "échec de l'envoie de l'id_code";
                else
                {
                    QString str = recvData();
                    qDebug()<<"Server : " << str;
                    if (str == QString("<ID_VALID>"))
                    {
                        qDebug()<<"Appli : Connected";
                        connect=true;
                        return true;
                    }
                    else if (str == QString("<ID_INVALID>"))
                        qDebug()<<"Appli : échec de la connection";
                }
            }
        }
    }
    else if (data==QString("STOP"))
    {
        if(send("<STOP>\n"))
        {
            qDebug() << "Start disconnection";
            socket->disconnectFromHost();
            socket->close();
            qDebug()<< "Disconnected";
            connect=false;
            return true;
        }
    }
    else
        return send(data.toUtf8().data());
    return false;
}

QPointer <QTcpSocket> MyTcpSocket::get_socket()
{
    return socket;
}
