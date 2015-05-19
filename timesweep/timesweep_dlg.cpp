#include "timesweep_dlg.h"

TimeSweep_Dlg::TimeSweep_Dlg(QWidget *parent) :
        QDialog(parent,Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint)
{  
    setupUi(this);
    this->Btn_Quit->setFocus();
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    init();
    connect(this->Btn_Quit,SIGNAL(clicked()),this,SLOT(close()));
    connect(this->Btn_ParaSettings,SIGNAL(clicked()),this,SLOT(do_Btn_ParaSettings()));
    this->Btn_Save->setVisible(false);
    this->Btn_Loder->setVisible(false);
    if(Data::testType==serialTest)
        if(!serialCOMinit())
            qDebug()<<"serialCOM init failed,alarm information!!!";
}

TimeSweep_Dlg::~TimeSweep_Dlg()
{
    //    if(Data::testType==serialTest)
    //    {
    //        delete p_serialCOM5;
    ////        delete p_sample_infoinput;
    //    }
}
void TimeSweep_Dlg::do_Btn_ParaSettings()
{
    p_timesweepsrt_dlg->show();
    p_timesweepsrt_dlg->exec();
    if( Data::sign_TestCHNum == 0)
    {
        bsetEnable(false);
    }
    else
    {
        bsetEnable(true);
    }
}
void TimeSweep_Dlg::init()
{
    ///reference to CurSet
    CurSet.SpecMode=0;
    CurSet.TimeScanWL=600;
//    CurSet.TimeStep=1;
    CurSet.TimeScanAllTime=180;
    CurSet.MaxYdata=5000;
    CurSet.MinYdata=0;
    CurSet.CCDTime=0.005;
    CurSet.AverageS=16;
    Data::SetTime=CurSet.CCDTime*1000;
    Data::sign_TestCHNum = 0x07;
    sign_draw=0;
    Sign_Testing = 0;
    Sign_TestFinish = 0;
    createColorComboBox(this->comboBox_CH1);
    createColorComboBox(this->comboBox_CH2);
    createColorComboBox(this->comboBox_CH3);
    this->comboBox_CH1->setCurrentIndex(0);
    this->comboBox_CH2->setCurrentIndex(1);
    this->comboBox_CH3->setCurrentIndex(2);
//    this->comboBox_CH1->setEnabled(false);
//    this->comboBox_CH2->setEnabled(false);
//    this->comboBox_CH3->setEnabled(false);
    bsetEnable(true);
    this->Btn_Save->setEnabled(false);
    p_timesweepsrt_dlg = new timesweepsrt_dlg;
    connect(this->p_timesweepsrt_dlg,SIGNAL(transStrToDlg(struct TimesweepSetting)),this,SLOT(do_transStrToDlg(struct TimesweepSetting)));
}
bool TimeSweep_Dlg::serialCOMinit()
{
    bool ok=true;
    bool b_tmp;
    p_serialCOM5 =new serialCOM;
    ////connect
    b_tmp=p_serialCOM5->MultiCMD(readwave,CHEN123);
    if(!b_tmp)
        QMessageBox::warning(this,tr("警告"),tr("read wave failed"),tr("确定"));
    //        qDebug()<<"read wave failed,alarm information!!!";
    ok &=b_tmp;
    ///light on
    b_tmp=p_serialCOM5->MultiCMD(lightSW,lighton);
    if(!b_tmp)
        QMessageBox::warning(this,tr("警告"),tr("light on failed"),tr("确定"));
    //        qDebug()<<"light on failed,alarm information!!!";
    ok &=b_tmp;
    return ok;
}
void TimeSweep_Dlg::bsetEnable(bool bset)
{
    if(CurSet.SpecMode == EnergyTime)
    {
        this->Btn_Measure->setEnabled(bset);
        this->Btn_IsZero->setEnabled(false);
    }
    else
    {
        this->Btn_Measure->setEnabled(false);
        this->Btn_IsZero->setEnabled(bset);
    }
}
void TimeSweep_Dlg::on_Btn_IsZero_clicked()
{
    int j;
    bool ok=true;
    bool b_tmp;
    int tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
    bsetEnable(false);
    sign_draw=0;
    CurrentTime = 0;
    Sign_TestFinish = 0;
    if(Sign_Testing == 1)
    {
        Sign_Testing = 0;
        Btn_Measure -> setText( tr( "测量" ) );
        Btn_IsZero -> setText( tr( "校零" ) );
        bsetEnable(true);
    }
    else
    {
        this->Btn_ParaSettings->setEnabled(false);
        this->checkBox_CH1->setEnabled(false);
        this->checkBox_CH2->setEnabled(false);
        this->checkBox_CH3->setEnabled(false);
        this->Btn_Save->setEnabled(false);
        if(Data::testType==serialTest)
        {
            b_tmp=p_serialCOM5->MultiCMD(settime,CHEN123);
            if(!b_tmp)
                QMessageBox::warning(this,tr("警告"),tr("CCD set failed"),tr("确定"));
        }
        ///启动测量
        if(Data::testType==serialTest)       ////serial test mode
        {
            b_tmp = p_serialCOM5->MultiCMD(readinit, CHEN123);
            if(!b_tmp)
                QMessageBox::warning(this,tr("警告"),tr("read init failed"),tr("确定"));
        }
        for(j=0;j<3;j++)
        {
            if(tmpFlag&0x01)
            {
                if(Data::testType==serialTest)
                {
                    b_tmp = p_serialCOM5->MultiCMD(readsigwave, j+1);///////////////////////////////////
                    if(!b_tmp)
                        QMessageBox::warning(this,tr("警告"),tr("read wave failed"),tr("确定"));
                    Data::BaseMajor[j] = Data::GetADvalue;
                }
                else
                {
                    Data::BaseMajor[j] = 2000;
                }
            }
            tmpFlag=tmpFlag>>1;
        }
        if(Data::testType==serialTest)
        {
            b_tmp = p_serialCOM5->MultiCMD(lightSW,lightoff);
            if(!b_tmp)
                QMessageBox::warning(this,tr("警告"),tr("light off failed"),tr("确定"));

            ///启动测量
            b_tmp = p_serialCOM5->MultiCMD(readinit, CHEN123);
            if(!b_tmp)
                QMessageBox::warning(this,tr("警告"),tr("read init failed"),tr("确定"));
        }
        tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
        for(j=0;j<3;j++)
        {
            if(tmpFlag&0x01)
            {
                if(Data::testType==serialTest)
                {
                    b_tmp = p_serialCOM5->MultiCMD(readsigwave, j+1);///////////////////////////////////
                    if(!b_tmp)
                        QMessageBox::warning(this,tr("警告"),tr("read wave failed"),tr("确定"));
                    Data::DarkMajor[j] = Data::GetADvalue;
                }
                else
                {
                    Data::DarkMajor[j] = 0;
                }
            }
            tmpFlag=tmpFlag>>1;
        }
        if(Data::testType==serialTest)
        {
            b_tmp = p_serialCOM5->MultiCMD(lightSW,lighton);
            if(!b_tmp)
                QMessageBox::warning(this,tr("警告"),tr("light on failed"),tr("确定"));
        }
        if(CurSet.SpecMode == EnergyTime)
        {
            this->Btn_IsZero->setEnabled(false);
        }
        else
        {
            this->Btn_IsZero->setEnabled(ok);
        }
        this->Btn_Measure->setEnabled(ok);
    }
    this->Btn_ParaSettings->setEnabled(ok);
    this->checkBox_CH1->setEnabled(ok);
    this->checkBox_CH2->setEnabled(ok);
    this->checkBox_CH3->setEnabled(ok);
    this->Btn_Save->setEnabled(false);
}
void TimeSweep_Dlg::on_Btn_Measure_clicked()
{
    QTime ntt,nntt;
    int j;
    bool b_tmp;
    double tmp;
    int tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
    int times = CurSet.TimeScanAllTime;
    this->Btn_ParaSettings->setEnabled(false);
    this->checkBox_CH1->setEnabled(false);
    this->checkBox_CH2->setEnabled(false);
    this->checkBox_CH3->setEnabled(false);
    this->Btn_Save->setEnabled(false);
    if(Sign_TestFinish == 0)
    {
        TestTimes = 0;
        StopTime = 0;
        Sign_TestFinish = 2;
    }
    if(Sign_Testing == 1)
    {
        Sign_Testing = 0;
        Btn_Measure -> setText( tr( "继续" ) );
        StopTime = CurrentTime + StopTime;
    }
    else if(Sign_Testing == 0)
    {
        Sign_Testing = 1;
        Btn_Measure -> setText( tr( "暂停" ) );
        this->Btn_IsZero->setEnabled(true);
        Btn_IsZero -> setText( tr( "停止" ) );
        sign_draw=1;
        ntt.start();
        while(ntt.elapsed()<(times*1000-StopTime))//
        {
//            QCoreApplication::processEvents();
            CurrentTime = ntt.elapsed();
            TimeSTime[TestTimes] = ntt.elapsed()+StopTime;
//            qDebug()<< CurrentTime;
            tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
            if(Sign_Testing == 1)
            {
                ///启动测量
                if(Data::testType==serialTest)       ////serial test mode
                {
                    b_tmp = p_serialCOM5->MultiCMD(readinit, CHEN123);
                    if(!b_tmp)
                        QMessageBox::warning(this,tr("警告"),tr("read init failed"),tr("确定"));
                }
                else
                {
                    nntt.start();
                    while(nntt.elapsed()<2000)
                        QCoreApplication::processEvents();
                }
                for(j=0;j<3;j++)
                {
                    if(tmpFlag&0x01)
                    {
                        if(Data::testType==serialTest)
                        {
                            b_tmp = p_serialCOM5->MultiCMD(readsigwave, j+1);///////////////////////////////////
                            if(!b_tmp)
                                QMessageBox::warning(this,tr("警告"),tr("read wave failed"),tr("确定"));
                            Data::SampleMajor[j] = Data::GetADvalue;
                        }
                        else
                        {
                            tmp = TestTimes+j*45;

                            Data::SampleMajor[j] = 2000*(sin(tmp/180*3.141)+1);
                        }

                        if(CurSet.SpecMode == EnergyTime)
                        {
                            TimeSTest[j][TestTimes] = Data::SampleMajor[j];
                        }
                        else if(CurSet.SpecMode == TransmitTime)
                        {

                            TimeSTest[j][TestTimes] = (float)(Data::SampleMajor[j] - Data::DarkMajor[j])/(float)(Data::BaseMajor[j] - Data::DarkMajor[j])*100;
                        }
                        else if(CurSet.SpecMode == AbsorbTime)
                        {
                            TimeSTest[j][TestTimes] = -log10((float)(Data::SampleMajor[j] - Data::DarkMajor[j])/(float)(Data::BaseMajor[j] - Data::DarkMajor[j]));
                            if(TimeSTest[j][TestTimes] > 10)
                            {
                                TimeSTest[j][TestTimes] = 10;
                            }
                            else if(TimeSTest[j][TestTimes] < -10)
                            {
                                TimeSTest[j][TestTimes] = -10;
                            }
                        }
                    }
                    tmpFlag=tmpFlag>>1;
                }
                update();
                TestTimes++;
            }
            else
            {
                break;
            }
        }
        if(Sign_Testing == 1)
        {
            Sign_Testing = 0;
            CurrentTime = 0;
            Sign_TestFinish = 0;
            Btn_Measure -> setText( tr( "测量" ) );
            Btn_IsZero -> setText( tr( "校零" ) );
            this->checkBox_CH1->setEnabled(true);
            this->checkBox_CH2->setEnabled(true);
            this->checkBox_CH3->setEnabled(true);
            this->Btn_Save->setEnabled(true);
            this->Btn_ParaSettings->setEnabled(true);
        }
    }
}
void TimeSweep_Dlg::do_transStrToDlg(struct TimesweepSetting SpecT)
{
    CurSet=SpecT;
    Data::SetTime=CurSet.CCDTime*1000;
    Data::GetWaveLength = CurSet.TimeScanWL;
    sign_draw =0;
    update();
}

void TimeSweep_Dlg::createColorComboBox(QComboBox *comboBox)
{
    int pixvalue=10;
    QPixmap pix(pixvalue, pixvalue);

    QPainter pt(&pix);
    pt.fillRect(0, 0, pixvalue, pixvalue, Qt::black);
    comboBox->addItem(QIcon(pix), tr("black"), Qt::black);
    pt.fillRect(0, 0, pixvalue, pixvalue, Qt::red);
    comboBox->addItem(QIcon(pix), tr("red"), Qt::red);
    pt.fillRect(0, 0, pixvalue, pixvalue, Qt::green);
    comboBox->addItem(QIcon(pix), tr("green"), Qt::green);
    pt.fillRect(0, 0, pixvalue, pixvalue, Qt::blue);
    comboBox->addItem(QIcon(pix), tr("blue"), Qt::blue);
    pt.fillRect(0, 0, pixvalue, pixvalue, Qt::yellow);
    comboBox->addItem(QIcon(pix), tr("yellow"), Qt::yellow);
    pt.fillRect(0, 0, pixvalue, pixvalue, Qt::cyan);
    comboBox->addItem(QIcon(pix), tr("cyan"), Qt::cyan);
    pt.fillRect(0, 0, pixvalue, pixvalue, Qt::magenta);
    comboBox->addItem(QIcon(pix), tr("magenta"), Qt::magenta);
}

void TimeSweep_Dlg::on_checkBox_CH1_stateChanged(int )
{
    if(this->checkBox_CH1->isChecked())
        Data::sign_TestCHNum |=0x01;
    else
        Data::sign_TestCHNum &=0x06;
    this->comboBox_CH1->setEnabled(this->checkBox_CH1->isChecked());
    if( Data::sign_TestCHNum == 0)
    {
        bsetEnable(false);
    }
    else
    {
        bsetEnable(true);
    }
}

void TimeSweep_Dlg::on_checkBox_CH2_stateChanged(int )
{
    if(this->checkBox_CH2->isChecked())
        Data::sign_TestCHNum |=0x02;
    else
        Data::sign_TestCHNum &=0x05;
    this->comboBox_CH2->setEnabled(this->checkBox_CH2->isChecked());
    if( Data::sign_TestCHNum == 0)
    {
        bsetEnable(false);
    }
    else
    {
        bsetEnable(true);
    }
}

void TimeSweep_Dlg::on_checkBox_CH3_stateChanged(int )
{
    if(this->checkBox_CH3->isChecked())
        Data::sign_TestCHNum |=0x04;
    else
        Data::sign_TestCHNum &=0x03;
    this->comboBox_CH3->setEnabled(this->checkBox_CH3->isChecked());
    if( Data::sign_TestCHNum == 0)
    {
        bsetEnable(false);
    }
    else
    {
        bsetEnable(true);
    }
}

void TimeSweep_Dlg::on_comboBox_CH1_currentIndexChanged(int index)
{
    color[0] = comboBox_CH1->itemData(comboBox_CH1->currentIndex(), Qt::UserRole).value<QColor>();
    if(sign_draw == 1)
        update();
}

void TimeSweep_Dlg::on_comboBox_CH2_currentIndexChanged(int index)
{
    color[1] = comboBox_CH2->itemData(comboBox_CH2->currentIndex(), Qt::UserRole).value<QColor>();
    if(sign_draw == 1)
        update();
}

void TimeSweep_Dlg::on_comboBox_CH3_currentIndexChanged(int index)
{
    color[2] = comboBox_CH3->itemData(comboBox_CH3->currentIndex(), Qt::UserRole).value<QColor>();
    if(sign_draw == 1)
        update();
}
void TimeSweep_Dlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawLine(80+10,350+50,600+10,350+50);   //x1,y1,x2,y2坐标轴
    painter.drawLine(595+10,355+50,600+10,350+50);
    painter.drawLine(595+10,345+50,600+10,350+50);
    painter.drawLine(80+10,40+50,80+10,350+50);
    painter.drawLine(80+10,40+50,85+10,45+50);
    painter.drawLine(80+10,40+50,75+10,45+50);
    for(int i =1;i <5;i++)
    {
        painter.drawLine(80+10+i*125,350+50,80+10+i*125,352+50);
        painter.drawLine(78+10,50+(i-1)*75+50,80+10,50+(i-1)*75+50);
    }
    for(int m =0;m <5;m++)
    {
        painter.drawText(70+m*125,364+50,QString::number(CurSet.TimeScanAllTime/4*m,'f',1));
        if(CurSet.SpecMode == AbsorbTime)
        {
            painter.drawText(40,354-m*75+50,QString::number(CurSet.MinYdata+(CurSet.MaxYdata - CurSet.MinYdata)/4*m,'f',3));
        }
        else
        {
            painter.drawText(40,354-m*75+50,QString::number(CurSet.MinYdata+(CurSet.MaxYdata - CurSet.MinYdata)/4*m,'f',1));
        }
    }
    if(sign_draw == 1)
    {
        float tmp = 500/CurSet.TimeScanAllTime;
        int tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
        float Y1tmp[3],Y2tmp[3];
    //    int tmpSum;
///        if(Sign_TestFinish == 1)
 //       {
 //           tmpSum = CurSet.TimeScanAllTime/CurSet.TimeStep;
 //       }
 //       else
 //       {
  //          tmpSum = TestTimes;
 //       }
        for(int j=0;j<3;j++)
        {
            if(tmpFlag&0x01)
            {
                CurPen.setWidth(CurPenWidth);
                CurPen.setColor(color[j]);
                painter.setPen(CurPen);
                //
                if(CurSet.SpecMode == AbsorbTime)
                {
                    painter.drawText(140+j*160,60,QString::number(TimeSTest[j][TestTimes-1],'f',3));
                }
                else
                {
                    painter.drawText(140+j*160,60,QString::number(TimeSTest[j][TestTimes-1],'f',1));
                }
                ////draw measure lines and draw text from ShowSpecNm value
                for(int k = 1;k < TestTimes;k++)
                {

                    Y1tmp[j] = TimeSTest[j][k-1];
                    if(Y1tmp[j]>CurSet.MaxYdata)
                    {
                        Y1tmp[j] = CurSet.MaxYdata;
                    }
                    else if(Y1tmp[j]<CurSet.MinYdata)
                    {
                        Y1tmp[j] = CurSet.MinYdata;
                    }
                    Y2tmp[j] = TimeSTest[j][k];
                    if(Y2tmp[j]>CurSet.MaxYdata)
                    {
                        Y2tmp[j] = CurSet.MaxYdata;
                    }
                    else if(Y2tmp[j]<CurSet.MinYdata)
                    {
                        Y2tmp[j] = CurSet.MinYdata;
                    }

                    painter.drawLine(TimeSTime[k-1]/1000*tmp+80+10,350+50 - ( Y1tmp[j]-CurSet.MinYdata)/(CurSet.MaxYdata - CurSet.MinYdata)*300,TimeSTime[k]/1000*tmp+80+10,350+50 - (Y2tmp[j] - CurSet.MinYdata)/(CurSet.MaxYdata - CurSet.MinYdata)*300);
                }
            }
            tmpFlag=tmpFlag>>1;
        }
    }
}
