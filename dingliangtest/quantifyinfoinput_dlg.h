#ifndef QUANTIFYINFOINPUT_DLG_H
#define QUANTIFYINFOINPUT_DLG_H

#include <QDialog>
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
#include "ui_quantifyinfoinput_dlg.h"
#include "quantifymeasurement_dlg.h"
namespace Ui {
    class quantifyinfoinput_dlg;
}

class quantifyinfoinput_dlg : public QDialog,public Ui::quantifyinfoinput_dlg
{
    Q_OBJECT
public:
    explicit quantifyinfoinput_dlg(QWidget *parent = 0);
    ~quantifyinfoinput_dlg();
    QString QuantifyTestName;
    QString DUint_Name;       //样品单位
    float Num_FormulaA;         //系数A
    float Num_FormulaB;         //系数B
    float Num_FormulaC;         //系数C
    int Sign_Save;
public slots:
    void init();
private:
    QuantifyMeasurement_Dlg* p_quantifymeasurement;
private slots:
    void on_comboBox_WaveMode_currentIndexChanged(int index);
    void on_Btn_Del_clicked();
    void on_Btn_NewTestLine_clicked();
    void on_Btn_Quit_clicked();
    void on_Btn_Save_clicked();
};

#endif // QUANTIFYINFOINPUT_DLG_H
