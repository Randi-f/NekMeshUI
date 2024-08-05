#include "addprocessmoduledialog.h"
#include "ui_addprocessmoduledialog.h"

AddProcessModuleDialog::AddProcessModuleDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddProcessModuleDialog)
{
    ui->setupUi(this);
    connect(ui->btnPeralign, &QPushButton::clicked, this, &AddProcessModuleDialog::onButtonClicked);
    connect(ui->btnLoadoctree, &QPushButton::clicked, this, &AddProcessModuleDialog::onButtonClicked);
    connect(ui->btn2dgenerator, &QPushButton::clicked, this, &AddProcessModuleDialog::onButtonClicked);
    connect(ui->btnBoundaryLayer, &QPushButton::clicked, this, &AddProcessModuleDialog::onButtonClicked);
}

AddProcessModuleDialog::~AddProcessModuleDialog()
{
    delete ui;
}

void AddProcessModuleDialog::onButtonClicked() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        selectedOption = button->text();
        accept(); // 关闭对话框
    }
}

QString AddProcessModuleDialog::getSelectedOption() const {
    return selectedOption;
}
