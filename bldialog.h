#ifndef BLDIALOG_H
#define BLDIALOG_H

#include <QDialog>
using namespace std;
namespace Ui {
class BlDialog;
}

class BlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BlDialog(map<string, string>* values, QWidget *parent = nullptr);
    ~BlDialog();
    map<string, string>& getValues() {
        return *values;
    }

private:
    Ui::BlDialog *ui;
    map<string, string> *values;
    void onSaveBtnClicked();
};

#endif // BLDIALOG_H



