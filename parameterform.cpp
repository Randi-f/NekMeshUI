#include "parameterform.h"
#include "ui_parameterform.h"

ParameterForm::ParameterForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ParameterForm)
{
    ui->setupUi(this);
}

ParameterForm::~ParameterForm()
{
    delete ui;
}

void ParameterForm::getSettings(){  // bools

}
