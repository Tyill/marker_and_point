#include "gui/mainwin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
#ifdef _WIN32
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("plugins");
    QCoreApplication::setLibraryPaths(paths);
#endif

    QApplication a(argc, argv);
    MainWin w;
    w.show();
    return a.exec();
}
