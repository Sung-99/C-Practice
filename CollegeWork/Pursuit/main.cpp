#include "Pursuit.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Pursuit w;
    w.show();
    return a.exec();
}
