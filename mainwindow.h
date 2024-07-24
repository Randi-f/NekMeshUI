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

private:
    Ui::MainWindow *ui;
    GLWidget *glWidget;
    // TreeView Model
    QStandardItemModel* model;
    // TableVIEW Model
    QStandardItemModel* table_model;

    std::shared_ptr<NekMeshObject> nekMeshObjectPtr;

    QButtonGroup btnGroup;
    SourceWindow *sourceWnd;
    OptionsWindow *optionsWnd;

    void importFile();
    void importCertainFile(char type);
    void browseFile();
public slots:
    void onAddModuleBtnClicked();
private slots:


    void onDeleteModuleBtnClicked();
    void onRunBtnClicked();
    void onSaveBtnClicked();

};


#endif // MAINWINDOW_H
