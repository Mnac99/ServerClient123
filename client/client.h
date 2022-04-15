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
    void on_pushButton_clicked();

    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetButton();


private:
     QNetworkAccessManager* manager;

    QComboBox *hostCombo ;
    QLineEdit *portLineEdit;
    QLabel *statusLabel ;
    QPushButton *getButton ;
    QPushButton *showButton;


    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;

    QStringList list;
    QStringList imagelist;
    QString name;
    QLabel * label_img;
     int counter ;
};



#endif // CLIENT_H
