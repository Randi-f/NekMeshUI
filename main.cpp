#include "mainwindow.h"

#include <QApplication>

#include "glwidget.h"
// #include "CADWidget.h"
#include "parameterform.h"
// #include "myglwidget.h"
#include "generator2ddialog.h"

#include <QCoreApplication> // for test
#include "testnekmeshobject.h"
#include <QDir> // test
#include <QDebug>

// #include <QApplication>
// #include <QMainWindow>
// #include "ui_simplemainwindow.h"



void mainFunction(int argc, char *argv[]){
    // main function
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.exec();
}

void testCAD(int argc, char *argv[]){
    //CAD test with CAD system
    QApplication a(argc, argv);
    GLWidget widget;
    auto logOutput = std::make_shared<StreamOutput>(std::cout);
    Logger m_log(logOutput, INFO);
    MeshSharedPtr mesh = std::shared_ptr<Mesh>(new Mesh());

    ModuleSharedPtr module;
        ////**** CAD ****////
    module = GetModuleFactory().CreateInstance(
        ModuleKey(eProcessModule, "loadcad"), mesh);
    module->SetLogger(m_log);
    module->RegisterConfig("filename", "/Users/shihan/Desktop/t106a.geo");
    module->RegisterConfig("voidpoints", "");
    module->RegisterConfig("2D", "");

    module->SetDefaults();
    try{
        module->Process();
    }
    catch (NekMeshError &e)
    {
        cout << "process error" << endl;
    }
    widget.setMesh(mesh);
    widget.show();
    a.exec();
}

int main(int argc, char *argv[])
{

    mainFunction(argc, argv);
    // testCAD(argc, argv);
    return 0;

    // QApplication app(argc, argv);
    // map<string, string> values;
    // Generator2DDialog widget(&values);
    // widget.show();
    // return app.exec();

    // QApplication app(argc, argv);  // 创建应用程序实例

    // QMainWindow mainWindow;  // 创建一个主窗口实例
    // Ui::MainWindow ui; // 创建自动生成的 UI 实例

    // ui.setupUi(&mainWindow); // 将 UI 应用于主窗口
    // mainWindow.show();       // 显示主窗口

    // return app.exec();       // 进入 Qt 的事件循环



    // test
    // QCoreApplication app(argc, argv);
    // TestNekMeshObject test;
    // return QTest::qExec(&test, argc, argv);




    // QCoreApplication app(argc, argv);
    // Get the current working directory
    // QString currentPath = QDir::currentPath();
    // qDebug() << "Current working directory:" << currentPath;
    // return 0;


    // different colors test
    // QApplication app(argc, argv);

    // MyGLWidget widget;
    // widget.resize(400, 400);
    // widget.show();
    // return app.exec();

    //OpenGL test
    // QApplication a(argc, argv);

    // GLWidget widget;
    // widget.resize(800, 600);
    // widget.show();

    // return a.exec();

    //CAD test
    // GeoParser parser;
    // parser.readGeoFile("/Users/shihan/Desktop/t106a.geo");
    // // parser.printData();
    // QApplication a(argc, argv);
    // CADWidget widget;
    // widget.setGeometryData(parser.points,parser.splines,parser.lineLoops,parser.planeSurfaces);
    // widget.show();
    // return a.exec();

    //CAD test2
    // QApplication a(argc, argv);
    // CADWidget widget;

    // GeoParser parser;
    // parser.readGeoFile("/Users/shihan/Desktop/t106a.geo");
    // widget.setCADData(parser.getCADData());
    // widget.show();
    // return a.exec();


    //parameterform
    // QApplication a(argc, argv);
    // ParameterForm widget;
    // widget.show();

    // return a.exec();






}




