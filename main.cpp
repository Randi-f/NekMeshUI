#include "mainwindow.h"
#include "MeshRenderer.h"
#include <QApplication>

#include "glwidget.h"

int main(int argc, char *argv[])
{
    // main function
    // QApplication a(argc, argv);
    // MainWindow w;
    // w.show();
    // return a.exec();



    // QApplication app(argc, argv);

    // MeshRenderer renderer;
    // renderer.resize(800, 600);
    // renderer.show();

    // return app.exec();



    //OpenGL test
    QApplication a(argc, argv);

    GLWidget widget;
    widget.resize(800, 600);
    widget.show();

    return a.exec();
}




