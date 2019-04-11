#include "platformclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    platformclient w;
    w.show();

    return a.exec();
}
