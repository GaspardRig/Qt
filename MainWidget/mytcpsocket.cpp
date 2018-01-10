#include "MyTcpSocket.h"
#include "QTcpSocket"
#include "MainWindow.h"

QString MyTcpSocket::Id_code= "azertyuiop1234\n";
QString MyTcpSocket::Ip_adrr = "192.168.2.1";
qint16 MyTcpSocket::Id_port = 975;
QPointer<QTcpSocket> MyTcpSocket::socket = new QTcpSocket(nullptr);

bool MyTcpSocket::send(const char * str)
{
    socket->write(str,sizeof(str));
    if(!(socket->waitForBytesWritten(500)))
    {
        qDebug()<<"erreur d'écriture";
        return false;
    }
    return true;
}

QString MyTcpSocket::recvData()
{
    if(!(socket->waitForReadyRead(500)))
        qDebug()<<"pas de donnee recu";
    return QString(socket->readAll());
}

bool MyTcpSocket::select_mode(QString mode)
{
    if(MyTcpSocket::send(mode.toUtf8().data()))
    {
        if(MyTcpSocket::recvData()=="<OK>\n")
        {
            qDebug()<<"On passse en mode : "<< mode;
            return true;
        }
        else
        {
            qDebug()<<"impossible de passser en : "<<mode;
            return false;
        }
    }
    return false;
}


bool MyTcpSocket::sendData(QString data)
{
    if (data==QString("INIT"))
    {
        qDebug() << "Start connection";
        socket->open(QIODevice::ReadWrite);
        socket->connectToHost(Ip_adrr,Id_port); //IP Adress, Port ...This ain't blocking call
        if (!socket->waitForConnected(500)) //0,5s waiting
        {
            qDebug() << "Error: " << socket->errorString();
            return false;
        }
        else
        {
            qDebug()<<"Server : "<<recvData(); //confirmtion de la connection
            if(send(Id_code.toStdString().c_str()))
            {
                qDebug()<<"Server : "<<recvData(); //confirmtion de l'ID
                //qDebug()<<"Server : "<<recvData(); //message d'acceuil
                return true;
            }
            return false;
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
            return true;
        }
        return false;
    }
    else if (data==QString("AUTO"))
    {
        return select_mode("<AUTO_MODE>\n");
    }
    else if (data==QString("MANU"))
    {
        return select_mode("<MANUAL_MODE>\n");
    }
    else if (data==QString("VIRT"))
    {
        return select_mode("<VIRTUAL_MODE>\n");
    }
    else if (data==QString("TAKE_CONTROL"))
    {
        return send ("<TAKE_CONTROL>\n");
    }
    else
    {
        if(MainWindow::b_control)
        {
            return send(data.toUtf8().data());
        }
        return false;
    }
}
