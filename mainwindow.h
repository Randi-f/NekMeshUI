#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QPushButton>
#include <QButtonGroup>

#include "NekMeshObject.h"
#include "glwidget.h"

// Forward declaration
class SourceWindow;
class OptionsWindow;

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
    map<string, string> allParams;

private:
    Ui::MainWindow *ui;
    GLWidget *glWidget;
    // TreeView Model
    QStandardItemModel* model;
    // TableVIEW Model
    QStandardItemModel* table_model;

    std::shared_ptr<NekMeshObject> nekMeshObjectPtr;

    QButtonGroup *btnGroup;

    void importFile();
    void importCertainFile(char type);
    void browseFile();
    void process();

public slots:
    void onAddModuleBtnClicked();

private slots:
    void onDeleteModuleBtnClicked();
    void onRunBtnClicked();
    void onSaveBtnClicked();
    void onAddProcessModuleBtnClicked();
    void onRunAndSaveBtnClicked();

};


#endif // MAINWINDOW_H
