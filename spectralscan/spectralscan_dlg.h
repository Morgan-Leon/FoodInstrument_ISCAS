#ifndef SPECTRALSCAN_DLG_H
#define SPECTRALSCAN_DLG_H

#include <QDialog>
#include "ui_spectralscan_dlg.h"
#include "spectralscansettings_dlg.h"
#include "spectralscandata_dlg.h"
#include <QPainter>
#include <QColor>
#include "../global/externdata.h"
#include "../serialcom/serialcom.h"

#include "math.h"
#include <QtGui/QPainter>
#include <QMouseEvent>
#include <QPoint>

#include <QComboBox>
#include <QMessageBox>
#include <QString>
#include <QDateTime>
#include <ctime>
#include <QTime>


#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
//#include <QModelIndex>
#include <QHeaderView>
#include <QDebug>

struct Mdata
{
    float Nm;
    float Abs;
    int Xtmp;
    int order;
};
struct MaxorMinlist
{
    QList<struct Mdata*> maxlist;
    QList<struct Mdata*> minlist;
};

//#define SPECBASENAME  "./files/specscan.db"

namespace Ui {
    class SpectralScan_Dlg;
}

class SpectralScan_Dlg : public QDialog,public Ui::SpectralScan_Dlg
{
    Q_OBJECT

public:
    explicit SpectralScan_Dlg(QWidget *parent = 0);
    ~SpectralScan_Dlg();

private:
    QPen CurPen;
    QColor color[3];
    float StepSpecNM;
    int sign_draw;
    struct SpecParaSetting CurSet;
    float DisSpecData[3][500];         //各通道图表显示值 500
    float AbsData[3][RESOLUTION];             //各通道测量数据值
    float ShowSpecNm;
    float ShowTemp[3];
    int ShowSignmaxormin;
    float ShowNmaxormin[3];
    float ShowVmaxormin[3];

    MaxorMinlist* maxorminlist[3];
    serialCOM* p_serialCOM;
    SpectralScanSettings_Dlg* p_SpectralScanSettings_Dlg;
    SpectralScanData_Dlg* p_SpectralScanData_Dlg;

    int maxNumOder;
    int minNumOder;

    //QSql dispose
    QSqlDatabase db_specscan;
    bool bsuccess;
    bool createConnection();
    bool readSpeclistSQL();
    void saveSpeclistSQL();


private:
    void init(void);
    bool serialCOMinit(void);
    void createColorComboBox(QComboBox *comboBox);
    float matchdata(int num,float value);
    int matchOder(int num,float value,int flag);         ///flag 0(min),1(max)
    void saveAbsData(bool bool_ok);
    void saveDisSpecData(bool bool_ok);
    void bsetEnable(bool bset);

    void findMaxMin();
    void disposeOder();
    struct Mdata* findNMf(int num,int MaxorMin,float curNM);
    struct Mdata* findNMi(int num,int MaxorMin,int order);            //MaxorMin=0,1,2
    void draw1();
    void draw2();
    void draw3();
    bool initdraw;
    void mydelay(int needtime);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void do_transStrToDlg(struct SpecParaSetting);
     void do_transIntToDlg(int displayNum);
private slots:    
    void on_lineEdit_fTh_textChanged(QString );
    void on_Btn_readSaveScan_clicked();
    void on_Btn_Save_clicked();
    void on_tabWidget_currentChanged(int index);
    void mousePressEvent(QMouseEvent *);
    void on_lineEdit_ShowNm_textChanged(QString );
    void on_Btn_IsZero_clicked();
    void on_Btn_Measure_clicked();
    void on_Btn_showMoveRight_clicked();
    void on_Btn_showMoveLeft_clicked();
    void on_Btn_FindPeakMin_clicked();
    void on_Btn_FindPeakMax_clicked();
    void on_comboBox_CH3_currentIndexChanged(int index);
    void on_comboBox_CH2_currentIndexChanged(int index);
    void on_comboBox_CH1_currentIndexChanged(int index);
    void on_checkBox_CH3_stateChanged(int );
    void on_checkBox_CH2_stateChanged(int );
    void on_checkBox_CH1_stateChanged(int );
    void do_Btn_ParaSettings();
};

#endif // SPECTRALSCAN_DLG_H
