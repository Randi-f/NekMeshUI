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
#include "glwidget.h"
#include "sourcewindow.h"
#include "optionswindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sourceWnd(new SourceWindow(this))
    , optionsWnd(new OptionsWindow(this))
{
    ui->setupUi(this);


    nekMeshObjectPtr = std::make_shared<NekMeshObject>();

    glWidget = new GLWidget(ui->frame);
    glWidget->setGeometry(0, 0, ui->frame->width(), ui->frame->height());
    glWidget->setMesh(nekMeshObjectPtr->mesh);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->btn_addModule, &QPushButton::clicked, this, &MainWindow::onAddModuleBtnClicked);
    connect(ui->btn_deleteModule, &QPushButton::clicked, this, &MainWindow::onDeleteModuleBtnClicked);
    connect(ui->btn_run, &QPushButton::clicked, this, &MainWindow::onRunBtnClicked);
    connect(ui->btn_save, &QPushButton::clicked, this, &MainWindow::onSaveBtnClicked);

    connect(ui->actionmsh, &QAction::triggered, this, [this]() { importCertainFile('0'); });
    connect(ui->actionmcf, &QAction::triggered, this, [this]() { importCertainFile('1'); });
    connect(ui->actionRun, &QAction::triggered, this, &MainWindow::onRunBtnClicked);

    connect(ui->btnBrowseFile, &QPushButton::clicked, this, &MainWindow::browseFile);

    ui->stackedWidget->addWidget(sourceWnd);
    ui->stackedWidget->addWidget(optionsWnd);

    btnGroup.addButton(ui->btnSource, 0);
    btnGroup.addButton(ui->btnOptions, 1);

    connect(&btnGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            [=](QAbstractButton* button){
                int index = btnGroup.id(button); // 获取按钮在按钮组中的索引
                ui->stackedWidget->setCurrentIndex(index);
            });

    // 设置默认选中的页面
    btnGroup.button(0)->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete sourceWnd;  // Cleanup SourceWindow
    delete optionsWnd;
}

void MainWindow::onDeleteModuleBtnClicked(){
    QModelIndex index = ui->treeView->currentIndex();
    if (index.isValid()) {
        model->removeRow(index.row(), index.parent());
    }

}

void MainWindow::onSaveBtnClicked(){

    map<string, string>values;
    // 遍历 model，将第一列作为键，第二列作为值存入 map
    for (int row = 0; row < table_model->rowCount(); ++row) {
        QModelIndex keyIndex = table_model->index(row, 0);
        QModelIndex valueIndex = table_model->index(row, 1);
        QString key = table_model->data(keyIndex).toString();
        QString value = table_model->data(valueIndex).toString();
        values[key.toStdString()] = value.toStdString();
    }

    //update TreeView
    // 在模型中添加新元素的示例
    if (!ui->treeView->model()) {
        model = new QStandardItemModel(ui->treeView);
        model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("modules"));
        ui->treeView->setStyle(QStyleFactory::create("windows")); // 设置虚线
        ui->treeView->setModel(model);
        QStandardItem* item = new QStandardItem(QString::fromStdString(values["moduleType"]));
        model->appendRow(item);
        for (const auto& pair : values) {
            QStandardItem *childItem = new QStandardItem(QString::fromStdString(pair.first+":"+pair.second));
            item->appendRow(childItem);
        }
    }
    else{
        QStandardItem *rootItem = model->invisibleRootItem();
        QStandardItem* item = new QStandardItem(QString::fromStdString(values["moduleType"]));
        rootItem->appendRow(item);
        for (const auto& pair : values) {
            QStandardItem *childItem = new QStandardItem(QString::fromStdString(pair.first+":"+pair.second));
            item->appendRow(childItem);
        }
    }

}

void MainWindow::onRunBtnClicked(){

    qDebug() << "loading configurations";

    QStandardItem *rootItem = model->invisibleRootItem();
    for (int row = 0; row < rootItem->rowCount(); ++row) {
        QStandardItem *item = rootItem->child(row);
        if (item->text().toStdString()=="input") {
            QStandardItem *inputFilePath = item->child(0);
            nekMeshObjectPtr->addInputModule(inputFilePath->text().toStdString());
            qDebug()<< "input loaded:" << inputFilePath->text();
        }
        else if(item->text().toStdString()=="output"){
            QStandardItem *outputType = item->child(0);
            nekMeshObjectPtr->addOutputModule("default",outputType->text().toStdString());
            qDebug()<< "output loaded:" << outputType->text();
            QMessageBox::information(this, "hint", "file stored at Desktop!");


        }
        else if(item->text().toStdString()=="peralign"){
            map<string, string>values;
            qDebug() << "peralign loaded";
            for(int childrow=0; childrow < item->rowCount(); ++childrow){
                QStandardItem *childItem = item->child(childrow);
                QStringList parts = childItem->text().split(":");
                values[parts[0].toStdString()]=parts[1].toStdString();
            }
            nekMeshObjectPtr->addProcessModule(values);
        }
        else {
            map<string, string>values;
            for(int childrow=0; childrow < item->rowCount(); ++childrow){
                QStandardItem *childItem = item->child(childrow);
                QStringList parts = childItem->text().split(":");
                values[parts[0].toStdString()]=parts[1].toStdString();
            }
            qDebug() << "module: " << values["moduleType"] << " loaded";
            nekMeshObjectPtr->addProcessModule(values);
        }
    }

    nekMeshObjectPtr->process();
    // QMessageBox::information(this, "hint", "file stored at Desktop");

    glWidget->setMesh(nekMeshObjectPtr->mesh);
    glWidget->update();
}

void MainWindow::onAddModuleBtnClicked(){

    // Get the model from the tree view
    QAbstractItemModel *model = ui->treeView->model();

    SelectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString selectedOption = dialog.getSelectedOption();
        // QMessageBox::information(this, "选择结果", "你在对话框中选择了: " + selectedOption);
        if(selectedOption.compare("input", Qt::CaseSensitive) == 0 ){
            importFile();
        }
        else if(selectedOption.compare("CAD", Qt::CaseSensitive) == 0){
            // Init TableView
            table_model = new QStandardItemModel();
            ui->tableView->setModel(table_model); // 设置m_pMyTableView的数据模型为m_model

            // 设置列字段名
            table_model->setColumnCount(2);
            table_model->setHeaderData(0,Qt::Horizontal, "param");
            table_model->setHeaderData(1,Qt::Horizontal, "value");
            // 设置一条数据
            table_model->setItem(0, 0, new QStandardItem("moduleType"));
            table_model->setItem(0, 1, new QStandardItem("loadcad"));
            table_model->setItem(1, 0, new QStandardItem("filename"));
            table_model->setItem(1, 1, new QStandardItem("6412"));
            table_model->setItem(2, 0, new QStandardItem("voidpoints"));
            table_model->setItem(2, 1, new QStandardItem(""));
            table_model->setItem(3, 0, new QStandardItem("2D"));
            table_model->setItem(3, 1, new QStandardItem("true"));
            table_model->setItem(4, 0, new QStandardItem("NACA"));
            table_model->setItem(4, 1, new QStandardItem("-1.0,-1.0,3.0,1.0,15.0")); // default false / -1.0,-1.0,3.0,1.0,15.0
        }
        else if(selectedOption.compare("loadoctree", Qt::CaseSensitive) == 0){
            // Init TableView
            table_model = new QStandardItemModel();
            ui->tableView->setModel(table_model); // 设置m_pMyTableView的数据模型为m_model

            // 设置列字段名
            table_model->setColumnCount(2);
            table_model->setHeaderData(0,Qt::Horizontal, "param");
            table_model->setHeaderData(1,Qt::Horizontal, "value");
            // 设置一条数据
            table_model->setItem(0, 0, new QStandardItem("moduleType"));
            table_model->setItem(0, 1, new QStandardItem("loadoctree"));
            table_model->setItem(1, 0, new QStandardItem("MinDelta"));
            table_model->setItem(1, 1, new QStandardItem("0.04"));
            table_model->setItem(2, 0, new QStandardItem("MaxDelta"));
            table_model->setItem(2, 1, new QStandardItem("3.0"));
            table_model->setItem(3, 0, new QStandardItem("EPS"));
            table_model->setItem(3, 1, new QStandardItem("0.1"));
            table_model->setItem(4, 0, new QStandardItem("refinement"));
            table_model->setItem(4, 1, new QStandardItem("0.0,0.0,0.0,3.0,0.0,0.0,0.3,0.2")); // default false / -1.0,-1.0,3.0,1.0,15.0
            table_model->setItem(5, 0, new QStandardItem("curve_refinement"));
            table_model->setItem(5, 1, new QStandardItem("false"));
            table_model->setItem(6, 0, new QStandardItem("writeoctree"));
            table_model->setItem(6, 1, new QStandardItem("false"));
        }
        else if(selectedOption.compare("2dgenerator", Qt::CaseSensitive) == 0){
            // Init TableView
            table_model = new QStandardItemModel();
            ui->tableView->setModel(table_model); // 设置m_pMyTableView的数据模型为m_model

            // 设置列字段名
            table_model->setColumnCount(2);
            table_model->setHeaderData(0,Qt::Horizontal, "param");
            table_model->setHeaderData(1,Qt::Horizontal, "value");
            // 设置一条数据
            table_model->setItem(0, 0, new QStandardItem("moduleType"));
            table_model->setItem(0, 1, new QStandardItem("2dgenerator"));

            table_model->setItem(1, 0, new QStandardItem("makeBL"));
            table_model->setItem(1, 1, new QStandardItem("true"));
            table_model->setItem(2, 0, new QStandardItem("blcurves"));
            table_model->setItem(2, 1, new QStandardItem("5,6"));
            table_model->setItem(3, 0, new QStandardItem("blthick"));
            table_model->setItem(3, 1, new QStandardItem("0.07"));
            table_model->setItem(4, 0, new QStandardItem("adjust"));
            table_model->setItem(4, 1, new QStandardItem("false"));
            table_model->setItem(5, 0, new QStandardItem("adjustall"));
            table_model->setItem(5, 1, new QStandardItem("false"));
            table_model->setItem(6, 0, new QStandardItem("smoothbl"));
            table_model->setItem(6, 1, new QStandardItem("false"));
            table_model->setItem(7, 0, new QStandardItem("spaceoutbl"));
            table_model->setItem(7, 1, new QStandardItem("false"));

        }
        else if(selectedOption.compare("volumemesh", Qt::CaseSensitive) == 0){
            // Init TableView
            table_model = new QStandardItemModel();
            ui->tableView->setModel(table_model); // 设置m_pMyTableView的数据模型为m_model

            // 设置列字段名
            table_model->setColumnCount(2);
            table_model->setHeaderData(0,Qt::Horizontal, "param");
            table_model->setHeaderData(1,Qt::Horizontal, "value");
            // 设置一条数据
            table_model->setItem(0, 0, new QStandardItem("moduleType"));
            table_model->setItem(0, 1, new QStandardItem("volumemesh"));
            table_model->setItem(1, 0, new QStandardItem("makeBL"));
            table_model->setItem(1, 1, new QStandardItem("true"));
            table_model->setItem(2, 0, new QStandardItem("blsurfs"));
            table_model->setItem(2, 1, new QStandardItem("5,6"));
            table_model->setItem(3, 0, new QStandardItem("blthick"));
            table_model->setItem(3, 1, new QStandardItem("0.07"));
            table_model->setItem(4, 0, new QStandardItem("bllayers"));
            table_model->setItem(4, 1, new QStandardItem("3"));
            table_model->setItem(5, 0, new QStandardItem("blprog"));
            table_model->setItem(5, 1, new QStandardItem("2"));
        }
        else if(selectedOption.compare("bl", Qt::CaseSensitive) == 0){
            // Init TableView
            table_model = new QStandardItemModel();
            ui->tableView->setModel(table_model);

            // 设置列字段名
            table_model->setColumnCount(2);
            table_model->setHeaderData(0,Qt::Horizontal, "param");
            table_model->setHeaderData(1,Qt::Horizontal, "value");
            // 设置一条数据
            table_model->setItem(0, 0, new QStandardItem("moduleType"));
            table_model->setItem(0, 1, new QStandardItem("bl"));
            table_model->setItem(1, 0, new QStandardItem("bllayers"));
            table_model->setItem(1, 1, new QStandardItem("3"));
            table_model->setItem(2, 0, new QStandardItem("blsurfs"));
            table_model->setItem(2, 1, new QStandardItem("5,6"));
            table_model->setItem(3, 0, new QStandardItem("order"));
            table_model->setItem(3, 1, new QStandardItem("4"));
            table_model->setItem(4, 0, new QStandardItem("blprog"));
            table_model->setItem(4, 1, new QStandardItem("2"));
        }
        else if(selectedOption.compare("hosurface", Qt::CaseSensitive) == 0){
            // Init TableView
            table_model = new QStandardItemModel();
            ui->tableView->setModel(table_model);

            // 设置列字段名
            table_model->setColumnCount(2);
            table_model->setHeaderData(0,Qt::Horizontal, "param");
            table_model->setHeaderData(1,Qt::Horizontal, "value");
            // 设置一条数据
            table_model->setItem(0, 0, new QStandardItem("moduleType"));
            table_model->setItem(0, 1, new QStandardItem("hosurface"));
            table_model->setItem(1, 0, new QStandardItem("surfopti"));
            table_model->setItem(1, 1, new QStandardItem("true"));

        }

        else if(selectedOption.compare("peralign", Qt::CaseSensitive) == 0 ){
            // Init TableView
            table_model = new QStandardItemModel();
            ui->tableView->setModel(table_model);
            // 设置列字段名
            table_model->setColumnCount(2);
            table_model->setHeaderData(0,Qt::Horizontal, "param");
            table_model->setHeaderData(1,Qt::Horizontal, "value");
            // 设置一条数据
            table_model->setItem(0, 0, new QStandardItem("moduleType"));
            table_model->setItem(0, 1, new QStandardItem("peralign"));
            table_model->setItem(1, 0, new QStandardItem("surf1"));
            table_model->setItem(2, 0, new QStandardItem("surf2"));
            table_model->setItem(3, 0, new QStandardItem("dir"));
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
            QStandardItem *newItem = new QStandardItem("output");
            rootItem->appendRow(newItem);
            QStandardItem* childItem = new QStandardItem(selectedOption);
            newItem->appendRow(childItem);
        }
    }




}

void MainWindow::importFile(){

    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "", "All Files (*)");
    if (!filePath.isEmpty()) {
        //update TreeView
        model = new QStandardItemModel(ui->treeView);
        model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("modules"));
        QStandardItem* item = new QStandardItem("input");
        model->appendRow(item);
        QStandardItem* childItem = new QStandardItem(filePath);
        item->appendRow(childItem);
        ui->treeView->setStyle(QStyleFactory::create("windows")); // 设置虚线
        ui->treeView->setModel(model);

    }

}

void MainWindow::importCertainFile(char type)
{
    QString filter;
    switch(type){
        case '0':filter = "Mesh Files (*.msh)";break;
        case '1':   filter = "Mcf Files (*.mcf)";break;
        default:filter = "All Files (*)";break;
    }

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", filter);
    if (!filePath.isEmpty()) {
        //update TreeView
        model = new QStandardItemModel(ui->treeView);
        model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("modules"));
        QStandardItem* item = new QStandardItem("input");
        model->appendRow(item);
        QStandardItem* childItem = new QStandardItem(filePath);
        item->appendRow(childItem);
        ui->treeView->setStyle(QStyleFactory::create("windows")); // 设置虚线
        ui->treeView->setModel(model);

    }
}

void MainWindow::browseFile()
{
    int index = ui->comboFileType->currentIndex();
    QString filter;
    switch(index){
    case 0: filter = "Mesh Files (*.msh)";break;
    case 1: filter = "CAD Files (*.geo)";break;
    case 2: filter = "Mcf Files (*.mcf)";break;
    case 3: filter = "Xml Files (*.xml)";break;
    default:filter = "All Files (*)";break;
    }




    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", filter);
    if (!filePath.isEmpty()) {
        ui->textFileName->setText(filePath);
        nekMeshObjectPtr->addInputModule(filePath.toStdString());
        nekMeshObjectPtr->process();
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(nekMeshObjectPtr->mesh->GetNumElements()));
        ui->tableSource->setItem(0, 1, item);
    }
}


