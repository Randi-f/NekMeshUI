#ifndef ADDPROCESSMODULEDIALOG_H
#define ADDPROCESSMODULEDIALOG_H

#include <QDialog>

namespace Ui {
class AddProcessModuleDialog;
}

class AddProcessModuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddProcessModuleDialog(QWidget *parent = nullptr);
    ~AddProcessModuleDialog();
    QString getSelectedOption() const;

private:
    Ui::AddProcessModuleDialog *ui;
    QString selectedOption;

private slots:
    void onButtonClicked();
};

#endif // ADDPROCESSMODULEDIALOG_H

