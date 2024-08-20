#ifndef REFINEMENTDIALOG_H
#define REFINEMENTDIALOG_H

#include <QDialog>
// #include "mainwindow.h"
namespace Ui {
class RefinementDialog;
}

class RefinementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RefinementDialog(QWidget *parent = nullptr);
    ~RefinementDialog();
    QString getRefinement();

private:
    Ui::RefinementDialog *ui;
    void onSaveBtnClicked();
    QString refinement;
    // MainWindow *mainWindow;
};

#endif // REFINEMENTDIALOG_H
