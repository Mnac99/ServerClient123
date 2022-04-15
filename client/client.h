#ifndef CLIENT_H
#define CLIENT_H


#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QtNetwork>
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit  Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void requestNew();
    void read();
    void fileHandler();
    void on_pushButton_clicked();

    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetButton();

private:
    void deleteImages();


private:


    QComboBox *hostCombo ;
    QLineEdit *portLineEdit;
    QLabel *statusLabel ;
    QPushButton *getButton ;
    QPushButton *showButton;
    QDataStream in;
    QStringList imagelist;
    QLabel * label_img;
    int counter ;
    QNetworkAccessManager* manager;
    QTcpSocket *tcpSocket;
};



#endif // CLIENT_H
