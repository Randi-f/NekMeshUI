#include "outputdialog.h"
#include "ui_outputdialog.h"

OutputDialog::OutputDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OutputDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &OutputDialog::onButtonClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &OutputDialog::onButtonClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &OutputDialog::onButtonClicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &OutputDialog::onButtonClicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &OutputDialog::onButtonClicked);
}

OutputDialog::~OutputDialog()
{
    delete ui;
}

void OutputDialog::onButtonClicked() {
    // 获取发送信号的按钮
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        selectedOption = button->text();
        accept(); // 关闭对话框
    }
}

QString OutputDialog::getSelectedOption() const {
    return selectedOption;
}
