#ifndef SYSTEMSETTINGS_DLG_H
#define SYSTEMSETTINGS_DLG_H

#include <QDialog>
#include <QDebug>
#include <QString>
#include "ui_systemsettings_dlg.h"
#include "../global/externdata.h"

namespace Ui {
    class SystemSettings_Dlg;
}

class SystemSettings_Dlg : public QDialog,public Ui::SystemSettings_Dlg
{
    Q_OBJECT

public:
    explicit SystemSettings_Dlg(QWidget *parent = 0);
    ~SystemSettings_Dlg();

private slots:
    void on_Btn_datanetset_clicked();
    void on_Btn_netset_clicked();
    void on_Btn_systeminitialise_clicked();
    void on_Btn_timeset_clicked();
//    void on_checkBox_stateChanged(int );
    void on_comboBox_test_currentIndexChanged(int index);
    void on_comboBox_user_currentIndexChanged(int index);
};

#endif // SYSTEMSETTINGS_DLG_H
