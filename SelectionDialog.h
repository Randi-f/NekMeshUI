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


        // Create a QString list
        QStringList buttonLabels = {"input", "CAD", "loadoctree", "peralign",
                                    "2dgenerator", "hosurface", "bl",
                                    "volumemesh", "output"};

        // Iterate over the QString list and create buttons
        for (const QString &label : buttonLabels) {
            QPushButton *button = new QPushButton(label, this);
            layout->addWidget(button);
            connect(button, &QPushButton::clicked, this, &SelectionDialog::onButtonClicked);
        }

        // Set the layout to the widget
        this->setLayout(layout);

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
