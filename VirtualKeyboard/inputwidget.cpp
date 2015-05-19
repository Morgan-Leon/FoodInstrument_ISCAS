#include "inputwidget.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QtGui>
#include <QWSServer>
#include <QWSInputMethod>
#include <QSignalMapper>
#include <QWSInputMethod>
#include <QtSql>
#include <QTextCodec>
#include <QPalette>
#include "database.h"

inputWidget::inputWidget(QWSInputMethod* im) :
    QWidget(0
       , Qt::Tool| Qt::WindowStaysOnTopHint
       | Qt::FramelessWindowHint)
{
    setFocusPolicy(Qt::NoFocus);
//    connectDatabase();
    db = QSqlDatabase::addDatabase("QSQLITE","db");
    db.setDatabaseName("data.db");
    db.open();
    setupUi(this);
    //
    QPalette palette;
    palette.setColor(QPalette::Background,QColor(220,220,220));//Qt::darkGray
    setPalette(palette);
    //init
    displayNumBegin =0;
    displayNumEnd =0;    
    isCapsLock = this->capslockBtn->isChecked();
    isSign = this->signBtn->isChecked();
    isSign2 = this->sign2Btn->isChecked();
    isEnglish = !this->ecBtn->isChecked();
    isPinyin = !this->pwBtn->isChecked();
    this->pwBtn->setDisabled(true);
    this->lineEditInput->setVisible(false);
    this->labelInput->setVisible(false);
    changeTextCaps(false);
    this->lhzBtnBack->setDisabled(true);
    this->hzBtnPre->setDisabled(true);    

    int x = (QApplication::desktop()->width()-this->width())/2;
    int y = QApplication::desktop()->height()-this->height();
    this->move(x,y);
    mapper = new QSignalMapper(this);
    allButtons = findChildren<QPushButton *>();
    for (int i=0;i<allButtons.count();i++) {
        connect(allButtons.at(i), SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(allButtons.at(i), i);
    }
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(btn_clicked(int)));
    connect(this,SIGNAL(sendString(const QString)),im,SLOT(confirmContent(const QString)));
    connect(this,SIGNAL(sendHide()),im,SLOT(hideInput()));
    model = new QSqlTableModel(this,db);
}

void inputWidget::btn_clicked(int btn)
{
    QString strKeyId;
    QString ch;
    strKeyId = allButtons.at(btn)->accessibleName();    
    bool isOk;
    int keyId = strKeyId.toInt(&isOk, 16);
    if(checkNotTextKey(keyId)){
        if(keyId == Qt::Key_Enter)
            emit sendHide();
        else if(keyId == Qt::Key_Backspace)
        {
            if(!isEnglish && !(this->lineEditInput->text().isEmpty()))
            {
                if(isPinyin)
                {
                    pinyinStr.chop(1);
                    this->lineEditInput->setText(pinyinStr);                    
                }
                else
                {                    
                    wubinStr.chop(1);
                    this->lineEditInput->setText(wubinStr);
                }
            }
            else
                QWSServer::sendKeyEvent(0,Qt::Key_Backspace,Qt::NoModifier,true,false);
        }
        else if(keyId == Qt::Key_Return)              
            emit sendHide();
        else if(keyId == Qt::Key_Delete)
            QWSServer::sendKeyEvent(0,Qt::Key_Delete,Qt::NoModifier,true,false);
        else if(keyId == Qt::Key_Left)
            QWSServer::sendKeyEvent(0,Qt::Key_Left,Qt::NoModifier,true,false);
        else if(keyId == Qt::Key_Right)
            QWSServer::sendKeyEvent(0,Qt::Key_Right,Qt::NoModifier,true,false);
        else if(keyId == Qt::Key_PageUp)
        {
            if(!isEnglish)
                changeDisplay(isEnglish);
        }
        else if(keyId == Qt::Key_PageDown)
        {
            if(!isEnglish)
                changeDisplay(!isEnglish);
        }
        return;
    }
    if(keyId == Qt::Key_Space)
        ch = " ";
    else
        ch = allButtons.at(btn)->text().trimmed();

    if(!isEnglish)
    {
        if(checkNUMKey(keyId))
        {
            int tmpNum=keyId-48;
            if(!(tmpNum>displayNumEnd-displayNumBegin || tmpNum==0))
            {
                tmpNum=tmpNum+displayNumBegin-1;
                if(isPinyin)
                {
//                    QSqlTableModel model(this->db);
                    model->setTable("pinyin");
                    model->setFilter(QString("py like '%1'").arg(pinyinStr+"%"));
                    model->setSort(0,Qt::AscendingOrder);
                    model->select();
                    chineseStr=model->record(tmpNum).value("chn").toString();
                }
                else
                {
//                    QSqlTableModel model(this->db);
                    model->setTable("wubi");
                    model->setFilter(QString("wb like '%1'").arg(wubinStr+"%"));
                    model->setSort(0,Qt::AscendingOrder);
                    model->select();
                    chineseStr=model->record(tmpNum).value("chn").toString();
                }
                this->hzBtnPre->setDisabled(true);
                this->lhzBtnBack->setDisabled(true);
                this->lineEditInput->setText("");
                this->labelInput->setText("");
                displayNumBegin =0;
                displayNumEnd =0;
                emit sendString(chineseStr);
            }
        }
        else
            this->lineEditInput->setText(this->lineEditInput->text().trimmed()+ch);        
    }
    else
        emit sendString(ch);
}

void inputWidget::on_lineEditInput_textChanged(QString )
{
    QString tmp1;
    QString tmp2;
    if(this->lineEditInput->text().isEmpty())
    {
        this->lhzBtnBack->setDisabled(true);
        this->hzBtnPre->setDisabled(true);
        this->labelInput->setText("");
        displayNumBegin =0;
        displayNumEnd =0;        
        return;
    }
    if(isPinyin)
    {
        pinyinStr=this->lineEditInput->text().trimmed();
//        QSqlTableModel model(this->db);
        model->setTable("pinyin");
        model->setFilter(QString("py like '%1'").arg(pinyinStr+"%"));
        model->setSort(0,Qt::AscendingOrder);
        model->select();
        if(model->rowCount()>DISPLAYNUM)
        {
            displayNumEnd=DISPLAYNUM;
            this->hzBtnPre->setDisabled(false);
        }
        else if (model->rowCount()>0)
        {
            displayNumEnd=model->rowCount();
            this->hzBtnPre->setDisabled(true);
        }
        else
        {
            this->lhzBtnBack->setDisabled(true);
            this->hzBtnPre->setDisabled(true);
            displayNumBegin =0;
            displayNumEnd =0;
        }
        this->labelInput->setText("");
        for (int i = 0; i < displayNumEnd; ++i)
        {
            tmp1=this->labelInput->text();
            tmp2=QString("%1.").arg(i+1)+model->record(i).value("chn").toString()+" ";
            this->labelInput->setText(tmp1+tmp2);
        }
    }
    else
    {
        wubinStr=this->lineEditInput->text().trimmed();
//        QSqlTableModel model(this->db);
        model->setTable("wubi");
        model->setFilter(QString("wb like '%1'").arg(wubinStr+"%"));
        model->setSort(0,Qt::AscendingOrder);
        model->select();
        if(model->rowCount()>DISPLAYNUM)
        {
            displayNumEnd=DISPLAYNUM;
            this->hzBtnPre->setDisabled(false);
        }
        else if (model->rowCount()>0)
        {
            displayNumEnd=model->rowCount();
            this->hzBtnPre->setDisabled(true);
        }
        else
        {
            this->lhzBtnBack->setDisabled(true);
            this->hzBtnPre->setDisabled(true);
            displayNumBegin =0;
            displayNumEnd =0;
        }
        this->labelInput->setText("");
        for (int i = 0; i < displayNumEnd; ++i)
        {
            tmp1=this->labelInput->text();
            tmp2=QString("%1.").arg(i+1)+model->record(i).value("chn").toString()+" ";
            this->labelInput->setText(tmp1+tmp2);
        }
    }
}

void inputWidget::changeDisplay(bool isPre)
{
    QString tmp3;
    QString tmp4;
    if(isPre)
    {
        if(isPinyin)
        {
//            QSqlTableModel model(this->db);
            model->setTable("pinyin");
            model->setFilter(QString("py like '%1'").arg(pinyinStr+"%"));
            model->setSort(0,Qt::AscendingOrder);
            model->select();
            if(model->rowCount()-displayNumEnd>DISPLAYNUM)
            {
                displayNumBegin=displayNumEnd;
                displayNumEnd=displayNumBegin+DISPLAYNUM;
            }
            else
            {
                displayNumBegin=displayNumEnd;
                displayNumEnd=model->rowCount();
                this->hzBtnPre->setDisabled(true);
            }
            this->labelInput->setText("");
            for (int i = displayNumBegin; i < displayNumEnd; ++i)
            {
                tmp3=this->labelInput->text();
                tmp4=QString("%1.").arg(i-displayNumBegin+1)+model->record(i).value("chn").toString()+" ";
                this->labelInput->setText(tmp3+tmp4);
            }
            this->lhzBtnBack->setDisabled(false);
        }
        else
        {
//            QSqlTableModel model(this->db);
            model->setTable("wubi");
            model->setFilter(QString("wb like '%1'").arg(wubinStr+"%"));
            model->setSort(0,Qt::AscendingOrder);
            model->select();
            if(model->rowCount()-displayNumEnd>DISPLAYNUM)
            {
                displayNumBegin=displayNumEnd;
                displayNumEnd=displayNumBegin+DISPLAYNUM;
            }
            else
            {
                displayNumBegin=displayNumEnd;
                displayNumEnd=model->rowCount();
                this->hzBtnPre->setDisabled(true);
            }
            this->labelInput->setText("");
            for (int i = displayNumBegin; i < displayNumEnd; ++i)
            {
                tmp3=this->labelInput->text();
                tmp4=QString("%1.").arg(i-displayNumBegin+1)+model->record(i).value("chn").toString()+" ";
                this->labelInput->setText(tmp3+tmp4);
            }
            this->lhzBtnBack->setDisabled(false);
        }
    }
    else
    {
        if(isPinyin)
        {
//            QSqlTableModel model(this->db);
            model->setTable("pinyin");
            model->setFilter(QString("py like '%1'").arg(pinyinStr+"%"));
            model->setSort(0,Qt::AscendingOrder);
            model->select();
            if (displayNumBegin>DISPLAYNUM)
            {
                displayNumEnd=displayNumBegin;
                displayNumBegin=displayNumBegin-DISPLAYNUM;
            }
            else
            {
                displayNumEnd=displayNumBegin;
                displayNumBegin=displayNumBegin-DISPLAYNUM;
                this->lhzBtnBack->setDisabled(true);
            }
            this->labelInput->setText("");
            for (int i = displayNumBegin; i < displayNumEnd; ++i)
            {
                tmp3=this->labelInput->text();
                tmp4=QString("%1.").arg(i-displayNumBegin+1)+model->record(i).value("chn").toString()+" ";
                this->labelInput->setText(tmp3+tmp4);
            }
            this->hzBtnPre->setDisabled(false);
        }
        else
        {
//            QSqlTableModel model(this->db);
            model->setTable("wubi");
            model->setFilter(QString("wb like '%1'").arg(pinyinStr+"%"));
            model->setSort(0,Qt::AscendingOrder);
            model->select();
            if (displayNumBegin>DISPLAYNUM)
            {
                displayNumEnd=displayNumBegin;
                displayNumBegin=displayNumBegin-DISPLAYNUM;
            }
            else
            {
                displayNumEnd=displayNumBegin;
                displayNumBegin=displayNumBegin-DISPLAYNUM;
                this->lhzBtnBack->setDisabled(true);
            }
            this->labelInput->setText("");
            for (int i = displayNumBegin; i < displayNumEnd; ++i)
            {
                tmp3=this->labelInput->text();
                tmp4=QString("%1.").arg(i-displayNumBegin+1)+model->record(i).value("chn").toString()+" ";
                this->labelInput->setText(tmp3+tmp4);
            }
            this->hzBtnPre->setDisabled(false);
        }
    }
}

void inputWidget::changeTextCaps(bool isCaps)
{
    if (isCaps) {
        this->num0Btn->setText(QChar('0'));
        this->num1Btn->setText(QChar('1'));
        this->num2Btn->setText(QChar('2'));
        this->num3Btn->setText(QChar('3'));
        this->num4Btn->setText(QChar('4'));
        this->num5Btn->setText(QChar('5'));
        this->num6Btn->setText(QChar('6'));
        this->num7Btn->setText(QChar('7'));
        this->num8Btn->setText(QChar('8'));
        this->num9Btn->setText(QChar('9'));

        this->qBtn->setText(QChar('Q'));
        this->wBtn->setText(QChar('W'));
        this->eBtn->setText(QChar('E'));
        this->rBtn->setText(QChar('R'));
        this->tBtn->setText(QChar('T'));
        this->yBtn->setText(QChar('Y'));
        this->uBtn->setText(QChar('U'));
        this->iBtn->setText(QChar('I'));
        this->oBtn->setText(QChar('O'));
        this->pBtn->setText(QChar('P'));

        this->aBtn->setText(QChar('A'));
        this->sBtn->setText(QChar('S'));
        this->dBtn->setText(QChar('D'));
        this->fBtn->setText(QChar('F'));
        this->gBtn->setText(QChar('G'));
        this->hBtn->setText(QChar('H'));
        this->jBtn->setText(QChar('J'));
        this->kBtn->setText(QChar('K'));
        this->lBtn->setText(QChar('L'));

        this->zBtn->setText(QChar('Z'));
        this->xBtn->setText(QChar('X'));
        this->cBtn->setText(QChar('C'));
        this->vBtn->setText(QChar('V'));
        this->bBtn->setText(QChar('B'));
        this->nBtn->setText(QChar('N'));
        this->mBtn->setText(QChar('M'));
    }
    else {
        this->num0Btn->setText(QChar('0'));
        this->num1Btn->setText(QChar('1'));
        this->num2Btn->setText(QChar('2'));
        this->num3Btn->setText(QChar('3'));
        this->num4Btn->setText(QChar('4'));
        this->num5Btn->setText(QChar('5'));
        this->num6Btn->setText(QChar('6'));
        this->num7Btn->setText(QChar('7'));
        this->num8Btn->setText(QChar('8'));
        this->num9Btn->setText(QChar('9'));

        this->qBtn->setText(QChar('q'));
        this->wBtn->setText(QChar('w'));
        this->eBtn->setText(QChar('e'));
        this->rBtn->setText(QChar('r'));
        this->tBtn->setText(QChar('t'));
        this->yBtn->setText(QChar('y'));
        this->uBtn->setText(QChar('u'));
        this->iBtn->setText(QChar('i'));
        this->oBtn->setText(QChar('o'));
        this->pBtn->setText(QChar('p'));

        this->aBtn->setText(QChar('a'));
        this->sBtn->setText(QChar('s'));
        this->dBtn->setText(QChar('d'));
        this->fBtn->setText(QChar('f'));
        this->gBtn->setText(QChar('g'));
        this->hBtn->setText(QChar('h'));
        this->jBtn->setText(QChar('j'));
        this->kBtn->setText(QChar('k'));
        this->lBtn->setText(QChar('l'));

        this->zBtn->setText(QChar('z'));
        this->xBtn->setText(QChar('x'));
        this->cBtn->setText(QChar('c'));
        this->vBtn->setText(QChar('v'));
        this->bBtn->setText(QChar('b'));
        this->nBtn->setText(QChar('n'));
        this->mBtn->setText(QChar('m'));
    }
}

void inputWidget::changeSign(bool isSign)
{
    if(isSign){
        this->num0Btn->setText(QChar(')'));
        this->num1Btn->setText(QChar('!'));
        this->num2Btn->setText(QChar('@'));
        this->num3Btn->setText(QChar('#'));
        this->num4Btn->setText(QChar('$'));
        this->num5Btn->setText(QChar('%'));
        this->num6Btn->setText(QChar('^'));
        this->num7Btn->setText(QChar('|'));
        this->num8Btn->setText(QChar('*'));
        this->num9Btn->setText(QChar('('));

        this->qBtn->setText(QChar('/'));
        this->wBtn->setText(QChar('_'));
        this->eBtn->setText(QChar('-'));
        this->rBtn->setText(QChar('+'));
        this->tBtn->setText(QChar('='));
        this->yBtn->setText(QChar(','));
        this->uBtn->setText(QChar(':'));
        this->iBtn->setText(QChar(';'));
        this->oBtn->setText(QChar('.'));
        this->pBtn->setText(QChar('?'));

        this->aBtn->setText(QChar('['));
        this->sBtn->setText(QChar(']'));
        this->dBtn->setText(QObject::tr("{"));
        this->fBtn->setText(QObject::tr("}"));
        this->gBtn->setText(QChar('<'));
        this->hBtn->setText(QChar('>'));
        this->jBtn->setText(QObject::tr("《"));
        this->kBtn->setText(QObject::tr("》"));
        this->lBtn->setText(QChar('"'));

        this->zBtn->setText(QObject::tr("…"));
        this->xBtn->setText(QObject::tr("～"));
        this->cBtn->setText(QObject::tr("‘"));
        this->vBtn->setText(QObject::tr("’"));
        this->bBtn->setText(QObject::tr("、"));
        this->nBtn->setText(QObject::tr("；"));
        this->mBtn->setText(QObject::tr("。"));
    }
    else{
        this->num0Btn->setText(QObject::tr("‰"));
        this->num1Btn->setText(QObject::tr("±"));
        this->num2Btn->setText(QObject::tr("×"));
        this->num3Btn->setText(QObject::tr("÷"));
        this->num4Btn->setText(QObject::tr("∑"));
        this->num5Btn->setText(QObject::tr("√"));
        this->num6Btn->setText(QObject::tr("Ω"));
        this->num7Btn->setText(QObject::tr("Δ"));
        this->num8Btn->setText(QObject::tr("￥"));
        this->num9Btn->setText(QObject::tr("℃"));

        this->qBtn->setText(QObject::tr("α"));
        this->wBtn->setText(QObject::tr("β"));
        this->eBtn->setText(QObject::tr("γ"));
        this->rBtn->setText(QObject::tr("δ"));
        this->tBtn->setText(QObject::tr("ε"));
        this->yBtn->setText(QObject::tr("ζ"));
        this->uBtn->setText(QObject::tr("η"));
        this->iBtn->setText(QObject::tr("θ"));
        this->oBtn->setText(QObject::tr("ι"));
        this->pBtn->setText(QObject::tr("→"));

        this->aBtn->setText(QObject::tr("κ"));
        this->sBtn->setText(QObject::tr("λ"));
        this->dBtn->setText(QObject::tr("μ"));
        this->fBtn->setText(QObject::tr("ν"));
        this->gBtn->setText(QObject::tr("ξ"));
        this->hBtn->setText(QObject::tr("ο"));
        this->jBtn->setText(QObject::tr("π"));
        this->kBtn->setText(QObject::tr("ρ"));
        this->lBtn->setText(QObject::tr("←"));

        this->zBtn->setText(QObject::tr("σ"));
        this->xBtn->setText(QObject::tr("τ"));
        this->cBtn->setText(QObject::tr("υ"));
        this->vBtn->setText(QObject::tr("φ"));
        this->bBtn->setText(QObject::tr("χ"));
        this->nBtn->setText(QObject::tr("ψ"));
        this->mBtn->setText(QObject::tr("ω"));
    }
}

inputWidget::~inputWidget()
{   
    delete mapper;
}

bool inputWidget::checkNotTextKey(int keyId)
{
        if (keyId==Qt::Key_Shift
            || keyId==Qt::Key_Control
            || keyId==Qt::Key_Tab
            || keyId==Qt::Key_Escape
            || keyId==Qt::Key_Return
            || keyId==Qt::Key_Insert
            || keyId==Qt::Key_NumLock
            || keyId==Qt::Key_F1
            || keyId==Qt::Key_F2
            || keyId==Qt::Key_F3
            || keyId==Qt::Key_F4
            || keyId==Qt::Key_F5
            || keyId==Qt::Key_F6
            || keyId==Qt::Key_F7
            || keyId==Qt::Key_F8
            || keyId==Qt::Key_F9
            || keyId==Qt::Key_F10
            || keyId==Qt::Key_F11
            || keyId==Qt::Key_F12
            || keyId==Qt::Key_Print
            || keyId==Qt::Key_Pause
            || keyId==Qt::Key_ScrollLock
            || keyId==Qt::Key_Enter
            || keyId==Qt::Key_Home
            || keyId==Qt::Key_End
            || keyId==Qt::Key_CapsLock
            || keyId==Qt::Key_Insert
            || keyId==Qt::Key_Delete
            || keyId==Qt::Key_PageUp
            || keyId==Qt::Key_PageDown
            || keyId==Qt::Key_Down
            || keyId==Qt::Key_Up
            || keyId==Qt::Key_Left
            || keyId==Qt::Key_Right
            || keyId==Qt::Key_Alt
            || keyId==Qt::Key_Backspace) {
                                return true;
        }
        else
                return false;
}

bool inputWidget::checkNUMKey(int keyId)
{
    if (keyId==Qt::Key_0
        || keyId==Qt::Key_1
        || keyId==Qt::Key_2
        || keyId==Qt::Key_3
        || keyId==Qt::Key_4
        || keyId==Qt::Key_5
        || keyId==Qt::Key_6
        || keyId==Qt::Key_7
        || keyId==Qt::Key_8
        || keyId==Qt::Key_9)
    {
        return true;
    }
    else
        return false;
}


void inputWidget::on_capslockBtn_toggled(bool checked)
{
    isCapsLock =checked;
    if(isCapsLock)
    {
        if(isSign)
            this->signBtn->setChecked(!isSign);
        if(isSign2)
            this->sign2Btn->setChecked(!isSign2);
        if(!isEnglish)
            this->ecBtn->setChecked(isEnglish);
        this->labelInputname->setText(QObject::tr("大写输入"));
        this->capslockBtn->setText(QObject::tr("大写"));
    }
    else
    {
        this->labelInputname->setText(QObject::tr("小写输入"));
        this->capslockBtn->setText(QObject::tr("小写"));
    }
    changeTextCaps(isCapsLock);
}

void inputWidget::on_signBtn_toggled(bool checked)
{
    isSign =checked;
    if(isSign)
    {
        if(isCapsLock)
           this->capslockBtn->setChecked(!isCapsLock);
        if(isSign2)
           this->sign2Btn->setChecked(!isSign2);
        if(!isEnglish)
        {
            this->ecBtn->setChecked(isEnglish);
        }
        changeSign(isSign);
        this->labelInputname->setText(QObject::tr("标点符号"));
    }
    else
    {
        changeTextCaps(isSign);
        this->labelInputname->setText(QObject::tr("小写输入"));
    }
}

void inputWidget::on_sign2Btn_toggled(bool checked)
{
    isSign2 =checked;
    if(isSign2)
    {
        if(isCapsLock)
           this->capslockBtn->setChecked(!isCapsLock);
        if(isSign)
           this->signBtn->setChecked(!isSign);
        if(!isEnglish)
        {
            this->ecBtn->setChecked(isEnglish);
        }
        changeSign(!isSign2);
        this->labelInputname->setText(QObject::tr("特殊符号"));
    }
    else
    {
        changeTextCaps(!isSign2);
        this->labelInputname->setText(QObject::tr("小写输入"));
    }
}

void inputWidget::on_ecBtn_toggled(bool checked)
{
    isEnglish=!checked;
    if(!isEnglish)
    {
        if(isCapsLock)
           this->capslockBtn->setChecked(!isCapsLock);
        if(isSign)
           this->signBtn->setChecked(!isSign);
        if(isSign2)
           this->sign2Btn->setChecked(!isSign2);
        this->pwBtn->setDisabled(false);
        changeTextCaps(isEnglish);
        this->labelInputname->setText(QObject::tr("拼音输入"));
        this->lineEditInput->setVisible(true);
        this->labelInput->setVisible(true);
    }
    else
    {
        if(!isPinyin)
            this->pwBtn->setChecked(isPinyin);
        this->pwBtn->setDisabled(true);
        this->labelInputname->setText(QObject::tr("小写输入"));
        this->lineEditInput->setVisible(false);
        this->labelInput->setVisible(false);
    }
}

void inputWidget::on_pwBtn_toggled(bool checked)
{
    isPinyin =!checked;
    if(isPinyin)
        this->labelInputname->setText(QObject::tr("拼音输入"));
    else
        this->labelInputname->setText(QObject::tr("五笔输入"));
    this->lineEditInput->setText("");
}

void inputWidget::mousePressEvent(QMouseEvent *event)
{

    this->windowPos = this->pos();
    this->mousePos = event->globalPos();
    this->dPos = mousePos - windowPos;

}
void inputWidget::mouseMoveEvent(QMouseEvent *event)
{

    this->move(event->globalPos() - this->dPos);

}
