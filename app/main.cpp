#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application info
    app.setApplicationName("Shrinkly");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Shrinkly");

    // Create and show main window
    MainWindow window;
    window.show();

    return app.exec();
}
