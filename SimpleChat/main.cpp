#include "SimpleChat.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimpleChat w;

    return a.exec();
}
