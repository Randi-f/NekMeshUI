#ifndef LOADOCTREEDIALOG_H
#define LOADOCTREEDIALOG_H

#include <QDialog>
#include <QListWidget>
#include "mainwindow.h"

using namespace std;
namespace Ui {
class LoadoctreeDialog;
}

class LoadoctreeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadoctreeDialog(MainWindow *mainWindow ,map<string, string>* values, QWidget *parent = nullptr);
    ~LoadoctreeDialog();
    map<string, string>& getValues();
signals:
    void itemSelected(const QString &itemText);


private:
    Ui::LoadoctreeDialog *ui;
    map<string, string> *values;
    MainWindow *mainWindow;
    void onSaveBtnClicked();

public slots:
    void onAddBtnClicked();
    void onRemoveBtnClicked();
    void onItemClicked(QListWidgetItem *item);
};

#endif // LOADOCTREEDIALOG_H
