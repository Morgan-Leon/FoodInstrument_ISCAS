#include "quantifyinfoinput_dlg.h"

quantifyinfoinput_dlg::quantifyinfoinput_dlg(QWidget *parent) :
    QDialog(parent)
{

    setupUi(this);
    this->Btn_Quit->setFocus();
    init();
//    this->setWindowFlags(Qt::FramelessWindowHint);
    QRegExp RX_SpecNM("^(([4-7][0-9]{2}|[4-7][0-9]{2}[\.][0-9]|[8][0]{2}))$");       //400-800
    Edit_MajorWave->setValidator(new QRegExpValidator(RX_SpecNM,this));
    Edit_MinorWave->setValidator(new QRegExpValidator(RX_SpecNM,this));
}
quantifyinfoinput_dlg::~quantifyinfoinput_dlg()
{
    ;
}
void quantifyinfoinput_dlg::init()
{
    qDebug()<<Data::DLoadsign;
    if(Data::DLoadsign == 1)
    {
        this->comboBox_WaveMode->setCurrentIndex(Data::Sign_DoubleWave[0]);
        this->Edit_MajorWave->setText(QString::number(Data::WaveLengthMajor[0],'f',1));         //系数A
        this->Edit_MinorWave->setText(QString::number(Data::WaveLengthMinor[0],'f',1));         //系数A
        this->Btn_Del->setEnabled(true);
        this->Btn_Del->setVisible(true);
    }
    else
    {
        QuantifyTestName = tr("");
        DUint_Name = tr("");
        this->comboBox_WaveMode->setCurrentIndex(0);
        this->Edit_MajorWave->setText(QString::number(500,'f',1));         //系数A
        this->Edit_MinorWave->setText(QString::number(0,'f',1));         //系数A
        Num_FormulaA = 0;
        Num_FormulaB = 1;
        Num_FormulaC = 0;
        this->Btn_Del->setEnabled(false);
        this->Btn_Del->setVisible(false);
    }
    if(Data::Sign_DoubleWave[0] == 0)
    {
        this->Edit_MinorWave->setEnabled(false);
        this->Edit_MinorWave->setVisible(false);
    }
    else if(Data::Sign_DoubleWave[0] == 1)
    {
        this->Edit_MinorWave->setEnabled(true);
        this->Edit_MinorWave->setVisible(true);
    }

    this->Edit_TestName->setText(QuantifyTestName);         //系数A
    this->Edit_UnitName->setText(DUint_Name);         //系数A
    this->Edit_FormulaA->setText(QString::number(Num_FormulaA,'f',3));         //系数A
    this->Edit_FormulaB->setText(QString::number(Num_FormulaB,'f',3));         //系数A
    this->Edit_FormulaC->setText(QString::number(Num_FormulaC,'f',3));         //系数A

}
void quantifyinfoinput_dlg::on_Btn_Save_clicked()
{
    int j;
    bool ok = true;
    Sign_Save = 1;
    QuantifyTestName = this->Edit_TestName->text();
    DUint_Name = this->Edit_UnitName->text();
    for(j=0;j<3;j++)
    {
        Data::Sign_DoubleWave[j] = this->comboBox_WaveMode->currentIndex();
        Data::WaveLengthMajor[j] = this->Edit_MajorWave->text().toFloat();
        Data::WaveLengthMinor[j] = this->Edit_MinorWave->text().toFloat();
    }

    if(QuantifyTestName==tr(""))
    {
        QMessageBox::warning(this,tr("警告"),tr("检测项目名称不能为空"),tr("确定"));
        ok = false;
    }
    if((Data::WaveLengthMajor[0]==0)||((Data::WaveLengthMinor[0]==0)&&(Data::Sign_DoubleWave[0]==1)))
    {
        QMessageBox::warning(this,tr("警告"),tr("波长输入信息错误"),tr("确定"));
        ok = false;
    }
    if(ok == true)
    {
        Num_FormulaA = this->Edit_FormulaA->text().toFloat();         //系数A
        Num_FormulaB = this->Edit_FormulaB->text().toFloat(); ;         //系数B
        Num_FormulaC = this->Edit_FormulaC->text().toFloat(); ;         //系数C
        //--------------------------------------将新建项目信息保存至自定义方法库内
        qDebug()<< Num_FormulaA;
        qDebug()<< Num_FormulaB;
        qDebug()<< Num_FormulaC;
        hide();
    }
}

void quantifyinfoinput_dlg::on_Btn_Quit_clicked()
{
    Sign_Save = 0;
    hide();
}


void quantifyinfoinput_dlg::on_Btn_NewTestLine_clicked()
{
    int j;
    for(j=0;j<3;j++)
    {
        Data::Sign_DoubleWave[j] = this->comboBox_WaveMode->currentIndex();
        Data::WaveLengthMajor[j] = this->Edit_MajorWave->text().toFloat();
        Data::WaveLengthMinor[j] = this->Edit_MinorWave->text().toFloat();
    }
    if((Data::WaveLengthMajor[0]==0)||((Data::WaveLengthMinor[0]==0)&&(Data::Sign_DoubleWave[0]==1)))
    {
        QMessageBox::warning(this,tr("警告"),tr("波长输入信息错误"),tr("确定"));
    }
    else
    {
        p_quantifymeasurement = new QuantifyMeasurement_Dlg;
        p_quantifymeasurement->show();
        p_quantifymeasurement->exec();
        if(p_quantifymeasurement->Sign_LineSave == 1)
        {
            Num_FormulaA = p_quantifymeasurement->Num_FormulaA;
            this->Edit_FormulaA->setText(QString::number(Num_FormulaA,'f',3));         //系数A
            Num_FormulaB = p_quantifymeasurement->Num_FormulaB;
            this->Edit_FormulaB->setText(QString::number(Num_FormulaB,'f',3));         //系数A
            Num_FormulaC = p_quantifymeasurement->Num_FormulaC;
            this->Edit_FormulaC->setText(QString::number(Num_FormulaC,'f',3));         //系数A
        }
    }
}

void quantifyinfoinput_dlg::on_Btn_Del_clicked()
{
    Sign_Save = 2;//删除标志
    hide();
}

void quantifyinfoinput_dlg::on_comboBox_WaveMode_currentIndexChanged(int index)
{
    int j;
    for(j=0;j<3;j++)
    {
        Data::Sign_DoubleWave[j] = this->comboBox_WaveMode->currentIndex();
    }
    if(Data::Sign_DoubleWave[0] == 0)
    {
        this->Edit_MinorWave->setEnabled(false);
        this->Edit_MinorWave->setVisible(false);
    }
    else if(Data::Sign_DoubleWave[0] == 1)
    {
        this->Edit_MinorWave->setEnabled(true);
        this->Edit_MinorWave->setVisible(true);
    }
}
