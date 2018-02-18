#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QPointer>

class QTcpSocket;

class MyTcpSocket : public QObject
{
    Q_OBJECT
public:
    static bool connect;
    static bool sendData(QString data);
    static QString recvData();
    static QPointer <QTcpSocket> get_socket();
    static void set_ip(QString ip);
private:
    static QPointer <QTcpSocket> socket;
    static QString Id_code ;
    static QString Ip_adrr ;
    static int Id_port ;
    static bool send(const char* str);
};


#endif // MYTCPSOCKET_H
