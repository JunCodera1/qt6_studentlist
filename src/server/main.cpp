#include <QCoreApplication>
#include "appserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    AppServer server;

    return a.exec();
}
