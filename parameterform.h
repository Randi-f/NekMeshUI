#ifndef PARAMETERFORM_H
#define PARAMETERFORM_H

#include <QWidget>

namespace Ui {
class ParameterForm;
}

class ParameterForm : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterForm(QWidget *parent = nullptr);
    ~ParameterForm();

private:
    Ui::ParameterForm *ui;

    void getSettings(); // bools
    void gerParameters(); //strings

};

#endif // PARAMETERFORM_H
