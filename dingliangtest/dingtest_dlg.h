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
    int DTestItems_Sum;               //测试方法列表总数
    QString DTestItems_List[100];     //测试方法列表
    float Num_FormulaA;         //系数A
    float Num_FormulaB;         //系数B
    float Num_FormulaC;         //系数C
    float AbsSampleTest[3];     //各通道样品吸光度
    float Num_content[3];       //各通道样品浓度结果
    float AbsMajor[3];          //各通道样品主波长吸光度
    float AbsMinor[3];          //各通道样品参考波长吸光度
    QString DSample_Name;       //样品单位
    QString TestItems_Name;     //检测项目
    int Sum_FinishTest;         //已检测结果总和

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
    void GetMeasureInfo();    //从数据库中得到测量信息
    void GetList_DTestItems();
    void init();
    bool serialCOMinit();
    void display_QTable(QString);
    void Printchar(char Pdata1);//打印命令ASCII码
    void PrintString(QString Pdata2);//打印内容字符串
    void PrintInit(void);//打印机初始化
};

#endif // DINGTEST_DLG_H
