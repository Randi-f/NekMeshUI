#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QStyleFactory>
#include <QTableWidget>
#include <QFileDialog>
#include <NekMesh/Module/Module.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nekMeshObjectPtr = std::make_shared<NekMeshObject>();

    //连接addModuleBtn信号槽
    connect(ui->btn_addModule, &QPushButton::clicked, this, &MainWindow::onAddModuleBtnClicked);
    connect(ui->btn_run, &QPushButton::clicked, this, &MainWindow::onRunBtnClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    // delete nekMeshObject;
}

void MainWindow::onRunBtnClicked(){
    qDebug() << "running";
    nekMeshObjectPtr->addOutputFile("default");
    nekMeshObjectPtr->process();



}
void MainWindow::onAddModuleBtnClicked(){
    qDebug() << "add module";
    // Get the model from the tree view
    QAbstractItemModel *model = ui->treeView->model();

    if (model != nullptr) {
        // Check if the model has any rows
        if (model->rowCount() > 0) {
            qDebug() << "Tree view is not empty.";
        } else {
            qDebug() << "Tree view is empty.";
        }
    } else {
        qDebug() << "No model is set for the tree view.";
        importFile();
    }
}

void MainWindow::importFile(){
    // Get the directory of the application
    // QString appDir = QCoreApplication::applicationDirPath();
    // qDebug() << appDir;
    // Move up two levels
    // QDir dir(appDir);
    // dir.cdUp(); // Move up one level
    // dir.cdUp(); // Move up another level

    // Create the full file path
    // QString parentDirPath = dir.absolutePath();

    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "", "All Files (*)");

    if (!filePath.isEmpty()) {
        // QFileInfo fileInfo(filePath);
        // QString fileName = fileInfo.fileName();
        // ui->label->setText("File Path: " + filePath + "\nFile Name: " + fileName);
        qDebug() << filePath;
        nekMeshObjectPtr->addInputModule(filePath.toStdString());
        model = new QStandardItemModel(ui->treeView);
        model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("modules"));    //设置列头
        QStandardItem* item = new QStandardItem("input module");
        model->appendRow(item);
        QStandardItem* childItem = new QStandardItem(filePath);
        item->appendRow(childItem);
        // model->setItem(model->indexFromItem(item).row(),1,new QStandardItem(filePath));

        ui->treeView->setStyle(QStyleFactory::create("windows")); // 设置虚线
        ui->treeView->setModel(model);

    }

    //
}
