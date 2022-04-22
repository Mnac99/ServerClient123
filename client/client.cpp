#include "client.h"
#include <QtWidgets>
#include <QDateTime>
#include <QDebug>
#include <QTcpSocket>


Client::Client(QWidget *parent)
    : QDialog(parent)
    , hostCombo (new QComboBox)
    , portLineEdit(new QLineEdit)
    , getButton(new QPushButton(tr("get")))
    , showButton(new QPushButton(tr("show")))
    , label_img (new QLabel (this))
    , counter(0)
    , manager(new QNetworkAccessManager(this))
    , tcpSocket(new QTcpSocket(this))


{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    hostCombo->setEditable(true);
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty())
    {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();

        if (!domain.isEmpty())
        {
            hostCombo->addItem(name + QChar('.') + domain);
        }
    }
    if (name != QLatin1String("localhost"))
    {
        hostCombo->addItem(QString("localhost"));
    }
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    for(int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    auto hostLabel = new QLabel(tr("&Server name:"));
    hostLabel->setBuddy(hostCombo);
    auto portLabel = new QLabel(tr("&Server port:"));
    portLabel->setBuddy(portLineEdit);

    statusLabel = new QLabel(tr("This Client project requires that you run the "
                                " Server project as well."));
    getButton->setDefault(false);
    getButton->setEnabled(true);
    auto quitButton = new QPushButton(tr("Quit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(getButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(showButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    in.setDevice(tcpSocket);
    connect(hostCombo, &QComboBox::editTextChanged,
            this, &Client::enableGetButton);
    connect(portLineEdit, &QLineEdit::textChanged,
            this, &Client::enableGetButton);
    connect(getButton, &QAbstractButton::clicked,
            this, &Client::requestNew);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::read);
    connect(tcpSocket, &QAbstractSocket::errorOccurred, this, &Client::displayError);
    connect(showButton, &QAbstractButton::clicked,
            this, &Client::on_pushButton_clicked);


    QGridLayout *mainLayout = nullptr;
    if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized())
    {
        auto outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        auto outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QGridLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    }
    else
    {
        mainLayout = new QGridLayout(this);
    }
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostCombo, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(statusLabel, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);
    setWindowTitle(QGuiApplication::applicationDisplayName());
    portLineEdit->setFocus();

}
void Client::requestNew()
{
    getButton->setEnabled(false);
    tcpSocket->abort();

    tcpSocket->connectToHost(hostCombo->currentText(),
                             portLineEdit->text().toInt());
    deleteImages();
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr(" Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr(" Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the  server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr(" Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    getButton->setEnabled(true);
}

void Client::read()
{
     QStringList Urldata;
    in >> Urldata;
    for(int i = 0; i <  Urldata.size(); ++i)
    {
        QUrl url = QUrl::fromUserInput( Urldata[i]);
        QNetworkReply *reply = manager->get(QNetworkRequest(url));

        in.startTransaction();

        connect(reply, &QNetworkReply::finished, this, &Client::fileHandler);
    }

}

void Client::fileHandler()
{
    auto reply = dynamic_cast<QNetworkReply*>(sender());
    if(!reply)
    {
        return;
    }

    QString name = QString("imageFiles_%1.png").arg(QDateTime::currentMSecsSinceEpoch());
    imagelist.push_back(name);

    QFile destinationFile(name);

    if(destinationFile.open(QFile::WriteOnly))
    {

        auto data = reply->readAll();
        destinationFile.write(data);

    }


    reply->deleteLater();

}

void Client::enableGetButton()
{
    getButton->setEnabled(!hostCombo->currentText().isEmpty() &&
                          !portLineEdit->text().isEmpty());
}
void Client::on_pushButton_clicked()
{
    if(imagelist.isEmpty())
        return;

    if(counter >= imagelist.size())
    {
        counter = 0;
    }

    QString name = imagelist[counter];

    qDebug()<< name;

    label_img->setWindowFlags(Qt::Window);
    QPixmap pm(name); // <- path to image file
    label_img->setPixmap(pm);
    counter++;
    label_img->setScaledContents(true);
    if (!label_img->isVisible()) {
        label_img->show();
    }
}

void Client::deleteImages()
{
    for(int i = 0; i <imagelist.size(); ++i)
    {
        QFile file(imagelist[i]);
        file.remove();
    }
     imagelist.clear();
}

Client::~Client()
{

   deleteImages();


}


