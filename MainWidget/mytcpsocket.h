#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <Qpointer>

class QTcpSocket;

class MyTcpSocket : public QObject
{
    Q_OBJECT
public:
    static bool sendData(QString data);
    static QString recvData();
private:
    static QPointer <QTcpSocket> socket;
    static QString Id_code ;
    static QString Ip_adrr ;
    static qint16 Id_port ;
};


#endif // MYTCPSOCKET_H
