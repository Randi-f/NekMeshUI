#include "bldialog.h"
#include "ui_bldialog.h"

BlDialog::BlDialog(map<string, string>* values, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BlDialog)
    , values(values)
{
    ui->setupUi(this);
    connect(ui->btnSave, &QPushButton::clicked, this, &BlDialog::onSaveBtnClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    // Ensure that values is not nullptr
    if (values) {
        if (values->find("desc") != values->end()) {
            ui->textDesc->setText(QString::fromStdString((*values)["desc"]));
        }
        if (values->find("bllayers") != values->end()) {
            ui->textBllayers->setText(QString::fromStdString((*values)["bllayers"]));
        }
        if (values->find("blsurfs") != values->end()) {
            // split with ","
            string blsurfs = (*values)["blsurfs"];
            size_t end = blsurfs.find(','); // only one ",", so use if not while
            if (end != std::string::npos) {
                ui->lineSurf1->setText(QString::fromStdString(blsurfs.substr(0, end)));
                ui->lineSurf2->setText(QString::fromStdString(blsurfs.substr(end+1)));
            }
        }
        if (values->find("order") != values->end()) {
            ui->textOrder->setText(QString::fromStdString((*values)["order"]));
        }
        if (values->find("blprog") != values->end()) {
            ui->textBlprog->setText(QString::fromStdString((*values)["blprog"]));
        }
    }
}

BlDialog::~BlDialog()
{
    delete ui;
}

void BlDialog::onSaveBtnClicked()
{
    (*values)["desc"] = ui->textDesc->text().toStdString();
    (*values)["bllayers"] = ui->textBllayers->text().toStdString();
    string surf1 = ui->lineSurf1->text().toStdString();
    string surf2 = ui->lineSurf2->text().toStdString();
    (*values)["blsurfs"] = surf1 + "," + surf2;
    (*values)["order"] = ui->textOrder->text().toStdString();
    (*values)["blprog"] = ui->textBlprog->text().toStdString();
    accept();
}
