#include "role.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Role w;
    w.show();
    return a.exec();
}
