#ifndef DINGTEST_DLG_H
#define DINGTEST_DLG_H

#include <QDialog>
#include "ui_dingtest_dlg.h"

#include "quantifyinfoinput_dlg.h"
#include <QMessageBox>
#include <QString>
#include <QRegExp>
#include <QRegExpValidator>
#include "../global/externdata.h"
#include "../serialcom/serialcom.h"
#include "../serialcom/serialprint.h"

#define DLDATANAME  "./files/dingliang.db"
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHeaderView>
#include <QDateTime>
#include <ctime>
#include "math.h"

namespace Ui {
    class DingTest_dlg;
}

class DingTest_dlg : public QDialog,public Ui::DingTest_dlg
{
    Q_OBJECT

public:
    explicit DingTest_dlg(QWidget *parent = 0);
    ~DingTest_dlg();

private:
    QSqlDatabase db_dingliang;
    QSqlTableModel *qTableModel;
private:
    serialCOM* p_serialCOM3;
    serialPrint* p_serialPrint3;
    quantifyinfoinput_dlg* p_quantifyinfoinput;
    int DTestItems_Sum;               //���Է����б�����
    QString DTestItems_List[100];     //���Է����б�
    float Num_FormulaA;         //ϵ��A
    float Num_FormulaB;         //ϵ��B
    float Num_FormulaC;         //ϵ��C
    float AbsSampleTest[3];     //��ͨ����Ʒ�����
    float Num_content[3];       //��ͨ����ƷŨ�Ƚ��
    float AbsMajor[3];          //��ͨ����Ʒ�����������
    float AbsMinor[3];          //��ͨ����Ʒ�ο����������
    QString DSample_Name;       //��Ʒ��λ
    QString TestItems_Name;     //�����Ŀ
    int Sum_FinishTest;         //�Ѽ�����ܺ�

private slots:
    void on_Btn_Del_clicked();
    //void on_Btn_Save_clicked();
    void on_Btn_Display_clicked();
    void on_Btn_Printer_clicked();
    void on_Btn_EditTestItems_clicked();
    void on_Btn_NewTestItems_clicked();
    void on_comboBox_TestItems_currentIndexChanged(QString );
    void on_checkBox_CH3_stateChanged(int );
    void on_checkBox_CH2_stateChanged(int );
    void on_checkBox_CH1_stateChanged(int );
    void on_Btn_Measure_clicked();
    void on_Btn_IsZero_clicked();
    void bsetEnable(bool bset);
    void GetMeasureInfo();    //�����ݿ��еõ�������Ϣ
    void GetList_DTestItems();
    void init();
    bool serialCOMinit();
    void display_QTable(QString);
    void Printchar(char Pdata1);//��ӡ����ASCII��
    void PrintString(QString Pdata2);//��ӡ�����ַ���
    void PrintInit(void);//��ӡ����ʼ��
};

#endif // DINGTEST_DLG_H
