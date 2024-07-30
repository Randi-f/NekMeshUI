#include "peraligndialog.h"
#include "ui_peraligndialog.h"

using namespace std;

PerAlignDialog::PerAlignDialog(map<string, string>* values, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PerAlignDialog)
    , values(values)
{
    ui->setupUi(this);

    connect(ui->btnSave, &QPushButton::clicked, this, &PerAlignDialog::onSaveBtnClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);


    // Ensure that values is not nullptr
    if (values) {
        if (values->find("desc") != values->end()) {
            ui->lineDescription->setText(QString::fromStdString((*values)["desc"]));
        }
        if (values->find("surf1") != values->end()) {
            ui->comboBox->setCurrentText(QString::fromStdString((*values)["surf1"]));
        }
        if (values->find("surf2") != values->end()) {
            ui->comboBox_2->setCurrentText(QString::fromStdString((*values)["surf2"]));
        }
        if (values->find("dir") != values->end()) {
            QString dir = QString::fromStdString((*values)["dir"]);
            if (dir == "x") {
                ui->radioBtnX->setChecked(true);
            } else if (dir == "y") {
                ui->radioBtnY->setChecked(true);
            } else {
                ui->radioBtnZ->setChecked(true);
            }
        }
    }


}

PerAlignDialog::~PerAlignDialog()
{
    delete ui;
    // delete values;
}


std::map<std::string, std::string>& PerAlignDialog::getValues() {
    return *values; // Return a reference to the map
}

void PerAlignDialog::onSaveBtnClicked() {
    // Modify the map through the pointer
    (*values)["desc"] = ui->lineDescription->text().toStdString();
    (*values)["surf1"] = ui->comboBox->currentText().toStdString();
    (*values)["surf2"] = ui->comboBox_2->currentText().toStdString();

    if (ui->radioBtnX->isChecked()) {
        (*values)["dir"] = "x";
    } else if (ui->radioBtnY->isChecked()) {
        (*values)["dir"] = "y";
    } else {
        (*values)["dir"] = "z";
    }

    // Debugging output
    qDebug() << QString::fromStdString((*values)["desc"]);

    accept(); // Close the dialog with an "Accepted" result
}

