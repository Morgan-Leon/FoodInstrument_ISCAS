#include "quantifymeasurement_dlg.h"
#include "math.h"
QuantifyMeasurement_Dlg::QuantifyMeasurement_Dlg(QWidget *parent) :
    QDialog(parent,Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint)
{

   setupUi(this);
   this->Btn_Quit->setFocus();
   init();
   connect(this->Btn_Quit,SIGNAL(clicked()),this,SLOT(hide()));
   if(Data::testType==serialTest)
       if(!serialCOMinit())
           hide();
}

QuantifyMeasurement_Dlg::~QuantifyMeasurement_Dlg()
{
    if(Data::testType==serialTest)
    {
        delete p_serialCOM4;
    }
}
void QuantifyMeasurement_Dlg::init()
{
    Sign_GoalInput = 1;
    Sign_LineSave = 0;
    Data::sign_TestCHNum = 7;
    Btn_GoalconInput -> setText( tr( "新建浓度输入" ) );
    choose_Sum_Goalcon(0);
    this->Btn_IsZero->setEnabled(false);
    this->comboBox_Sum_Goalcon->setEnabled(false);
    bsetEnable(false);
}
void QuantifyMeasurement_Dlg::bsetEnable(bool bset)
{
    this->Btn_Measure->setEnabled(bset);
    this->Btn_Save->setEnabled(false);
    this->Btn_MakeLine->setEnabled(false);
    this->comboBox_FormulaMode->setEnabled(false);

}

void QuantifyMeasurement_Dlg::on_Btn_IsZero_clicked()
{
	QTime nntt;
    int j;
    bool ok=true;
    bool b_tmp;
    int tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
    bsetEnable(false);
	this->Btn_IsZero->setEnabled(false);
    Sum_FinishTest = 0;
    Data::SetTime = 5;
    QMessageBox *messageBox=new QMessageBox(QMessageBox::Information,tr("等待"),tr("校零中，请稍后"),QMessageBox::Ok,this);
    messageBox->show();
    if(Data::testType!=serialTest)
    {
        nntt.start();
        while(nntt.elapsed()<2000)
            QCoreApplication::processEvents();
    }
    if(Data::testType==serialTest)
    {
        b_tmp=p_serialCOM4->MultiCMD(settime,CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("警告"),tr("CCD set failed"),tr("确定"));
    }
    ///启动测量
    if(Data::testType==serialTest)       ////serial test mode
    {
        b_tmp = p_serialCOM4->MultiCMD(readinit, CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("警告"),tr("read init failed"),tr("确定"));
    }
    for(j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            if(Sum_FinishTest < Sum_Goalcon)
            {
                 if(j == 0)
                 {
                    label_51->setText(tr( "将样品放入第一通道内" )); //设置Label显示内容
                    lineEdit_Abs1->hide();
                 }
                 else if(j == 1)
                 {
                    label_52->setText(tr( "将样品放入第二通道内" )); //设置Label显示内容
                    lineEdit_Abs2->hide();
                 }
                 else if(j == 2)
                 {
                     label_53->setText(tr( "将样品放入第三通道内" )); //设置Label显示内容
                     lineEdit_Abs3->hide();
                 }
                 Sum_FinishTest++;
            }
            Data::GetWaveLength = Data::WaveLengthMajor[0];
            if(Data::testType==serialTest)
            {
                b_tmp = p_serialCOM4->MultiCMD(readsigwave, j+1);///////////////////////////////////
                if(!b_tmp)
                    QMessageBox::warning(this,tr("警告"),tr("read wave failed"),tr("确定"));
            }
            else
            {
                Data::GetADvalue = 2050;
            }
            Data::BaseMajor[j] = Data::GetADvalue;
            if(Data::Sign_DoubleWave[0] == 1)
            {
                Data::GetWaveLength = Data::WaveLengthMinor[0];
                if(Data::testType==serialTest)
                {
                    b_tmp = p_serialCOM4->MultiCMD(readsigwave, j+1);///////////////////////////////////
                    if(!b_tmp)
                        QMessageBox::warning(this,tr("警告"),tr("read wave failed"),tr("确定"));
                }
                else
                {
                    Data::GetADvalue = 2050;
                }
                Data::BaseMinor[j] = Data::GetADvalue;
            }
        }
        tmpFlag=tmpFlag>>1;
    }
    if(Data::testType==serialTest)
    {
        b_tmp = p_serialCOM4->MultiCMD(lightSW,lightoff);
        if(!b_tmp)
            QMessageBox::warning(this,tr("警告"),tr("light off failed"),tr("确定"));
//        qDebug()<<"light off failed,alarm information!!!";
//    ok &=b_tmp;
    }
    ///启动测量
    if(Data::testType==serialTest)       ////serial test mode
    {
        b_tmp = p_serialCOM4->MultiCMD(readinit, CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("警告"),tr("read init failed"),tr("确定"));
    }
    tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
    for(j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            Data::GetWaveLength = Data::WaveLengthMajor[0];
            if(Data::testType==serialTest)
            {
                b_tmp = p_serialCOM4->MultiCMD(readsigwave, j+1);///////////////////////////////////
                if(!b_tmp)
                    QMessageBox::warning(this,tr("警告"),tr("read wave failed"),tr("确定"));
            }
            else
            {
                Data::GetADvalue = 50;
            }
            Data::DarkMajor[j] = Data::GetADvalue;
            if(Data::Sign_DoubleWave[0] == 1)
            {
                Data::GetWaveLength = Data::WaveLengthMinor[0];
                if(Data::testType==serialTest)
                {
                    b_tmp = p_serialCOM4->MultiCMD(readsigwave, j+1);///////////////////////////////////
                    if(!b_tmp)
                        QMessageBox::warning(this,tr("警告"),tr("read wave failed"),tr("确定"));
                }
                else
                {
                    Data::GetADvalue = 50;
                }
                Data::DarkMinor[j] = Data::GetADvalue;
            }
        }
        tmpFlag=tmpFlag>>1;
    }
    if(Data::testType==serialTest)
    {
        b_tmp = p_serialCOM4->MultiCMD(lightSW,lighton);
        if(!b_tmp)
            QMessageBox::warning(this,tr("警告"),tr("light on failed"),tr("确定"));
    //        qDebug()<<"light on failed,alarm information!!!";
        ok &=b_tmp;
    }
    bsetEnable(true);
    this->Btn_IsZero->setEnabled(true);
    Sum_FinishTest = 0;
    messageBox->close();
    delete messageBox;
}

void QuantifyMeasurement_Dlg::on_Btn_GoalconInput_clicked()
{
    int i,j;
    int tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
    sign_draw = 0;
    update();
    if(Sign_GoalInput == 0)         //完成输入
    {
        Sign_GoalInput = 1;
        Sum_FinishTest = 0;
        this->comboBox_Sum_Goalcon->setEnabled(false);
         this->Btn_IsZero->setEnabled(true);
        for(i=0;i<Sum_Goalcon;i++)
        {
            if(i == 0)
            {
                Num_content[i] = this->lineEdit_GoalCon_1->text().toFloat();
            }
            else if(i == 1)
            {
                Num_content[i] = this->lineEdit_GoalCon_2->text().toFloat();
            }
            else if(i == 2)
            {
                Num_content[i] = this->lineEdit_GoalCon_3->text().toFloat();
            }
            else if(i == 3)
            {
                Num_content[i] = this->lineEdit_GoalCon_4->text().toFloat();
            }
            else if(i == 4)
            {
                Num_content[i] = this->lineEdit_GoalCon_5->text().toFloat();
            }
            else if(i == 5)
            {
                Num_content[i] = this->lineEdit_GoalCon_6->text().toFloat();
            }
        }
        for(j=0;j<3;j++)
        {
            if(tmpFlag&0x01)
            {
                if(Sum_FinishTest < Sum_Goalcon)
                {
                     if(j == 0)
                     {
                        label_51->setText(tr( "将样品放入第一通道内" )); //设置Label显示内容
                        lineEdit_Abs1->hide();
                     }
                     else if(j == 1)
                     {
                        label_52->setText(tr( "将样品放入第二通道内" )); //设置Label显示内容
                        lineEdit_Abs2->hide();
                     }
                     else if(j == 2)
                     {
                         label_53->setText(tr( "将样品放入第三通道内" )); //设置Label显示内容
                         lineEdit_Abs3->hide();
                     }
                     Sum_FinishTest++;
                }          
            }

            tmpFlag=tmpFlag>>1;
        }
        Btn_GoalconInput -> setText( tr( "新建浓度输入" ) );
        choose_Sum_Goalcon(0);

    }
    else if(Sign_GoalInput == 1)    //新建输入
    {
        Sign_GoalInput = 0;
        Sum_FinishTest = 0;
        this->comboBox_Sum_Goalcon->setEnabled(true);
        Btn_GoalconInput -> setText( tr( "完成浓度输入" ) );
        this->comboBox_Sum_Goalcon->setCurrentIndex(0);
        Sum_Goalcon = this->comboBox_Sum_Goalcon->currentText().toInt();
        label_51->setText(tr( "测量结果1" )); //设置Label显示内容
        label_52->setText(tr( "测量结果2" )); //设置Label显示内容
        label_53->setText(tr( "测量结果3" )); //设置Label显示内容
        label_54->setText(tr( "测量结果4" )); //设置Label显示内容
        label_55->setText(tr( "测量结果5" )); //设置Label显示内容
        label_56->setText(tr( "测量结果6" )); //设置Label显示内容
        lineEdit_Abs1->show();
        lineEdit_Abs2->show();
        lineEdit_Abs3->show();
        lineEdit_Abs4->show();
        lineEdit_Abs5->show();
        lineEdit_Abs6->show();
        lineEdit_Abs1->clear();
        lineEdit_Abs2->clear();
        lineEdit_Abs3->clear();
        lineEdit_Abs4->clear();
        lineEdit_Abs5->clear();
        lineEdit_Abs6->clear();
        choose_Sum_Goalcon(Sum_Goalcon);
    }
    Sum_FinishTest = 0;
}
void QuantifyMeasurement_Dlg::on_comboBox_Sum_Goalcon_currentIndexChanged(QString )
{
    Sum_Goalcon = this->comboBox_Sum_Goalcon->currentText().toInt();
    choose_Sum_Goalcon(Sum_Goalcon);
}

void QuantifyMeasurement_Dlg::choose_Sum_Goalcon(int Sign)
{
    if(Sign > 0)
    {
        this->lineEdit_GoalCon_1->clear ();
        this->lineEdit_Abs1->clear();
        this->lineEdit_GoalCon_2->clear ();
        this->lineEdit_Abs2->clear();
        this->lineEdit_GoalCon_3->clear ();
        this->lineEdit_Abs3->clear();
        this->lineEdit_GoalCon_4->clear ();
        this->lineEdit_Abs4->clear();
        this->lineEdit_GoalCon_5->clear ();
        this->lineEdit_Abs5->clear();
        this->lineEdit_GoalCon_6->clear ();
        this->lineEdit_Abs6->clear();

        this->lineEdit_GoalCon_1->setEnabled(true);
        this->label_6->setEnabled(true);
        this->label_51->setEnabled(true);
        this->lineEdit_Abs1->setEnabled(true);
    }
    else
    {
        this->lineEdit_GoalCon_1->setEnabled(false);
        this->label_6->setEnabled(false);
        this->label_51->setEnabled(false);
        this->lineEdit_Abs1->setEnabled(false);
    }

    if(Sign > 1)
    {
        this->lineEdit_GoalCon_2->setEnabled(true);
        this->label_52->setEnabled(true);
        this->label_9->setEnabled(true);
        this->lineEdit_Abs2->setEnabled(true);
    }
    else
    {
        this->lineEdit_GoalCon_2->setEnabled(false);
        this->label_52->setEnabled(false);
        this->label_9->setEnabled(false);
        this->lineEdit_Abs2->setEnabled(false);
    }
    if(Sign > 2)
    {
        this->lineEdit_GoalCon_3->setEnabled(true);
        this->label_53->setEnabled(true);
        this->label_11->setEnabled(true);
        this->lineEdit_Abs3->setEnabled(true);
    }
    else
    {
        this->lineEdit_GoalCon_3->setEnabled(false);
        this->label_53->setEnabled(false);
        this->label_11->setEnabled(false);
        this->lineEdit_Abs3->setEnabled(false);
    }
    if(Sign > 3)
    {
        this->lineEdit_GoalCon_4->setEnabled(true);
        this->label_54->setEnabled(true);
        this->label_13->setEnabled(true);
        this->lineEdit_Abs4->setEnabled(true);
    }
    else
    {
        this->lineEdit_GoalCon_4->setEnabled(false);
        this->label_54->setEnabled(false);
        this->label_13->setEnabled(false);
        this->lineEdit_Abs4->setEnabled(false);
    }
    if(Sign > 4)
    {
        this->lineEdit_GoalCon_5->setEnabled(true);
        this->label_55->setEnabled(true);
        this->label_15->setEnabled(true);
        this->lineEdit_Abs5->setEnabled(true);
    }
    else
    {
        this->lineEdit_GoalCon_5->setEnabled(false);
        this->label_55->setEnabled(false);
        this->label_15->setEnabled(false);
        this->lineEdit_Abs5->setEnabled(false);
    }
    if(Sign > 5)
    {
        this->lineEdit_GoalCon_6->setEnabled(true);
        this->label_56->setEnabled(true);
        this->label_17->setEnabled(true);
        this->lineEdit_Abs6->setEnabled(true);
    }
    else
    {
        this->lineEdit_GoalCon_6->setEnabled(false);
        this->label_56->setEnabled(false);
        this->label_17->setEnabled(false);
        this->lineEdit_Abs6->setEnabled(false);
    }
}

void QuantifyMeasurement_Dlg::on_Btn_Measure_clicked()
{
    int j;
	QTime nntt;
    bool ok=true;
    bool b_tmp;
    int tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
    QMessageBox *messageBox=new QMessageBox(QMessageBox::Information,tr("等待"),tr("测量中，请稍后"),QMessageBox::Ok,this);
    messageBox->show();
    this->Btn_IsZero->setEnabled(false);
    bsetEnable(false);
    if(Data::testType!=serialTest)
    {
        nntt.start();
        while(nntt.elapsed()<2000)
            QCoreApplication::processEvents();
    }				
    ///启动测量
    if(Data::testType==serialTest)       ////serial test mode
    {
        b_tmp = p_serialCOM4->MultiCMD(readinit, CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("警告"),tr("read init failed"),tr("确定"));
    }
    for(j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            if(Sum_FinishTest < Sum_Goalcon)
            {
                Data::GetWaveLength = Data::WaveLengthMajor[0];
                if(Data::testType==serialTest)
                {
                    b_tmp = p_serialCOM4->MultiCMD(readsigwave, j+1);///////////////////////////////////
                    if(!b_tmp)
                        QMessageBox::warning(this,tr("警告"),tr("read wave failed"),tr("确定"));
                }
                else
                {
                    Data::GetADvalue = 1050;
                }
                Data::SampleMajor[j] = Data::GetADvalue;
                AbsMajor[j] =  -log10((float)(Data::SampleMajor[j] - Data::DarkMajor[j])/(float)(Data::BaseMajor[j] - Data::DarkMajor[j]));
                AbsSampleTest[Sum_FinishTest] = AbsMajor[j];
                if(Data::Sign_DoubleWave[0] == 1)
                {
                    Data::GetWaveLength = Data::WaveLengthMinor[0];
                    if(Data::testType==serialTest)
                    {
                        b_tmp = ok;//p_serialCOM4->MultiCMD(readsigwave, j+1);///////////////////////////////////
                        if(!b_tmp)
                            QMessageBox::warning(this,tr("警告"),tr("read wave failed"),tr("确定"));
                    }
                    else
                    {
                        Data::GetADvalue = 1550;
                    }
                    Data::SampleMinor[j] = Data::GetADvalue;
                    AbsMinor[j] =  -log10((float)(Data::SampleMinor[j] - Data::DarkMinor[j])/(float)(Data::BaseMinor[j] - Data::DarkMinor[j]));
                    //qDebug()<< AbsMinor[j];
                    AbsSampleTest[Sum_FinishTest] = AbsMajor[j] - AbsMinor[j];
                }
                if(AbsSampleTest[Sum_FinishTest]<-10)
                {
                    AbsSampleTest[Sum_FinishTest] = -10;
                }
                else if(AbsSampleTest[Sum_FinishTest]>10)
                {
                    AbsSampleTest[Sum_FinishTest] = 10;
                }
                if(Sum_FinishTest == 0)
                {
                    label_51->setText(tr( "测量结果1" )); //设置Label显示内容
                    lineEdit_Abs1->setText(QString::number(AbsSampleTest[Sum_FinishTest],'f',3));
                    lineEdit_Abs1->setEnabled(true);
                    lineEdit_Abs1->show();
                }
                else if(Sum_FinishTest == 1)
                {
                    label_52->setText(tr( "测量结果2" )); //设置Label显示内容
                    lineEdit_Abs2->setText(QString::number(AbsSampleTest[Sum_FinishTest],'f',3));
                    lineEdit_Abs2->setEnabled(true);
                    lineEdit_Abs2->show();
                }
                else if(Sum_FinishTest == 2)
                {
                    label_53->setText(tr( "测量结果3" )); //设置Label显示内容
                    lineEdit_Abs3->setText(QString::number(AbsSampleTest[Sum_FinishTest],'f',3));
                    lineEdit_Abs3->setEnabled(true);
                    lineEdit_Abs3->show();
                }
                else if(Sum_FinishTest == 3)
                {
                    label_54->setText(tr( "测量结果4" )); //设置Label显示内容
                    lineEdit_Abs4->setText(QString::number(AbsSampleTest[Sum_FinishTest],'f',3));
                    lineEdit_Abs4->setEnabled(true);
                    lineEdit_Abs4->show();
                }
                else if(Sum_FinishTest == 4)
                {
                    label_55->setText(tr( "测量结果5" )); //设置Label显示内容
                    lineEdit_Abs5->setText(QString::number(AbsSampleTest[Sum_FinishTest],'f',3));
                    lineEdit_Abs5->setEnabled(true);
                    lineEdit_Abs5->show();
                }
                else if(Sum_FinishTest == 5)
                {
                    label_56->setText(tr( "测量结果6" )); //设置Label显示内容
                    lineEdit_Abs6->setText(QString::number(AbsSampleTest[Sum_FinishTest],'f',3));
                    lineEdit_Abs6->setEnabled(true);
                    lineEdit_Abs6->show();
                }
                Sum_FinishTest++;
             }

        }
        tmpFlag=tmpFlag>>1;
    }
    if(Sum_FinishTest < Sum_Goalcon)
    {
        if(Sum_Goalcon>3)
        {
            label_54->setText(tr( "将样品放入第一通道内" )); //设置Label显示内容
            lineEdit_Abs4->hide();
        }
        if(Sum_Goalcon>4)
        {
            label_55->setText(tr( "将样品放入第二通道内" )); //设置Label显示内容
            lineEdit_Abs5->hide();
        }
        if(Sum_Goalcon>5)
        {
            label_56->setText(tr( "将样品放入第三通道内" )); //设置Label显示内容
            lineEdit_Abs6->hide();
        }
		bsetEnable(true);
    }
    else
    {
        this->Btn_Measure->setEnabled(false);
        this->Btn_MakeLine->setEnabled(true);
        this->comboBox_FormulaMode->setEnabled(true);
    }
    messageBox->close();
    delete messageBox;
}

void QuantifyMeasurement_Dlg::on_Btn_MakeLine_clicked()
{
    int i;
    NiHeMode = this->comboBox_FormulaMode->currentIndex();
    if(Data::testType!=serialTest)
    {
        for(i=0;i<Sum_Goalcon;i++)
        {
            AbsSampleTest[i] = (Num_content[i]-0.050)/20.55;
        }
    }
    if(NiHeMode == 0)
    {
        Curve_Regess1();
    }
    else if(NiHeMode == 1)
    {
        Curve_Regess2();
    }

    this->Btn_Save->setEnabled(true);
    Sign_LineSave = 1;
    StartX = Num_content[0];
    StartY = AbsSampleTest[0];
    EndX = Num_content[0];
    EndY = AbsSampleTest[0];
    for(i=0;i<Sum_Goalcon;i++)
    {
        if(Num_content[i] < StartX)
        {
            StartX = Num_content[i];
        }
        if(AbsSampleTest[i] < StartY)
        {
            StartY = AbsSampleTest[i];
        }
        if(Num_content[i] > EndX)
        {
            EndX = Num_content[i];
        }
        if(AbsSampleTest[i] > EndY)
        {
            EndY = AbsSampleTest[i];
        }
    }
    sign_draw = 1;
    update();
}

void QuantifyMeasurement_Dlg::on_Btn_Save_clicked()
{
    if(Sign_LineSave == 1)
    {
        hide();
    }
}
void QuantifyMeasurement_Dlg::paintEvent(QPaintEvent *event)
{
    float StepX,StepY,tmpX,tmpY,tmpYlast;
    int i,m;
    QPainter painter(this);
    painter.drawLine(60,400,390,400);   //x1,y1,x2,y2坐标轴
    painter.drawLine(385,405,390,400);
    painter.drawLine(385,395,390,400);
    painter.drawLine(60,270,60,400);
    painter.drawLine(60,270,55,275);
    painter.drawLine(60,270,65,275);
    for(i =1;i <5;i++)
    {
        painter.drawLine(60+i*80,400,60+i*80,402);
        painter.drawLine(58,280+(i-1)*30,60,280+(i-1)*30);
    }

    if(sign_draw == 1)
    {
        for(m =0;m <5;m++)
        {
            painter.drawText(60+m*80,415,QString::number(StartX+(EndX - StartX)/4*m,'f',1));
            painter.drawText(15,400-m*30,QString::number(StartY+(EndY - StartY)/4*m,'f',3));
        }
        StepX = 320/(EndX - StartX);
        StepY = 120/(EndY - StartY);
        for(m=0;m<Sum_Goalcon;m++)
        {
            painter.drawEllipse((Num_content[m]-StartX)*StepX+60-2, 400-(AbsSampleTest[m]-StartY)*StepY-2, 4, 4);
        }
        if(NiHeMode == 0)
        {
            painter.drawText(100,280,tr("公式y = ")+QString::number(Num_FormulaB,'f',3)+tr("x+")+QString::number(Num_FormulaA,'f',3));
        }
        else if(NiHeMode == 1)
        {
            painter.drawText(100,280,tr("公式y = ")+QString::number(Num_FormulaC,'f',3)+tr("x*x+")+QString::number(Num_FormulaB,'f',3)+tr("x+")+QString::number(Num_FormulaA,'f',3));
        }

        painter.drawText(100,290,tr("R = ")+QString::number(g_fSampleR,'f',3));

        for(m =0;m <120;m++)
        {
            tmpY = m*(EndY - StartY)/120 + StartY;
            if(NiHeMode == 0)
            {
                tmpX = Num_FormulaB*tmpY + Num_FormulaA;
            }
            else if(NiHeMode == 1)
            {
                tmpX = Num_FormulaC*tmpY*tmpY + Num_FormulaB*tmpY + Num_FormulaA;
            }
            painter.drawPoint((tmpX-StartX)*StepX+60,400-m);
            qDebug()<<(tmpX-StartX)*StepX+60;
            qDebug()<<tmpX;
            tmpYlast = tmpY;
        }
    }

}
//**********************************************************************

//----------  建立一元回归曲线  ----------
void QuantifyMeasurement_Dlg::Curve_Regess1()
{
        int 	i;
        float	Sxx = 0;
        float	Syy = 0;
        float	Sxy = 0;
        float	fConcAver = 0;
        float	fAbsAver = 0;
        float	fTemp1,fTemp2;

        for(i=0;i<Sum_Goalcon;i++)
        {
                fAbsAver+=AbsSampleTest[i];
                fConcAver+=Num_content[i];
        }
        fAbsAver/=Sum_Goalcon;
        fConcAver/=Sum_Goalcon;


        for(i=0;i<Sum_Goalcon;i++)
        {
                fTemp1=AbsSampleTest[i] - fAbsAver;                          //标样吸光度与平均吸光度之差2005-9-8
                fTemp2=Num_content[i] - fConcAver;                        //标样浓度与平均浓度之差2005-9-8
                Syy+=fTemp1*fTemp1;								/* 平方 */                //Syy=吸光度差平方之和    //2005-9-8
                Sxx+=fTemp2*fTemp2;                                         //Sxx=浓度差之和           2005-9-8
                Sxy+=fTemp1*fTemp2;                                         //Sxy=吸光度之差与浓度之差相乘之和2005-9-8
        }
        if(Sxx == 0)Sxx = 0.001;
        if(Sxy == 0)Sxy = 0.001;
        if(Syy == 0)Syy = 0.001;
        Num_FormulaB = Sxx/Sxy;                                         //一次相系数2005-9-8
        Num_FormulaA = fConcAver - Num_FormulaB*fAbsAver;                 //截距2005-9-8      //fConcAver=Num_FormulaA +Num_FormulaB*fAbsAver
        g_fSampleR = Sxy/sqrt((Sxx*Syy));                             //相关系数2005-9-8
//	g_fSampleR = (Sxy*Sxy)/(Sxx*Syy);
}

/////////////////////二次拟合曲线/////////////////////////////////
void QuantifyMeasurement_Dlg::Curve_Regess2()
{

  int m=3;                                                     //拟合多项式的项数,最高项为m-1
        int i, j, k;
        float z, p, c, g, q, d1, d2,S,U,y,fConcAver, fAbsAver,s[3], t[3], b[3],a[3];

        for(i=0;i<=m-1;i++)
        {
                a[i]=0.0;
        }                                                            //初始化，各系数均为0
        if(m>Sum_Goalcon)                                          //忽略m>n的情况
  {
        m=Sum_Goalcon;
        }
        if(m>20)                                                     //忽略m>20的情况
  {
        m=20;
        }
        z=0.0;
        for(i=0;i<Sum_Goalcon;i++)
        {
        z=z+AbsSampleTest[i]/(1.0 *Sum_Goalcon);                  // z=xavr
        }
        b[0]=1.0;
        d1=1.0*Sum_Goalcon;
        p=0.0;
        c=0.0;
        for(i=0;i<Sum_Goalcon;i++)
        {
                p=p+(AbsSampleTest[i]-z);                                   //p=(x[i]-xavr)之和， x[i]=AbsSampleTest[i]
                c=c+Num_content[i];                                      //c=y[i]之和，y[i]=Num_content[i]
        }
        c=c/d1;                                                      //  yavr
        p=p/d1;                                                      // (x[i]-xavr)之和/n
        a[0]=c*b[0];                                                 //一项时候的a[0]
        if(m>1)                                                      //如果大于一项，一次回归曲线
        {
                t[1]=1.0;
                t[0]=-p;
                d2=0.0;
                c=0.0;
                g=0.0;
                for(i=0;i<Sum_Goalcon;i++)
                {
                        q=AbsSampleTest[i]-z-p;                                    //xavr-x[i]之和
                        d2=d2+q*q;                                                //(x[i]-xavr)的平方之和
                        c=c+Num_content[i] *q;                                  //c=y[i]*(x-xavr)之和
                        g=g+(AbsSampleTest[i]-z)*q*q;
                }
                c=c/d2;
                p=g/d2;
                q=d2/d1;
                d1=d2;
                a[1]=c*t[1];                                                 //两项时候的a[1]
                a[0]=c*t[0]+a[0];                                            //两项时候的a[0]
        }
        for(j=2;j<=m-1;j++)                                            //二次以上回归曲线
        {
                s[j]=t[j-1];
                s[j-1]=-p*t[j-1]+t[j-2];
                if(j >= 3)                                                   //大于三项以上用
                for(k=j-2;k>=1;k--)
            {
                s[k]=-p*t[k]+t[k-1]-q*b[k];
            }
                s[0]=-p*t[0]-q*b[0];
                d2=0.0;
                c=0.0;
                g=0.0;
                for(i=0;i<Sum_Goalcon;i++)
                {
                        q=s[j];
                        for(k=j-1;k>=0;k--)
                        {
                                q=q*(AbsSampleTest[i]-z)+s[k];
                        }
                        d2=d2+q*q;
                        c=c+Num_content[i]*q;
                        g=g+(AbsSampleTest[i]-z)*q*q;
                }
                c=c/d2;
                p=g/d2;
                q=d2/d1;
                d1=d2;
                a[j]=c*s[j];                                                 //三项时候a[2]
                t[j]=s[j];
                for(k=j-1;k>= 0;k--)
                {
                        a[k]=c*s[k]+a[k];                                           //三项时候的a[1]、a[0]
                        b[k]=t[k];
                        t[k]=s[k];
                }
        }
                  fConcAver=0;
            fAbsAver=0;
          for(i=0;i<Sum_Goalcon;i++)
    {
      fAbsAver=fAbsAver+AbsSampleTest[i];
      fConcAver=fConcAver+Num_content[i];
    }
    fAbsAver=fAbsAver/Sum_Goalcon;
    fConcAver=fConcAver/Sum_Goalcon;
    Num_FormulaA=a[0]-a[1]*fAbsAver+a[2]*fAbsAver*fAbsAver;          //截距
    Num_FormulaB=a[1]-2*a[2]*fAbsAver;                               //一次系数
    Num_FormulaC=a[2];                                               //二次系数

       S=0;
       U=0;
          for(i=0;i<Sum_Goalcon;i++)
          {
          //  y=Num_FormulaA+Num_FormulaB*AbsSampleTest[i]+Num_FormulaC*AbsSampleTest[i]*AbsSampleTest[i];                       //y=b0+b1x+b2x*x
                y=a[0]+a[1]*(AbsSampleTest[i]-fAbsAver)+a[2]*(AbsSampleTest[i]-fAbsAver)*(AbsSampleTest[i]-fAbsAver);     //y=a0+a1(x-xavr)+a2(x-xavr)*(x-xavr)
                S=S+(Num_content[i]-fConcAver)*(Num_content[i]-fConcAver);
            U=U+(y-fConcAver)*(y-fConcAver);
                }
         //   if(U == 0)U = 0.001;                                           //2005-9-15
         //   if(S == 0)S = 0.001;
      g_fSampleR =sqrt(U/S);                                             //相关系数r


}
bool QuantifyMeasurement_Dlg::serialCOMinit()
{
    bool ok=true;
    bool b_tmp;
    p_serialCOM4 =new serialCOM;
    ////connect
    b_tmp=p_serialCOM4->MultiCMD(online,CHEN123);
    ok &=b_tmp;
    if(!b_tmp)
    {
        QMessageBox::warning(this,tr("警告"),tr("on line failed"),tr("确定"));
        return ok;
    }
    b_tmp=p_serialCOM4->MultiCMD(readwave,CHEN123);
    ok &=b_tmp;
    if(!b_tmp)
    {
        QMessageBox::warning(this,tr("警告"),tr("read wave failed"),tr("确定"));
        return ok;
    }
    return ok;
}
