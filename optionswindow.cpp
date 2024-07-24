#include "optionswindow.h"
#include "ui_optionswindow.h"

OptionsWindow::OptionsWindow(MainWindow *mainWindow, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OptionsWindow)
    , mainWindow(mainWindow)
{
    ui->setupUi(this);
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}
