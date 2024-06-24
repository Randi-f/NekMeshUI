#ifndef OUTPUTDIALOG_H
#define OUTPUTDIALOG_H

#include <QDialog>

namespace Ui {
class OutputDialog;
}

class OutputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OutputDialog(QWidget *parent = nullptr);
    ~OutputDialog();
    QString getSelectedOption() const;

private:
    Ui::OutputDialog *ui;
    QString selectedOption;

private slots:
    void onButtonClicked();
};

#endif // OUTPUTDIALOG_H
