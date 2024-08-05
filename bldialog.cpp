#include "bldialog.h"
#include "ui_bldialog.h"

BlDialog::BlDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BlDialog)
{
    ui->setupUi(this);
}

BlDialog::~BlDialog()
{
    delete ui;
}
