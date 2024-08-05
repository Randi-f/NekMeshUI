#ifndef GENERATOR2DDIALOG_H
#define GENERATOR2DDIALOG_H

#include <QDialog>
#include "processdialog.h"
namespace Ui {
class Generator2DDialog;
}

class Generator2DDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Generator2DDialog(map<string, string>* values, QWidget *parent = nullptr);
    ~Generator2DDialog();
    map<string, string>& getValues() {
        return *values;
    }

private:
    Ui::Generator2DDialog *ui;
    map<string, string> *values;

protected:
    void onSaveBtnClicked() ;

private slots:
    void setCheckAdjustState();
};

#endif // GENERATOR2DDIALOG_H
