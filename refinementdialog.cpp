#include "refinementdialog.h"
#include "ui_refinementdialog.h"
#include <QDoubleValidator>
RefinementDialog::RefinementDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RefinementDialog)
    // , mainWindow(mainWindow)
{
    ui->setupUi(this);
    connect(ui->btnSave, &QPushButton::clicked, this, &RefinementDialog::onSaveBtnClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    QDoubleValidator *validator = new QDoubleValidator(-1000.0, 1000.0, 4, this);
    validator->setNotation(QDoubleValidator::StandardNotation);//标准浮点表示法
    ui->textX->setValidator(validator);
    ui->textX_2->setValidator(validator);
    ui->textY->setValidator(validator);
    ui->textY_2->setValidator(validator);
    ui->textZ->setValidator(validator);
    ui->textZ_2->setValidator(validator);
    ui->textD->setValidator(validator);
    ui->textR->setValidator(validator);
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
