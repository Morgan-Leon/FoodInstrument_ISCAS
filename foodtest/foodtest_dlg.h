#ifndef FOODTEST_DLG_H
#define FOODTEST_DLG_H

#include <QDialog>
#include "ui_foodtest_dlg.h"

#include "sample_infoinput.h"
#include "../global/externdata.h"
#include "../serialcom/serialcom.h"
#include "../serialcom/serialprint.h"

#include <QMessageBox>
#include <QString>
#include <QDateTime>
#include <ctime>


#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
//#include <QModelIndex>
#include <QHeaderView>


#include <QRegExp>
#include "math.h"

//#include <QCombobox>
//#include <QDebug>
//#include <QTableWidget>
//#include <QTableWidgetItem>
//#include <QAbstractScrollArea>
#include <QRegExpValidator>

#define DATABASENAME  "./files/food.db"

namespace Ui {
    class FoodTest_Dlg;
}

class FoodTest_Dlg : public QDialog,public Ui::FoodTest_Dlg
{
    Q_OBJECT

public:
    explicit FoodTest_Dlg(QWidget *parent = 0);
    ~FoodTest_Dlg();

 private:
    QSqlDatabase db_food;
    QSqlTableModel *qTableModel;

private:
    struct Info_Sample FSampleInfo;
    //�ṹ����Ϣ
    QString Num_TestItems[3];       //�����Ŀ���
    QString Num_SampleTypes[3];     //��Ʒ���ͱ��

    QString FSample_Num[3];         //��Ʒ���
    QString FSample_Name[3];        //��Ʒ��λ
    QString FSample_From[3];        //����
    QString FSample_Unit[3];        //�ͼ쵥λ
    QString FTest_Name[3];          //������
    QString FTest_Unit[3];           //���Ե�λ

    float Num_InterDilute[3];   //��Ƕϡ�ͱ���
    float Num_ExtraDilute[3];   //�ⲿ����ϡ�ͱ���
    float Num_FormulaA[3];      //ϵ��A
    float Num_FormulaB[3];      //ϵ��B
    float Num_FormulaC[3];      //ϵ��C
    float AbsSampleTest[3];     //��Ʒ�����
    float Num_content[3];       //��ƷŨ�Ƚ��
    float AbsMajor[3];          //��Ʒ�����������
    float AbsMinor[3];          //��Ʒ�ο����������
    int SampleInfo_CHNum;       //¼����Ʒ��Ϣ��ͨ����
    QString Uint_TestItems[3];     //Ũ�Ƚ����λ
    QString Standards_TestItems[3];     //�ο���׼
    QString FTestResult[3];       //����ж����newadd------------------------------------
    int FComparemode[3];          //�ж�ģʽnewadd------------------------------------
    float FCompareMin[3];          //�ж��±߽�newadd------------------------------------
    float FCompareMax[3];          //�ж��ϱ߽�newadd------------------------------------
    float FTestRangMin[3];          //��ⷶΧ�±߽�newadd------------------------------------
    float FTestRangMax[3];          //��ⷶΧ�ϱ߽�newadd------------------------------------
    serialCOM* p_serialCOM2;
    serialPrint* p_serialPrint2;
    sample_infoinput* p_sample_infoinput;
public slots:
    void do_transStrToDlg(struct Info_Sample);
private slots:
    void on_Btn_Print_clicked();
    void on_Btn_Display_clicked();
    void on_Btn_Del_clicked();    
    void on_Btn_SampleInfo_CH3_clicked();
    void on_Btn_SampleInfo_CH2_clicked();
    void on_Btn_SampleInfo_CH1_clicked();
    void on_checkBox_CH3_stateChanged(int );
    void on_checkBox_CH2_stateChanged(int );
    void on_checkBox_CH1_stateChanged(int );

    void on_DiluteNum_CH3_editingFinished();
    void on_DiluteNum_CH2_editingFinished();
    void on_DiluteNum_CH1_editingFinished();
    void on_comboBox_SampleTypes_CH3_currentIndexChanged(QString );
    void on_comboBox_SampleTypes_CH2_currentIndexChanged(QString );
    void on_comboBox_SampleTypes_CH1_currentIndexChanged(QString );
    void on_comboBox_TestItems_CH3_currentIndexChanged(QString );
    void on_comboBox_TestItems_CH2_currentIndexChanged(QString );
    void on_comboBox_TestItems_CH1_currentIndexChanged(QString );


    void on_Btn_Measure_clicked();
    void on_Btn_IsZero_clicked();
private:
    bool serialCOMinit();
    void bsetEnable(bool bset);
    void mydelay(int needtime);     ///ms

    void display_QTable(QString);

    void init();
    void GetList_FTestItems();
    void GetList_SampleTypes(int CH_Num);
    void GetFoodInfo(int CH_Num);  //�����ݿ��еõ�����/��Ϣ
    void Printchar(char Pdata1);//��ӡ����ASCII��
    void PrintString(QString Pdata2);//��ӡ�����ַ���
    void PrintInit(void);//��ӡ����ʼ��
};

#endif // FOODTEST_DLG_H
