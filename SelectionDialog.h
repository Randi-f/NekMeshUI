#ifndef SELECTIONDIALOG_H
#define SELECTIONDIALOG_H
#include <QVBoxLayout>
#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
class SelectionDialog : public QDialog {
    Q_OBJECT

public:
    SelectionDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("选择对话框");

        QVBoxLayout *layout = new QVBoxLayout(this);

        // 创建三个按钮
        QPushButton *button1 = new QPushButton("peralign", this);
        QPushButton *button2 = new QPushButton("output", this);
        QPushButton *button3 = new QPushButton("选项3", this);

        // 将按钮添加到布局中
        layout->addWidget(button1);
        layout->addWidget(button2);
        layout->addWidget(button3);

        // 连接按钮点击信号到槽函数
        connect(button1, &QPushButton::clicked, this, &SelectionDialog::onButtonClicked);
        connect(button2, &QPushButton::clicked, this, &SelectionDialog::onButtonClicked);
        connect(button3, &QPushButton::clicked, this, &SelectionDialog::onButtonClicked);
    }
    QString getSelectedOption() const {
        return selectedOption;
    }
private:
    QString selectedOption;

private slots:
    void onButtonClicked() {
        // 获取发送信号的按钮
        QPushButton *button = qobject_cast<QPushButton *>(sender());
        if (button) {
            selectedOption = button->text();
            accept(); // 关闭对话框
        }
    }


};


#endif // SELECTIONDIALOG_H
