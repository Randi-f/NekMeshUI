#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QToolButton>
#include <map>
#include <string>
#include "mainwindow.h"
using namespace std;

class CustomButton : public QToolButton {
    Q_OBJECT

public:
    explicit CustomButton(MainWindow *mainWindow, const map<string, string>& config, QWidget* parent = nullptr);

    map<string, string> getConfig() const;
    void setConfig(const map<string, string>& newConfig);
    void updateBtnName();

private slots:
    void onClicked();

private:
    map<string, string> config;
    MainWindow *mainWindow;
    void openDialog();
};

#endif // CUSTOMBUTTON_H
