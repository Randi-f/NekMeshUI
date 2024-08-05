#ifndef BLDIALOG_H
#define BLDIALOG_H

#include <QDialog>

namespace Ui {
class BlDialog;
}

class BlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BlDialog(QWidget *parent = nullptr);
    ~BlDialog();

private:
    Ui::BlDialog *ui;
};

#endif // BLDIALOG_H
