#include "server.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationDisplayName(Server::tr(" Server"));
    Server s;
    s.show();
    return a.exec();
}
