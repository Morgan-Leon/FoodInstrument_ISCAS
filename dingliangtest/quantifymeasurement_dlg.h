#ifndef QUANTIFYMEASUREMENT_DLG_H
#define QUANTIFYMEASUREMENT_DLG_H

#include <QDialog>
#include "ui_quantifymeasurement_dlg.h"
#include <QMessageBox>
#include"QLabel"
//#include <QCombobox>
#include "../global/externdata.h"
#include "../serialcom/serialcom.h"
#include <QtGui/QPainter>

namespace Ui {
    class QuantifyMeasurement_Dlg;
}

class QuantifyMeasurement_Dlg : public QDialog,public Ui::QuantifyMeasurement_Dlg
{
    Q_OBJECT

public:
    explicit QuantifyMeasurement_Dlg(QWidget *parent = 0);
    ~QuantifyMeasurement_Dlg();
    int Sign_LineSave;
    float Num_FormulaA;      //系数A
    float Num_FormulaB;      //系数B
    float Num_FormulaC;      //系数C
    float g_fSampleR;
private:

    float AbsSampleTest[6];     //样品吸光度
    float Num_content[6];       //样品浓度结果
    float AbsMajor[3];          //样品主波长吸光度
    float AbsMinor[3];          //样品参考波长吸光度
    int Sign_GoalInput;       //浓度输入完成标志
    int Sum_Goalcon;        //目标浓度总数
    int Sum_FinishTest;     //完成测量数
    int sign_draw;         //绘制曲线标志
    int NiHeMode;           //拟合模式
    float StartX;
    float StartY;
    float EndX;
    float EndY;

    serialCOM* p_serialCOM4;
private slots:
    void on_Btn_Save_clicked();
    void on_Btn_MakeLine_clicked();
    void on_Btn_Measure_clicked();
    void on_comboBox_Sum_Goalcon_currentIndexChanged(QString );
    void on_Btn_GoalconInput_clicked();
    void on_Btn_IsZero_clicked();
    void bsetEnable(bool bset);
    void choose_Sum_Goalcon(int Sign);
    void init();
    void paintEvent(QPaintEvent *event);
    void Curve_Regess1();
    void Curve_Regess2();
    bool serialCOMinit();

};

#endif // QUANTIFYMEASUREMENT_DLG_H
