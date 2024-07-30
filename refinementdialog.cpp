#include "refinementdialog.h"
#include "ui_refinementdialog.h"

RefinementDialog::RefinementDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RefinementDialog)
{
    ui->setupUi(this);
    connect(ui->btnSave, &QPushButton::clicked, this, &RefinementDialog::onSaveBtnClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

RefinementDialog::~RefinementDialog()
{
    delete ui;
}

QString RefinementDialog::getRefinement(){
    return refinement;

}

void RefinementDialog::onSaveBtnClicked(){
    refinement = ui->textX->text() + "," + ui->textY->text() + "," + ui->textZ->text()
         + "," + ui->textX_2->text()+ "," + ui->textY_2->text() + "," + ui->textZ_2->text()
         + "," + ui->textR->text() + "," + ui->textD->text();
    accept();
}
