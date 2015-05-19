#ifndef SPECTRALSCANSETTINGS_DLG_H
#define SPECTRALSCANSETTINGS_DLG_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>


#include "ui_spectralscansettings_dlg.h"

enum ParaKind          ///for SpecMode
{
    EnergyScan=0,        ///Energy scanning
    AbsorbScan=1,       ///Absorbance scanning
    TransmitScan=2,    ///Transmittance scanning
};

struct SpecParaSetting
{
    int SpecMode;
    int StartSpecNM;
    int EndSpecNM;
    float MaxYdata;
    float MinYdata;
    float CCDTime;
    int AverageS;
};

namespace Ui {
    class SpectralScanSettings_Dlg;
}
class SpectralScanSettings_Dlg : public QDialog,public Ui::SpectralScanSettings_Dlg
{
    Q_OBJECT

public:
    explicit SpectralScanSettings_Dlg(QWidget *parent = 0);
    ~SpectralScanSettings_Dlg();
    void setMyValidator(int modetype);
    bool fileread(int modetype);
    bool filewrite(int modetype);

public:
   struct SpecParaSetting SettingPara;
private:
   bool testIput();

signals:
   void transStrToDlg(struct SpecParaSetting);

private slots:
    void on_Btn_Quit_clicked();
    void on_Btn_Save_clicked();
    void on_Box_SpeMode_currentIndexChanged(int index);
};

#endif // SPECTRALSCANSETTINGS_DLG_H


