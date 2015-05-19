#ifndef TIMESWEEPSRT_DLG_H
#define TIMESWEEPSRT_DLG_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
#include "ui_timesweepsrt_dlg.h"

enum SpecKind          ///for SpecMode
{
    EnergyTime=0,        ///Energy scanning
    AbsorbTime=1,       ///Absorbance scanning
    TransmitTime=2,    ///Transmittance scanning
};

struct TimesweepSetting
{
    int SpecMode;
    float TimeScanWL;		//固定波长
//    float TimeStep;		//时间扫描 的时间间隔
    float TimeScanAllTime;	//时间扫描 的运行次数
    float MaxYdata;
    float MinYdata;
    float CCDTime;
    int AverageS;
};

namespace Ui {
    class timesweepsrt_dlg;
}

class timesweepsrt_dlg : public QDialog,public Ui::timesweepsrt_dlg
{
    Q_OBJECT
public:
    explicit timesweepsrt_dlg(QWidget *parent = 0);
    ~timesweepsrt_dlg();
    void setMyValidator(int modetype);
    bool fileread(int modetype);
    bool filewrite(int modetype);
public:
   struct TimesweepSetting SettingPara;
private:
    bool testIput();
signals:
   void transStrToDlg(struct TimesweepSetting);
private slots:
    void on_Box_SpeMode_currentIndexChanged(int index);
    void on_Btn_Quit_clicked();
    void on_Btn_Save_clicked();
};

#endif // TIMESWEEPSRT_DLG_H
