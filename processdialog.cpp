#include "processdialog.h"
#include "ui_processdialog.h"

ProcessDialog::ProcessDialog(map<string, string>* values, QWidget *parent)
    : QDialog(parent), ui(new Ui::ProcessDialog), values(values)
{
    ui->setupUi(this);

    // connect(ui->btnSave, &QPushButton::clicked, this, &ProcessDialog::onSaveBtnClicked);
    // connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

ProcessDialog::~ProcessDialog()
{
    delete ui;
}


void ProcessDialog::onSaveBtnClicked()
{
    // Default implementation or empty if only to be overridden
}
