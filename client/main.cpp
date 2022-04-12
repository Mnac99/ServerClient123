#include "client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationDisplayName(Client::tr(" Client"));
    Client c;

    c.show();
    return a.exec();
}
