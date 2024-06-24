#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "outputdialog.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QStyleFactory>
#include <QTableWidget>
#include <QFileDialog>
#include <NekMesh/Module/Module.h>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialog>
#include <QMessageBox>
#include "SelectionDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nekMeshObjectPtr = std::make_shared<NekMeshObject>();

    connect(ui->btn_addModule, &QPushButton::clicked, this, &MainWindow::onAddModuleBtnClicked);
    connect(ui->btn_run, &QPushButton::clicked, this, &MainWindow::onRunBtnClicked);
    connect(ui->btn_save, &QPushButton::clicked, this, &MainWindow::onSaveBtnClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    // delete nekMeshObject;
}

void MainWindow::onSaveBtnClicked(){

    // QModelIndex index;
    // QString data = table_model->data(index, Qt::DisplayRole).toString();
    // QMessageBox::information(this, "单元格内容", QString("单元格 (%1, %2) 的内容是: %3").arg(row).arg(column).arg(data));
    map<string, string>values;
    // 遍历 model，将第一列作为键，第二列作为值存入 map
    for (int row = 0; row < table_model->rowCount(); ++row) {
        QModelIndex keyIndex = table_model->index(row, 0);
        QModelIndex valueIndex = table_model->index(row, 1);
        QString key = table_model->data(keyIndex).toString();
        QString value = table_model->data(valueIndex).toString();
        values[key.toStdString()] = value.toStdString();
    }
    nekMeshObjectPtr->addProcessModule(values);

    //update TreeView
    // 在模型中添加新元素的示例
    QStandardItem *rootItem = model->invisibleRootItem();
    QStandardItem* item = new QStandardItem("peralign module");
    rootItem->appendRow(item);
    for (const auto& pair : values) {
        QStandardItem *childItem = new QStandardItem(QString::fromStdString(pair.first+":"+pair.second));
        item->appendRow(childItem);
    }
}

void MainWindow::onRunBtnClicked(){
    qDebug() << "running";
    // OutputDialog dialog(this);
    // QString selectedOption;
    // if (dialog.exec() == QDialog::Accepted) {
    //     selectedOption = dialog.getSelectedOption();
    //     QMessageBox::information(this, "hint", "you choose: " + selectedOption);
    // }
    // nekMeshObjectPtr->addOutputFile("default",selectedOption.toStdString());
    nekMeshObjectPtr->process();
    QMessageBox::information(this, "hint", "file stored at Desktop");
}

void MainWindow::onAddModuleBtnClicked(){
    qDebug() << "add module";
    // Get the model from the tree view
    QAbstractItemModel *model = ui->treeView->model();

    if (model != nullptr) {
        // Check if the model has any rows
        if (model->rowCount() > 0) {
            qDebug() << "Tree view is not empty.";
            SelectionDialog dialog(this);
            if (dialog.exec() == QDialog::Accepted) {
                QString selectedOption = dialog.getSelectedOption();
                // QMessageBox::information(this, "选择结果", "你在对话框中选择了: " + selectedOption);

                if(selectedOption.compare("peralign", Qt::CaseSensitive) == 0 ){
                    // Init TableView
                    table_model = new QStandardItemModel();
                    ui->tableView->setModel(table_model); // 设置m_pMyTableView的数据模型为m_model

                    // 设置列字段名
                    table_model->setColumnCount(2);
                    table_model->setHeaderData(0,Qt::Horizontal, "param");
                    table_model->setHeaderData(1,Qt::Horizontal, "value");
                    // 设置一条数据
                    table_model->setItem(0, 0, new QStandardItem("surf1"));
                    table_model->setItem(1, 0, new QStandardItem("surf2"));
                    table_model->setItem(2, 0, new QStandardItem("dir"));
                }
                if(selectedOption.compare("output", Qt::CaseSensitive) == 0 ){
                    OutputDialog dialog(this);
                    QString selectedOption;
                    if (dialog.exec() == QDialog::Accepted) {
                        selectedOption = dialog.getSelectedOption();
                        QMessageBox::information(this, "hint", "you choose: " + selectedOption);
                    }

                    // update TreeView
                    QStandardItem *rootItem = this->model->invisibleRootItem();
                    QStandardItem *newItem = new QStandardItem("output module");
                    rootItem->appendRow(newItem);
                    QStandardItem* childItem = new QStandardItem(selectedOption);
                    newItem->appendRow(childItem);
                    nekMeshObjectPtr->addOutputFile("default",selectedOption.toStdString());
                }

            }
        } else {
            qDebug() << "Tree view is empty.";
        }
    } else {
        qDebug() << "Firstly add an input module.";
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

        //update TreeView
        model = new QStandardItemModel(ui->treeView);
        model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("modules"));    //设置列头
        QStandardItem* item = new QStandardItem("input module");
        model->appendRow(item);
        QStandardItem* childItem = new QStandardItem(filePath);
        item->appendRow(childItem);
        ui->treeView->setStyle(QStyleFactory::create("windows")); // 设置虚线
        ui->treeView->setModel(model);

    }

}




