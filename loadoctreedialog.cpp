#include "loadoctreedialog.h"
#include "ui_loadoctreedialog.h"
#include "refinementdialog.h"

LoadoctreeDialog::LoadoctreeDialog(MainWindow *mainWindow,map<string, string>* values, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoadoctreeDialog)
    , values(values)
    , mainWindow(mainWindow)
{
    ui->setupUi(this);
    connect(ui->btnAdd, &QPushButton::clicked, this, &LoadoctreeDialog::onAddBtnClicked);
    connect(ui->btnRemove, &QPushButton::clicked, this, &LoadoctreeDialog::onRemoveBtnClicked);
    connect(ui->btnSave, &QPushButton::clicked, this, &LoadoctreeDialog::onSaveBtnClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->listRefinement, &QListWidget::itemClicked, this, &LoadoctreeDialog::onItemClicked);

    // Connect dialog's signal to MainWindow's slot directly within Dialog
    if (mainWindow) {
        connect(this, &LoadoctreeDialog::itemSelected, mainWindow, &MainWindow::drawRefinement);
    }

    // Ensure that values is not nullptr
    if (values) {
        if (values->find("desc") != values->end()) {
            ui->textDesc->setText(QString::fromStdString((*values)["desc"]));
        }
        if (values->find("MinDelta") != values->end()) {
            ui->textMin->setText(QString::fromStdString((*values)["MinDelta"]));
        }
        if (values->find("MaxDelta") != values->end()) {
            ui->textMax->setText(QString::fromStdString((*values)["MaxDelta"]));
        }
        if (values->find("EPS") != values->end()) {
            ui->textEPS->setText(QString::fromStdString((*values)["EPS"]));
        }
        if (values->find("curve_refinement") != values->end()) {
            QString curve_refinement = QString::fromStdString((*values)["curve_refinement"]);
            ui->checkCurve->setChecked(curve_refinement == "true");
        }

        if (values->find("writeoctree") != values->end()) {
            QString writeoctree = QString::fromStdString((*values)["writeoctree"]);
            ui->checkWrite->setChecked(writeoctree == "true");
        }
    }
}

LoadoctreeDialog::~LoadoctreeDialog()
{
    delete ui;
}

void LoadoctreeDialog::onItemClicked(QListWidgetItem *item) {
    // Emit the text of the clicked item
    emit itemSelected(item->text());
}
void LoadoctreeDialog::onAddBtnClicked(){
    RefinementDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString itemText = dialog.getRefinement();
        if (!itemText.isEmpty()) {
            ui->listRefinement->addItem(itemText);
        }
    }
}

void LoadoctreeDialog::onRemoveBtnClicked() {
    QListWidgetItem *selectedItem = ui->listRefinement->currentItem();
    if (selectedItem) {
        delete ui->listRefinement->takeItem(ui->listRefinement->row(selectedItem));
    }
}

std::map<std::string, std::string>& LoadoctreeDialog::getValues() {
    return *values; // Return a reference to the map
}

void LoadoctreeDialog::onSaveBtnClicked() {
    // Modify the map through the pointer
    (*values)["desc"] = ui->textDesc->text().toStdString();
    (*values)["MinDelta"] = ui->textMin->text().toStdString();
    (*values)["MaxDelta"] = ui->textMax->text().toStdString();
    (*values)["EPS"] = ui->textEPS->text().toStdString();
    (*values)["curve_refinement"] = ui->checkCurve->isChecked()?"true":"false";
    (*values)["writeoctree"] = ui->checkWrite->isChecked()?"true":"false";

    QStringList items;
    for (int i = 0; i < ui->listRefinement->count(); ++i) {
        QListWidgetItem *item = ui->listRefinement->item(i);
        items.append(item->text());
    }
    QString concatenatedText = items.join(":");  // You can change the delimiter here if needed
    qDebug() << concatenatedText;
    accept();
}
