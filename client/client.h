#ifndef CLIENT_H
#define CLIENT_H


#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QtNetwork>
QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
QT_END_NAMESPACE

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit  Client(QWidget *parent = nullptr);

private slots:
    void requestNew();
    void read();
    void fileHandler();

    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetButton();


private:
     QNetworkAccessManager* manager;

    QComboBox *hostCombo ;
    QLineEdit *portLineEdit;
    QLabel *statusLabel ;
    QPushButton *getButton ;


    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;

    QStringList list;
};



#endif // CLIENT_H
