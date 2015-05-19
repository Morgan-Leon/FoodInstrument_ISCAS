#include "timesweepsrt_dlg.h"
timesweepsrt_dlg::timesweepsrt_dlg(QWidget *parent) :
    QDialog(parent)
{

    setupUi(this);
    this->Btn_Quit->setFocus();
    this->Box_SpeMode->setCurrentIndex(0);
    setMyValidator(this->Box_SpeMode->currentIndex());
    if(fileread(this->Box_SpeMode->currentIndex()))
    {
        SettingPara.SpecMode=this->Box_SpeMode->currentIndex();
        SettingPara.TimeScanWL=this->Edit_SpeWave->text().toFloat();
        SettingPara.TimeScanAllTime=this->Edit_SpeSumTime->text().toFloat();
        //SettingPara.TimeStep=this->Edit_SpeStep->text().toFloat();
        SettingPara.MaxYdata=this->Edit_YMax->text().toFloat();
        SettingPara.MinYdata=this->Edit_YMin->text().toFloat();
        SettingPara.CCDTime=this->Edit_CCDtime->text().toFloat();
        SettingPara.AverageS=this->Edit_AverageS->text().toInt();
    }
    else
        QMessageBox::warning(this,tr("警告"),tr("文件操作错误！"),tr("确定"));
}

timesweepsrt_dlg::~timesweepsrt_dlg()
{
    ;
}
void timesweepsrt_dlg::on_Box_SpeMode_currentIndexChanged(int index)
{
    setMyValidator(index);
    if(!fileread(index))
        QMessageBox::warning(this,tr("警告"),tr("文件操作错误！"),tr("确定"));
}
void timesweepsrt_dlg::setMyValidator(int modetype)
{
    QRegExp RX_SpecNM("^(([4-7][0-9]{2}|[4-7][0-9]{2}[\.][0-9]|[8][0]{2}))$");       //400--800:0.1
    QRegExp RX_Time("^(([1][0-9]{1,3}|[2-9][0-9]{1,2}|[2][0-9]{3}|[3][0-6][0-9]{2}))$");      //10--3600:1
    QRegExp RX_Y0("^(([0]|[5][0]{3}|[1-4][0-9]{3}|[1-9][0-9]{2}|[1-4][0-9]{3}[\.][0-9]|[1-9][0-9]{2}[\.][0-9]))$");    //0--5000:0.1
    QRegExp RX_Y1("^(([0]|[-]?[5]|[-]?[1-4]|[-]?[0-4][\.][0-9]{3}))$");    //-5--5 0.001
    QRegExp RX_Y2("^(([0]|[-]?[0-9][\.][0-9]|[-]?[1][0-9][\.][0-9]|[-][2][0]|[2-9][0-9][\.][0-9]|[1][0-1][0-9][\.][0-9]|[1][2][0][\.][0]))$");   //-20--120 0.1
    QRegExp RX_CCDTime("^(([1]|[0][\.][0]{2}[1-9]|[0][\.][0-9][1-9][0-9]|[0][\.][1-9][0-9]{2}))$");       //0.001-1
    QRegExp RX_AverageS("^(([1-9]|[1][0-6]))$"); // 1-16

    Edit_SpeWave->setValidator(new QRegExpValidator(RX_SpecNM,this));
    Edit_SpeSumTime->setValidator(new QRegExpValidator(RX_Time,this));
    Edit_CCDtime->setValidator(new QRegExpValidator(RX_CCDTime,this));
    Edit_AverageS->setValidator(new QRegExpValidator(RX_AverageS,this));
    switch(modetype)
    {
    case EnergyTime:
        Edit_YMax->setValidator(new QRegExpValidator(RX_Y0,this));
        Edit_YMin->setValidator(new QRegExpValidator(RX_Y0,this));
        Edit_YMax->setToolTip(tr("输入范围0至5000"));
        Edit_YMin->setToolTip(tr("输入范围0至5000"));
        label_3->setText(tr("0 -- 5000"));
        label_27->setText(tr("0 -- 5000"));
        break;
    case AbsorbTime:
        Edit_YMax->setValidator(new QRegExpValidator(RX_Y1,this));
        Edit_YMin->setValidator(new QRegExpValidator(RX_Y1,this));
        Edit_YMax->setToolTip(tr("输入范围-5至5"));
        Edit_YMin->setToolTip(tr("输入范围-5至5"));
        label_3->setText(tr("-5 -- 5"));
        label_27->setText(tr("-5 -- 5"));
        break;
    case TransmitTime:
        Edit_YMax->setValidator(new QRegExpValidator(RX_Y2,this));
        Edit_YMin->setValidator(new QRegExpValidator(RX_Y2,this));
        Edit_YMax->setToolTip(tr("输入范围-20至120"));
        Edit_YMin->setToolTip(tr("输入范围-20至120"));
        label_3->setText(tr("-20 -- 120"));
        label_27->setText(tr("-20 -- 120"));
        break;
    default:
        qDebug()<<"modetype error!";
        break;
    }
}

bool timesweepsrt_dlg::fileread(int modetype)
{
    QString str_tmpdir;
    QDir dir_tmp;
    str_tmpdir=dir_tmp.currentPath();
    dir_tmp.setCurrent(str_tmpdir+"/config");
    QFile file("timesweep.txt");
    QRegExp regExp("([A-Za-z]*)([_])([0-2])([=])([-]?[0-9]+|[-]?[0-9]+[\.][0-9]+)");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream txtout_stream(&file);
        while(!txtout_stream.atEnd())
        {
            if(regExp.indexIn(txtout_stream.readLine())>-1 && regExp.cap(3).toInt()==modetype)
            {
                if(regExp.cap(1)=="TimeScanWL")
                {
                    this->Edit_SpeWave->setText(QString::number(regExp.cap(5).toFloat()));
                }
                else if(regExp.cap(1)=="TimeScanAllTime")
                    this->Edit_SpeSumTime->setText(QString::number(regExp.cap(5).toFloat()));
      //          else if(regExp.cap(1)=="TimeStep")
      //              this->Edit_SpeStep->setText(QString::number(regExp.cap(5).toFloat()));
                else if(regExp.cap(1)=="MaxYdata")
                    this->Edit_YMax->setText(QString::number(regExp.cap(5).toFloat()));
                else if(regExp.cap(1)=="MinYdata")
                    this->Edit_YMin->setText(QString::number(regExp.cap(5).toFloat()));
                else if(regExp.cap(1)=="CCDTime")
                    this->Edit_CCDtime->setText(QString::number(regExp.cap(5).toFloat()));
                else if(regExp.cap(1)=="AverageS")
                    this->Edit_AverageS->setText(QString::number(regExp.cap(5).toInt(),10));
            }
        }
    }
    else
    {
        qDebug()<<"can not open!!";
        return false;
    }
    file.close();
    dir_tmp.setCurrent(str_tmpdir);
    return true;
}

bool timesweepsrt_dlg::filewrite(int modetype)
{
    QRegExp regExp("([A-Za-z]*)([_])([0-2])([=])([-]?[0-9]+|[-]?[0-9]+[\.][0-9]+)");
    QStringList lines;
    QString str_tmpdir;
    QDir dir_tmp;
    str_tmpdir=dir_tmp.currentPath();
    dir_tmp.setCurrent(str_tmpdir+"/config");
    lines.clear();
    QFile file("timesweep.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream txtout_stream(&file);
        while(!txtout_stream.atEnd())
        {
            lines <<txtout_stream.readLine();
        }
    }
    else
    {
        qDebug()<<"can not open!!";
        return false;
    }
    file.close();
    file.remove();
    //begin
    QFile file2("timesweep.txt");
    if ( file2.open(QIODevice::WriteOnly))
    {
        QTextStream txtin_stream(&file2);

        for(int i=0;i<lines.size();i++)
        {
            if(regExp.indexIn(lines.at(i))>-1)
            {
                if(regExp.cap(3).toInt()==modetype)
                {
                    if(regExp.cap(1)=="TimeScanWL")
                        txtin_stream<<"TimeScanWL"<<"_"<<modetype<<"="<<this->Edit_SpeWave->text()<<"\n";
                    else if(regExp.cap(1)=="TimeScanAllTime")
                        txtin_stream<<"TimeScanAllTime"<<"_"<<modetype<<"="<<this->Edit_SpeSumTime->text()<<"\n";
              //      else if(regExp.cap(1)=="TimeStep")
              //          txtin_stream<<"TimeStep"<<"_"<<modetype<<"="<<this->Edit_SpeStep->text()<<"\n";
                    else if(regExp.cap(1)=="MaxYdata")
                        txtin_stream<<"MaxYdata"<<"_"<<modetype<<"="<<this->Edit_YMax->text()<<"\n";
                    else if(regExp.cap(1)=="MinYdata")
                        txtin_stream<<"MinYdata"<<"_"<<modetype<<"="<<this->Edit_YMin->text()<<"\n";
                    else if(regExp.cap(1)=="CCDTime")
                        txtin_stream<<"CCDTime"<<"_"<<modetype<<"="<<this->Edit_CCDtime->text()<<"\n";
                    else if(regExp.cap(1)=="AverageS")
                        txtin_stream<<"AverageS"<<"_"<<modetype<<"="<<this->Edit_AverageS->text()<<"\n";
                }
                else
                {
                  txtin_stream<<lines.at(i)<<"\n";
                }
            }
        }
    }
    else
    {
        qDebug()<<"can not open!!";
        return false;
    }
    file.close();
    dir_tmp.setCurrent(str_tmpdir);
    //
    SettingPara.SpecMode=modetype;
    SettingPara.TimeScanWL=this->Edit_SpeWave->text().toFloat();
    SettingPara.TimeScanAllTime=this->Edit_SpeSumTime->text().toFloat();
    //SettingPara.TimeStep=this->Edit_SpeStep->text().toFloat();
    SettingPara.MaxYdata=this->Edit_YMax->text().toFloat();
    SettingPara.MinYdata=this->Edit_YMin->text().toFloat();
    SettingPara.CCDTime=this->Edit_CCDtime->text().toFloat();
    SettingPara.AverageS=this->Edit_AverageS->text().toInt();
    return true;
}
void timesweepsrt_dlg::on_Btn_Save_clicked()
{
    if(testIput())
    {
        if(filewrite(this->Box_SpeMode->currentIndex()))
        {
            hide();
            qDebug()<<"***emit***";
            emit transStrToDlg(SettingPara);
        }
        else
            QMessageBox::warning(this,tr("警告"),tr("文件操作错误！"),tr("确定"));
    }
}

void timesweepsrt_dlg::on_Btn_Quit_clicked()
{
    if(SettingPara.SpecMode!=this->Box_SpeMode->currentIndex())
    {
        int r = QMessageBox::warning(this, tr("参数设置"),
                                     tr("已修改参数设置\n"
                                        "是否保存？"),
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (r == QMessageBox::Yes)
        {
           on_Btn_Save_clicked();
        }
        else if (r == QMessageBox::No)
        {
            hide();
            this->Box_SpeMode->setCurrentIndex(SettingPara.SpecMode);
            this->Edit_SpeWave->setText(QString::number(SettingPara.TimeScanWL));
            this->Edit_SpeSumTime->setText(QString::number(SettingPara.TimeScanAllTime));
            //this->Edit_SpeStep->setText(QString::number(SettingPara.TimeStep));
            this->Edit_YMax->setText(QString::number(SettingPara.MaxYdata));
            this->Edit_YMin->setText(QString::number(SettingPara.MinYdata));
            this->Edit_CCDtime->setText(QString::number(SettingPara.CCDTime));
            this->Edit_AverageS->setText(QString::number(SettingPara.AverageS,10));
        }
        else
        {
            this->Box_SpeMode->setFocus();
        }
    }
    else
    {
        hide();
    }
}

bool timesweepsrt_dlg::testIput()
{
    if(this->Edit_SpeWave->text().toInt()<400)
    {
        QMessageBox::warning(this,tr("警告"),tr("请输入有效的波长！"),tr("确定"));
        this->Edit_SpeWave->setFocus();
        return false;
    }
    else if(this->Edit_YMax->text().toFloat()<=this->Edit_YMin->text().toFloat())
    {
        QMessageBox::warning(this,tr("警告"),tr("请输入有效的Y轴最大、最小值！"),tr("确定"));
        this->Edit_YMax->setFocus();
        return false;
    }
    else if(this->Edit_CCDtime->text().toFloat()<0.001)
    {
        QMessageBox::warning(this,tr("警告"),tr("请输入有效的积分时间！"),tr("确定"));
        this->Edit_CCDtime->setFocus();
        return false;
    }
    else
        return true;
}

