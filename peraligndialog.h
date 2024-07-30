#ifndef PERALIGNDIALOG_H
#define PERALIGNDIALOG_H

#include <QDialog>
using namespace std;

namespace Ui {
class PerAlignDialog;
}

class PerAlignDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PerAlignDialog(map<string, string>* values, QWidget *parent = nullptr);
    ~PerAlignDialog();
    std::map<std::string, std::string>& getValues();

private:
    Ui::PerAlignDialog *ui;
    map<string, string> *values;
    void onSaveBtnClicked();
};

#endif // PERALIGNDIALOG_H
