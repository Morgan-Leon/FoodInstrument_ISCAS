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
    float Num_FormulaA;      //ϵ��A
    float Num_FormulaB;      //ϵ��B
    float Num_FormulaC;      //ϵ��C
    float g_fSampleR;
private:

    float AbsSampleTest[6];     //��Ʒ�����
    float Num_content[6];       //��ƷŨ�Ƚ��
    float AbsMajor[3];          //��Ʒ�����������
    float AbsMinor[3];          //��Ʒ�ο����������
    int Sign_GoalInput;       //Ũ��������ɱ�־
    int Sum_Goalcon;        //Ŀ��Ũ������
    int Sum_FinishTest;     //��ɲ�����
    int sign_draw;         //�������߱�־
    int NiHeMode;           //���ģʽ
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
