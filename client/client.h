#ifndef CLIENT_H
#define CLIENT_H


#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
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

    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetButton();

private:

    QComboBox *hostCombo = nullptr;
    QLineEdit *portLineEdit = nullptr;
    QLabel *statusLabel = nullptr;
    QPushButton *getButton = nullptr;


    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString currentFortune;
    QStringList list;
};



#endif // CLIENT_H
