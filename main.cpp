#include "mainwindow.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("cgal-qt-demos");

    Q_INIT_RESOURCE(File);
    Q_INIT_RESOURCE(View);
    Q_INIT_RESOURCE(Input);
    Q_INIT_RESOURCE(CGAL);

    MainWindow mainWindow;
    mainWindow.show();

    QStringList args = app.arguments();
    args.removeAt(0);
    Q_FOREACH(QString filename, args)
    {
        mainWindow.open(filename);
    }

    return app.exec();
}
