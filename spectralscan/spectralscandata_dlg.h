#ifndef SPECTRALSCANDATA_DLG_H
#define SPECTRALSCANDATA_DLG_H

#include <QDialog>
#include "ui_spectralscandata_dlg.h"

#include "../global/externdata.h"

#include <QMessageBox>
#include <QString>
#include <QDateTime>
#include <ctime>
#include <QTime>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
//#include <QModelIndex>
#include <QHeaderView>


#include <QRegExp>
#include "math.h"

#define SPECBASENAME  "./files/specscan.db"

namespace Ui {
    class SpectralScanData_Dlg;
}

class SpectralScanData_Dlg : public QDialog,public Ui::SpectralScanData_Dlg
{
    Q_OBJECT

public:
    explicit SpectralScanData_Dlg(QWidget *parent = 0);
    ~SpectralScanData_Dlg();

private:
    //QSql dispose
    QSqlDatabase db_specscan;
    QSqlTableModel *qTableModel;
    bool createConnection();
    void display_QTable(QString);
    void mydelay(int needtime);

signals:
   void transIntToDlg(int displayNum);


private slots:
    void on_Btn_Quit_clicked();
    void on_Btn_Dis_clicked();
    void on_Btn_Del_clicked();
    void on_Btn_Display_clicked();
};

#endif // SPECTRALSCANDATA_DLG_H
