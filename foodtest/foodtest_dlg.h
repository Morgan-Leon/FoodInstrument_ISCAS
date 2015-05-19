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
    //结构体信息
    QString Num_TestItems[3];       //检测项目编号
    QString Num_SampleTypes[3];     //样品类型编号

    QString FSample_Num[3];         //样品编号
    QString FSample_Name[3];        //样品单位
    QString FSample_From[3];        //产地
    QString FSample_Unit[3];        //送检单位
    QString FTest_Name[3];          //操作人
    QString FTest_Unit[3];           //测试单位

    float Num_InterDilute[3];   //内嵌稀释倍数
    float Num_ExtraDilute[3];   //外部输入稀释倍数
    float Num_FormulaA[3];      //系数A
    float Num_FormulaB[3];      //系数B
    float Num_FormulaC[3];      //系数C
    float AbsSampleTest[3];     //样品吸光度
    float Num_content[3];       //样品浓度结果
    float AbsMajor[3];          //样品主波长吸光度
    float AbsMinor[3];          //样品参考波长吸光度
    int SampleInfo_CHNum;       //录入样品信息的通道号
    QString Uint_TestItems[3];     //浓度结果单位
    QString Standards_TestItems[3];     //参考标准
    QString FTestResult[3];       //检测判定结果newadd------------------------------------
    int FComparemode[3];          //判定模式newadd------------------------------------
    float FCompareMin[3];          //判定下边界newadd------------------------------------
    float FCompareMax[3];          //判定上边界newadd------------------------------------
    float FTestRangMin[3];          //检测范围下边界newadd------------------------------------
    float FTestRangMax[3];          //检测范围上边界newadd------------------------------------
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
    void GetFoodInfo(int CH_Num);  //从数据库中得到测量/信息
    void Printchar(char Pdata1);//打印命令ASCII码
    void PrintString(QString Pdata2);//打印内容字符串
    void PrintInit(void);//打印机初始化
};

#endif // FOODTEST_DLG_H
