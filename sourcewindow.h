#ifndef SOURCEWINDOW_H
#define SOURCEWINDOW_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class SourceWindow;
}

class SourceWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SourceWindow(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~SourceWindow();

private:
    Ui::SourceWindow *ui;
    MainWindow *mainWindow;
};

#endif // SOURCEWINDOW_H
