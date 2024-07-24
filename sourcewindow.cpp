#include "sourcewindow.h"
#include "ui_sourcewindow.h"
#include "mainwindow.h"

SourceWindow::SourceWindow(MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SourceWindow),
    mainWindow(mainWindow)
{
    ui->setupUi(this);

    // Correctly reference MainWindow's slot
    connect(ui->btnImport, &QPushButton::clicked, mainWindow, &MainWindow::onAddModuleBtnClicked);
}

SourceWindow::~SourceWindow()
{
    delete ui;
}
