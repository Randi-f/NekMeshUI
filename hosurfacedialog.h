#ifndef HOSURFACEDIALOG_H
#define HOSURFACEDIALOG_H

#include <QDialog>
using namespace std;
namespace Ui {
class HOSurfaceDialog;
}

class HOSurfaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HOSurfaceDialog(map<string, string>* values,  QWidget *parent = nullptr);
    ~HOSurfaceDialog();
    map<string, string>& getValues() {
        return *values;
    }

private:
    Ui::HOSurfaceDialog *ui;
    map<string, string> *values;
    void onSaveBtnClicked();

};

#endif // HOSURFACEDIALOG_H
