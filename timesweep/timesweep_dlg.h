#ifndef TIMESWEEP_DLG_H
#define TIMESWEEP_DLG_H

#include <QDialog>
#include "ui_timesweep_dlg.h"
#include "timesweepsrt_dlg.h"
#include "../global/externdata.h"
#include "../serialcom/serialcom.h"
#include "math.h"
#include <QPainter>
#include <QColor>
namespace Ui {
    class TimeSweep_Dlg;
}

class TimeSweep_Dlg : public QDialog,public Ui::TimeSweep_Dlg
{
    Q_OBJECT

public:
    explicit TimeSweep_Dlg(QWidget *parent = 0);
    ~TimeSweep_Dlg();
private:
    QPen CurPen;
    QColor color[3];
    float TimeSTest[3][3600];
	float TimeSTime[3600];
    int sign_draw;
    double CurrentTime;
    double StopTime;
    int Sign_Testing;
    int Sign_TestFinish;
	int TestTimes;
    struct TimesweepSetting CurSet;

    serialCOM* p_serialCOM5;
    timesweepsrt_dlg* p_timesweepsrt_dlg;

public slots:
    void do_transStrToDlg(struct TimesweepSetting);

private slots:
    void init(void);
    bool serialCOMinit(void);
    void createColorComboBox(QComboBox *comboBox);
    void bsetEnable(bool bset);
    void paintEvent(QPaintEvent *event);
    void on_Btn_IsZero_clicked();
    void on_Btn_Measure_clicked();
    void on_comboBox_CH3_currentIndexChanged(int index);
    void on_comboBox_CH2_currentIndexChanged(int index);
    void on_comboBox_CH1_currentIndexChanged(int index);
    void on_checkBox_CH3_stateChanged(int );
    void on_checkBox_CH2_stateChanged(int );
    void on_checkBox_CH1_stateChanged(int );
    void do_Btn_ParaSettings();

};

#endif // TIMESWEEP_DLG_H
