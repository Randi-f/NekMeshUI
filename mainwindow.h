#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "NekMeshObject.h"
#include <QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

    // TreeView Model
    QStandardItemModel* model;
    // TableVIEW Model
    QStandardItemModel* table_model;

    std::shared_ptr<NekMeshObject> nekMeshObjectPtr;
    void importFile();

private slots:

    void onAddModuleBtnClicked();
    void onRunBtnClicked();
    void onSaveBtnClicked();

};


#endif // MAINWINDOW_H
