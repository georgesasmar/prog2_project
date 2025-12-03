#include "mainwindow.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    // Create directories
    QDir dir;
    dir.mkpath("data");
    dir.mkpath("reports");
    
    QApplication app(argc, argv);
    
    // Use default system theme - no custom styling
    
    MainWindow window;
    window.show();
    
    return app.exec();
}