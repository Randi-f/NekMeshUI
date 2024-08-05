#include "hosurfacedialog.h"
#include "ui_hosurfacedialog.h"

HOSurfaceDialog::HOSurfaceDialog(map<string, string>* values,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HOSurfaceDialog)
    , values(values)
{
    ui->setupUi(this);
    connect(ui->btnSave, &QPushButton::clicked, this, &HOSurfaceDialog::onSaveBtnClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    if (values) {
        if (values->find("desc") != values->end()) {
            ui->textDesc->setText(QString::fromStdString((*values)["desc"]));
        }
        if (values->find("surfopti") != values->end()) {
            QString surfopti = QString::fromStdString((*values)["surfopti"]);
            ui->checkSurfopti->setChecked(surfopti == "true");
        }
    }
}

HOSurfaceDialog::~HOSurfaceDialog()
{
    delete ui;
}


void HOSurfaceDialog::onSaveBtnClicked()
{
    (*values)["desc"] = ui->textDesc->text().toStdString();
    (*values)["surfopti"] = ui->checkSurfopti->isChecked()?"true":"false";
    accept();
}
