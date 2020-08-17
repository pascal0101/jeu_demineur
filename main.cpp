#include "plateau.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Plateau w;
    w.show();
    return a.exec();
}
