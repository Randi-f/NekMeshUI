#ifndef LOADOCTREEDIALOG_H
#define LOADOCTREEDIALOG_H

#include <QDialog>

using namespace std;
namespace Ui {
class LoadoctreeDialog;
}

class LoadoctreeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadoctreeDialog(map<string, string>* values, QWidget *parent = nullptr);
    ~LoadoctreeDialog();
    map<string, string>& getValues();

private:
    Ui::LoadoctreeDialog *ui;
    map<string, string> *values;
    void onSaveBtnClicked();

public slots:
    void onAddBtnClicked();
    void onRemoveBtnClicked();
};

#endif // LOADOCTREEDIALOG_H
