#include "generator2ddialog.h"
#include "ui_generator2ddialog.h"

Generator2DDialog::Generator2DDialog(map<string, string>* values, QWidget *parent)
    : QDialog(parent), ui(new Ui::Generator2DDialog), values(values)
{
    ui->setupUi(this);

    connect(ui->btnSave, &QPushButton::clicked, this, &Generator2DDialog::onSaveBtnClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    // connect(ui->checkAdjust, &QCheckBox::stateChanged, ui->checkAdjustAll, &Generator2DDialog::setCheckAdjustState);
    // Use lambda to call the existing slot with the correct parameters
    connect(ui->checkAdjustAll, &QCheckBox::stateChanged, this, [this](int state) {
        setCheckAdjustState(); // Call the existing slot with the state parameter
    });

    // Ensure that values is not nullptr
    if (values) {
        if (values->find("desc") != values->end()) {
            ui->textDesc->setText(QString::fromStdString((*values)["desc"]));
        }
        if (values->find("makeBL") != values->end()) {
            string makeBL = (*values)["makeBL"];
            ui->checkMakeBL->setChecked(makeBL == "true");
        }
        if (values->find("blcurves") != values->end()) {
            ui->textBlcurves->setText(QString::fromStdString((*values)["blcurves"]));
        }
        if (values->find("blthick") != values->end()) {
            ui->textBlthick->setText(QString::fromStdString((*values)["blthick"]));
        }
        if (values->find("adjust") != values->end()) {
            QString adjust = QString::fromStdString((*values)["adjust"]);
            ui->checkAdjust->setChecked(adjust == "true");
        }
        if (values->find("adjustall") != values->end()) {
            QString adjustall = QString::fromStdString((*values)["adjustall"]);
            ui->checkAdjustAll->setChecked(adjustall == "true");
        }
    }
}

Generator2DDialog::~Generator2DDialog()
{
    delete ui;
}

void Generator2DDialog::setCheckAdjustState(){
    if(ui->checkAdjustAll->isChecked()){
        ui->checkAdjust->setChecked(true);
    }
}

void Generator2DDialog::onSaveBtnClicked() {
    // Modify the map through the pointer
    (*values)["desc"] = ui->textDesc->text().toStdString();
    (*values)["blcurves"] = ui->textBlcurves->text().toStdString();
    (*values)["blthick"] = ui->textBlthick->text().toStdString();
    (*values)["adjust"] = ui->checkAdjust->isChecked()?"true":"false";
    (*values)["adjustall"] = ui->checkAdjustAll->isChecked()?"true":"false";
    accept();
}
