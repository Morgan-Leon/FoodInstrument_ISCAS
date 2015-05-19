#include "spectralscan_dlg.h"

SpectralScan_Dlg::SpectralScan_Dlg(QWidget *parent) :
        QDialog(parent,Qt::Dialog|Qt::FramelessWindowHint)
{  
    setupUi(this);
    this->Btn_Quit->setFocus();
    init();
    connect(this->Btn_Quit,SIGNAL(clicked()),this,SLOT(close()));
    connect(this->Btn_ParaSettings,SIGNAL(clicked()),this,SLOT(do_Btn_ParaSettings()));
    if(Data::testType==serialTest)
    {
        if(!serialCOMinit())
            hide();
    }
}

SpectralScan_Dlg::~SpectralScan_Dlg()
{        
    db_specscan.close();
    //    delete p_SpectralScanSettings_Dlg;
    //    delete p_SpectralScanData_Dlg;
    //    delete p_serialCOM;
    //    delete maxorminlist;
}
void SpectralScan_Dlg::do_Btn_ParaSettings()
{
    p_SpectralScanSettings_Dlg->show();
    p_SpectralScanSettings_Dlg->exec();
    if(Data::sign_TestCHNum)
    {
        if(CurSet.SpecMode == 0)
        {
            this->Btn_IsZero->setEnabled(false);
            this->Btn_Measure->setEnabled(true);
        }
        else
        {
            this->Btn_IsZero->setEnabled(true);
            this->Btn_Measure->setEnabled(false);
        }
    }
    else
    {
        this->Btn_IsZero->setEnabled(false);
        this->Btn_Measure->setEnabled(false);
    }
    update();
}

void SpectralScan_Dlg::on_Btn_readSaveScan_clicked()
{
    p_SpectralScanData_Dlg->show();
}

void SpectralScan_Dlg::init()
{
    initdraw = true;
    ////init sqlite
    if (!createConnection())
        QMessageBox::warning(this,tr("警告"),tr("read specscan.db failed"),tr("确定"));
    ////init max and min data
    for(int i=0;i<3;i++)
        maxorminlist[i] =new MaxorMinlist;
    ////init draw
    this->tab_1->installEventFilter(this);
    this->tab_2->installEventFilter(this);
    this->tab_3->installEventFilter(this);
    ///reference to CurSet
    CurSet.SpecMode=0;
    CurSet.StartSpecNM=400;
    CurSet.EndSpecNM=800;
    CurSet.MaxYdata=5000;
    CurSet.MinYdata=0;
    CurSet.CCDTime=0.005;
    CurSet.AverageS=16;
    Data::SetTime=CurSet.CCDTime*1000;
    StepSpecNM = (float)(CurSet.EndSpecNM - CurSet.StartSpecNM)/500;
    this->lineEdit_ShowNm->setText(QString::number((CurSet.EndSpecNM+CurSet.StartSpecNM)/2));
    ///others init
    sign_draw=0;
    ShowSignmaxormin = 0;
    createColorComboBox(this->comboBox_CH1);
    createColorComboBox(this->comboBox_CH2);
    createColorComboBox(this->comboBox_CH3);
    this->comboBox_CH1->setCurrentIndex(0);
    this->comboBox_CH2->setCurrentIndex(1);
    this->comboBox_CH3->setCurrentIndex(2);
//    this->comboBox_CH1->setEnabled(false);
//    this->comboBox_CH2->setEnabled(false);
//    this->comboBox_CH3->setEnabled(false);
    Data::sign_TestCHNum = 0x07;
    if(Data::sign_TestCHNum)
    {
        if(CurSet.SpecMode == 0)
        {
            this->Btn_IsZero->setEnabled(false);
            this->Btn_Measure->setEnabled(true);
        }
        else
        {
            this->Btn_IsZero->setEnabled(true);
            this->Btn_Measure->setEnabled(false);
        }
    }
    else
    {
        this->Btn_IsZero->setEnabled(false);
        this->Btn_Measure->setEnabled(false);
    }
    this->Btn_FindPeakMax->setEnabled(false);
    this->Btn_FindPeakMin->setEnabled(false);
    this->lineEdit_ShowNm->setEnabled(false);
    this->Btn_showMoveLeft->setEnabled(false);
    this->Btn_showMoveRight->setEnabled(false);
    this->Btn_Save->setEnabled(false);
//    this->Btn_IsZero->setEnabled(false);
//    this->Btn_Measure->setEnabled(false);
    p_SpectralScanSettings_Dlg =new SpectralScanSettings_Dlg(this);
    //    p_SpectralScanSettings_Dlg->setWindowModality(Qt::ApplicationModal);
    connect(this->p_SpectralScanSettings_Dlg,SIGNAL(transStrToDlg(struct SpecParaSetting)),this,SLOT(do_transStrToDlg(struct SpecParaSetting)));
    p_SpectralScanData_Dlg =new SpectralScanData_Dlg(this);
    //    p_SpectralScanData_Dlg->setWindowModality(Qt::ApplicationModal);
    connect(this->p_SpectralScanData_Dlg,SIGNAL(transIntToDlg(int)),this,SLOT(do_transIntToDlg(int)));
}

bool SpectralScan_Dlg::serialCOMinit()
{
    bool ok=true;
    bool b_tmp;
    p_serialCOM =new serialCOM;
    ////connect
    b_tmp=p_serialCOM->MultiCMD(online,CHEN123);
    ok &=b_tmp;
    if(!b_tmp)
    {
        QMessageBox::warning(this,tr("警告"),tr("on line failed"),tr("确定"));
        return ok;
    }
    ////read version
    b_tmp=p_serialCOM->MultiCMD(readversion,CHEN123);
    ok &=b_tmp;
    if(!b_tmp)
    {
        QMessageBox::warning(this,tr("警告"),tr("read version failed"),tr("确定"));
        return ok;
    }
    ////read wave length list
    if(Data::flagInit == 0)
    {
        if(!readSpeclistSQL())
        {
            b_tmp=p_serialCOM->MultiCMD(readwave,CHEN123);
            ok &=b_tmp;
            if(!b_tmp)
            {
                QMessageBox::warning(this,tr("警告"),tr("read wave list failed"),tr("确定"));
                return ok;
            }
            saveSpeclistSQL();
        }
        Data::flagInit = 2;
    }
    else if(Data::flagInit == 1)
    {
        b_tmp=p_serialCOM->MultiCMD(readwave,CHEN123);
        ok &=b_tmp;
        if(!b_tmp)
        {
            QMessageBox::warning(this,tr("警告"),tr("read wave list failed"),tr("确定"));
            return ok;
        }
        saveSpeclistSQL();
        Data::flagInit = 2;
    }
    ///light on
    //    b_tmp=p_serialCOM->MultiCMD(lightSW,lighton);
    //    ok &=b_tmp;
    //    if(!b_tmp)
    //    {
    //        QMessageBox::warning(this,tr("警告"),tr("light on failed"),tr("确定"));
    //        return ok;
    //    }
    return ok;
}

void SpectralScan_Dlg::on_Btn_IsZero_clicked()
{
    //    this->Btn_IsZero->setEnabled(false);
    QTime t1;
    bool ok=true;
    bool b_tmp;
    ////init
    sign_draw = 0;
    update();
    QMessageBox *messageBox=new QMessageBox(QMessageBox::Information,tr("等待"),tr("校零中，请稍后"),QMessageBox::Ok,this);
    messageBox->show();
    bsetEnable(!ok); 
    this->Btn_IsZero->setEnabled(false);
    this->Btn_Measure->setEnabled(false);
    ////dispose
    ////set time
    if(Data::testType==serialTest)
    {
        b_tmp=p_serialCOM->MultiCMD(settime,Data::sign_TestCHNum);
        ok &=b_tmp;
        if(!b_tmp)
        {
            QMessageBox::warning(this,tr("警告"),tr("CCD set time failed"),tr("确定"));
            return;
        }
        ///启动测量
        b_tmp = p_serialCOM->MultiCMD(readinit, CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("警告"),tr("read init failed"),tr("确定"));
        ////read base data
        b_tmp=p_serialCOM->MultiCMD(readspec1,Data::sign_TestCHNum);
        ok &=b_tmp;
        if(!b_tmp)
        {
            QMessageBox::warning(this,tr("警告"),tr("read base data failed"),tr("确定"));
            return;
        }
        ////light off
        b_tmp=p_serialCOM->MultiCMD(lightSW,lightoff);
        ok &=b_tmp;
        if(!b_tmp)
        {
            QMessageBox::warning(this,tr("警告"),tr("light off failed"),tr("确定"));
            return;
        }
        ////wait
        t1.start();
        while(t1.elapsed()<WaitTime*1000)
            QCoreApplication::processEvents();
        ///启动测量
        b_tmp = p_serialCOM->MultiCMD(readinit, CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("警告"),tr("read init failed"),tr("确定"));
        ////read spectroscopic dark data
        b_tmp=p_serialCOM->MultiCMD(readspec0,Data::sign_TestCHNum);
        ok &=b_tmp;
        if(!b_tmp)
        {
            QMessageBox::warning(this,tr("警告"),tr("read dark data failed"),tr("确定"));
            return;
        }
        ///light on
        b_tmp=p_serialCOM->MultiCMD(lightSW,lighton);
        ok &=b_tmp;
        if(!b_tmp)
        {
            QMessageBox::warning(this,tr("警告"),tr("light on failed"),tr("确定"));
            return;
        }
    }
    else
    {
        t1.start();
        while(t1.elapsed()<2000)
            QCoreApplication::processEvents();
    }
    this->Btn_IsZero->setEnabled(true);
    this->Btn_Measure->setEnabled(ok);
    bsetEnable(ok);
    this->Btn_Measure->setFocus();
    messageBox->close();
    delete messageBox;
}

void SpectralScan_Dlg::on_Btn_Measure_clicked()
{
    QTime t1;
    bool b_tmp=true;
    sign_draw = 0;
    update();
    QMessageBox *messageBox=new QMessageBox(QMessageBox::Information,tr("等待"),tr("测量中，请稍后"),QMessageBox::Ok,this);
    messageBox->show();
    bsetEnable(false);
    this->Btn_IsZero->setEnabled(false);
    this->Btn_Measure->setEnabled(false);
    if(Data::testType==serialTest)
    {
        //init draw
        if(!initdraw)
        {
            sign_draw =0;
            update();
        }
        bool b_tmp=true;
        ///启动测量
        b_tmp = p_serialCOM->MultiCMD(readinit,CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("警告"),tr("read init failed"),tr("确定"));
        b_tmp=p_serialCOM->MultiCMD(readspec2,Data::sign_TestCHNum);
        if(!b_tmp)
        {
            QMessageBox::warning(this,tr("警告"),tr("read sample data failed"),tr("确定"));
            return;
        }
    }
    else
    {
        t1.start();
        while(t1.elapsed()<2000)
            QCoreApplication::processEvents();
    }
    bsetEnable(true); //  
    saveAbsData(b_tmp);
    saveDisSpecData(b_tmp);
    //find max and min list,then dispose
    mydelay(1000);
    findMaxMin();
    disposeOder();
    sign_draw =1;
    maxNumOder=0;
    minNumOder=0;
    if(sign_draw==1)
    {
        this->label_max->setText(tr("峰值个数 %1").arg(this->maxorminlist[this->tabWidget->currentIndex()]->maxlist.size()));
        this->label_min->setText(tr("谷值个数 %1").arg(this->maxorminlist[this->tabWidget->currentIndex()]->minlist.size()));
    }
    update();
    initdraw=false;
    messageBox->close();
    delete messageBox;
    if(CurSet.SpecMode == 0)
    {
        this->Btn_IsZero->setEnabled(false);
        this->Btn_Measure->setEnabled(true);
    }
    else
    {
        this->Btn_IsZero->setEnabled(true);
        this->Btn_Measure->setEnabled(true);
    }
}

void SpectralScan_Dlg::bsetEnable(bool bset)
{
    this->Btn_FindPeakMax->setEnabled(bset);
    this->Btn_FindPeakMin->setEnabled(bset);
    this->lineEdit_fTh->setEnabled(bset);
    this->lineEdit_ShowNm->setEnabled(bset);
    this->Btn_showMoveLeft->setEnabled(bset);
    this->Btn_showMoveRight->setEnabled(bset);
    this->Btn_Save->setEnabled(bset);    
    this->Btn_ParaSettings->setEnabled(bset);
    this->Btn_readSaveScan->setEnabled(bset);
}

void SpectralScan_Dlg::do_transStrToDlg(struct SpecParaSetting SpecA)
{
    CurSet=SpecA;
    Data::average_time=CurSet.AverageS;
    Data::SetTime=CurSet.CCDTime*1000;
    StepSpecNM = (float)(CurSet.EndSpecNM - CurSet.StartSpecNM)/500;
    this->lineEdit_ShowNm->setText (QString::number((CurSet.EndSpecNM+CurSet.StartSpecNM)/2));
    sign_draw =0;
    ShowSignmaxormin = 0;
}

void SpectralScan_Dlg::createColorComboBox(QComboBox *comboBox)
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

void SpectralScan_Dlg::on_checkBox_CH1_stateChanged(int )
{
    if(this->checkBox_CH1->isChecked())
        Data::sign_TestCHNum |=0x01;    
    else
        Data::sign_TestCHNum &=0x06;
    this->comboBox_CH1->setEnabled(this->checkBox_CH1->isChecked());
    if(Data::sign_TestCHNum)
    {
        if(CurSet.SpecMode == 0)
        {
            this->Btn_IsZero->setEnabled(false);
            this->Btn_Measure->setEnabled(true);
        }
        else
        {
            this->Btn_IsZero->setEnabled(true);
            this->Btn_Measure->setEnabled(false);
        }
    }
    else
    {
        this->Btn_IsZero->setEnabled(false);
        this->Btn_Measure->setEnabled(false);
    }
}

void SpectralScan_Dlg::on_checkBox_CH2_stateChanged(int )
{
    if(this->checkBox_CH2->isChecked())
        Data::sign_TestCHNum |=0x02;
    else
        Data::sign_TestCHNum &=0x05;
    this->comboBox_CH2->setEnabled(this->checkBox_CH2->isChecked());
    if(Data::sign_TestCHNum)
    {
        if(CurSet.SpecMode == 0)
        {
            this->Btn_IsZero->setEnabled(false);
            this->Btn_Measure->setEnabled(true);
        }
        else
        {
            this->Btn_IsZero->setEnabled(true);
            this->Btn_Measure->setEnabled(false);
        }
    }
    else
    {
        this->Btn_IsZero->setEnabled(false);
        this->Btn_Measure->setEnabled(false);
    }

}

void SpectralScan_Dlg::on_checkBox_CH3_stateChanged(int )
{
    if(this->checkBox_CH3->isChecked())
        Data::sign_TestCHNum |=0x04;
    else
        Data::sign_TestCHNum &=0x03;
    this->comboBox_CH3->setEnabled(this->checkBox_CH3->isChecked());
    if(Data::sign_TestCHNum)
    {
        if(CurSet.SpecMode == 0)
        {
            this->Btn_IsZero->setEnabled(false);
            this->Btn_Measure->setEnabled(true);
        }
        else
        {
            this->Btn_IsZero->setEnabled(true);
            this->Btn_Measure->setEnabled(false);
        }
    }
    else
    {
        this->Btn_IsZero->setEnabled(false);
        this->Btn_Measure->setEnabled(false);
    }
}

void SpectralScan_Dlg::on_comboBox_CH1_currentIndexChanged(int index)
{
    color[0] = comboBox_CH1->itemData(comboBox_CH1->currentIndex(), Qt::UserRole).value<QColor>();
    if(sign_draw == 1)
        update();
}

void SpectralScan_Dlg::on_comboBox_CH2_currentIndexChanged(int index)
{
    color[1] = comboBox_CH2->itemData(comboBox_CH2->currentIndex(), Qt::UserRole).value<QColor>();
    if(sign_draw == 1)
        update();
}

void SpectralScan_Dlg::on_comboBox_CH3_currentIndexChanged(int index)
{
    color[2] = comboBox_CH3->itemData(comboBox_CH3->currentIndex(), Qt::UserRole).value<QColor>();
    if(sign_draw == 1)
        update();
}
////MouseEvent
void SpectralScan_Dlg::mousePressEvent(QMouseEvent *event)
{
    QPoint TestPointXY = event->pos();
    if(sign_draw == 1)
    {
        if((TestPointXY.ry() >= 50)&&(TestPointXY.ry() <= 350)&&(TestPointXY.rx() >= 90)&&(TestPointXY.rx() <= 590))
        {
            ShowSpecNm  = (float)(TestPointXY.rx()-90)/500*((float)(CurSet.EndSpecNM-CurSet.StartSpecNM))+CurSet.StartSpecNM;
            int tmpChn =this->tabWidget->currentIndex();
            qDebug()<<"tmpChn============="<<tmpChn;
            int tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
            tmpFlag=tmpFlag>>tmpChn;
            if(tmpFlag&0x01)
            {
                struct Mdata* pData=findNMf(tmpChn,ShowSignmaxormin,ShowSpecNm);
                if(pData!=NULL)
                {
                    qDebug()<<"tmpChn%%%%%%%"<<tmpChn;
                    qDebug()<<"pData->Abs"<<pData->Abs;
                    qDebug()<<"pData->Nm"<<pData->Nm;
                    ShowVmaxormin[tmpChn]=pData->Abs;
                    ShowNmaxormin[tmpChn]=pData->Nm;
                    this->lineEdit_ShowNm->setText (QString::number(pData->Nm));
                    update();
                }
                else
                    this->lineEdit_ShowNm->setText (QString::number(ShowSpecNm));
            }
        }
    }
}

///Qpaint
bool SpectralScan_Dlg::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == this->tab_1)
    {
        if(event->type() == QEvent::Paint)
        {
            draw1();
            return true;
        }
    }
    if(obj == this->tab_2)
    {
        if(event->type() == QEvent::Paint)
        {
            draw2();
            return true;
        }
    }
    if(obj == this->tab_3)
    {
        if(event->type() == QEvent::Paint)
        {
            draw3();
            return true;
        }
    }
    return QDialog::eventFilter(obj,event);
}

void SpectralScan_Dlg::draw1()
{
    QPainter painter(this->tab_1);
    //x1,y1,x2,y2坐标轴
    painter.drawLine(80+10-ModifyX,350-ModifyY,600+10-ModifyX,350-ModifyY);
    painter.drawLine(595+10-ModifyX,355-ModifyY,600+10-ModifyX,350-ModifyY);
    painter.drawLine(595+10-ModifyX,345-ModifyY,600+10-ModifyX,350-ModifyY);
    painter.drawLine(80+10-ModifyX,40-ModifyY,80+10-ModifyX,350-ModifyY);
    painter.drawLine(80+10-ModifyX,40-ModifyY,85+10-ModifyX,45-ModifyY);
    painter.drawLine(80+10-ModifyX,40-ModifyY,75+10-ModifyX,45-ModifyY);
    for(int i =1;i <5;i++)
    {
        painter.drawLine(80+10+i*125-ModifyX,350-ModifyY,80+10+i*125-ModifyX,352-ModifyY);
        painter.drawLine(78+10-ModifyX,50+(i-1)*75-ModifyY,80+10-ModifyX,50+(i-1)*75-ModifyY);
    }
    for(int m =0;m <5;m++)
    {
        painter.drawText(70+m*125-ModifyX,364-ModifyY,QString::number(CurSet.StartSpecNM+(CurSet.EndSpecNM - CurSet.StartSpecNM)/4*m,'f',1));
        if(CurSet.SpecMode == AbsorbScan)
        {
            painter.drawText(40-ModifyX,354-m*75-ModifyY,QString::number(CurSet.MinYdata+(CurSet.MaxYdata - CurSet.MinYdata)/4*m,'f',3));
        }
        else
        {
            painter.drawText(40-ModifyX,354-m*75-ModifyY,QString::number(CurSet.MinYdata+(CurSet.MaxYdata - CurSet.MinYdata)/4*m,'f',1));
        }
    }
    //draw picture
    int tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
    int tmpChn=0;
    float tmpf=0;
    if(sign_draw == 1 && (tmpFlag&CHEN1))
    {
        int tmp = (ShowSpecNm - CurSet.StartSpecNM)/StepSpecNM;
        CurPen.setWidth(CurPenWidth);
        CurPen.setColor(color[tmpChn]);
        painter.setPen(CurPen);
        ////draw measure lines and draw text from ShowSpecNm value
        for(int k = 0;k < 499;k++)
        {
            painter.drawLine(k+80+10-ModifyX,350 - (DisSpecData[tmpChn][k] -CurSet.MinYdata)/(CurSet.MaxYdata - CurSet.MinYdata)*300-ModifyY,k+1+80+10-ModifyX,350 - (DisSpecData[tmpChn][k+1] - CurSet.MinYdata)/(CurSet.MaxYdata - CurSet.MinYdata)*300-ModifyY);
        }
        CurPen.setWidth(CurPenWidth);
        CurPen.setColor(Qt::darkYellow);            //深黄色曲线
        painter.setPen(CurPen);
        tmpf=CurSet.StartSpecNM+(CurSet.EndSpecNM - CurSet.StartSpecNM)*3/4;
        if(ShowSignmaxormin == 0)
        {
            painter.drawLine(tmp+80+11-ModifyX,50-ModifyY,tmp+80+11-ModifyX,350-ModifyY);
            if(tmpf < this->lineEdit_ShowNm->text().toFloat())
                painter.drawText(tmp+80+20-ModifyX-ModifyD,50+tmpChn*10-ModifyY,QString::number(ShowTemp[tmpChn],'f',3));
            else
                painter.drawText(tmp+80+20-ModifyX,50+tmpChn*10-ModifyY,QString::number(ShowTemp[tmpChn],'f',3));
        }
        else if(ShowSignmaxormin == 1 or ShowSignmaxormin == 2)    ///draw max or min
        {
            int tmp1 = (int)((ShowNmaxormin[tmpChn] - (float)CurSet.StartSpecNM)/StepSpecNM);
            painter.drawLine(tmp1+80+11-ModifyX,350-ModifyY,tmp1+80+11-ModifyX,50+tmpChn*10-ModifyY);
            if(tmpf < this->lineEdit_ShowNm->text().toFloat())
                painter.drawText(tmp1+80+10-ModifyX-ModifyD,50+tmpChn*10-ModifyY,QString::number(ShowVmaxormin[tmpChn],'f',3));
            else
                painter.drawText(tmp1+80+10-ModifyX,50+tmpChn*10-ModifyY,QString::number(ShowVmaxormin[tmpChn],'f',3));
        }        
    }
}

void SpectralScan_Dlg::draw2()
{
    QPainter painter(this->tab_2);
    //x1,y1,x2,y2坐标轴
    painter.drawLine(80+10-ModifyX,350-ModifyY,600+10-ModifyX,350-ModifyY);
    painter.drawLine(595+10-ModifyX,355-ModifyY,600+10-ModifyX,350-ModifyY);
    painter.drawLine(595+10-ModifyX,345-ModifyY,600+10-ModifyX,350-ModifyY);
    painter.drawLine(80+10-ModifyX,40-ModifyY,80+10-ModifyX,350-ModifyY);
    painter.drawLine(80+10-ModifyX,40-ModifyY,85+10-ModifyX,45-ModifyY);
    painter.drawLine(80+10-ModifyX,40-ModifyY,75+10-ModifyX,45-ModifyY);
    for(int i =1;i <5;i++)
    {
        painter.drawLine(80+10+i*125-ModifyX,350-ModifyY,80+10+i*125-ModifyX,352-ModifyY);
        painter.drawLine(78+10-ModifyX,50+(i-1)*75-ModifyY,80+10-ModifyX,50+(i-1)*75-ModifyY);
    }
    for(int m =0;m <5;m++)
    {
        painter.drawText(70+m*125-ModifyX,364-ModifyY,QString::number(CurSet.StartSpecNM+(CurSet.EndSpecNM - CurSet.StartSpecNM)/4*m,'f',1));
        if(CurSet.SpecMode == AbsorbScan)
        {
            painter.drawText(40-ModifyX,354-m*75-ModifyY,QString::number(CurSet.MinYdata+(CurSet.MaxYdata - CurSet.MinYdata)/4*m,'f',3));
        }
        else
        {
            painter.drawText(40-ModifyX,354-m*75-ModifyY,QString::number(CurSet.MinYdata+(CurSet.MaxYdata - CurSet.MinYdata)/4*m,'f',1));
        }
    }
    //draw picture
    int tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
    int tmpChn=1;
    float tmpf=0;
    if(sign_draw == 1 && (tmpFlag&CHEN2))
    {
        int tmp = (ShowSpecNm - CurSet.StartSpecNM)/StepSpecNM;
        CurPen.setWidth(CurPenWidth);
        CurPen.setColor(color[tmpChn]);
        painter.setPen(CurPen);
        tmpf=CurSet.StartSpecNM+(CurSet.EndSpecNM - CurSet.StartSpecNM)*3/4;
        ////draw measure lines and draw text from ShowSpecNm value
        for(int k = 0;k < 499;k++)
        {
            painter.drawLine(k+80+10-ModifyX,350 - (DisSpecData[tmpChn][k] -CurSet.MinYdata)/(CurSet.MaxYdata - CurSet.MinYdata)*300-ModifyY,k+1+80+10-ModifyX,350 - (DisSpecData[tmpChn][k+1] - CurSet.MinYdata)/(CurSet.MaxYdata - CurSet.MinYdata)*300-ModifyY);
        }
        CurPen.setWidth(CurPenWidth);
        CurPen.setColor(Qt::darkYellow);            //深黄色曲线
        painter.setPen(CurPen);
        if(ShowSignmaxormin == 0)
        {
            painter.drawLine(tmp+80+11-ModifyX,50-ModifyY,tmp+80+11-ModifyX,350-ModifyY);
            if(tmpf < this->lineEdit_ShowNm->text().toFloat())
                painter.drawText(tmp+80+20-ModifyX-ModifyD,50+tmpChn*10-ModifyY,QString::number(ShowTemp[tmpChn],'f',3));
            else
                painter.drawText(tmp+80+20-ModifyX,50+tmpChn*10-ModifyY,QString::number(ShowTemp[tmpChn],'f',3));
        }
        else if(ShowSignmaxormin == 1 or ShowSignmaxormin == 2)    ///draw max or min
        {
            int tmp1 = (int)((ShowNmaxormin[tmpChn] - (float)CurSet.StartSpecNM)/StepSpecNM);
            painter.drawLine(tmp1+80+11-ModifyX,350-ModifyY,tmp1+80+11-ModifyX,50+tmpChn*10-ModifyY);
            if(tmpf < this->lineEdit_ShowNm->text().toFloat())
                painter.drawText(tmp1+80+10-ModifyX-ModifyD,50+tmpChn*10-ModifyY,QString::number(ShowVmaxormin[tmpChn],'f',3));
            else
                painter.drawText(tmp1+80+10-ModifyX,50+tmpChn*10-ModifyY,QString::number(ShowVmaxormin[tmpChn],'f',3));
        }
    }
}
void SpectralScan_Dlg::draw3()
{
    QPainter painter(this->tab_3);
    //x1,y1,x2,y2坐标轴
    painter.drawLine(80+10-ModifyX,350-ModifyY,600+10-ModifyX,350-ModifyY);
    painter.drawLine(595+10-ModifyX,355-ModifyY,600+10-ModifyX,350-ModifyY);
    painter.drawLine(595+10-ModifyX,345-ModifyY,600+10-ModifyX,350-ModifyY);
    painter.drawLine(80+10-ModifyX,40-ModifyY,80+10-ModifyX,350-ModifyY);
    painter.drawLine(80+10-ModifyX,40-ModifyY,85+10-ModifyX,45-ModifyY);
    painter.drawLine(80+10-ModifyX,40-ModifyY,75+10-ModifyX,45-ModifyY);
    for(int i =1;i <5;i++)
    {
        painter.drawLine(80+10+i*125-ModifyX,350-ModifyY,80+10+i*125-ModifyX,352-ModifyY);
        painter.drawLine(78+10-ModifyX,50+(i-1)*75-ModifyY,80+10-ModifyX,50+(i-1)*75-ModifyY);
    }
    for(int m =0;m <5;m++)
    {
        painter.drawText(70+m*125-ModifyX,364-ModifyY,QString::number(CurSet.StartSpecNM+(CurSet.EndSpecNM - CurSet.StartSpecNM)/4*m,'f',1));
        if(CurSet.SpecMode == AbsorbScan)
        {
            painter.drawText(40-ModifyX,354-m*75-ModifyY,QString::number(CurSet.MinYdata+(CurSet.MaxYdata - CurSet.MinYdata)/4*m,'f',3));
        }
        else
        {
            painter.drawText(40-ModifyX,354-m*75-ModifyY,QString::number(CurSet.MinYdata+(CurSet.MaxYdata - CurSet.MinYdata)/4*m,'f',1));
        }
    }
    //draw picture
    int tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
    int tmpChn=2;
    float tmpf=0;
    if(sign_draw == 1 && (tmpFlag&CHEN3))
    {
        int tmp = (ShowSpecNm - CurSet.StartSpecNM)/StepSpecNM;
        CurPen.setWidth(CurPenWidth);
        CurPen.setColor(color[tmpChn]);
        painter.setPen(CurPen);
        tmpf=CurSet.StartSpecNM+(CurSet.EndSpecNM - CurSet.StartSpecNM)*3/4;
        ////draw measure lines and draw text from ShowSpecNm value
        for(int k = 0;k < 499;k++)
        {
            painter.drawLine(k+80+10-ModifyX,350 - (DisSpecData[tmpChn][k] -CurSet.MinYdata)/(CurSet.MaxYdata - CurSet.MinYdata)*300-ModifyY,k+1+80+10-ModifyX,350 - (DisSpecData[tmpChn][k+1] - CurSet.MinYdata)/(CurSet.MaxYdata - CurSet.MinYdata)*300-ModifyY);
        }
        CurPen.setWidth(CurPenWidth);
        CurPen.setColor(Qt::darkYellow);            //深黄色曲线
        painter.setPen(CurPen);
        if(ShowSignmaxormin == 0)
        {
            painter.drawLine(tmp+80+11-ModifyX,50-ModifyY,tmp+80+11-ModifyX,350-ModifyY);
            if(tmpf < this->lineEdit_ShowNm->text().toFloat())
                painter.drawText(tmp+80+20-ModifyX-ModifyD,50+tmpChn*10-ModifyY,QString::number(ShowTemp[tmpChn],'f',3));
            else
                painter.drawText(tmp+80+20-ModifyX,50+tmpChn*10-ModifyY,QString::number(ShowTemp[tmpChn],'f',3));
        }
        else if(ShowSignmaxormin == 1 or ShowSignmaxormin == 2)    ///draw max or min
        {
            int tmp1 = (int)((ShowNmaxormin[tmpChn] - (float)CurSet.StartSpecNM)/StepSpecNM);
            painter.drawLine(tmp1+80+11-ModifyX,350-ModifyY,tmp1+80+11-ModifyX,50+tmpChn*10-ModifyY);
            if(tmpf < this->lineEdit_ShowNm->text().toFloat())
                painter.drawText(tmp1+80+10-ModifyX-ModifyD,50+tmpChn*10-ModifyY,QString::number(ShowVmaxormin[tmpChn],'f',3));
            else
                painter.drawText(tmp1+80+10-ModifyX,50+tmpChn*10-ModifyY,QString::number(ShowVmaxormin[tmpChn],'f',3));
        }
    }
}

float SpectralScan_Dlg::matchdata(int num, float value)
{
    int low,high,mid;
    low=0;
    high=RESOLUTION;
    while(low <= high)
    {
        mid = (low + high)/2;
        if(value == Data::ListSpecNm[num][mid])
        {
            break;
        }
        if(value > Data::ListSpecNm[num][mid])
        {
            low = mid + 1;
        }
        if(value < Data::ListSpecNm[num][mid])
        {
            high = mid - 1;
        }
    }
    if(low <= high)
        return AbsData[num][mid];
    else
    {
        if( AbsData[num][low] > AbsData[num][high])
        {
            return  AbsData[num][high] + (AbsData[num][low] - AbsData[num][high])/(Data::ListSpecNm[num][low] - Data::ListSpecNm[num][high])*(value - Data::ListSpecNm[num][high]);
        }
        else if( AbsData[num][low] < AbsData[num][high])
        {
            return  AbsData[num][low] + (AbsData[num][high] - AbsData[num][low])/(Data::ListSpecNm[num][high] - Data::ListSpecNm[num][low])*(value - Data::ListSpecNm[num][low]);
        }
        else
        {
            return AbsData[num][low];
        }
    }
}
int SpectralScan_Dlg::matchOder(int num, float value, int flag)
{
    int low,high,mid;
    low=0;
    high=RESOLUTION;
    while(low <= high)
    {
        mid = (low + high)/2;
        if(value == Data::ListSpecNm[num][mid])
        {
            break;
        }
        if(value > Data::ListSpecNm[num][mid])
        {
            low = mid + 1;
        }
        if(value < Data::ListSpecNm[num][mid])
        {
            high = mid - 1;
        }
    }
    if(low <= high)
        return mid;
    else
    {
        if(flag==0)
            return high;
        else if(flag==1)
            return low;
    }
}

void SpectralScan_Dlg::on_Btn_FindPeakMax_clicked()
{ 
    struct Mdata* pData=NULL;
    int tmpChn =this->tabWidget->currentIndex();
    ShowSignmaxormin = 1;
    int tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
    tmpFlag=tmpFlag>>tmpChn;
    if(tmpFlag&0x01)
    {
        pData=findNMi(tmpChn,ShowSignmaxormin,maxNumOder++);
        if(pData!=NULL)
        {
            qDebug()<<"on_Btn_FindPeakMax_clicked()";
            ShowVmaxormin[tmpChn]=pData->Abs;
            ShowNmaxormin[tmpChn]=pData->Nm;
            this->lineEdit_ShowNm->setText (QString::number(pData->Nm));
            qDebug()<<"maxNumoder***"<<pData->order;
            this->label_oder->setText(tr("当前顺序 %1").arg(pData->order+1));
            update();
        }
        else
        {
            maxNumOder=0;
            ShowSignmaxormin =0;
        }
    }
}

void SpectralScan_Dlg::on_Btn_FindPeakMin_clicked()
{   
    struct Mdata* pData=NULL;
    int tmpChn =this->tabWidget->currentIndex();
    ShowSignmaxormin = 2;
    int tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
    tmpFlag=tmpFlag>>tmpChn;
    if(tmpFlag&0x01)
    {
        qDebug()<<"&&&&&&&&&1&&&&&&&"<<tmpChn;
        qDebug()<<"&&&&&&&&2&&&&&&&&"<<ShowSignmaxormin;
        qDebug()<<"&&&&&&&&2&&&&&&&&"<<minNumOder;
        pData=findNMi(tmpChn,ShowSignmaxormin,minNumOder++);
        qDebug()<<" tmpChn ----------"<<tmpChn;
        if(pData!=NULL)
        {
            qDebug()<<"on_Btn_FindPeakMin_clicked()";
            ShowVmaxormin[tmpChn]=pData->Abs;
            ShowNmaxormin[tmpChn]=pData->Nm;
            this->lineEdit_ShowNm->setText (QString::number(pData->Nm));
            qDebug()<<"minNumoder***"<<pData->order;
            this->label_oder->setText(tr("当前顺序 %1").arg(pData->order+1));
            update();
        }
        else
        {
            minNumOder=0;
            ShowSignmaxormin =0;
        }
    }
}

void SpectralScan_Dlg::on_lineEdit_ShowNm_textChanged(QString )
{
    if(sign_draw == 1 && ShowSignmaxormin ==0)
    {
        //init
        ShowSpecNm = this->lineEdit_ShowNm->text().toFloat();
        if(ShowSpecNm < CurSet.StartSpecNM)
        {
            ShowSpecNm=CurSet.StartSpecNM;
            this->lineEdit_ShowNm->setText (QString::number(ShowSpecNm));
        }
        else if(ShowSpecNm > CurSet.EndSpecNM)
        {
            ShowSpecNm=CurSet.EndSpecNM;
            this->lineEdit_ShowNm->setText (QString::number(ShowSpecNm));
        }
        //display
        qDebug()<<"liuhaifeng";
        int tmpFlag = Data::sign_TestCHNum^Data::errorFlag;
        int tmpChn =this->tabWidget->currentIndex();
        tmpFlag=tmpFlag>>tmpChn;
        if(tmpFlag&0x01)
            ShowTemp[tmpChn]= matchdata(tmpChn,ShowSpecNm);        
        update();
    }
}

void SpectralScan_Dlg::on_Btn_showMoveLeft_clicked()
{
    maxNumOder=0;
    minNumOder=0;
    ShowSignmaxormin =0;
    ShowSpecNm = this->lineEdit_ShowNm->text().toFloat();
    if(ShowSpecNm>CurSet.StartSpecNM)
    {
        ShowSpecNm--;
        this->lineEdit_ShowNm->setText (QString::number(ShowSpecNm));
        Btn_showMoveRight->setEnabled(true);
    }
    else
    {
        Btn_showMoveRight->setEnabled(true);
        Btn_showMoveLeft->setEnabled(false);
    }
}

void SpectralScan_Dlg::on_Btn_showMoveRight_clicked()
{
    maxNumOder=0;
    minNumOder=0;
    ShowSignmaxormin =0;
    ShowSpecNm = this->lineEdit_ShowNm->text().toFloat();
    if(ShowSpecNm<CurSet.EndSpecNM)
    {
        ShowSpecNm++;
        this->lineEdit_ShowNm->setText (QString::number(ShowSpecNm));
        Btn_showMoveLeft->setEnabled(true);
    }
    else
    {
        Btn_showMoveRight->setEnabled(false);
        Btn_showMoveLeft->setEnabled(true);
    }
}

void SpectralScan_Dlg::saveAbsData(bool bool_ok)
{
    int tmpFlag;
    if(bool_ok)
        tmpFlag=Data::sign_TestCHNum;
    else
        tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
    for(int j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            for(int i = 0;i < RESOLUTION;i++)
            {
                if(CurSet.SpecMode == EnergyScan)
                {
                    AbsData[j][i] = Data::SampleData[j][i];
                }
                else if(CurSet.SpecMode == TransmitScan)
                {
                    //                    Data::DarkData[j][i]=0;
                    AbsData[j][i] = (float)(Data::SampleData[j][i] - Data::DarkData[j][i])/(float)(Data::BaseData[j][i] - Data::DarkData[j][i])*100;
                }
                else if(CurSet.SpecMode == AbsorbScan)
                {
                    //                    Data::DarkData[j][i]=0;
                    AbsData[j][i] = -log10((float)(Data::SampleData[j][i] - Data::DarkData[j][i])/(float)(Data::BaseData[j][i] - Data::DarkData[j][i]));
                    if(AbsData[j][i] > 10)
                    {
                        AbsData[j][i] = 10;
                    }
                    else if(AbsData[j][i] < -10)
                    {
                        AbsData[j][i] = -10;
                    }
                }
            }
        }
        tmpFlag=tmpFlag>>1;
    }
}
void SpectralScan_Dlg::saveDisSpecData(bool bool_ok)
{
    int tmpFlag;
    if(bool_ok)
        tmpFlag=Data::sign_TestCHNum;
    else
        tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
    for(int j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            for(int i = 0;i < 500;i++)
            {
                DisSpecData [j][i] = matchdata(j, CurSet.StartSpecNM + i*StepSpecNM);
                if(DisSpecData [j][i] > CurSet.MaxYdata)
                {
                    DisSpecData [j][i] = CurSet.MaxYdata;
                }
                else if(DisSpecData [j][i] < CurSet.MinYdata)
                {
                    DisSpecData [j][i] = CurSet.MinYdata;
                }
            }
        }
        tmpFlag=tmpFlag>>1;
    }
}

void SpectralScan_Dlg::findMaxMin()
{    
    bool bUp = true;
    float fCurrData;
    float fMaxValue,fMinValue;
    int nMax = 0;
    int nMin = 0;
    struct Mdata* mydata=NULL;
    int m,j,Nstart[4],Nend[4];
    int tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
    for(j=0;j<3;j++)
    {
        this->maxorminlist[j]->maxlist.clear();
        this->maxorminlist[j]->minlist.clear();
    }
    for(j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            for(m = 0;m < RESOLUTION;m++)
            {
                if((Data::ListSpecNm[j][m] > CurSet.StartSpecNM)&&(Data::ListSpecNm[j][m - 1] < CurSet.StartSpecNM))
                {
                    Nstart[j] = m;
                }
                else if(Data::ListSpecNm[j][m] == CurSet.StartSpecNM)
                {
                    Nstart[j] = m;
                }
                if((Data::ListSpecNm[j][m] > CurSet.EndSpecNM)&&(Data::ListSpecNm[j][m - 1] < CurSet.EndSpecNM))
                {
                    Nend[j] = m;
                }
                else if(Data::ListSpecNm[j][m] == CurSet.EndSpecNM)
                {
                    Nend[j] = m;
                }
            }
            ////dispose begin
            fMaxValue=fMinValue= AbsData[j][Nstart[j]];
            for (int i = Nstart[j]; i < Nend[j]+1; i++)
            {
                fCurrData = AbsData[j][i];
                if (fCurrData >= fMaxValue)
                {
                    fMaxValue = fCurrData;
                    nMax = i;
                    bUp = true;
                }
                if (fCurrData <= fMinValue)
                {
                    fMinValue = fCurrData;
                    nMin = i;
                    bUp = false;
                }

                if (bUp)
                {
                    if (fabs(fMaxValue - fCurrData) > Data::fThreshold)
                    {
                        bUp = false;
                        fMaxValue = fMinValue = fCurrData;
                        mydata= new Mdata;
                        mydata->Abs=AbsData[j][nMax];
                        mydata->Nm=Data::ListSpecNm[j][nMax];
                        mydata->Xtmp=(int)((Data::ListSpecNm[j][nMax] - (float)CurSet.StartSpecNM)/StepSpecNM);
                        mydata->order=nMax;
                        this->maxorminlist[j]->maxlist.push_back(mydata);
                    }
                }
                else
                {
                    if (fabs(fCurrData - fMinValue) > Data::fThreshold)
                    {
                        bUp = true;
                        fMaxValue = fMinValue = fCurrData;
                        mydata= new Mdata;
                        mydata->Abs=AbsData[j][nMin];
                        mydata->Nm=Data::ListSpecNm[j][nMin];
                        mydata->Xtmp=(int)((Data::ListSpecNm[j][nMin] - (float)CurSet.StartSpecNM)/StepSpecNM);
                        mydata->order=nMin;
                        this->maxorminlist[j]->minlist.push_back(mydata);

                    }
                }
            }
            ////dispose over
        }
        tmpFlag=tmpFlag>>1;
    }
    mydata=NULL;
    //    delete mydata;
}

void SpectralScan_Dlg::disposeOder()
{
    int tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
    for(int j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            float temp;
            int num;
            //*********************************max dispose*********************************
            int maxNum = this->maxorminlist[j]->maxlist.size();
            float fmaxValue[maxNum];
            int  fmaxOder[maxNum];
            ////init value
            for(int k=0;k<maxNum;k++)
            {
                fmaxValue[k] = this->maxorminlist[j]->maxlist.at(k)->Abs;
                fmaxOder[k] = k;
            }
            ////order number   maxID 0
            for(int k=0;k<maxNum-1;k++)
            {
                for (int i=0;i<maxNum-1-k;i++)
                {
                    if (fmaxValue[i]<fmaxValue[i+1])
                    {
                        temp=fmaxValue[i];
                        fmaxValue[i]=fmaxValue[i+1];
                        fmaxValue[i+1]=temp;
                        num=fmaxOder[i];
                        fmaxOder[i]=fmaxOder[i+1];
                        fmaxOder[i+1]=num;
                    }
                }
            }
            ////save
            for(int k=0;k<maxNum;k++)
            {
                this->maxorminlist[j]->maxlist.at(fmaxOder[k])->order = k;
            }
            //*********************************min dispose*********************************
            int minNum = this->maxorminlist[j]->minlist.size();
            float fminValue[minNum];
            int  fminOder[minNum];
            ////init value
            for(int k=0;k<minNum;k++)
            {
                fminValue[k] = this->maxorminlist[j]->minlist.at(k)->Abs;
                fminOder[k] = k;
            }
            ////order number  minID 0
            for(int k=0;k<minNum-1;k++)
            {
                for (int i=0;i<minNum-1-k;i++)
                {
                    if (fminValue[i]>fminValue[i+1])
                    {
                        temp=fminValue[i];
                        fminValue[i]=fminValue[i+1];
                        fminValue[i+1]=temp;
                        num=fminOder[i];
                        fminOder[i]=fminOder[i+1];
                        fminOder[i+1]=num;
                    }
                }
            }
            ////save
            for(int k=0;k<minNum;k++)
            {
                this->maxorminlist[j]->minlist.at(fminOder[k])->order = k;
            }
        }
        tmpFlag=tmpFlag>>1;
    }
}
void SpectralScan_Dlg::on_tabWidget_currentChanged(int index)
{    
    maxNumOder=0;
    minNumOder=0;
    ShowSignmaxormin =0;
    if(sign_draw==1)
    {
        this->label_max->setText(tr("峰值个数 %1").arg(this->maxorminlist[index]->maxlist.size()));
        this->label_min->setText(tr("谷值个数 %1").arg(this->maxorminlist[index]->minlist.size()));
    }
}

struct Mdata* SpectralScan_Dlg::findNMi(int num,int MaxorMin,int order)
{
    if(MaxorMin==1)
    {
        for(int i=0;i<this->maxorminlist[num]->maxlist.size();i++)
            if(this->maxorminlist[num]->maxlist.at(i)->order==order)
                return this->maxorminlist[num]->maxlist.at(i);
    }
    else if(MaxorMin==2)
    {
        qDebug()<<"dfdsasadfsfdf";
        for(int i=0;i<this->maxorminlist[num]->minlist.size();i++)
            if(this->maxorminlist[num]->minlist.at(i)->order==order)
                return this->maxorminlist[num]->minlist.at(i);
    }
    return NULL;
}

struct Mdata* SpectralScan_Dlg::findNMf(int num,int MaxorMin,float curNM)
{
    int ID=0;
    int tmpNM,tmpVal;
    tmpNM = (curNM- CurSet.StartSpecNM)/StepSpecNM;
    if(MaxorMin==1)
    {
        if(this->maxorminlist[num]->maxlist.size()>0)
            tmpVal=abs(this->maxorminlist[num]->maxlist.at(0)->Xtmp-tmpNM);
        else
            return NULL;
        for(int i=1;i<this->maxorminlist[num]->maxlist.size();i++)
        {
            if(abs(this->maxorminlist[num]->maxlist.at(i)->Xtmp-tmpNM)<tmpVal)
            {
                tmpVal=abs(this->maxorminlist[num]->maxlist.at(i)->Xtmp-tmpNM);
                ID=i;
            }
        }
        return this->maxorminlist[num]->maxlist.at(ID);
    }
    else if(MaxorMin==2)
    {
        if(this->maxorminlist[num]->minlist.size()>0)
            tmpVal=abs(this->maxorminlist[num]->minlist.at(0)->Xtmp-tmpNM);
        else
            return NULL;
        for(int i=1;i<this->maxorminlist[num]->minlist.size();i++)
        {
            if(abs(this->maxorminlist[num]->minlist.at(i)->Xtmp-tmpNM)<tmpVal)
            {
                tmpVal=abs(this->maxorminlist[num]->minlist.at(i)->Xtmp-tmpNM);
                ID=i;
            }
        }
        return this->maxorminlist[num]->minlist.at(ID);
    }
    return NULL;
}

bool SpectralScan_Dlg::createConnection()
{
    db_specscan = QSqlDatabase::addDatabase("QSQLITE");
    db_specscan.setDatabaseName(SPECBASENAME);
    if (!db_specscan.open())
    {
        qDebug()<<"Database Error";
        return false;
    }
    return true;
}

bool SpectralScan_Dlg::readSpeclistSQL()
{
    QSqlTableModel model;
    int chnNum,oderNum;
    model.setTable("listspecNm");
    model.setSort(0,Qt::AscendingOrder);   //DescendingOrder
    model.select();
    while(model.canFetchMore())
        model.fetchMore();
    if(model.rowCount()!= RESOLUTION*3)
        return false;
    for(int i=0;i<RESOLUTION*3;i++)
    {
        chnNum=model.record(i).value("chnNum").toInt();
        oderNum=model.record(i).value("oderNum").toInt();
        Data::ListSpecNm[chnNum][oderNum]=model.record(i).value("specvalue").toFloat();
    }
    return true;
}

void SpectralScan_Dlg::saveSpeclistSQL()
{
    QSqlQuery query;
    QSqlTableModel model;
    model.setTable("listspecNm");
    model.setSort(0,Qt::AscendingOrder);   //DescendingOrder
    model.select();
    while(model.canFetchMore())
        model.fetchMore();
    if(model.rowCount()!=RESOLUTION*3)
    {
        for(int i=0;i<model.rowCount();++i)
            model.removeRow(i);
        model.submitAll();
    }
    db_specscan.transaction();
    for (int j=0;j<3;j++)
    {
        for(int i = 0;i <RESOLUTION;i++)
        {
            bsuccess = query.exec("INSERT INTO  listspecNm VALUES (NULL, "
                                  " '"+QString::number(j)+"', "
                                  " '"+QString::number(i)+"', "
                                  " '"+QString::number(Data::ListSpecNm[j][i],'f',1)+"')");
            if (!bsuccess)
            {
                qDebug()<<"Error occur";
                break;
            }
        }
    }
    db_specscan.commit();
}

void SpectralScan_Dlg::on_Btn_Save_clicked()
{
    bsetEnable(false);
    this->Btn_Measure->setEnabled(false);
    QMessageBox *messageBox5=new QMessageBox(QMessageBox::Information,tr("等待"),tr("数据保存，请稍后"),QMessageBox::Ok,this);
    messageBox5->show();
    QTime nt;
    nt.start();
    while(nt.elapsed()<5000)
        QCoreApplication::processEvents();
    QSqlQuery query;
    int a[3],b[3];
    //information
    int tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
    for(int j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            a[j]=matchOder(j,CurSet.StartSpecNM,0);
            b[j]=matchOder(j,CurSet.EndSpecNM,1);
        }
        else
        {
            a[j]=RESOLUTION-1;
            b[j]=RESOLUTION-1;
        }
        tmpFlag=tmpFlag>>1;
    }
    query.exec("INSERT INTO  specscan VALUES (NULL, "
               " '"+QString::number(Data::sign_TestCHNum)+"', "
               " '"+QString::number(CurSet.SpecMode)+"', "
               " '"+QString::number(CurSet.StartSpecNM)+"', "
               " '"+QString::number(CurSet.EndSpecNM)+"', "
               " '"+QString::number(CurSet.MaxYdata,'f',1)+"', "
               " '"+QString::number(CurSet.MinYdata,'f',1)+"', "
               " '"+QString::number(CurSet.CCDTime,'f',3)+"', "
               " '"+QString::number(CurSet.AverageS)+"', "
               " '"+QString::number(Data::fThreshold,'f',1)+"', "
               " '"+(QDateTime::currentDateTime()).toString("yyyy-MM-dd hh:mm:ss")+"', "
               " '"+QString::number(a[0])+"', "
               " '"+QString::number(b[0])+"', "
               " '"+QString::number(a[1])+"', "
               " '"+QString::number(b[1])+"', "
               " '"+QString::number(a[2])+"', "
               " '"+QString::number(b[2])+"')");
    //save absdata
    QSqlTableModel model;
    model.setTable("specscan");
    model.setSort(0,Qt::AscendingOrder);
    model.select();
    while(model.canFetchMore())
        model.fetchMore();
    int tmp = model.record(model.rowCount()-1).value("id").toInt();
    //save result
    tmpFlag=Data::sign_TestCHNum^Data::errorFlag;    
    db_specscan.transaction();
    for(int j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            for(int k = a[j];k <b[j]+1;k++)
            {
                query.exec("INSERT INTO  result VALUES ('"+QString::number(tmp)+"', "
                           " '"+QString::number(j)+"', "
                           " '"+QString::number(k)+"', "
                           " '"+QString::number(AbsData[j][k],'f',1)+"')");
            }
        }
        tmpFlag=tmpFlag>>1;
    }
    db_specscan.commit();    
    messageBox5->close();
    bsetEnable(true);
    this->Btn_Measure->setEnabled(true);
    delete messageBox5;
}

void SpectralScan_Dlg::do_transIntToDlg(int displayNum)
{    
    int a[3],b[3];    
    //save absdata
    QSqlTableModel model;
    model.setTable("specscan");
    model.setSort(0,Qt::AscendingOrder);
    model.setFilter(QObject::tr("id = '%1'").arg(displayNum));
    model.select();
    while(model.canFetchMore())
        model.fetchMore();
    //read parameters    
    Data::errorFlag = 0;
    Data::sign_TestCHNum = model.record(0).value("CHNum").toInt();
    CurSet.SpecMode = model.record(0).value("specmode").toInt();
    CurSet.StartSpecNM = model.record(0).value("startspecNM").toInt();
    CurSet.EndSpecNM = model.record(0).value("endspecNM").toInt();
    CurSet.MaxYdata = model.record(0).value("maxYdata").toFloat();
    CurSet.MinYdata = model.record(0).value("minYdata").toFloat();
    CurSet.CCDTime = model.record(0).value("CCDtime").toFloat();
    CurSet.AverageS = model.record(0).value("averageS").toInt();
    Data::fThreshold = model.record(0).value("fThreshold").toFloat();
    a[0] = model.record(0).value("startNum0").toInt();
    b[0] = model.record(0).value("endNum0").toInt();
    a[1] = model.record(0).value("startNum1").toInt();
    b[1] = model.record(0).value("endNum1").toInt();
    a[2] = model.record(0).value("startNum2").toInt();
    b[2] = model.record(0).value("endNum2").toInt();
    //deal something
    StepSpecNM = (float)(CurSet.EndSpecNM - CurSet.StartSpecNM)/500;
    int tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
    this->checkBox_CH1->setChecked(tmpFlag&0x01);
    this->checkBox_CH2->setChecked(tmpFlag&0x02);
    this->checkBox_CH3->setChecked(tmpFlag&0x04);
    //read result
    QSqlTableModel model2;
    int chnNum,oderNum;
    model2.setTable("result");
    model2.setSort(0,Qt::AscendingOrder);
    model2.setFilter(QObject::tr("id = '%1'").arg(displayNum));
    model2.select();
    while(model2.canFetchMore())
        model2.fetchMore();
    for(int row=0;row <model2.rowCount();++row)
    {
        chnNum=model2.record(row).value("chnNum").toInt();
        oderNum=model2.record(row).value("oderNum").toInt();
        AbsData[chnNum][oderNum]=model2.record(row).value("absdata").toFloat();
    }
    ///save others
//    int tmpFlag=Data::sign_TestCHNum^Data::errorFlag;
    for(int j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            for(int k=0;k<a[j];k++)
            {
                AbsData[j][k]=AbsData[j][a[j]];
            }
            for(int i=b[j]+1;i<RESOLUTION;i++)
            {
                AbsData[j][i]=AbsData[j][b[j]];
            }
        }
        tmpFlag=tmpFlag>>1;
    }
    saveDisSpecData(true);
    //find max and min list,then dispose
    sign_draw =1;
    float tmpfTh =0;
    tmpfTh=this->lineEdit_fTh->text().toFloat();
    if(abs(tmpfTh-Data::fThreshold)<0.001)
    {
        findMaxMin();
        disposeOder();
        maxNumOder=0;
        minNumOder=0;
        ShowSignmaxormin =0;
        this->label_max->setText(tr("峰值个数 %1").arg(this->maxorminlist[this->tabWidget->currentIndex()]->maxlist.size()));
        this->label_min->setText(tr("谷值个数 %1").arg(this->maxorminlist[this->tabWidget->currentIndex()]->minlist.size()));
        update();
    }
    else
    {
        this->lineEdit_fTh->setText (QString::number(Data::fThreshold));
    }
}

void SpectralScan_Dlg::on_lineEdit_fTh_textChanged(QString )
{
    Data::fThreshold = this->lineEdit_fTh->text().toFloat();
    findMaxMin();
    disposeOder();
    maxNumOder=0;
    minNumOder=0;
    ShowSignmaxormin =0;
    if(sign_draw==1)
    {
        this->label_max->setText(tr("峰值个数 %1").arg(this->maxorminlist[this->tabWidget->currentIndex()]->maxlist.size()));
        this->label_min->setText(tr("谷值个数 %1").arg(this->maxorminlist[this->tabWidget->currentIndex()]->minlist.size()));
    }
    update();
}

void SpectralScan_Dlg::mydelay(int needtime)
{
    ////wait
    QTime nt;
    nt.start();
    while(nt.elapsed()<needtime)
        ;
}
