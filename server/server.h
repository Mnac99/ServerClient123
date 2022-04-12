#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>
#include <QList>
QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
QT_END_NAMESPACE

class Server : public QDialog
{
    Q_OBJECT

public:
   explicit Server(QWidget *parent = nullptr);

private slots:
    void send();
    QStringList filereader();
private:
    void initServer();


    QLabel *statusLabel = nullptr;
    QTcpServer * tcpServer = nullptr;
    QList<QString> list;
    QStringList l;


};
#endif // SERVER_H
