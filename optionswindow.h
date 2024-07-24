#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class OptionsWindow;
}

class OptionsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OptionsWindow(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~OptionsWindow();

private:
    Ui::OptionsWindow *ui;
    MainWindow *mainWindow;
};

#endif // OPTIONSWINDOW_H
