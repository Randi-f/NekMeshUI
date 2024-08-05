#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include <QDialog>

using namespace std;
namespace Ui {
class ProcessDialog;
}

class ProcessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessDialog(map<string, string>* values, QWidget *parent = nullptr);
    virtual ~ProcessDialog();
    map<string, string>& getValues() {
        return *values;
    }

protected:
    virtual void onSaveBtnClicked();

private:
    Ui::ProcessDialog *ui;
    map<string, string> *values;
};

#endif // PROCESSDIALOG_H
