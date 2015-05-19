#ifndef MAIN_DLG_H
#define MAIN_DLG_H

#include <QDialog>
#include "ui_main_dlg.h"
#include "../foodtest/foodtest_dlg.h"
#include "../spectralscan/spectralscan_dlg.h"
//#include "../datamanagement/datamanagement_dlg.h"
#include "../dingliangtest/dingtest_dlg.h"
#include "../timesweep/timesweep_dlg.h"
#include "../systemsettings/systemsettings_dlg.h"

namespace Ui {
    class Main_Dlg;
}

class Main_Dlg : public QDialog,public Ui::Main_Dlg
{
    Q_OBJECT

public:
    explicit Main_Dlg(QWidget *parent = 0);
    ~Main_Dlg();

public:
    void delet_Dlg();
    int flag_p;
private:
    FoodTest_Dlg* p_FoodTest_Dlg;
    SpectralScan_Dlg* p_SpectralScan_Dlg;
//    DataManagement_Dlg* p_DataManagement_Dlg;
    DingTest_dlg* p_DingliangTest_dlg;
    TimeSweep_Dlg* p_TimeSweep_Dlg;
    SystemSettings_Dlg* p_SystemSettings_Dlg;
private slots:
    void do_Btn_FoodTest();
    void do_SpectralScan();
//    void do_DataManagement();
    void do_QuantifyMeasurement();
    void do_TimeSweep();
    void do_SystemSettings();
     void bsetEnable(bool bset);
};

#endif // MAIN_DLG_H
