#include "custombutton.h"
#include "peraligndialog.h"
#include "loadoctreedialog.h"
#include "generator2ddialog.h"
#include "bldialog.h"
#include "hosurfacedialog.h"
#include <QDebug>

using namespace std;

CustomButton::CustomButton(MainWindow *mainWindow, const map<string, string>& config, QWidget* parent)
    : QToolButton(parent), config(config), mainWindow(mainWindow) {
    // Initialize button text and other properties
    this->setText(QString::fromStdString(config.at("type") +"\n"+ config.at("desc")));
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setCheckable(true);  // 启用选中状态
    connect(this, &QToolButton::clicked, this, &CustomButton::onClicked);
}

map<string, string> CustomButton::getConfig() const {
    return config;
}

void CustomButton::setConfig(const map<string, string>& newConfig) {
    config = newConfig;
    this->setText(QString::fromStdString(config.at("type") +"\n"+ config.at("desc")));
}

void CustomButton::onClicked() {
    openDialog();
}

void CustomButton::openDialog() {
    // Create a new PerAlignDialog instance each time the button is clicked
    if(config["moduleType"]=="peralign"){
        PerAlignDialog dialog(&config, this);
        if (dialog.exec() == QDialog::Accepted) {
            setConfig(dialog.getValues());
        }
    }
    else if(config["moduleType"]=="loadoctree"){
        LoadoctreeDialog dialog(mainWindow, &config, this);
        if (dialog.exec() == QDialog::Accepted) {
            setConfig(dialog.getValues());
        }
    }
    else if(config["moduleType"]=="2dgenerator"){
        Generator2DDialog dialog(&config, this);
        if (dialog.exec() == QDialog::Accepted) {
            setConfig(dialog.getValues());
        }
    }
    else if(config["moduleType"]=="bl"){
        BlDialog dialog(&config, this);
        if (dialog.exec() == QDialog::Accepted) {
            setConfig(dialog.getValues());
        }
    }
    else if(config["moduleType"]=="hosurface"){
        HOSurfaceDialog dialog(&config, this);
        if (dialog.exec() == QDialog::Accepted) {
            setConfig(dialog.getValues());
        }
    }



}

void CustomButton::updateBtnName(){
    this->setText(QString::fromStdString(config.at("type") +"\n"+ config.at("desc")));
}
