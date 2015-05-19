#include "main_dlg.h"

Main_Dlg::Main_Dlg(QWidget *parent) :
        QDialog(parent,Qt::Dialog|Qt::FramelessWindowHint)
{
   setupUi(this);
   this->dateEdit->setDate(QDate::currentDate());
   this->dateEdit->setCalendarPopup(true);
   connect(this->Btn_FoodTest,SIGNAL(clicked()),this,SLOT(do_Btn_FoodTest()));
   connect(this->Btn_SpectralScan,SIGNAL(clicked()),this,SLOT(do_SpectralScan()));
//   connect(this->Btn_Database,SIGNAL(clicked()),this,SLOT(do_DataManagement()));
   connect(this->Btn_QuanMeasure,SIGNAL(clicked()),this,SLOT(do_QuantifyMeasurement()));
   connect(this->Btn_TimeSweep,SIGNAL(clicked()),this,SLOT(do_TimeSweep()));
   connect(this->Btn_SystemSet,SIGNAL(clicked()),this,SLOT(do_SystemSettings()));
   flag_p=0;
}

Main_Dlg::~Main_Dlg()
{
    ;
}
void Main_Dlg::bsetEnable(bool bset)
{
    this->dateEdit->setEnabled(bset);
    this->Btn_FoodTest->setEnabled(bset);
    this->Btn_SpectralScan->setEnabled(bset);
    this->Btn_QuanMeasure->setEnabled(bset);
    this->Btn_TimeSweep->setEnabled(bset);
    this->Btn_SystemSet->setEnabled(bset);
}
void Main_Dlg::do_Btn_FoodTest()
{
    bsetEnable(false);
    delet_Dlg();
    flag_p |= 0x01;
    qDebug()<<"0x01"<<flag_p;
    p_FoodTest_Dlg =new FoodTest_Dlg;    
//    p_FoodTest_Dlg->setWindowModality(Qt::ApplicationModal);
    p_FoodTest_Dlg->show();
    bsetEnable(true);
}
void Main_Dlg::do_SpectralScan()
{
    bsetEnable(false);
    QTime nntt;
    delet_Dlg();
    flag_p |= 0x02;
    qDebug()<<"0x02"<<flag_p;
    p_SpectralScan_Dlg =new SpectralScan_Dlg;
//    p_SpectralScan_Dlg->setWindowModality(Qt::ApplicationModal);
        nntt.start();
        while(nntt.elapsed()<2000)
            QCoreApplication::processEvents();
    p_SpectralScan_Dlg->show();
    bsetEnable(true);
}
/*
void Main_Dlg::do_DataManagement()
{
    delet_Dlg();
    flag_p |= 0x04;
    p_DataManagement_Dlg =new DataManagement_Dlg;
//    p_DataManagement_Dlg->setWindowModality(Qt::ApplicationModal);
    p_DataManagement_Dlg->show();
}
*/
void Main_Dlg::do_QuantifyMeasurement()
{
    bsetEnable(false);
    delet_Dlg();
    flag_p |= 0x08;
    p_DingliangTest_dlg =new DingTest_dlg;
//    p_QuantifyMeasurement_Dlg->setWindowModality(Qt::ApplicationModal);   
    p_DingliangTest_dlg->show();
    bsetEnable(true);

}
void Main_Dlg::do_TimeSweep()
{
    bsetEnable(false);
    delet_Dlg();
    flag_p |= 0x10;
    p_TimeSweep_Dlg =new TimeSweep_Dlg;
//    p_TimeSweep_Dlg->setWindowModality(Qt::ApplicationModal);    
    p_TimeSweep_Dlg->show();
  //  p_TimeSweep_Dlg->exec();
    bsetEnable(true);
}
void Main_Dlg::do_SystemSettings()
{
    bsetEnable(false);
    delet_Dlg();
    flag_p |= 0x20;
    qDebug()<<"0x20"<<flag_p;
    p_SystemSettings_Dlg =new SystemSettings_Dlg;
//    p_SystemSettings_Dlg->setWindowModality(Qt::ApplicationModal);    
    p_SystemSettings_Dlg->show();
 //   p_SystemSettings_Dlg->exec();
    bsetEnable(true);
}
void Main_Dlg::delet_Dlg()
{
    qDebug()<<"flag_p="<<flag_p;
    int i;
    for(i=0;i<6;i++)
    {
        if(flag_p&01)
            break;
        flag_p=flag_p>>1;
    }
    flag_p=0;
    qDebug()<<"i="<<i;
    switch(i)
    {
    case 0:
        delete p_FoodTest_Dlg;
        break;
    case 1:
        delete p_SpectralScan_Dlg;
        break;
    case 2:
        //delete p_DataManagement_Dlg;
        break;
    case 3:
        delete p_DingliangTest_dlg;
        break;
    case 4:
         delete p_TimeSweep_Dlg;
         break;
    case 5:
         delete p_SystemSettings_Dlg;
         break;
    default:
        qDebug()<<"do nothing!";
        break;
    }
}
