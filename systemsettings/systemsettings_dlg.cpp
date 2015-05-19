#include "systemsettings_dlg.h"

SystemSettings_Dlg::SystemSettings_Dlg(QWidget *parent) :
        QDialog(parent,Qt::Dialog|Qt::FramelessWindowHint)
{    
    setupUi(this);
    this->Btn_Quit->setFocus();
    connect(this->Btn_Quit,SIGNAL(clicked()),this,SLOT(hide()));
    this->dateEdit->setDate(QDate::currentDate());
    this->timeEdit->setTime(QTime::currentTime());
}


SystemSettings_Dlg::~SystemSettings_Dlg()
{
    ;
}

void SystemSettings_Dlg::on_comboBox_user_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        Data::userType=commonUser;
        break;
    case 1:
        Data::userType=adminiUser;
        break;
    default:
        qDebug()<<"error!";
        break;
    }
}

void SystemSettings_Dlg::on_comboBox_test_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        Data::testType=serialTest;
        break;
    case 1:
        Data::testType=noserialTest;
        break;
    case 2:
        Data::testType=showType;
        break;
    default:
        qDebug()<<"error!";
        break;
    }
}


void SystemSettings_Dlg::on_Btn_timeset_clicked()
{

}

void SystemSettings_Dlg::on_Btn_systeminitialise_clicked()
{
    Data::flagInit = 1;
}

void SystemSettings_Dlg::on_Btn_netset_clicked()
{

 }

void SystemSettings_Dlg::on_Btn_datanetset_clicked()
{

}
